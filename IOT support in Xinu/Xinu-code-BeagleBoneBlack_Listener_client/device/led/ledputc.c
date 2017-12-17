#include<xinu.h>

devcall ledputc ( struct dentry* devptr , char data ) {
	volatile uint32 *l = devptr->dvcsr;
	*l = data;
	return OK;
}
