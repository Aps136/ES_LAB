#include<LPC17xx.h>
#include <stdio.h>
#define FIRST_SEG 0xF87FFFFF
#define SECOND_SEG 0xF8FFFFFF
#define THIRD_SEG 0xF97FFFFF
#define FOURTH_SEG 0xF9FFFFFF
#define DISABLE_ALL 0xFA7FFFFF

unsigned int dig1 = 0x9, dig2 = 0x9, dig3 = 0x9, dig4 = 0x9;
unsigned int digu1 = 0x00, digu2 = 0x00, digu3 = 0x00, digu4 = 0x00;
unsigned int twenty_count = 0x00, dig_count = 0x00, temp1 = 0x00,dig_countu=0x00;
unsigned char array_dec[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
unsigned char tmr0_flg = 0x00, one_sec_flg = 0x00;
unsigned long int temp2 = 0x00000000, i = 0;
unsigned int temp3 = 0x00;
unsigned int UP=1;

void Delay(void);
void Display(void);
void UDisplay(void);
void down(void);
void up(void);

int main(void){
      SystemInit();
      SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 &= 0xFF0000FF; // P0.4 to P0.11
    // GPIO data lines
    LPC_PINCON->PINSEL3 &= 0xFFC03FFF; // P1.23 to P1.26
    // GPIO enable lines
    LPC_GPIO0->FIODIR |= 0x00000FF0; // P0.4 to P0.11 output
    LPC_GPIO1->FIODIR |= 0x07800000; // P1.23 to P1.26 output
      while(1){
            
                        

                        
                  if(UP==1)
                  {
                        up();
                  }
                  else
                  {down();}
            
            
      }}
      
void Delay(void)
{
    unsigned int i;
    for (i = 0; i < 1000; i++);
    if (twenty_count == 1000) // multiplied by 500x2msec for 1 Sec
    {
        one_sec_flg = 0xFF;
        twenty_count = 0x00;
    }
    else
    {
        twenty_count += 1;
    }
}

void down(void)
{
            
                  Delay();
        dig_count += 1;
        if (dig_count == 0x05)
        {
            dig_count = 0x00;
        }

        if (one_sec_flg == 0xFF)
        {
            one_sec_flg = 0x00;
            dig1 -= 1;
            if (dig1 == 0x0)
            {
                dig1 = 0X9;
                dig2 -= 1;
                if (dig2 == 0x0)
                {
                    dig2 = 0X9;
                    dig3 -= 1;
                    if (dig3 == 0x0)
                    {
                        dig3 = 0X9;
                        dig4 -= 1;
                        if (dig4 == 0x0)
                        {
                            dig4 = 0X9;
                        } // end of dig4
                    } // end of dig3
                } // end of dig2
            } // end of dig1
        } // end of one_sec if

        Display();
}
void up(void)
{
      Delay();
      dig_countu +=1;
      if(dig_countu == 0x05)
      { dig_countu = 0x00;
      }
      if(one_sec_flg == 0xFF)
      {
      one_sec_flg = 0x00;
      digu1 +=1;
      if(digu1 == 0x0A)
      {
      digu1 = 0;
      digu2 +=1;
      if(digu2 == 0x0A)
      {
      digu2 = 0;
      digu3+=1;
      if(digu3 == 0x0A)
      {
      digu3 = 0;
      digu4 += 1;
      if(digu4 == 0x0A)
      {
      digu4 = 0;
      } //end of dig4
      } //end of dig3
      } //end of dig2
      } //end of dig1
      } //end of one_sec if
      UDisplay();
      } //end of while(1)


void Display(void) // To Display on 7-segments
{
    if (dig_count == 0x01) // For Segment U8
    {
        temp1 = dig1;
        LPC_GPIO1->FIOPIN = FIRST_SEG;
    }
    else if (dig_count == 0x02) // For Segment U9
    {
        temp1 = dig2;
        LPC_GPIO1->FIOPIN = SECOND_SEG;
    }
    else if (dig_count == 0x03) // For Segment U10
    {
        temp1 = dig3;
        LPC_GPIO1->FIOPIN = THIRD_SEG;
    }
    else if (dig_count == 0x04) // For Segment U11
    {
        temp1 = dig4;
        LPC_GPIO1->FIOPIN = FOURTH_SEG;
    }

    temp1 &= 0x0F;
    temp2 = array_dec[temp1]; // Decoding to 7-segment
    temp2 = temp2 << 4;
    LPC_GPIO0->FIOPIN = temp2; // Taking Data Lines for 7-Seg

    for (i = 0; i < 500; i++);
    LPC_GPIO0->FIOCLR = 0x00000FF0;
    // LPC_GPIO1->FIOPIN = DISABLE_ALL; // Disable all the segments
            if(!(LPC_GPIO0->FIOPIN & 1<<21) && UP==0)//P0.21
                  {
                        UP=1;
                  }

            
}

void UDisplay(void) // To Display on 7-segments
{
    if (dig_countu == 0x01) // For Segment U8
    {
        temp1 = digu1;
        LPC_GPIO1->FIOPIN = FIRST_SEG;
    }
    else if (dig_countu == 0x02) // For Segment U9
    {
        temp1 = digu2;
        LPC_GPIO1->FIOPIN = SECOND_SEG;
    }
    else if (dig_countu == 0x03) // For Segment U10
    {
        temp1 = digu3;
        LPC_GPIO1->FIOPIN = THIRD_SEG;
    }
    else if (dig_countu == 0x04) // For Segment U11
    {
        temp1 = digu4;
        LPC_GPIO1->FIOPIN = FOURTH_SEG;
    }

    temp1 &= 0x0F;
    temp2 = array_dec[temp1]; // Decoding to 7-segment
    temp2 = temp2 << 4;
    LPC_GPIO0->FIOPIN = temp2; // Taking Data Lines for 7-Seg

    for (i = 0; i < 500; i++);
    LPC_GPIO0->FIOCLR = 0x00000FF0;
    // LPC_GPIO1->FIOPIN = DISABLE_ALL; // Disable all the segments

            if(!(LPC_GPIO0->FIOPIN & 1<<21) && UP==1)//P0.21
                  {
                        UP=0;
                  }
            
}
