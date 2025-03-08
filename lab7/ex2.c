#include <LPC17xx.h>

void delay(void); 

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    // Configure P0.4 - P0.11 as output (LEDs)
    LPC_PINCON->PINSEL0 = 0x00000000;
    LPC_GPIO0->FIODIR |= 0x00000FF0; 

    // Configure P2.10 as input (Button)
    LPC_PINCON->PINSEL4 &= ~(3 << 20);  // Set as GPIO
    LPC_GPIO2->FIODIR &= ~(1 << 10);    // Set as input

    unsigned char count = 0;

    while (1) {
        if ((LPC_GPIO2->FIOPIN & (1 << 10)) == 0) { 
            // Button pressed -> Down Counter
            count--;
        } else {
            // Button not pressed -> Up Counter
            count++;
        }

        // Display the counter value on LEDs
        LPC_GPIO0->FIOPIN = (count << 4);

        delay(); 
    }
}

void delay(void) {
    for (volatile unsigned int i = 0; i < 1000000; i++);
}
