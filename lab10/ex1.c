#include <LPC17xx.h>
#include <stdio.h>
#include "AN_LCD.h"

#define Ref_Vtg 3.300
#define Full_Scale 0xFFF  // 12 bit ADC

int main(void)
{
    unsigned long adc_temp_ch4, adc_temp_ch5;
    unsigned int i;
    float in_vtg_ch4, in_vtg_ch5, diff_vtg;
    unsigned char vtg_diff[7], dval_diff[7];
    unsigned char Msg1[11] = {"CH4 - CH5:"};
    unsigned char Msg2[12] = {"DIFF VALUE:"};

    SystemInit();
    SystemCoreClockUpdate();
    
    LPC_SC->PCONP |= (1<<15);  // Power for GPIO block
    lcd_init();
    
    // Configure pins for ADC channels 4 and 5
    LPC_PINCON->PINSEL3 |= 0xF0000000;  // P1.30 as AD0.4 and P1.31 as AD0.5
    LPC_SC->PCONP |= (1<<12);  // Enable the peripheral ADC
    
    SystemCoreClockUpdate();
    temp1 = 0x80;  // First line of LCD
    lcd_com();
    delay_lcd(800);
    lcd_puts(&Msg1[0]);
    
    temp1 = 0xC0;  // Second line of LCD
    lcd_com();
    delay_lcd(800);
    lcd_puts(&Msg2[0]);
    
    while(1)
    {
        // Read ADC Channel 4
        LPC_ADC->ADCR = (1<<4)|(1<<21)|(1<<24);  // ADC0.4, start conversion and operational
        while((adc_temp_ch4 = LPC_ADC->ADGDR) == 0x80000000);  // Wait till conversion complete
        adc_temp_ch4 = LPC_ADC->ADGDR;
        adc_temp_ch4 >>= 4;
        adc_temp_ch4 &= 0x00000FFF;  // 12 bit ADC
        
        // Calculate input voltage for channel 4
        in_vtg_ch4 = (((float)adc_temp_ch4 * (float)Ref_Vtg))/((float)Full_Scale);
        
        // Small delay between conversions
        for(i=0; i<2000; i++);
        
        // Read ADC Channel 5
        LPC_ADC->ADCR = (1<<5)|(1<<21)|(1<<24);  // ADC0.5, start conversion and operational
        while((adc_temp_ch5 = LPC_ADC->ADGDR) == 0x80000000);  // Wait till conversion complete
        adc_temp_ch5 = LPC_ADC->ADGDR;
        adc_temp_ch5 >>= 4;
        adc_temp_ch5 &= 0x00000FFF;  // 12 bit ADC
        
        // Calculate input voltage for channel 5
        in_vtg_ch5 = (((float)adc_temp_ch5 * (float)Ref_Vtg))/((float)Full_Scale);
        
        // Calculate the difference in voltage
        diff_vtg = in_vtg_ch4 - in_vtg_ch5;
        
        // Calculate the digital value difference
        long diff_digital = (long)adc_temp_ch4 - (long)adc_temp_ch5;
        
        // Convert the readings into strings to display on LCD
        sprintf(vtg_diff, "%+3.2fV", diff_vtg);  // Include sign for voltage difference
        sprintf(dval_diff, "%+4d", (int)diff_digital);  // Include sign for digital value
        
        // Display voltage difference
        temp1 = 0x8A;  // Position on first line
        lcd_com();
        delay_lcd(800);
        lcd_puts(&vtg_diff[0]);
        
        // Display digital value difference
        temp1 = 0xCB;  // Position on second line
        lcd_com();
        delay_lcd(800);
        lcd_puts(&dval_diff[0]);
        
        // Delay before next reading
        for(i=0; i<200000; i++);
        
        // Clear variables for next reading
        for(i=0; i<7; i++)
            vtg_diff[i] = dval_diff[i] = 0x00;
            
        adc_temp_ch4 = 0;
        adc_temp_ch5 = 0;
        in_vtg_ch4 = 0;
        in_vtg_ch5 = 0;
        diff_vtg = 0;
    }
}
