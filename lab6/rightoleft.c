#include <LPC17xx.h>
#include <stdio.h>
void delay(void); 
unsigned char count = 0; 
unsigned int LED = 0x00000010;
unsigned int i;

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();
    LPC_PINCON->PINSEL0 = 0x00000000; // Set GPIO
    LPC_GPIO0->FIODIR = 0x00000FF0;   // Set P0.4 to P0.11 as output (8 LEDs)  
    while (1) {
		  for(i=0;i<9;i++){
			LPC_GPIO0->FIOSET=LED;
			LPC_GPIO0->FIOCLR=LED;
			LED<<=1;}	
			LED = 0x00000010;
			
    }
}

void delay(void) {
    for (i = 0; i < 1000000; i++);
}
