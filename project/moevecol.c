#include <LPC17xx.h>

void delay(void);
void EINT3_IRQHandler(void);

unsigned int row, col, i;
unsigned long int led[] = {
    1 << 4, 1 << 5, 1 << 6, 1 << 7, 1 << 8, 1 << 9, 1 << 10, 1 << 11
};

int main(void)
{
    // Set P0.4 - P0.11 (LEDs) and P0.17 (Buzzer) as output
    LPC_GPIO0->FIODIR |= (0xFF << 4) | (1 << 17);

    // Set keypad rows P2.10 and P2.11 as output
    LPC_GPIO2->FIODIR |= (1 << 10) | (1 << 11);

    // Initially make rows high
    LPC_GPIO2->FIOSET = (1 << 10) | (1 << 11);

    // Set keypad columns P0.23 - P0.25 as input
    LPC_GPIO0->FIODIR &= ~((1 << 23) | (1 << 24) | (1 << 25));

    // Enable rising edge interrupt for P0.23 to P0.25
    LPC_GPIOINT->IO0IntEnR |= (1 << 23) | (1 << 24) | (1 << 25);

    // Enable interrupt in NVIC
    NVIC_EnableIRQ(EINT3_IRQn);

    while (1);
}

void EINT3_IRQHandler(void)
{
    unsigned int temp;
    temp = LPC_GPIOINT->IO0IntStatR;

    // Identify which column triggered
    if (temp & (1 << 23)) col = 0;
    else if (temp & (1 << 24)) col = 1;
    else if (temp & (1 << 25)) col = 2;
    else return; // Invalid interrupt

    // Clear interrupt flags
    LPC_GPIOINT->IO0IntClr |= (1 << 23) | (1 << 24) | (1 << 25);

    for (row = 0; row < 2; row++)
    {
        // Clear all rows
        LPC_GPIO2->FIOCLR = (1 << 10) | (1 << 11);

        // Set one row high
        LPC_GPIO2->FIOSET = (1 << (10 + row));

        // Small delay for signal to settle
        for (i = 0; i < 1000; i++);

        // Check if column still high
        if ((LPC_GPIO0->FIOPIN >> (23 + col)) & 1)
        {
            LPC_GPIO0->FIOCLR = 0xFF0;                      // Turn off all LEDs
            LPC_GPIO0->FIOSET = led[row * 3 + col];         // Turn on correct LED

            if (row == 0 && col == 2)                       // YES key
                LPC_GPIO0->FIOSET = (1 << 17);              // Turn ON buzzer
            else if (row == 1 && col == 2)                  // NO key
                LPC_GPIO0->FIOCLR = (1 << 17);              // Turn OFF buzzer

            break;
        }
    }

    // Reset all rows high again
    LPC_GPIO2->FIOSET = (1 << 10) | (1 << 11);
}

void delay(void)
{
    for (i = 0; i < 100000; i++);
}
