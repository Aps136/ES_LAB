//switch pressed->jhon counter else ->ring counter
#include<LPC17xx.h>
#include <stdio.h>
unsigned int j,i;
unsigned long LED= 0X00000FF0;
int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();	
	LPC_PINCON->PINSEL0 = 0X00000000;
	LPC_GPIO0->FIODIR = 0X00000FF0;	
	while (1){
		if((LPC_GPIO2->FIOPIN&1<<12)){			
	  LED=0X00000010;		
		for(i=0; i<4; i++){		
			LPC_GPIO0->FIOSET=LED;
			//for(j=0; j<1000000; j++);
			LED<<=1;			
	}	
	LED=0X00000010;
	for(i=0; i<4; i++){
			
			LPC_GPIO0->FIOCLR=LED;
			//for(j=0; j<1000000; j++);
			LED<<=1;
	}
	
}	
	else
	{
		
		LED=0X0000010;
		LED<<=4;
		for(i=0; i<4; i++){		
			LPC_GPIO0->FIOSET=LED;
			//for(j=0; j<1000000; j++);
			
			LPC_GPIO0->FIOCLR=LED;
			//for(j=0; j<1000000; j++);
			LED<<=1;
	}
	}
	
}
}
