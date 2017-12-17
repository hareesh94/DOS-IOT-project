#include<xinu.h>

devcall sensorread ( 	  
		struct dentry	*devptr,		/* Entry in device switch table	*/
		char *buff,
		int count
		) {
	//kprintf("\nsensor read called %0xu\n",devptr->dvcsr);
		
	volatile uint32 *q=ADC_STEPENABLE;
	*q |= (0x01<<(1+1));	
	sleep(5);
	volatile uint32 *p = ADC_FIFO0DATA;
	int value = (*p & ADC_FIFO_MASK);

	//kprintf("Obtained value %d warpping it in buff\n",value);
	buff[3] = (value&(0xff000000))>>24;
	buff[2] = (value&(0x00ff0000))>>16;	
	buff[1] = (value&(0x0000ff00))>>8;
	buff[0] = (value&(0x000000ff));
	return OK;
}
