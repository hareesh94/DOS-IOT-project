#include<xinu.h>
devcall sensorinit ( 	  
		struct dentry	*devptr		/* Entry in device switch table	*/
		) {
	kprintf("\nsensor init called %0xu\n",devptr->dvcsr);
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
	return OK;

}
