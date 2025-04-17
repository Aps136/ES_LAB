#include <LPC17xx.h>
unsigned int row,col,i,led;
unsigned int pressed = 0;
unsigned int winner = 5;
//unsigned int keycode[4] = {0x00000010, 0x00000020, 0x00000040, 0x00000080};

void delay(void) {

    for(i = 0; i < 500000; i++);
}

void EINT3_IRQHandler(void) {
    unsigned long temp = LPC_GPIOINT->IO2IntStatR;
if(temp==1<<10)
col=0;
else if(temp==1<<11)
col=1;
else if(temp==1<<12)
col=2;
LPC_GPIOINT->IO2IntClr=1<<10|1<<11;
    if (!pressed) {
        for (row = 0; row < 2; row++) {
if((LPC_GPIO2->FIOPIN&(1<<10)|(1<<11))!=0){
winner = row * 2 + col;
pressed = 1;
break;
}
        }
    }
if (pressed) {
if(LPC_GPIO2->FIOPIN&(1<<10)!=0){
LPC_GPIO0->FIOSET=0x80000000;
}
if(LPC_GPIO2->FIOPIN&(1<<11)!=0){
LPC_GPIO0->FIOSET=0x40000000;
}
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
          LPC_GPIO0->FIOSET =(winner+1)<<4;  
//LPC_GPIO0->FIOSET = keycode[winner]; // Turn on corresponding LED
            LPC_GPIO0->FIOSET = (1 << 17);
            delay();
            LPC_GPIO0->FIOCLR = (1 << 17);
LPC_GPIO1->FIODIR =0x03800000;
while(1);
winner=5;
pressed=0;
        }
    }
}
