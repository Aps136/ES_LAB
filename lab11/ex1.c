#include <LPC17xx.H>

// Function prototypes
void pwm_init(void);
void keyboard_init(void);
unsigned char scan_key(void);
void process_key(unsigned char);

// Global variables
unsigned long int i;
unsigned char flag;

int main(void)
{
    unsigned char key;
    
    SystemInit();
    SystemCoreClockUpdate();
    
    // Initialize PWM and keyboard
    pwm_init();
    keyboard_init();
    
    while(1)
    {
        // Scan for key press
        key = scan_key();
        
        if(key != 0xFF) // If a key is pressed
        {
            process_key(key);
            
            // Wait for key release (debounce)
            for(i=0; i<500000; i++);
            while(scan_key() != 0xFF);
            for(i=0; i<500000; i++);
        }
    }
}

void pwm_init(void)
{
    LPC_SC->PCONP |= (1<<6);             // PWM1 is powered
    
    LPC_PINCON->PINSEL3 &= ~(0x0000C000); // Cleared if any other functions are enabled
    LPC_PINCON->PINSEL3 |= 0x00008000;    // PWM1.4 is selected for the pin P1.23
    
    LPC_PWM1->PCR = 0x00001000;          // Select PWM1 single edge for PWM4
    LPC_PWM1->MCR = 0x00000002;          // Reset on PWMMR0 (no interrupt)
    LPC_PWM1->MR0 = 30000;               // Setup match register 0 count for PWM period
    LPC_PWM1->MR4 = 0;                   // Initially set duty cycle to 0 (LED off)
    LPC_PWM1->LER = 0x000000FF;          // Enable shadow copy register
    LPC_PWM1->TCR = 0x00000002;          // Reset counter and prescaler
    LPC_PWM1->TCR = 0x00000009;          // Enable PWM and counter
    
    return;
}

void keyboard_init(void)
{
    // Configure keyboard pins - Rows as outputs, Columns as inputs
    // Using P2.10-P2.13 for rows and P1.23-P1.26 for columns as in typical matrix keyboard
    
    LPC_PINCON->PINSEL4 &= ~(0x00000F00); // Configure P2.10-P2.13 as GPIO (rows)
    LPC_GPIO2->FIODIR |= 0x00003C00;      // Set P2.10-P2.13 as outputs
    
    LPC_PINCON->PINSEL3 &= ~(0x0FF00000); // Configure P1.23-P1.26 as GPIO (columns)
    LPC_GPIO1->FIODIR &= ~(0x07800000);   // Set P1.23-P1.26 as inputs
    
    return;
}

unsigned char scan_key(void)
{
    unsigned long temp;
    
    // We only need to check ROW-0 (P2.10) as per the requirement
    LPC_GPIO2->FIOCLR = 0x00003C00;    // Clear all rows
    LPC_GPIO2->FIOSET = 0x00000400;    // Set ROW-0 (P2.10)
    
    // Small delay for settling
    for(i=0; i<1000; i++);
    
    // Read columns
    temp = LPC_GPIO1->FIOPIN & 0x07800000;
    
    if(temp != 0)
    {
        // Return key code based on which column is active
        if(temp & 0x00800000)      // Column 0 (P1.23)
            return 0;              // Key '0'
        else if(temp & 0x01000000) // Column 1 (P1.24)
            return 1;              // Key '1'
        else if(temp & 0x02000000) // Column 2 (P1.25)
            return 2;              // Key '2'
        else if(temp & 0x04000000) // Column 3 (P1.26)
            return 3;              // Key '3'
    }
    
    return 0xFF; // No key pressed
}

void process_key(unsigned char key)
{
    // Set PWM duty cycle based on the key pressed
    switch(key)
    {
        case 0: // Key '0' - 10% intensity
            LPC_PWM1->MR4 = 27000;  // 10% duty cycle (30000 - 10% of 30000)
            break;
            
        case 1: // Key '1' - 25% intensity
            LPC_PWM1->MR4 = 22500;  // 25% duty cycle (30000 - 25% of 30000)
            break;
            
        case 2: // Key '2' - 50% intensity
            LPC_PWM1->MR4 = 15000;  // 50% duty cycle (30000 - 50% of 30000)
            break;
            
        case 3: // Key '3' - 75% intensity
            LPC_PWM1->MR4 = 7500;   // 75% duty cycle (30000 - 75% of 30000)
            break;
            
        default:
            return; // Invalid key
    }
    
    // Update the PWM with new settings
    LPC_PWM1->LER = 0x000000FF;
}
