#include<xinu.h>

extern int32 slot;

devcall rledgetc ( struct dentry* devptr  ) {
	char buff[1000];
	int32	retval;			/* return value from sys calls	*/
	uint32	remip;			/* remote sender's IP address	*/
	uint16	remport;		/* remote sender's UDP port	*/
	int32	msglen;			/* length of outgoing message	*/

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

		if ( led == 1 ) { // this is normal led glow request
			if ( status ==1 ) {
				putc(LED,0x0);
			} else if ( status == 0 ) {
				putc(LED,0x27);
			}
		}  

		kprintf("\n\nmessage received:%s",buff);
		kprintf("\n\nNeed to glow:%d with status:%d",led,status);
		kprintf("\n\nREMOTE IP:PORT was %d:%d\n",remip,remport);
/*		
		retval = udp_sendto(slot, remip, remport, buff, msglen);
		if (retval == SYSERR) {
			kprintf("udp_sendto failed\n");
		} else {

			kprintf("udp send success\n");
		}*/
	
	
	
	
	}	
	
	
	return OK;
}
