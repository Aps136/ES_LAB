#include <LPC17xx.h>
#include <stdio.h>

#define FIRST_SEG 0xF87FFFFF
#define SECOND_SEG 0xF8FFFFFF
#define THIRD_SEG 0xF97FFFFF
#define FOURTH_SEG 0xF9FFFFFF
#define DISABLE_ALL 0xFA7FFFFF

unsigned int dig1 = 0x00, dig2 = 0x00, dig3 = 0x00, dig4 = 0x00;
unsigned char array_dec[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
unsigned char one_sec_flg = 0x00;
unsigned long int temp2 = 0x00000000;
unsigned int dig_count = 0x00;
unsigned int temp1 = 0x00;
unsigned int i = 0;

void delay(void);
void Display(void);
void TIMER0_IRQHandler(void);

// Timer0 Interrupt Handler (1-second delay)
void TIMER0_IRQHandler(void) {
    LPC_TIM0->IR = 1; // Clear interrupt flag
    one_sec_flg = 0xFF; // Set flag for 1-second event
}

void init_timer(void) {
    LPC_TIM0->MR0 = 25000000; // Match value for 1 second (25MHz clock)
    LPC_TIM0->MCR = 3; // Interrupt and reset on MR0 match
    LPC_TIM0->TCR = 1; // Enable Timer0
    NVIC_EnableIRQ(TIMER0_IRQn); // Enable Timer0 interrupt
}

// Display function for 7-segment display
void Display(void) {
    if (dig_count == 0x01)
        temp1 = dig1, LPC_GPIO1->FIOPIN = FIRST_SEG;
    else if (dig_count == 0x02)
        temp1 = dig2, LPC_GPIO1->FIOPIN = SECOND_SEG;
    else if (dig_count == 0x03)
        temp1 = dig3, LPC_GPIO1->FIOPIN = THIRD_SEG;
    else if (dig_count == 0x04)
        temp1 = dig4, LPC_GPIO1->FIOPIN = FOURTH_SEG;

    temp1 &= 0x0F;
    temp2 = array_dec[temp1] << 4; // Convert to 7-segment encoding
    LPC_GPIO0->FIOPIN = temp2;
    for (i = 0; i < 500; i++);
    LPC_GPIO0->FIOCLR = 0x00000FF0; // Clear display
}

// Main function
int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    // Configure 7-segment pins
    LPC_PINCON->PINSEL0 &= 0xFF0000FF; // P0.4 to P0.11 as GPIO
    LPC_PINCON->PINSEL3 &= 0xFFC03FFF; // P1.23 to P1.26 as GPIO
    LPC_GPIO0->FIODIR |= 0x00000FF0; // P0.4 to P0.11 as output
    LPC_GPIO1->FIODIR |= 0x07800000; // P1.23 to P1.26 as output

    // Configure Switch (P2.10) as input
    LPC_PINCON->PINSEL4 &= ~(3 << 20); // Set as GPIO
    LPC_GPIO2->FIODIR &= ~(1 << 10); // Set as input

    init_timer(); // Initialize Timer0

    while (1) {
        dig_count += 1;
        if (dig_count == 0x05) {
            dig_count = 0x00;
        }

        if (one_sec_flg == 0xFF) {
            one_sec_flg = 0x00; // Clear flag

            // Check switch state for up/down counting
            if ((LPC_GPIO2->FIOPIN & (1 << 10)) == 0) {
                // Switch Pressed -> Down Counter
                if (dig1 == 0) {
                    dig1 = 9;
                    if (dig2 == 0) {
                        dig2 = 9;
                        if (dig3 == 0) {
                            dig3 = 9;
                            if (dig4 == 0) {
                                dig4 = 9;
                            } else {
                                dig4--;
                            }
                        } else {
                            dig3--;
                        }
                    } else {
                        dig2--;
                    }
                } else {
                    dig1--;
                }
            } else {
                // Switch Not Pressed -> Up Counter
                dig1 += 1;
                if (dig1 == 10) {
                    dig1 = 0;
                    dig2 += 1;
                    if (dig2 == 10) {
                        dig2 = 0;
                        dig3 += 1;
                        if (dig3 == 10) {
                            dig3 = 0;
                            dig4 += 1;
                            if (dig4 == 10) {
                                dig4 = 0;
                            }
                        }
                    }
                }
            }
        }
        Display();
    }
}
