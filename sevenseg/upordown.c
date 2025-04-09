#include <LPC17xx.h>
#include <stdio.h>
#define FIRST_SEG (0 << 23)
#define SECOND_SEG (1 << 23)
#define THIRD_SEG (2 << 23)
#define FOURTH_SEG (3 << 23)
unsigned int dig_count=0;//current dig count
unsigned int digit_value[4]={0,0,0,0};//4 digit counter values
unsigned int select_segment[4] ={FIRST_SEG, SECOND_SEG, THIRD_SEG, FOURTH_SEG};
unsigned char seven_seg[10] ={0x3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F};
unsigned long int i=0;
unsigned int count=0, flag=0;
#define N 2000

void display(void);

void delayup(void);
void delayd(void);

int main(void){
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL0=0;
	LPC_PINCON->PINSEL3=0;
	LPC_GPIO0->FIODIR=0X00000FF0;
	LPC_GPIO1->FIODIR = 0X07800000;
	while(1){
		if(!(LPC_GPIO1->FIOPIN & 1<<12)){
			delayup();
		}
		else{
			delayd();
		}
		
		dig_count+=1;
		if(dig_count==4){
			dig_count=0;
			
		}
		display();
	
		
	}
}
void display(void){
	LPC_GPIO1->FIOPIN= select_segment[dig_count];
	LPC_GPIO1->FIOPIN= seven_seg[digit_value[dig_count]]<<4;
	//for(i=0;i<500;i++);
	LPC_GPIO0->FIOCLR = 0X00000FF0;//clear display before next update
		
}
void delayd(void){
	for(i=0;i<5;i++){
		if(count==N){
			flag=0xFF;
			count=0;
		}else{
			count+=1;
			if(flag==0xFF){
				flag=0;
				
				if(digit_value[0]==0){
					digit_value[0]=9;
					
					if(digit_value[1]==0){
						digit_value[1]=9;
					
						if(digit_value[2]==0){
							digit_value[2]=9;
							
						if(digit_value[3]==0){
							digit_value[3]=9;
							
						}else digit_value[3]--;
					}else digit_value[2]--;
				}else digit_value[1]--;
			}else digit_value[0]--;
		}
	}
}
	}
void delayup(void){
	for(i=0;i<5;i++){
		if(count==N){
			flag=0xFF;
			count=0;
		}else{
			count+=1;
			if(flag==0xFF){
				flag=0;
				digit_value[0]+=1;
				if(digit_value[0]==10){
					digit_value[0]=0;
					digit_value[1]+=1;
					if(digit_value[1]==10){
						digit_value[1]=0;
						digit_value[2]+=1;
						if(digit_value[2]==10){
							digit_value[2]=0;
							digit_value[3]+=1;
						if(digit_value[3]==10){
							digit_value[3]=0;
							
						}
					}
				}
			}
		}
	}
}}
