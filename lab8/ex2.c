#include <LPC17xx.h>

#define FIRST_SEG  0xF87FFFFF
#define SECOND_SEG 0xF8FFFFFF
#define THIRD_SEG  0xF97FFFFF
#define FOURTH_SEG 0xF9FFFFFF

unsigned char ring_counter[4] = {0x01, 0x02, 0x04, 0x08}; // Ring counter sequence
unsigned int dig1 = 0x00, dig2 = 0x00, dig3 = 0x00, dig4 = 0x00;
unsigned char array_bin[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F}; // 7-segment codes
unsigned int dig_count = 0x00;
unsigned long int temp2 = 0x00000000;
unsigned int i = 0;

void delay(void);
void Display(void);

void delay(void) {
    for (i = 0; i < 5000000; i++);  // Simple delay for ~1 second
}

void Display(void) {
    if (dig_count == 0x01)
        LPC_GPIO1->FIOPIN = FIRST_SEG, temp2 = array_bin[dig1];
    else if (dig_count == 0x02)
        LPC_GPIO1->FIOPIN = SECOND_SEG, temp2 = array_bin[dig2];
    else if (dig_count == 0x03)
        LPC_GPIO1->FIOPIN = THIRD_SEG, temp2 = array_bin[dig3];
    else if (dig_count == 0x04)
        LPC_GPIO1->FIOPIN = FOURTH_SEG, temp2 = array_bin[dig4];

    temp2 = temp2 << 4;
    LPC_GPIO0->FIOPIN = temp2;
    for (i = 0; i < 500; i++);
    LPC_GPIO0->FIOCLR = 0x00000FF0;
}

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 &= 0xFF0000FF;
    LPC_PINCON->PINSEL3 &= 0xFFC03FFF;
    LPC_GPIO0->FIODIR |= 0x00000FF0;
    LPC_GPIO1->FIODIR |= 0x07800000;

    unsigned int position = 0;

    while (1) {
        dig_count += 1;
        if (dig_count == 0x05) {
            dig_count = 0x00;
        }

        dig1 = (ring_counter[position] & 0x01);
        dig2 = (ring_counter[position] & 0x02) >> 1;
        dig3 = (ring_counter[position] & 0x04) >> 2;
        dig4 = (ring_counter[position] & 0x08) >> 3;

        position = (position + 1) % 4;
        delay();  // Simple delay function
        Display();
    }
}
