#include<LPC17xx.h>
#include <stdio.h>
unsigned int j,i;
unsigned long LED= 0X00000FF0;
unsigned char count=0;
int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();		
	LPC_PINCON->PINSEL0 = 0X00000000;
	LPC_GPIO0->FIODIR = 0X00000FF0;	
	while (1){
		LPC_GPIO0->FIOCLR=0x00000FF0;
		LPC_GPIO0->FIOSET=(count<<4);
		if(!(LPC_GPIO2->FIOPIN&1<<12)){									
					if(count==255){
						count=0;
					}
					else{
						count++;
					}	  			
	}		
	else
	{		
		if(count==0){
			count=255;
		}
		else{
			count--;
		}
		
	
	}
	
}
}
