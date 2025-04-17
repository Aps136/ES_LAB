#include <lpc17xx.h>

void EINT3_IRQHandler(void);
unsigned int row, col,i;

// P0.15-P0.18 as COLS (input with interrupt)
// P0.19-P0.20 as ROWS (output)
// P0.4-P0.11 as LEDs

unsigned long int led[] = {1<<4, 1<<5, 1<<6, 1<<7, 1<<8, 1<<10, 1<<11};

int main(void)
{
    // Make rows (P0.19-P0.20) and LEDs (P0.4-P0.11) as output
    LPC_GPIO0->FIODIR = (0x3<<19) | (0xFF<<4);
	
		LPC_GPIO0->FIOSET = 3<19;//SET ROWS AS HIGH INORDER FOR COLS TO GIVE OUTPUT
	
    // Enable rising edge interrupt on cols P0.15 - P0.18
    LPC_GPIOINT->IO0IntEnR = 0xF<<15;

    // Enable interrupt in NVIC
    NVIC_EnableIRQ(EINT3_IRQn);

    while(1); // Infinite loop
}

void EINT3_IRQHandler(void)
{
    unsigned int temp;
    temp = LPC_GPIOINT->IO0IntStatR;

    // Detect which column triggered
    if (temp & (1<<15)) col = 0;
    else if (temp & (1<<16)) col = 1;
    else if (temp & (1<<17)) col = 2;
    else if (temp & (1<<18)) col = 3;
    else return; // No valid column

    // Clear interrupt flag
    LPC_GPIOINT->IO0IntClr = 0xF<<15;

    // Scan rows
    for (row = 0; row < 2; row++)
    {
        // Clear both rows first
        LPC_GPIO0->FIOCLR = 0x3<<19;

        // Set one row high
        LPC_GPIO0->FIOSET = 1<<(row+19);

        
        for (i=0; i<10000; i++);

        // Read the row input back
        if ((LPC_GPIO0->FIOPIN & (0x3<<19)) != 0)
        {
						LPC_GPIO0->FIOCLR = 0xFF0;
            LPC_GPIO0->FIOSET = led[2*row + col]; // Light corresponding LED
            
            break;
        }
				
				
    }
		
		LPC_GPIO0->FIOSET = 0x3<<19;
		
}
