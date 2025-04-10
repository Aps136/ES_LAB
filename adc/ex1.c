#include<LPC17xx.h>
#include<stdio.h>
#define Ref_Vtg 3.300
#define Full_Scale 0xFFF//12 bit ADC
unsigned long int init_command[] = {0x30,0x30,0x30,0x20,0x28,0x0c,0x06,0x01,0x80};
unsigned int flag1,temp1,temp2,flag2;
 
#define RS_CTRL 0x08000000 //P0.27, 1<<27
#define EN_CTRL 0x10000000 //P0.28, 1<<28
#define DT_CTRL 0x07800000 //P0.23 to P0.26 data lines, F<<23
void lcd_write(void);
void port_write(void);
void delay_lcd(unsigned int);
void lcd_write(void)
{
flag2 = (flag1 == 1) ? 0 :((temp1 == 0x30) || (temp1 == 0x20)) ? 1 : 0;
temp2 = temp1 & 0xf0;
temp2 = temp2 << 19;
port_write();
if (flag2==0)
{
temp2 = temp1 & 0x0f; //26-4+1
temp2 = temp2 << 23;
port_write();
}
}
 
void port_write(void)
{
LPC_GPIO0->FIOPIN = temp2;
if (flag1 == 0)
LPC_GPIO0->FIOCLR = RS_CTRL;
else
LPC_GPIO0->FIOSET = RS_CTRL;
LPC_GPIO0->FIOSET = EN_CTRL;
delay_lcd(1000000);
LPC_GPIO0->FIOCLR = EN_CTRL;
delay_lcd(1000000);
}
void delay_lcd(unsigned int r1)
{
unsigned int r;
for(r=0;r<r1;r++);
return;
}
 
int main(void)
{
unsigned long adc_temp;
unsigned int i;
float in_vtg;
unsigned char vtg[7], dval[7];
unsigned char Msg3[] = {"ANALOG IP:"};
unsigned char Msg4[] = {"ADC OUTPUT:"};
SystemInit();
SystemCoreClockUpdate();
LPC_GPIO0->FIODIR = DT_CTRL | RS_CTRL | EN_CTRL;
LPC_PINCON->PINSEL3 |= 3<<28; //P1.31 as AD0.5
LPC_SC->PCONP |= (1<<12);//enable the peripheral ADC
flag1 =0;//Command
for (i=0; i<9;i++)
{ temp1 = init_command[i];
lcd_write(); }//send Init commands to LCD }
flag1=1;//Data
i =0;
while (Msg3[i++] != '\0')
{ temp1 = Msg3[i-1];
lcd_write();}//Send data bytes}
      flag1=0; //Command
temp1 = 0xC0;//Cursor at beginning of second line
lcd_write();
flag1=1;
i =0;
while (Msg4[i++] != '\0')
{
temp1 = Msg4[i-1];
lcd_write();//Send data bytes
}
while(1)
{
LPC_ADC->ADCR = (1<<4)|(1<<21)|(1<<24);//ADC0.5, start conversion and operational
while(((adc_temp=LPC_ADC->ADGDR) & (1<<31)) == 0);
adc_temp = LPC_ADC->ADGDR;
adc_temp >>= 4;
adc_temp &= 0x00000FFF; //12 bit ADC
in_vtg = (((float)adc_temp * (float)Ref_Vtg))/((float)Full_Scale); //calculating input analog voltage
sprintf(vtg,"%3.2fV",in_vtg); //convert the readings into string to display on LCD
sprintf(dval,"%x",adc_temp);
flag1=0;;
temp1 = 0x8A;
lcd_write();
flag1=1;
i =0;
while (vtg[i++] != '\0')
{
temp1 = vtg[i-1];
lcd_write();
}
flag1=0;
temp1 = 0xCB;
lcd_write();
flag1=1;
i =0;
while (dval[i++] != '\0')
{
temp1 = dval[i-1];
lcd_write();//Send data bytes
}
for(i=0;i<7;i++)
vtg[i] = dval[i] = 0;
}
}
