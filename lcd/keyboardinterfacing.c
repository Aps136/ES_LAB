#include <LPC17xx.h>

#define RS_CTRL 0x08000000   //P0.27
#define EN_CTRL 0x10000000   //P0.28
#define DT_CTRL 0x07800000   //P0.23–26

unsigned long int i, temp1, temp2;
unsigned char msg[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
unsigned char row, col, key;
unsigned int r,r1;
unsigned char scan_code[4][4] = {
  {0,1,2,3},
  {4,5,6,7},
  {8,9,10,11},
  {12,13,14,15}
};

unsigned long int init_command[] = {0x30,0x30,0x30,0x20,0x28,0x0C,0x06,0x01,0x80};
unsigned char flag1 = 0, flag2 = 0;

void delay_lcd(unsigned int r1){
	for(r=0;r<r1;r++);
}

void port_write(void){
	LPC_GPIO0->FIOPIN = temp2;
	if(flag1 == 0){
		LPC_GPIO0->FIOCLR = RS_CTRL;
	} else {
		LPC_GPIO0->FIOSET = RS_CTRL;
	}
	LPC_GPIO0->FIOSET = EN_CTRL;
	delay_lcd(25000);
	LPC_GPIO0->FIOCLR = EN_CTRL;
	delay_lcd(500000);
}

void lcd_write(void){
	flag2 = (flag1==1)?0:((temp1==0x30)||(temp1==0x20))?1:0;

	temp2 = temp1 & 0xF0;
	temp2 = temp2 << 19;
	port_write();

	if(!flag2){
		temp2 = temp1 & 0x0F;
		temp2 = temp2 << 23;
		port_write();
	}
}

void scan_keypad(void){
	while(1){
		for(row = 0; row < 4; row++){
			LPC_GPIO2->FIOCLR = 0x00003C00; // Clear P2.10–13
			LPC_GPIO2->FIOSET = (1 << (10 + row)); // Set one row at a time
			for(col = 0; col < 4; col++){
				if(!(LPC_GPIO1->FIOPIN & (1 << (23 + col)))){
					key = scan_code[row][col];
					
					// Display key on LCD
					flag1 = 1;
					temp1 = msg[key];
					lcd_write();

					while(!(LPC_GPIO1->FIOPIN & (1 << (23 + col)))); // Wait till key released
					delay_lcd(1000000);
				}
			}
		}
	}
}

int main(void){
	SystemInit();
	SystemCoreClockUpdate();

	LPC_GPIO0->FIODIR = DT_CTRL | EN_CTRL | RS_CTRL;     // LCD Control
	LPC_GPIO2->FIODIR = 0x00003C00;                      // Row lines as output
	LPC_GPIO1->FIODIR &= ~(0x07800000);                  // Column lines as input

	flag1 = 0;
	for(i = 0; i < 9; i++){
		temp1 = init_command[i];
		lcd_write();
	}

	scan_keypad(); // Begin scanning for keys
}
