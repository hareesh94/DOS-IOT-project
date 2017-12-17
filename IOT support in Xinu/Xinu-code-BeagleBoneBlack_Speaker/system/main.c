/*  main.c  - main */

#include <xinu.h>

pid32 blinker_id;


#define LED_PIN 0x44e10898
#define MMAP_OFFSET (0x44C00000)
#define MMAP_SIZE   (0x481AEFFF-MMAP_OFFSET)

/* Clock Module Memory Registers */
#define CM_WKUP (0x44E00400)
#define CM_WKUP_ADC_TSC_CLKCTRL (CM_WKUP+0xBC)
#define CM_WKUP_MODULEMODE_ENABLE (0x02)
#define CM_WKUP_IDLEST_DISABLED (0x03<<16)

#define CM_PER (0x44E00000)
#define CM_PER_EPWMSS1_CLKCTRL (CM_PER+0xCC)
#define CM_PER_EPWMSS0_CLKCTRL (CM_PER+0xD4)
#define CM_PER_EPWMSS2_CLKCTRL (CM_PER+0xD8)


/* GPIO Memory Registers */
#define GPIO_REGISTER_SIZE (4)

/*#define GPIO0 		(0x44E07000)
#define GPIO1		(0x4804C000)
#define GPIO2		(0x481AC000)
#define GPIO3		(0x481AE000)
*/
#define GPIO_CLEARDATAOUT (0x190)
#define GPIO_SETDATAOUT   (0x194)
#define GPIO_OE			      (0x134)
#define GPIO_DATAOUT      (0x13C)
#define GPIO_DATAIN       (0x138)

/* Analog Digital Converter Memory Registers */
#define ADC_TSC (0x44E0D000)

#define ADC_CTRL (0x44E0D040)
#define ADC_STEPCONFIG_WRITE_PROTECT_OFF (0x01<<2)
#define ADC_STEPENABLE (0x44E0D054)

#define ADCSTEPCONFIG1 0x44E0D064
#define ADCSTEPDELAY1  0x44E0D068
#define ADCSTEPCONFIG2 0x44E0D06C
#define ADCSTEPDELAY2  0x44E0D070
#define ADCSTEPCONFIG3 0x44E0D074
#define ADCSTEPDELAY3  0x44E0D078
#define ADCSTEPCONFIG4 (ADC_TSC+0x7C)
#define ADCSTEPDELAY4  (ADC_TSC+0x80)
#define ADCSTEPCONFIG5 (ADC_TSC+0x84)
#define ADCSTEPDELAY5  (ADC_TSC+0x88)
#define ADCSTEPCONFIG6 (ADC_TSC+0x8C)
#define ADCSTEPDELAY6  (ADC_TSC+0x90)
#define ADCSTEPCONFIG7 (ADC_TSC+0x94)
#define ADCSTEPDELAY7  (ADC_TSC+0x98)
#define ADCSTEPCONFIG8 (ADC_TSC+0x9C)
#define ADCSTEPDELAY8  (ADC_TSC+0xA0)

#define ADC_AVG0  (0x000)
#define ADC_AVG2  (0x001)
#define ADC_AVG4  (0x010)
#define ADC_AVG8  (0x011)
#define ADC_AVG16 (0x100) 

#define ADC_FIFO0DATA 0x44E0D100
#define ADC_FIFO_MASK (0xFFF)

#define PORTT 9

uint32	localip;		/* local IP address		*/
int32	slot;			/* slot in UDP table 		*/


process listener ( void ) {
	int32	retval;			/* return value from sys calls	*/
	uint32	remip;			/* remote sender's IP address	*/
	uint16	remport;		/* remote sender's UDP port	*/
	char	buff[1500];		/* buffer for incoming reply	*/
	int32	msglen;			/* length of outgoing message	*/


	/* Do forever: read an incoming datagram and send it back */
	kprintf("LISTENR STARTED\n");
	int i=0;
	while (TRUE) {
		kprintf("LOOPING");
		retval = udp_recvaddr(slot, &remip, &remport, buff,
				sizeof(buff), 600000);

		if (retval == TIMEOUT) {
			kprintf("\n\n\nLISTENER TIMED_OUT\n\n\n");
			continue;
		} else if (retval == SYSERR) {
			kprintf("\n\n\nerror receiving UDP\n");
		}
		msglen = retval;
		for ( i=0;i<msglen;i++){
			kprintf("\n\n\nMSG was %c\n",buff[i]);
		}	
	}
	return OK;

}



process speaker ( ) {

	int32	retval;			/* return value from sys calls	*/
	uint32	remip;			/* remote sender's IP address	*/
	uint16	remport;		/* remote sender's UDP port	*/
	char	buff[1500];		/* buffer for incoming reply	*/
	int32	msglen;			/* length of outgoing message	*/
	int count=0;
	kprintf("\n\nSTARTED SPEAKER\n");

	/* Do forever: read an incoming datagram and send it back */

	while (TRUE) {
		sleep(4);
		msglen = 2;
		buff[0]='6';
		buff[1]='9';
		buff[2]='\0';
		retval = udp_sendto(slot, 3232238450, PORTT, buff, msglen);
		if (retval == SYSERR) {
			kprintf("udp_sendto failed\n");
		} else {
			kprintf("\n\n\nudp data send:%d:%d:%d",localip,PORTT,msglen);
		}
		count++;
		if ( count == 10000 ) {
			return OK;
		}
	}
	return OK;
}





process blinker ( void ) {


	volatile uint32 *led = LED_PIN;

	{
		uint32 *p = CM_WKUP_ADC_TSC_CLKCTRL;
		*p |= CM_WKUP_MODULEMODE_ENABLE;
		while( !(*p & CM_WKUP_MODULEMODE_ENABLE) ){
			kprintf("waiting\n");
			sleep(1);
		}
		kprintf("clock wokeup\n");
	}

	{
		uint32 *p=ADC_CTRL;
		*p|= ADC_STEPCONFIG_WRITE_PROTECT_OFF;
		while( !(*p & ADC_STEPCONFIG_WRITE_PROTECT_OFF) ){
			kprintf("waiting\n");
			sleep(1);
		}
		kprintf("turned off the write protect\n");
	}

	{
		uint32 *p=ADCSTEPCONFIG1;
		*p= 0x00<<19 | ADC_AVG16;
	}

	{
		uint32 *p=ADCSTEPDELAY1;
		*p = (0x0F)<<24;
	}

	{
		uint32 *p=ADCSTEPCONFIG2;
		*p= 0x01<<19 | ADC_AVG16;
	}
	{
		uint32 *p=ADCSTEPDELAY2;
		*p = (0x0F)<<24;
	}


	{
		uint32 *p=ADCSTEPCONFIG3;
		*p= 0x02<<19 | ADC_AVG16;
	}
	{
		uint32 *p=ADCSTEPDELAY3;
		*p = (0x0F)<<24;
	}

	{
		uint32 *p=ADC_CTRL;
		*p |= 0x01;
	}


	while(1) { 

		volatile uint32 *q=ADC_STEPENABLE;
		*q |= (0x01<<(1+1));	
		sleep(5);
		volatile uint32 *p = ADC_FIFO0DATA;
		int value = (*p & ADC_FIFO_MASK);
		if ( value > 1800 ) {
			*led = 0x27;
		} else {
			*led = 0x0;
		}	

		kprintf("Reading is:%d\n", value);
	}

	/*	while(1) {

		sleep(5);
	 *p = 0x27;
	 kprintf("STARTING LED  %0xu:%0xd\n",p,*p);

	 sleep(5);
	 *p = 0x0;
	 kprintf("KILLING LED %0xu:%0xd\n",p,*p);	
	 }
	 */
	return OK;
}


process	main(void)
{

	/* Run the Xinu shell */

	recvclr();
	blinker_id   = create(blinker, 4096, 50, "blinker", 0);
	//resume(blinker_id); 

	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	
	localip = getlocalip();
	if (localip == SYSERR) {
		kprintf("MAIN could not obtain a local IP address");
		return 1;
	}

	/* register local UDP port */

	slot = udp_register(0, PORTT, PORTT);
	if (slot == SYSERR) {
		kprintf("\n\n\ncould not reserve UDP port %d\n",PORTT);
	} else {

		kprintf("\n\n\nMAIN Registered Successfully\n");
	}


	resume(create(speaker, 8192, 50, "speaker", 0));
	//resume(create(listener, 8192, 50, "listener",0 ));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;

}
