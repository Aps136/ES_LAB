#include <LPC17xx.H>
void pwm_init(void);
void PWM1_IRQHandler(void);
unsigned long int i;
unsigned char flag,flag1;
unsigned int ans[4]={0,1,2,3};
unsigned int col,row=0,flag3,temp,temp2;
void scan(void);

int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();
    pwm_init();
    LPC_PINCON->PINSEL0=0;
    LPC_GPIO0->FIODIR|=1<<8;
    while(1)
    {
        temp=1<<8;
        LPC_GPIO0->FIOPIN=temp;
        flag3=0;
        scan();
        if(flag3==1)
        {
            if(col==0)
                temp2=3000;
            if(col==1)
                temp2=7500;
            if(col==2)
                temp2=15000;
            if(col==3)
                temp2=22500;
            for(i=0;i<10000;i++);
        }
    }//end of while
}//end of main
    void pwm_init(void)
{
    LPC_SC->PCONP |= (1<<6); //PWM1 is powered
    LPC_PINCON->PINSEL3 &= ~(0x0000C000); //cleared if any other
    //functions are enabled
    LPC_PINCON->PINSEL3 |= 0x00008000; //pwm1.4 is selected for the pin
    //P1.23
    //LPC_PWM1->PR = 0x00000000; //Count frequency : Fpclk
    LPC_PWM1->PCR = 0x00001000; //select PWM1 single edge
    LPC_PWM1->MCR = 0x00000003; //Reset and interrupt on PWMMR0
    LPC_PWM1->MR0 = 30000; //setup match register 0 count
    LPC_PWM1->MR4 = 0x00000100; //setup match register MR1
    LPC_PWM1->LER = 0x000000FF; //enable shadow copy register
    LPC_PWM1->TCR = 0x00000002; //RESET COUNTER AND PRESCALER
    LPC_PWM1->TCR = 0x00000009; //enable PWM and counter
    NVIC_EnableIRQ(PWM1_IRQn);
    return;
}
void PWM1_IRQHandler(void)
{
    LPC_PWM1->IR = 0xff; //clear the interrupts
    if(flag3 == 1)
    {
    LPC_PWM1->MR4 =temp2;
    LPC_PWM1->LER = 0x000000FF;
}
}
void scan(void)
{
    unsigned long temp3;
    temp3=LPC_GPIO0->FIOPIN;
    temp3&=0x000000F0;
    if(temp3!=0x000000000)
    {
        flag3=1;
        if(temp3==1<<4)
            col=0;
        else if(temp3==1<<5)
            col=1;
        else if(temp3==1<<6)
            col=2;
        else if(temp3==1<<7)
            col=3;
    }
}

