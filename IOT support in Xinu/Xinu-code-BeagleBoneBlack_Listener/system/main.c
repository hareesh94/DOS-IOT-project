/*  main.c  - main */

#include <xinu.h>

#define ID 1

pid32 blinker_id;


uint32	localip;		/* local IP address		*/
int32	slot;			/* slot in UDP table 		*/

int32	rslot;			/* slot in UDP table 		*/

int32 allowcontrol;

process listener ( void ) {
	int32	retval;			/* return value from sys calls	*/
	uint32	remip;			/* remote sender's IP address	*/
	uint16	remport;		/* remote sender's UDP port	*/
	char	buff[1500];		/* buffer for incoming reply	*/
	char	rbuff[4];		/* buffer to read sensor */
	char    astatus[1500];
	int32	msglen;			/* length of outgoing message	*/

	volatile uint32 *l = LED_PIN;

	/* Do forever: read an incoming datagram and send it back */
	kprintf("LISTENR STARTED\n");
	int i=0;
	while (TRUE) {
		kprintf("WAITING FOR PACKET");
		retval = udp_recvaddr(slot, &remip, &remport, buff,
				sizeof(buff), 600000);

		if (retval == TIMEOUT) {
			kprintf("\n\n\nLISTENER TIMED_OUT\n\n\n");
			continue;
		} else if (retval == SYSERR) {
			kprintf("\n\n\nerror receiving UDP\n");
		}
		
		msglen = retval;
		buff[msglen] = '\0';
		
		int led,status;
		led = (buff[0] - '1')+1;
		status = (buff[2] - '1') +1;

		// always send current status with any outgoing packet 
		//read(SENSOR,(char *)rbuff,4);
		//unsigned int value = *(unsigned int*)rbuff;
		volatile uint32 *p = ADC_FIFO0DATA;
		int value = (*p & ADC_FIFO_MASK);

		buff[msglen] = ':';
		buff[msglen+1] = value /1000 + '0';
	        buff[msglen+2] = (value /100) % 10 + '0';
		buff[msglen+3] = (value / 10) % 10 + '0';
		buff[msglen+4] = (value % 10) + '0';
		buff[msglen+5] = '\0';
		msglen = msglen+5;


		if ( led == 1 ) { // this is normal led glow request
			if ( status ==1 ) {
				putc(LED,0x0);
			} else if ( status == 0 ) {
				putc(LED,0x27);
			}
		}  else if ( led == 8 ) {  // this is check status request
			kprintf(" check status request\n");
		} else if ( led == 9 ) {
			/* IOT request */
			if ( status == 1 ) {
				allowcontrol = 1;
				kprintf("STARTING AUTO MODE");
			} else if ( status == 0 ) {
				allowcontrol = 0;
				kprintf("SHUTTING DOWN AUTO MODE");
			}
		}	

		kprintf("\n\nmessage received:%s",buff);
		kprintf("\n\nNeed to glow:%d with status:%d",led,status);
		kprintf("\n\nREMOTE IP:PORT was %d:%d\n",remip,remport);
		
		retval = udp_sendto(slot, remip, remport, buff, msglen);
		if (retval == SYSERR) {
			kprintf("udp_sendto failed\n");
		} else {

			kprintf("udp send success\n");
		}
	
		kprintf("LIGHT Reading is:%d\n", value);
	}
	return OK;
}


process blinker ( void ) {


	//init(SENSOR);
	//init(LED);
	volatile uint32 *led = LED_PIN;
	char buff[4];
	char sbuff[4];
	uint32	remip;			/* remote sender's IP address	*/
	while(1) { 
		
		sleep(5);
		read(SENSOR,(char *)buff,4);
		unsigned int value = *(unsigned int*)buff;
		
		if ( allowcontrol == 1 ) {
			if ( value > 1800 ) {
				putc(LED,0x27);
				//putc(RLED,'0');
				sbuff[0]='1';
				sbuff[1]=':';
				sbuff[2]='0';
				udp_sendto(slot, 0xc0a80b6f, PORTT, sbuff, 3);
			} else {
				putc(LED,0);
				//putc(RLED,'1');
				sbuff[0] ='1';
				sbuff[1] =':';
				sbuff[2]='1';
				udp_sendto(slot, 0xc0a80b6f, PORTT, sbuff, 3);
			}
		}	
		kprintf("\n\nAfter reading string buffer:%d\n",value);
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


process broadcaster ( ) {

	int32	retval;			/* return value from sys calls	*/
	uint32	remip;			/* remote sender's IP address	*/
	uint16	remport;		/* remote sender's UDP port	*/
	char	buff[1500];		/* buffer for incoming reply	*/
	int32	msglen;			/* length of outgoing message	*/
	int count=0;
	kprintf("\n\nSTARTED BROADCASTER\n");

	char    astatus[1500];
	volatile uint32 *l = LED_PIN;
	/* Do forever: read an incoming datagram and send it back */

	while (TRUE) {
		sleep(4);
		msglen = 1;
		buff[0]='3';
		buff[1]='\0';
		retval = udp_sendto(slot, 4294967295, PORTT, buff, msglen);
		if (retval == SYSERR) {
			kprintf("udp_sendto broadcaster failed\n");
		} else {
			kprintf("\n\n\nBROADCAST DONE:%u:%d:%d",localip,PORTT,msglen);
		}
		count++;
		if ( count == 10000 ) {
			return SYSERR; // no edge found
		}
	
		/* waiting for ack after broadcast */

		retval = udp_recvaddr(slot, &remip, &remport, buff,
				sizeof(buff), 9000);

		if (retval == TIMEOUT) {
			kprintf("\n\n\nLISTENER TIMED_OUT\n\n\n");
			continue; // send broadcast again
		} else if (retval == SYSERR) {
			kprintf("\n\n\nerror receiving UDP\n");
			continue;
		}
	
		kprintf("\n\n\n OBTAINED ACK FROM EDGE !!!\n");	
		
		resume(blinker_id); 
		resume(create(listener, 8192, 50, "listener",0 ));
		return OK;
			
	}
	return OK;
}


process broadcaster2 ( ) {

	int32	retval;			/* return value from sys calls	*/
	uint32	remip;			/* remote sender's IP address	*/
	uint16	remport;		/* remote sender's UDP port	*/
	char	buff[1500];		/* buffer for incoming reply	*/
	int32	msglen;			/* length of outgoing message	*/
	int count=0;
	kprintf("\n\nSTARTED BROADCASTER\n");

	char    astatus[1500];
	volatile uint32 *l = LED_PIN;
	/* Do forever: read an incoming datagram and send it back */

	while (TRUE) {
		sleep(4);
		msglen = 1;
		buff[0]='1';
		buff[1]='\0';
		retval = udp_sendto(slot, 4294967295, PORTT, buff, msglen);
		if (retval == SYSERR) {
			kprintf("udp_sendto broadcaster2 failed\n");
		} else {
			kprintf("\n\n\nBROADCAST2 DONE:%u:%d:%d",localip,PORTT,msglen);
		}
		count++;
		if ( count == 10000 ) {
			return SYSERR; // no edge found
		}
	
		/* waiting for ack after broadcast */

		retval = udp_recvaddr(slot, &remip, &remport, buff,
				sizeof(buff), 9000);

		if (retval == TIMEOUT) {
			kprintf("\n\n\nLISTENER TIMED_OUT\n\n\n");
			continue; // send broadcast again
		} else if (retval == SYSERR) {
			kprintf("\n\n\nerror receiving UDP\n");
			continue;
		}
	
		kprintf("\n\n\n OBTAINED ACK FROM EDGE !!!\n");	
		return OK;
		}
	return OK;
}





process	main(void)
{

	/* Run the Xinu shell */

	recvclr();
	allowcontrol = 1;   /* by default auto mode on*/
	blinker_id   = create(blinker, 4096, 50, "blinker", 0);
	//resume(blinker_id); 

	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	
	localip = getlocalip();
	if (localip == SYSERR) {
		kprintf("MAIN could not obtain a local IP address");
		return 1;
	}

	/* register local UDP port */

	slot = udp_register(0, 0, PORTT);
	if (slot == SYSERR) {
		kprintf("\n\n\ncould not reserve UDP port %d\n",PORTT);
	} else {

		kprintf("\n\n\nMAIN Registered Successfully\n");
	}


	rslot = udp_register(0, 0, 10);
	if (rslot == SYSERR) {
		kprintf("\n\n\ncould not reserve RUDP port %d\n",10);
	} else {

		kprintf("\n\n\nMAIN Registered Successfully\n");
	}



	if ( ID == 1 ) {
		resume(create(broadcaster, 8192, 50, "broadcaster", 0));
	} else if ( ID == 2 ) {
		resume(create(broadcaster2, 8192, 50, "broadcaster2", 0));
	}


	return OK;

}
