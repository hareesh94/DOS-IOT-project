#include<xinu.h>

devcall ledinit ( 	  
		struct dentry	*devptr		/* Entry in device switch table	*/
		) {
	//kprintf("\nled init called %0xu\n",devptr->dvcsr);
	//ledputc(devptr,0x27);
	return OK;
}
