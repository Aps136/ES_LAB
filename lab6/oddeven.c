#include <LPC17xx.h>
#include <stdio.h>
void delay(void); 
unsigned char count_odd = 0; 
unsigned char count_even = 15; 
unsigned int i;

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();
    LPC_PINCON->PINSEL0 = 0x00000000; // Set GPIO
    LPC_GPIO0->FIODIR = 0x00000FF0;   // Set P0.4 to P0.11 as output (8 LEDs)  
    while (1) {
			LPC_GPIO0->FIOCLR=0x00000FF0;
		  LPC_GPIO0->FIOSET=((count_odd&0x0F)<<4)&0xAA;
			LPC_GPIO0->FIOSET =((count_even&0x0f)<<4)&0X55;
			count_odd++;
			count_even--;
			if(count_odd>15) count_odd=0;
			if(count_even>15) count_even=15;
			
    }
}

void delay(void) {
    for (i = 0; i < 1000000; i++);
}
