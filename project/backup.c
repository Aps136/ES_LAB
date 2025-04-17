#include <LPC17xx.h>
unsigned int row,col,i,led;
unsigned int pressed = 0;
unsigned int winner = 5;
unsigned int keycode[4] = {0x00000010, 0x00000020, 0x00000040, 0x00000080}; // LEDs P0.4 to P0.7

void delay(void) {

    for(i = 0; i < 500000; i++);
}

void EINT3_IRQHandler(void) {
    unsigned long temp = LPC_GPIOINT->IO2IntStatR;
if(temp==1<<10)
col=0;
else if(temp==1<<11)
col=1;
LPC_GPIOINT->IO2IntClr=1<<10|1<<11;
for(row=0;row<2;row++){
if(row==0)
temp=0x01;
else if(row==1)
temp=0x02;
LPC_GPIO1->FIOPIN=temp;

}
}
int main(void) {
    SystemInit();
    SystemCoreClockUpdate();
    LPC_PINCON->PINSEL0 &= 0xFF0000FF;
    LPC_GPIO0->FIODIR |= 0x00000FF0;
    LPC_GPIO0->FIODIR |= (1 << 17);
    LPC_GPIO0->FIOCLR = (1 << 17);
    LPC_GPIO1->FIODIR =0x01800000;
    LPC_GPIOINT->IO2IntEnR = (1 << 10) | (1 << 11); // Rising edge on P2.10 & P2.11
    NVIC_EnableIRQ(EINT3_IRQn);
    while (1) {
        if (pressed && winner >= 0 && winner < 4) {
            LPC_GPIO0->FIOSET = keycode[winner]; // Turn on corresponding LED
            LPC_GPIO0->FIOSET = (1 << 17);
            delay();
            LPC_GPIO0->FIOCLR = (1 << 17);
delay();
delay();
delay();
winner=5;
pressed=0;
        }
    }
}
