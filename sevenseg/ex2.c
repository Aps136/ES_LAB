#include <LPC17xx.h>
unsigned char seven_seg[10]={0x3F,0X06,0X5B,0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F};
unsigned char new_seg[4]={0,1,2,3};
unsigned int i,j;
void delay(void);

int main(void){
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL0=0X00000000;//4 to 11 gpio data lines
	LPC_GPIO0->FIODIR=0X00000FF0;
	LPC_GPIO1->FIODIR=0X07800000;//23 to 26 is output
	while(1){
		for(i=0;i<4;i++){
			LPC_GPIO1->FIOPIN=new_seg[i]<<23;
			LPC_GPIO0->FIOPIN = seven_seg[i+1]<<4;//displays 1234
			//delay;
			
		}
		
	}
	
}
void delay(void){
	for(int j=0;j<1000000;j++);
}
