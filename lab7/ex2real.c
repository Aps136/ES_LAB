//Bin up when switch pressed, down otherwise 
#include <LPC17xx.h>
unsigned int i,j;
unsigned long int LED=0x00000FF0;
int count=0;
int countd=255;
int main(void){
		SystemInit();
		SystemCoreClockUpdate();
		LPC_PINCON->PINSEL0=0X00000000;
	  LPC_GPIO0->FIODIR= 0X00000FF0;
	  
	  while(1){
			if(!(LPC_GPIO2->FIOPIN & 1<<12)){
				LPC_GPIO0->FIOCLR=0X00000FF0;
			LPC_GPIO0->FIOSET=(count<<4);
			//delay
			count++;
			
			}
			
			else{				
				LPC_GPIO0->FIOCLR=0X00000FF0;
				LPC_GPIO0->FIOSET=countd<<4;	
				//delay				
				countd--;		
				
			}
			
		}
	
	
}
