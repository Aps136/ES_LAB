#include <LPC17xx.h>
unsigned int i,j;
unsigned long int LED=0x00000FF0;
unsigned int count=0;
int main(void){
		SystemInit();
		SystemCoreClockUpdate();
		LPC_PINCON->PINSEL0=0X00000000;
	  LPC_GPIO0->FIODIR= 0X00000FF0;
	while(1){
		LPC_GPIO0->FIOCLR = 0X00000FF0;
		LPC_GPIO0->FIOSET=(count<<4);
		//delay
		if(!(LPC_GPIO2->FIOPIN & 1<<12)){//switch pressed: count up
			if(count<255){//only if count<255
				count++;
			}
			
		}else{
			if(count>0){
				count--;
			}
		}
		
	}
	  
	  
			
		
	
	
}
