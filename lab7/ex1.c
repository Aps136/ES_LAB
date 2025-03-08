#include <LPC17xx.h>
void delay(void); 

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();
    LPC_PINCON->PINSEL0 = 0x00000000; // Set GPIO
    LPC_GPIO0->FIODIR = 0x00000FF0;   // Set P0.4 to P0.11 as output (8 LEDs)

    unsigned char count = 0; 

    while (1) {
        LPC_GPIO0->FIOPIN = (count << 4); // Display counter value on LEDs
        delay(); 
        count++;  // Increment counter (0 to 255)
    }
}

void delay(void) {
    for (int i = 0; i < 1000000; i++);
}
