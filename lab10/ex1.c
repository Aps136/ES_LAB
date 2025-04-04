#include<LPC17xx.h>
#include<stdio.h>
#include<string.h>
#define Ref_Vtg 3.300
#define Full_Scale 0xFFF // 12 bit ADC
#define RS_CTRL  0x08000000  // P0.27
#define EN_CTRL  0x10000000  // P0.28
#define DT_CTRL  0x07800000  // P0.23 to P0.26 data lines
unsigned long int init_command[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0C, 0x06, 0x01, 0x80};
unsigned long int temp1 = 0, temp2 = 0, i, j;
unsigned char flag1 = 0, flag2 = 0;


void lcd_write(void);
void port_write(void);
void delay_lcd(unsigned int);

int main(void)
{
   unsigned long temp4;
unsigned long temp5;

    unsigned long adc_temp;
    unsigned int i;
    float in_vtg;
    unsigned char vtg[7], dval[7];
    unsigned char Msg3[] = {"ANALOG IP:"};
    unsigned char Msg4[] = {"ADC OUTPUT:"};

    SystemInit();
    SystemCoreClockUpdate();
LPC_GPIO0->FIODIR = DT_CTRL | RS_CTRL | EN_CTRL; // Config output
    //lcd_init(); // Initialize LCD
    LPC_PINCON->PINSEL3 |= (3<<30)| (3<<28) ; // P1.31 as AD0.5
LPC_SC->PCONP |= (1<<12); // Enable the peripheral ADC
flag1 = 0; // Command
    for(i = 0; i < 9; i++)
    {
        temp1 = init_command[i];
        lcd_write(); // Send Init commands to LCD
    }

    flag1 = 1; // Data
    i = 0;
    while (Msg3[i++] != '\0')
    {
        temp1 = Msg3[i-1];
        lcd_write(); // Send data bytes
    }

    flag1 = 0; // Command
    temp1 = 0xC0; // Cursor at beginning of second line
    lcd_write();
    flag1 = 1;
    i = 0;
    while (Msg4[i++] != '\0')
    {
        temp1 = Msg4[i-1];
        lcd_write(); // Send data bytes
    }

    while(1)
    {
        LPC_ADC->ADCR = (1<<5) | (1<<21) | (1<<24); // ADC0.5, start conversion and operational
        while(((temp4 = LPC_ADC->ADDR5) & (1<<31)) == 0); // Wait for conversion complete
for(i=0;i<2000;i++);
       
        temp4 = LPC_ADC->ADDR5;
        temp4 >>= 4;
        temp4 &= 0x00000FFF; // 12-bit ADC


/*LPC_ADC->ADCR = (1<<5)|(1<<21)|(1<<24);// //ADC0.5, start conversion and operational
for(i=0;i<2000;i++); //delay for conversion
while(((temp5=LPC_ADC->ADDR5) & (1<<31)) == 0); //wait till 'done' bit is 1, indicates conversion complete
temp5 = LPC_ADC->ADDR5;
temp5 >>= 4;
temp5 &= 0x00000FFF;*/
adc_temp= temp4;

        in_vtg = (((float)adc_temp * (float)Ref_Vtg) / ((float)Full_Scale)); // Calculating input analog voltage
        sprintf(vtg, "%3.2fV", in_vtg); // Convert the readings into string to display on LCD
        sprintf(dval, "%0x", adc_temp);

        flag1 = 0;
        temp1 = 0x8A;
        lcd_write();
        flag1 = 1;
        i = 0;
        while (vtg[i++] != '\0')
        {
            temp1 = vtg[i-1];
            lcd_write(); // Send data bytes
        }

        flag1 = 0;
        temp1 = 0xCB;
        lcd_write();
        flag1 = 1;
        i = 0;
        while (dval[i++] != '\0')
        {
            temp1 = dval[i-1];
            lcd_write(); // Send data bytes
        }

        for(i = 0; i < 7; i++)
        {
            vtg[i] = dval[i] = 0;
        }
    }
}
void lcd_write(void)
{
    flag2 = (flag1 == 1) ? 0 : ((temp1 == 0x30) || (temp1 == 0x20)) ? 1 : 0; // If command is 0x30 (Working in 8-bit mode initially)
   
    temp2 = temp1 & 0xF0;
    temp2 = temp2 << 19; // Data lines from 23 to 26
    port_write(); // Output the higher digit on P0.26-P0.23
   
    if (!flag2) // Other than command 0x30, send the lower 4-bit also
    {
        temp2 = temp1 & 0x0F;
        temp2 = temp2 << 23;
        port_write(); // Output the lower digit on P0.26-P0.23
    }
}

void port_write(void)
{
    LPC_GPIO0->FIOPIN = temp2;
   
    if (flag1 == 0)
        LPC_GPIO0->FIOCLR = RS_CTRL; // Select command register
    else
        LPC_GPIO0->FIOSET = RS_CTRL; // Select data register
   
    LPC_GPIO0->FIOSET = EN_CTRL; // Apply edge on Enable
    delay_lcd(25);
    LPC_GPIO0->FIOCLR = EN_CTRL;
    delay_lcd(500000);
}

void delay_lcd(unsigned int r1)
{
    unsigned int r;
    for (r = 0; r < r1; r++);
    return;
}
