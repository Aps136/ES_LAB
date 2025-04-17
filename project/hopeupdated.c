#include <LPC17xx.h>

unsigned int col, i;
unsigned int pressed = 0;
unsigned int winner = 5;

void delay(void) {
    for (i = 0; i < 500000; i++);
}

// Interrupt handler for GPIO interrupts on Port 2 (EINT3)
void EINT3_IRQHandler(void) {
    unsigned long temp = LPC_GPIOINT->IO2IntStatR;

    // Determine which column (P2.10 or P2.11) was pressed
    if (temp & (1 << 10))
        col = 0;
    else if (temp & (1 << 11))
        col = 1;

    LPC_GPIOINT->IO2IntClr = temp; // Clear only the triggered interrupt

    if (!pressed) {
        // Check which key was pressed (simulate row logic)
        if ((LPC_GPIO2->FIOPIN & (1 << 10)) || (LPC_GPIO2->FIOPIN & (1 << 11))) {
            winner = col;   // col = 0 for P2.10, col = 1 for P2.11
            pressed = 1;
        }
    }

    // Turn on corresponding LED if a key was pressed
    if (pressed) {
        if (LPC_GPIO2->FIOPIN & (1 << 10)) {
            LPC_GPIO0->FIOSET = 0x80000000; // Some LED signal
        }
        if (LPC_GPIO2->FIOPIN & (1 << 11)) {
            LPC_GPIO0->FIOSET = 0x40000000; // Another LED signal
        }
    }
}

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    // Configure P0.4–P0.7 as output for winner LEDs
    LPC_PINCON->PINSEL0 &= 0xFF0000FF;
    LPC_GPIO0->FIODIR |= 0x000000F0;

    // Configure P0.17 as output for buzzer
    LPC_GPIO0->FIODIR |= (1 << 17);
    LPC_GPIO0->FIOCLR = (1 << 17); // Turn off buzzer initially

    // Enable GPIO interrupt on rising edge for P2.10 and P2.11
    LPC_GPIOINT->IO2IntEnR = (1 << 10) | (1 << 11);
    NVIC_EnableIRQ(EINT3_IRQn);

    while (1) {
        if (pressed && winner >= 0 && winner < 4) {
            LPC_GPIO0->FIOSET = (1 << (winner + 4)); // Turn on corresponding LED on P0.4–P0.7
            LPC_GPIO0->FIOSET = (1 << 17);           // Turn on buzzer
            delay();
            LPC_GPIO0->FIOCLR = (1 << 17);           // Turn off buzzer

            while (1); // Halt system after one valid input
        }
    }
}
