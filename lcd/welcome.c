#include <LPC17xx.h>

#define RS_CTRL 0x08000000   //p0.27
#define EN_CTRL 0x10000000 //p0.28
#define DT_CTRL 0x07800000 //23 to 26

unsigned long int temp1,temp2=0,i,j;
unsigned char flag1=0, flag2=0;
unsigned char msg[]={"WELCOME"};
void lcd_write(void);
void port_write(void);
void delay_lcd(unsigned int);
unsigned long int init_command[]={0x30,0x30,0x30,0x20,0x28, 0x0c, 0x06, 0x01, 0x80};

int main(void){
	SystemInit();
	SystemCoreClockUpdate();
	LPC_GPIO0->FIODIR= DT_CTRL|EN_CTRL|RS_CTRL;
	flag1=0; //command
	for(i=0;i<9;i++){
			temp1= init_command[i];
		lcd_write();
	}
	flag1=1;  //data
	
	while(msg[i++]!='\0'){
		temp1=msg[i];
		lcd_write();


	}
	while(1);
	
}
void lcd_write(void){

	flag2= (flag1==1)?0:((temp1==0x30)||(temp1==0x20))?1:0;
	temp2= temp1&0XF0;//extract upper nibble, align w data lines
	temp2= temp2<<19;
	port_write();
	if(!flag2){
		temp2= temp1&0X0F;//extract lower niblle, align w data lines
		temp2= temp2<<23;
		port_write();
	}
}
void port_write(void){
	LPC_GPIO0->FIOPIN = temp2;//outputs the current niblle to port 0 
	if(flag1==0){//command mode
		LPC_GPIO0->FIOCLR = RS_CTRL;//clear the screen
		
	}else{
		LPC_GPIO0->FIOSET = RS_CTRL;//set RS high to indicate data
		LPC_GPIO0->FIOSET = EN_CTRL;//pulse enable line to latch the nibble
		delay_lcd(25);
		LPC_GPIO0->FIOCLR = EN_CTRL;	//complete enable pulse
		
	}delay_lcd(5000);
			
	
}
void delay_lcd(unsigned int r1){
	unsigned int r;
	for(r=0;r<r1;r++);
	return;
	
}
