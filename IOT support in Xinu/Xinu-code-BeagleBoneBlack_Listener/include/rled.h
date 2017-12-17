devcall rledputc ( struct dentry* devptr , char data );
devcall rledinit ( 	  
				struct dentry	*devptr		/* Entry in device switch table	*/
						);

devcall rledgetc ( struct dentry* devptr  );
