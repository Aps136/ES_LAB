#include <LPC17xx.h>
#include <stdio.h>
void delay(void); 
unsigned char count = 0; 
unsigned int i;
unsigned char count_up =1;
unsigned char count_down=15;

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();
    LPC_PINCON->PINSEL0 = 0x00000000; // Set GPIO
    LPC_GPIO0->FIODIR = 0x00000FF0;   // Set P0.4 to P0.11 as output (8 LEDs)  
    while (1) {
			LPC_GPIO0->FIOCLR=0x00000FF0;
			LPC_GPIO0->FIOSET=(count_up<<4)|(count_down<<8);
			count_up++;
			count_down--;
			if(count_up>15)count_up=1;
			if(count_down<1)count_down=15;
			
    }
}

void delay(void) {
    for (i = 0; i < 1000000; i++);
}
