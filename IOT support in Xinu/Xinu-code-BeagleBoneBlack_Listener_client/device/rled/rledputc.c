#include<xinu.h>

extern int32 slot;

devcall rledputc ( struct dentry* devptr , char data ) {
	//volatile uint32 *l = devptr->dvcsr;
	//*l = data;
	
	//send packet to other machine

	char sbuff[4];
	sbuff[0]='1';
	sbuff[1]=':';
	sbuff[2]=data;
	udp_sendto(slot, devptr->dvcsr, PORTT, sbuff, 3);
	
	
	return OK;
}
