#include<LPC17xx.h>
unsigned int j;
unsigned long LED =0x00000FF0;
int main(void){
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL0=0x0000000;
	LPC_GPIO0->FIODIR=0X00000FF0;
	while(1){
		if(!(LPC_GPIO2->FIOPIN & 1<<12))
			LPC_GPIO0->FIOSET=LED;
		else
			LPC_GPIO0->FIOCLR=LED;
	}
}  
