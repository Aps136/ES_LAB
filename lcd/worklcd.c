#include <lpc17xx.h>
#include <stdlib.h>
#include <stdio.h>

// LCD Control Pins
#define RS_CTRL 0x08000000 // P0.27
#define EN_CTRL 0x10000000 // P0.28
#define DT_CTRL 0x07800000 // P0.23 to P0.26 data lines

// Global Variables
unsigned long int temp = 0, temp11 = 0, temp12 = 0, i;
unsigned char flag = 0, flag1 = 0, flag2 = 0;
unsigned char col, row;
char display_buffer[16]; // Buffer to hold display text
int buffer_index = 0;

// Calculator variables
int first_operand = 0;
int second_operand = 0;
char operator = 0;
int result = 0;
int current_state = 0; // 0: first operand, 1: operator, 2: second operand, 3: result

// Keypad mapping table: 4x4 matrix
unsigned char keypad_map[16] = {
    '1', '2', '3', '+',  // Row 0
    '4', '5', '6', '-',  // Row 1
    '7', '8', '9', '*',  // Row 2
    'C', '0', '=', '/'   // Row 3
};

// Function prototypes
void lcd_write(void);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_puts(char*);
void port_write(void);
void scan(void);
void delay_lcd(unsigned int);
void clear_display(void);
void process_key(unsigned char);
void calculate_result(void);
void display_number(int);
void init_lcd(void);

unsigned long int init_command[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0c, 0x06, 0x01, 0x80};

int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();
    
    // Configure GPIO pins
    LPC_GPIO0->FIODIR = DT_CTRL | RS_CTRL | EN_CTRL; // LCD pins as outputs
    LPC_GPIO2->FIODIR = 0x00003C00;                  // Rows as outputs (P2.10 to P2.13)
    LPC_GPIO1->FIODIR = 0;                           // Columns as inputs (P1.23 to P1.26)
    
    // Initialize LCD
    init_lcd();
    
    // Display initial message
    clear_display();
    lcd_puts("Calculator");
    lcd_cmd(0xC0); // Move to second line
    
    while (1) {
        // Scan keypad rows
        for (row = 0; row < 4; row++) {
            if (row == 0) temp = 1 << 10;
            else if (row == 1) temp = 1 << 11;
            else if (row == 2) temp = 1 << 12;
            else if (row == 3) temp = 1 << 13;
            
            LPC_GPIO2->FIOPIN = temp;
            
            flag = 0;
            scan();
            
            if (flag == 1) {
                unsigned char key = keypad_map[row * 4 + col];
                process_key(key);
                
                // Debounce delay
                delay_lcd(300000);
                
                // Wait for key release
                while((LPC_GPIO1->FIOPIN & 0x07800000) != 0);
                
                delay_lcd(300000); // Additional debounce
                break;
            }
        }
    }
}

void init_lcd(void)
{
    flag1 = 0; // Command mode
    for (i = 0; i < 9; i++) {
        temp11 = init_command[i];
        lcd_write();
        delay_lcd(50000);
    }
}

void process_key(unsigned char key)
{
    // Process based on key pressed
    if (key >= '0' && key <= '9') {
        // Numeric key
        if (current_state == 0) {
            // First operand
            first_operand = first_operand * 10 + (key - '0');
            display_number(first_operand);
        } 
        else if (current_state == 2) {
            // Second operand
            second_operand = second_operand * 10 + (key - '0');
            display_number(second_operand);
        }
        else if (current_state == 3) {
            // After result, start new calculation
            clear_display();
            first_operand = key - '0';
            second_operand = 0;
            operator = 0;
            current_state = 0;
            display_number(first_operand);
        }
    } 
    else if (key == '+' || key == '-' || key == '*' || key == '/') {
        // Operator key
        if (current_state == 0 && first_operand != 0) {
            operator = key;
            current_state = 2;
            
            // Display operator
            lcd_data(operator);
        }
        else if (current_state == 3) {
            // Continue with result
            first_operand = result;
            second_operand = 0;
            operator = key;
            current_state = 2;
            
            clear_display();
            display_number(first_operand);
            lcd_data(operator);
        }
    } 
    else if (key == '=') {
        // Calculate result
        if (current_state == 2 && operator != 0) {
            calculate_result();
            
            // Display result
            lcd_cmd(0xC0); // Move to second line
            lcd_puts("= ");
            display_number(result);
            
            // Reset for next calculation but keep result
            first_operand = result;
            second_operand = 0;
            operator = 0;
            current_state = 3;
        }
    } 
    else if (key == 'C') {
        // Clear/reset calculator
        clear_display();
        first_operand = 0;
        second_operand = 0;
        operator = 0;
        result = 0;
        current_state = 0;
    }
}

void calculate_result(void)
{
    switch (operator) {
        case '+':
            result = first_operand + second_operand;
            break;
        case '-':
            result = first_operand - second_operand;
            break;
        case '*':
            result = first_operand * second_operand;
            break;
        case '/':
            if (second_operand != 0) {
                result = first_operand / second_operand;
            } else {
                // Division by zero error
                clear_display();
                lcd_puts("Error: Div by 0");
                result = 0;
                current_state = 3;
            }
            break;
    }
}

void display_number(int number)
{
    // Convert integer to string and display
    sprintf(display_buffer, "%d", number);
    
    // Position cursor at beginning of line
    if (current_state == 0 || current_state == 2) {
        // First or second operand - clear line first
        if (buffer_index == 0) {
            if (current_state == 0) {
                lcd_cmd(0x01); // Clear display
                lcd_cmd(0x80); // Move to first line
            }
        }
    }
    
    // Display the number
    lcd_puts(display_buffer);
}

void clear_display(void)
{
    lcd_cmd(0x01); // Clear display
    delay_lcd(50000);
    lcd_cmd(0x80); // Move cursor to beginning of first line
    buffer_index = 0;
}

void lcd_cmd(unsigned char command)
{
    flag1 = 0; // Command mode
    temp11 = command;
    lcd_write();
}

void lcd_data(unsigned char data)
{
    flag1 = 1; // Data mode
    temp11 = data;
    lcd_write();
    buffer_index++;
}

void lcd_puts(char *string)
{
    while (*string) {
        lcd_data(*string++);
    }
}

void scan(void)
{
    unsigned long temp3;
    temp3 = LPC_GPIO1->FIOPIN;
    temp3 &= 0x07800000;
    
    if (temp3 != 0x00000000) {
        flag = 1;
        if (temp3 == 1 << 23) col = 0;
        else if (temp3 == 1 << 24) col = 1;
        else if (temp3 == 1 << 25) col = 2;
        else if (temp3 == 1 << 26) col = 3;
    }
}

void lcd_write(void)
{
    flag2 = (flag1 == 1) ? 0 : ((temp11 == 0x30) || (temp11 == 0x20)) ? 1 : 0;
    temp12 = temp11 & 0xf0;
    temp12 = temp12 << 19; // Send higher nibble
    port_write();
    
    if (!flag2) {
        temp12 = temp11 & 0x0f;
        temp12 = temp12 << 23; // Send lower nibble
        port_write();
    }
}

void port_write(void)
{
    LPC_GPIO0->FIOPIN = temp12;
    
    if (flag1 == 0)
        LPC_GPIO0->FIOCLR = RS_CTRL; // Command mode
    else
        LPC_GPIO0->FIOSET = RS_CTRL; // Data mode
    
    LPC_GPIO0->FIOSET = EN_CTRL; // Apply enable pulse
    delay_lcd(25);
    LPC_GPIO0->FIOCLR = EN_CTRL;
    delay_lcd(50000);
}

void delay_lcd(unsigned int r1)
{
    unsigned int r;
    for (r = 0; r < r1; r++);
}
