#include<LPC17xx.h>
unsigned char seven_seg[10]={0X3F,0X06,0X5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F};
unsigned int i,j;
void delay(void);
int main(void){
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL0=0x00000000;
	LPC_GPIO0->FIODIR=0X00000FF0;
	
	LPC_GPIO1->FIODIR=0X07800000;
	while(1){
		for(i=0;i<10;i++){
			LPC_GPIO1->FIOPIN= 0<<23;//chooses 1st segment
			LPC_GPIO0->FIOPIN= seven_seg[i]<<4;
			//delay();
			
		}
	}	
}
void delay(void){
	for(j=0;j<10000000;j++);
	
}
