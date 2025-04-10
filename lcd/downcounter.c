#include <LPC17xx.h>
#define RS_CTRL 0x08000000// p0.27
#define EN_CTRL 0x10000000 //p0.28
#define DT_CTRL 0x07800000  //p0 23 to 26
unsigned int temp1=0, temp2=0, i,j,k;
unsigned char flag1=0, flag2=0;
unsigned char msg[]={"COUNTER"};
unsigned long int init_command[]={0x30,0x30,0x30,0x20,0x28,0x0C,0x06,0x01,0x80};
unsigned char counter='9';
unsigned char counter1='9';
unsigned char counter2='9';


void lcd_write(void);
void port_write(void);
void delay_lcd(unsigned int r1);

int main(void){
		SystemInit();
		SystemCoreClockUpdate();
		LPC_GPIO0->FIODIR= DT_CTRL | EN_CTRL | RS_CTRL;
		flag1=0;
	for(i=0;i<9;i++){
		temp1=init_command[i];
		lcd_write();
	}		
		flag1=1;
		i=0;
	while(msg[i]!='\0'){
		temp1=msg[i];
		lcd_write();
		
	}
	while(1){
		
		temp1=0xC2;
		flag1=0;
		lcd_write();
		flag1=1;
		temp1=counter;
		lcd_write();
		
		
		temp1=0xC1;
		flag1=0;
		lcd_write();
		flag1=1;
		temp1=counter1;
		lcd_write();
		
		
		temp1=0xC0;
		flag1=0;
		lcd_write();
		flag1=1;
		temp1=counter2;
		lcd_write();
		
		delay_lcd(500000);
		counter++;
		if(counter>'0'){
			counter--;	
			
		}else {
				counter='9';
			if(counter1>'0'){
				counter1--;
			}else{
				counter1='9';
				if(counter2>'0'){
					counter2--;
					}else{
						counter2='9';
					}
			}
		}
		
		
}}
void lcd_write(void){
	flag2= (flag1==1)?0:((temp1==0x30)||(temp2==0X20)?1:0);
	temp2= temp1&0XF0;
	temp2<<=19;
	port_write();
	if(flag2==0){
		temp2= temp1&0X0F;
		temp2<<=23;
		port_write();
		
	}
}
void port_write(void){
	LPC_GPIO0->FIOPIN = temp2;
	if(flag1==0){
		LPC_GPIO0->FIOCLR = RS_CTRL;
	}else{
		LPC_GPIO0->FIOSET = RS_CTRL;
	}
	LPC_GPIO0->FIOSET = EN_CTRL;
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = EN_CTRL;
	delay_lcd(5000);
}
void delay_lcd(unsigned int r1){
	unsigned int r;
	for(r=0;r<r1;r++);
}
