#include<xinu.h>

extern int32 rslot;

devcall rledputc ( struct dentry* devptr , char data ) {
	kprintf("RLEDPUTC STARTED\n");
	char sbuff[4];
	sbuff[0]='1';
	sbuff[1]=':';
	sbuff[2]=data;
	kprintf("RLEDPUTC PACKET SENT\n");
	udp_sendto(rslot, 0xc0a80b6f , PORTT, sbuff, 3);
	
	
	return OK;
}
