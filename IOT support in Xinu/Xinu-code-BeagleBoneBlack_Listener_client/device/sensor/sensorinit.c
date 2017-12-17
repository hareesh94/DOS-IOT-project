#include<xinu.h>

void TestRegAddress(struct adc_csreg* pReg)
{
	kprintf("revison = %x, %x\r\n", (unsigned int)&pReg->revison,pReg->revison);
	kprintf("sysconfig = %x, %x\r\n", &pReg->sysconfig,pReg->sysconfig);
	kprintf("irqStatusRaw = %x, %x\r\n", (unsigned int)&pReg->irqStatusRaw,pReg->irqStatusRaw);
	kprintf("irqStatus = %x, %x\r\n", (unsigned int)&pReg->irqStatus,pReg->irqStatus);
	kprintf("irqEnableSet = %x, %x\r\n", (unsigned int)&pReg->irqEnableSet,pReg->irqEnableSet);	
	kprintf("irqEnableClr = %x, %x\r\n", (unsigned int)&pReg->irqEnableClr,pReg->irqEnableClr);
	kprintf("irqWakeup = %x, %x\r\n", (unsigned int)&pReg->irqWakeup,pReg->irqWakeup);
	kprintf("dmaEnableSet = %x, %x\r\n", (unsigned int)&pReg->dmaEnableSet,pReg->dmaEnableSet);
	kprintf("dmaEnableClr = %x, %x\r\n", (unsigned int)&pReg->dmaEnableClr,pReg->dmaEnableClr);
	kprintf("ctrl = %x, %x\r\n", (unsigned int)&pReg->ctrl,pReg->ctrl);
	kprintf("adcStat = %x, %x\r\n", (unsigned int)&pReg->adcStat,pReg->adcStat);
	kprintf("adcRange = %x, %x\r\n", (unsigned int)&pReg->adcRange,pReg->adcRange);
	kprintf("adcClkDiv = %x, %x\r\n", (unsigned int)&pReg->adcClkDiv,pReg->adcClkDiv);
	kprintf("adcMisc = %x, %x\r\n", (unsigned int)&pReg->adcMisc,pReg->adcMisc);
	kprintf("stepEnable = %x, %x\r\n", (unsigned int)&pReg->stepEnable,pReg->stepEnable);
	kprintf("idleConfig = %x, %x\r\n", (unsigned int)&pReg->idleConfig,pReg->idleConfig);
	kprintf("tsChargeStepconfig = %x, %x\r\n", (unsigned int)&pReg->tsChargeStepconfig,pReg->tsChargeStepconfig);
	kprintf("tsChargeDelay = %x, %x\r\n", (unsigned int)&pReg->tsChargeDelay,pReg->tsChargeDelay);
	int i=0;
	for(i=0;i<16;i++)
	{
		kprintf("stepConfig%d = %x, %x\r\n", i, (unsigned int)&pReg->step[i].stepConfig,pReg->step[i].stepConfig);
		kprintf("stepDelay%d = %x, %x\r\n", i, (unsigned int)&pReg->step[i].stepDelay,pReg->step[i].stepDelay);
	}
	for(i=0;i<2;i++)
	{
		kprintf("fifoCount%d = %x, %x\r\n", i,(unsigned int)&pReg->fifoInfo[i].fifoCount,pReg->fifoInfo[i].fifoCount);
		kprintf("fifoThreshold%d = %x, %x\r\n", i, (unsigned int)&pReg->fifoInfo[i].fifoThreshold,pReg->fifoInfo[i].fifoThreshold);
		kprintf("dmaReq%d = %x, %x\r\n", i, (unsigned int)&pReg->fifoInfo[i].dmaReq,pReg->fifoInfo[i].dmaReq);
	}
	kprintf("fifoData0 = %x, %x\r\n", (unsigned int)&pReg->fifoData0,pReg->fifoData0);
	kprintf("fifoData1 = %x, %x\r\n", (unsigned int)&pReg->fifoData1,pReg->fifoData1);

	int bank =16;
	struct	intc_csreg *csrptr = (struct intc_csreg *)0x48200000;
	//kprintf("pending irq = %x\r\n",csrptr->);
	kprintf("ITR = %x\r\n",csrptr->banks[bank].itr);
	kprintf("mir = %x\r\n",csrptr->banks[bank].mir);
	kprintf("mir_clear = %x\r\n",csrptr->banks[bank].mir_clear);
	kprintf("mir_set = %x\r\n",csrptr->banks[bank].mir_set);
	kprintf("isr_set = %x\r\n",csrptr->banks[bank].isr_set);
	kprintf("isr_clear = %x\r\n",csrptr->banks[bank].isr_clear);
	kprintf("pending_irq = %x\r\n",csrptr->banks[bank].pending_irq);
	kprintf("pending_fiq = %x\r\n",csrptr->banks[bank].pending_fiq);
}



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
	
	
	TestRegAddress((struct adc_csreg*) devptr->dvcsr);
	return OK;

}
