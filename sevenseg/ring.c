#include <LPC17xx.h>
#define FIRST_SEG (0<<23)
#define SECOND_SEG (1<<23)
#define THIRD_SEG (2<<23)
#define FOURTH_SEG (3<<23)
#define N 2000
unsigned long int i;
unsigned int dig_count=0;
unsigned int dig_value[4]={1,0,0,0};
unsigned int select_seg[4]={FIRST_SEG, SECOND_SEG, THIRD_SEG, FOURTH_SEG};
unsigned char seven_seg[2]={0x3F, 0X06};
unsigned int count=0, flag=0;
unsigned int temp;
void display(void);
void delay(void);
void ring(void);
int main(void){
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL0=0;
	LPC_PINCON->PINSEL3=0;
	LPC_GPIO0->FIODIR=0X00000FF0;
	LPC_GPIO1->FIODIR=0X07800000;
	while(1){
		delay();
		ring();
		dig_count++;
		if(dig_count==4){
			dig_count=0;
		}display();
	}
}
void display(void){
	LPC_GPIO1->FIOPIN = select_seg[dig_count];
	LPC_GPIO0->FIOCLR= 0x00000FF0;
	LPC_GPIO1->FIOPIN = seven_seg[dig_value[dig_count]]<<4;
	//DEALY
	
	
}
void delay (void){
	for(i=0;i<1000;i++){
		if(count==N){
			flag=1;
			count=0;
			
		}else{
			count++;
		}
	}
}
void ring(void){
	if(flag){
		flag=0;
		temp= dig_value[3];
		dig_value[3]=dig_value[2];
		dig_value[2]=dig_value[1];
		dig_value[1]=dig_value[0];
		dig_value[0]=temp;
	}
}
