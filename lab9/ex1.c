#include <LPC17xx.h>

// LCD Control Pins on Port 0
#define RS_CTRL 0x08000000  // P0.27, 1<<27
#define EN_CTRL 0x10000000  // P0.28, 1<<28
#define DT_CTRL 0x07800000  // P0.23 to P0.26 data lines, F<<23

// Variables for LCD control
unsigned long int temp1 = 0, temp2 = 0, i, j;
unsigned char flag1 = 0, flag2 = 0;
unsigned char result[16];

// Variables for keyboard scanning
unsigned char col, row, flag, key;
unsigned long int var1, temp, temp3;
int first_num = -1, second_num = -1;
char operator = 0;
int input_state = 0;  // 0: waiting for first num, 1: waiting for operator, 2: waiting for second num

// LCD initialization commands
unsigned long int init_command[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0c, 0x06, 0x01, 0x80};

// Function prototypes
void lcd_write(void);
void port_write(void);
void delay_lcd(unsigned int);
void scan(void);
void lcd_puts(unsigned char *);
void lcd_command(unsigned char);
void process_key(unsigned char);
void calculate_and_display(void);

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();
    
    // Configure LCD pins
    LPC_GPIO0->FIODIR = DT_CTRL | RS_CTRL | EN_CTRL;
    
    // Configure keyboard pins
    LPC_PINCON->PINSEL3 &= 0xFFC03FFF;  // P1.23 to P1.26 made GPIO (columns)
    LPC_PINCON->PINSEL3 &= 0xF00FFFFF;  // P1.10 to P1.13 made GPIO (rows)
    LPC_GPIO2->FIODIR |= 0x00003C00;    // made output P2.10 to P2.13 (rows)
    LPC_GPIO1->FIODIR &= 0xF87FFFFF;    // made input P1.23 to P1.26 (columns)
    
    // Initialize LCD
    flag1 = 0;  // Command mode
    for (i = 0; i < 9; i++) {
        temp1 = init_command[i];
        lcd_write();
    }
    
    flag1 = 1;  // Data mode
    lcd_puts((unsigned char*)"Enter: A op B =");
    
    // Main loop to read keyboard and process input
    while(1) {
        // Scan keyboard
        for(row = 1; row < 5; row++) {
            if(row == 1)
                var1 = 0x00000400;
            else if(row == 2)
                var1 = 0x00000800;
            else if(row == 3)
                var1 = 0x00001000;
            else if(row == 4)
                var1 = 0x00002000;
                
            temp = var1;
            LPC_GPIO2->FIOCLR = 0x00003C00;  // Clear all rows
            LPC_GPIO2->FIOSET = var1;        // Set current row
            
            flag = 0;
            scan();  // Check for key press
            
            if(flag == 1) {  // Key detected
                // Get key code from row and column
                if(row == 1) {
                    if(col == 0) key = '1';
                    else if(col == 1) key = '2';
                    else if(col == 2) key = '3';
                    else if(col == 3) key = '+';
                }
                else if(row == 2) {
                    if(col == 0) key = '4';
                    else if(col == 1) key = '5';
                    else if(col == 2) key = '6';
                    else if(col == 3) key = '-';
                }
                else if(row == 3) {
                    if(col == 0) key = '7';
                    else if(col == 1) key = '8';
                    else if(col == 2) key = '9';
                    else if(col == 3) key = '*';
                }
                else if(row == 4) {
                    if(col == 0) key = 'C';  // Clear
                    else if(col == 1) key = '0';
                    else if(col == 2) key = '=';
                    else if(col == 3) key = '/';
                }
                
                process_key(key);  // Process the key press
                
                // Wait for key release
                while((LPC_GPIO1->FIOPIN & 0x07800000) != 0);
                delay_lcd(50000);  // Debounce delay
            }
        }
    }
}

// Process key press based on current input state
void process_key(unsigned char key) {
    // Clear command
    if(key == 'C') {
        input_state = 0;
        first_num = -1;
        second_num = -1;
        operator = 0;
        
        // Clear display and show prompt again
        lcd_command(0x01);  // Clear display
        lcd_command(0x80);  // Move to beginning of first line
        lcd_puts((unsigned char*)"Enter: A op B =");
        lcd_command(0xC0);  // Move to beginning of second line
        return;
    }
    
    // Handle equals key
    if(key == '=' && input_state == 2 && first_num != -1 && second_num != -1 && operator != 0) {
        calculate_and_display();
        return;
    }
    
    // Handle numeric keys (0-9)
    if(key >= '0' && key <= '9') {
        int digit = key - '0';
        
        if(input_state == 0) {
            // First number
            first_num = digit;
            input_state = 1;
            
            // Display the digit
            lcd_command(0xC0);  // Move to beginning of second line
            temp1 = key;
            lcd_write();
        }
        else if(input_state == 2) {
            // Second number
            second_num = digit;
            
            // Display the digit
            temp1 = key;
            lcd_write();
        }
        return;
    }
    
    // Handle operator keys (+ or -)
    if((key == '+' || key == '-') && input_state == 1 && first_num != -1) {
        operator = key;
        input_state = 2;
        
        // Display the operator
        temp1 = key;
        lcd_write();
    }
}

// Calculate and display the result
void calculate_and_display(void) {
    int result_val;
    
    // Perform calculation
    if(operator == '+') {
        result_val = first_num + second_num;
    }
    else if(operator == '-') {
        result_val = first_num - second_num;
    }
    
    // Display the equals sign
    temp1 = '=';
    lcd_write();
    
    // Convert result to string and display
    if(result_val < 0) {
        temp1 = '-';
        lcd_write();
        result_val = -result_val;
    }
    
    // Display result (simple case for single-digit results)
    temp1 = '0' + result_val;
    lcd_write();
    
    // Reset input state
    input_state = 0;
    first_num = -1;
    second_num = -1;
    operator = 0;
    
    // Delay before allowing new input
    delay_lcd(500000);
}

// Write string to LCD
void lcd_puts(unsigned char *string) {
    while(*string) {
        temp1 = *string++;
        lcd_write();
    }
}

// Send command to LCD
void lcd_command(unsigned char command) {
    flag1 = 0;  // Command mode
    temp1 = command;
    lcd_write();
    flag1 = 1;  // Data mode
}

// Scan columns to detect key press
void scan(void) {
    temp3 = LPC_GPIO1->FIOPIN;
    temp3 &= 0x07800000;
    
    if(temp3 != 0x00000000) {
        flag = 1;
        if(temp3 == 0x00800000)
            col = 0;
        else if(temp3 == 0x01000000)
            col = 1;
        else if(temp3 == 0x02000000)
            col = 2;
        else if(temp3 == 0x04000000)
            col = 3;
    }
}

// Write to LCD with nibble mode
void lcd_write(void) {
    flag2 = (flag1 == 1) ? 0 : ((temp1 == 0x30) || (temp1 == 0x20)) ? 1 : 0;
    
    // Send higher nibble
    temp2 = temp1 & 0xf0;
    temp2 = temp2 << 19;
    port_write();
    
    // Send lower nibble if needed
    if(flag2 == 0) {
        temp2 = temp1 & 0x0f;
        temp2 = temp2 << 23;
        port_write();
    }
}

// Write data/command to LCD port
void port_write(void) {
    LPC_GPIO0->FIOPIN = temp2;
    
    if(flag1 == 0)
        LPC_GPIO0->FIOCLR = RS_CTRL;  // Command mode
    else
        LPC_GPIO0->FIOSET = RS_CTRL;  // Data mode
    
    LPC_GPIO0->FIOSET = EN_CTRL;  // Enable high
    delay_lcd(25);
    LPC_GPIO0->FIOCLR = EN_CTRL;  // Enable low
    delay_lcd(30000);
}

// Delay function
void delay_lcd(unsigned int r1) {
    unsigned int r;
    for(r = 0; r < r1; r++);
    return;
}
