#include <LPC17xx.h>
#include <stdio.h>

// LCD Control Pins
#define RS_CTRL 0x08000000   // P0.27
#define EN_CTRL 0x10000000   // P0.28
#define DT_CTRL 0x07800000   // P0.23-26

// Buzzer and LED Pins
#define PLAYER1_BUZZER (1 << 0)  // P2.0 for Player 1 buzzer
#define PLAYER2_BUZZER (1 << 1)  // P2.1 for Player 2 buzzer
#define PLAYER1_LED    (1 << 2)  // P2.2 for Player 1 LED
#define PLAYER2_LED    (1 << 3)  // P2.3 for Player 2 LED
#define BUZZER_SOUND   (1 << 4)  // P2.4 for buzzer sound

// Keypad configuration
#define KEYPAD_ROW_START 10  // P2.10-13 for rows
#define KEYPAD_COL_START 23  // P1.23-26 for columns

// Keypad special keys
#define KEY_CORRECT 10  // Key 'A' for correct answer
#define KEY_WRONG   11  // Key 'B' for wrong answer
#define KEY_RESET   12  // Key 'C' for reset

// Global variables
unsigned int player1_score = 0;
unsigned int player2_score = 0;
unsigned char active_player = 0;  // 0=none, 1=player1, 2=player2
unsigned char game_state = 0;     // 0=waiting, 1=player1 answering, 2=player2 answering
unsigned char scores_updated = 1;  // Flag to indicate when scores need refresh

// LCD control variables
unsigned long int temp1, temp2;
unsigned char flag1 = 0, flag2 = 0;
unsigned long int init_command[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0C, 0x06, 0x01, 0x80};
unsigned char msg[16];  // Buffer for LCD messages

// Keypad variables
unsigned char row, col, key;
unsigned char scan_code[4][4] = {
    {0, 1, 2, 3},
    {4, 5, 6, 7},
    {8, 9, 10, 11},  // 10=A, 11=B
    {12, 13, 14, 15}  // 12=C, 13=D, 14=E, 15=F
};

// Function prototypes
void delay_ms(unsigned int ms);
void delay_lcd(unsigned int r1);
void port_write(void);
void lcd_write(void);
void lcd_clear(void);
void lcd_puts(unsigned char *string);
void lcd_set_cursor(unsigned char row, unsigned char col);
void update_score_display(void);
void process_keypad_input(void);
void check_player_buzzers(void);
void reset_game_state(void);

// Timer for debouncing and timing
void init_timer(void) {
    LPC_TIM0->PR = 25000 - 1;  // Set prescaler for 1ms timing (assuming 25MHz clock)
    LPC_TIM0->TCR = 0x02;      // Reset timer
    LPC_TIM0->TCR = 0x01;      // Enable timer
}

unsigned int get_ms(void) {
    return LPC_TIM0->TC;
}

void delay_ms(unsigned int ms) {
    unsigned int start = get_ms();
    while ((get_ms() - start) < ms);
}

// LCD functions
void delay_lcd(unsigned int r1) {
    unsigned int r;
    for (r = 0; r < r1; r++);
}

void port_write(void) {
    LPC_GPIO0->FIOPIN = temp2;
    if (flag1 == 0) {
        LPC_GPIO0->FIOCLR = RS_CTRL;
    } else {
        LPC_GPIO0->FIOSET = RS_CTRL;
    }
    LPC_GPIO0->FIOSET = EN_CTRL;
    delay_lcd(25000);
    LPC_GPIO0->FIOCLR = EN_CTRL;
    delay_lcd(25000);
}

void lcd_write(void) {
    flag2 = (flag1 == 1) ? 0 : ((temp1 == 0x30) || (temp1 == 0x20)) ? 1 : 0;

    temp2 = temp1 & 0xF0;
    temp2 = temp2 << 19;
    port_write();

    if (!flag2) {
        temp2 = temp1 & 0x0F;
        temp2 = temp2 << 23;
        port_write();
    }
}

void lcd_init(void) {
    flag1 = 0;
    for (int i = 0; i < 9; i++) {
        temp1 = init_command[i];
        lcd_write();
    }
}

void lcd_clear(void) {
    flag1 = 0;
    temp1 = 0x01;
    lcd_write();
    delay_lcd(50000);
}

void lcd_set_cursor(unsigned char row, unsigned char col) {
    flag1 = 0;
    if (row == 0)
        temp1 = 0x80 + col;
    else
        temp1 = 0xC0 + col;
    lcd_write();
}

void lcd_puts(unsigned char *string) {
    flag1 = 1;
    while (*string) {
        temp1 = *string++;
        lcd_write();
    }
}

// Game logic functions
void update_score_display(void) {
    if (!scores_updated) return;
    
    lcd_clear();
    
    // Display scores in descending order
    if (player1_score >= player2_score) {
        sprintf((char*)msg, "P1:%d  P2:%d", player1_score, player2_score);
    } else {
        sprintf((char*)msg, "P2:%d  P1:%d", player2_score, player1_score);
    }
    
    lcd_set_cursor(0, 0);
    lcd_puts(msg);
    
    // Display current state information
    lcd_set_cursor(1, 0);
    if (game_state == 0) {
        lcd_puts((unsigned char*)"Waiting...");
    } else if (game_state == 1) {
        lcd_puts((unsigned char*)"Player 1 Turn");
    } else if (game_state == 2) {
        lcd_puts((unsigned char*)"Player 2 Turn");
    }
    
    scores_updated = 0;
}

void reset_game_state(void) {
    game_state = 0;
    active_player = 0;
    scores_updated = 1;
    
    // Turn off player LEDs
    LPC_GPIO2->FIOCLR = PLAYER1_LED | PLAYER2_LED;
    
    update_score_display();
}

void sound_buzzer(void) {
    // Activate buzzer sound
    LPC_GPIO2->FIOSET = BUZZER_SOUND;
    delay_ms(200);
    LPC_GPIO2->FIOCLR = BUZZER_SOUND;
}

void process_answer(unsigned char correct) {
    if (correct) {
        // Correct answer - update score
        if (active_player == 1) {
            player1_score++;
        } else if (active_player == 2) {
            player2_score++;
        }
        scores_updated = 1;
        update_score_display();
        reset_game_state();
    } else {
        // Wrong answer
        if (active_player == 1) {
            // First player wrong, switch to second player if they buzzed
            if (game_state == 1) {
                // Turn off player 1 LED
                LPC_GPIO2->FIOCLR = PLAYER1_LED;
                
                // Check if player 2 has also buzzed
                if (!(LPC_GPIO2->FIOPIN & PLAYER2_BUZZER)) {
                    active_player = 2;
                    game_state = 2;
                    LPC_GPIO2->FIOSET = PLAYER2_LED;
                    
                    lcd_clear();
                    lcd_set_cursor(0, 0);
                    lcd_puts((unsigned char*)"Player 2 Turn");
                    
                    scores_updated = 1;
                    update_score_display();
                } else {
                    // No second player, reset
                    reset_game_state();
                }
            } else {
                // Both players wrong, reset
                reset_game_state();
            }
        } else if (active_player == 2) {
            // Second player wrong, reset
            reset_game_state();
        }
    }
}

void check_player_buzzers(void) {
    // Only check buzzers if in waiting state
    if (game_state != 0) return;
    
    // Check Player 1 buzzer
    if (!(LPC_GPIO2->FIOPIN & PLAYER1_BUZZER)) {
        delay_ms(20);  // Debounce
        if (!(LPC_GPIO2->FIOPIN & PLAYER1_BUZZER)) {
            active_player = 1;
            game_state = 1;
            
            // Turn on Player 1 LED
            LPC_GPIO2->FIOSET = PLAYER1_LED;
            
            // Sound the buzzer
            sound_buzzer();
            
            // Display on LCD
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_puts((unsigned char*)"Player 1 First!");
            lcd_set_cursor(1, 0);
            lcd_puts((unsigned char*)"Waiting answer...");
            
            scores_updated = 1;
            update_score_display();
            
            // Wait for button release
            while (!(LPC_GPIO2->FIOPIN & PLAYER1_BUZZER));
            delay_ms(20);
        }
    }
    
    // Check Player 2 buzzer if Player 1 hasn't already pressed
    if (game_state == 0 && !(LPC_GPIO2->FIOPIN & PLAYER2_BUZZER)) {
        delay_ms(20);  // Debounce
        if (!(LPC_GPIO2->FIOPIN & PLAYER2_BUZZER)) {
            active_player = 2;
            game_state = 2;
            
            // Turn on Player 2 LED
            LPC_GPIO2->FIOSET = PLAYER2_LED;
            
            // Sound the buzzer
            sound_buzzer();
            
            // Display on LCD
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_puts((unsigned char*)"Player 2 First!");
            lcd_set_cursor(1, 0);
            lcd_puts((unsigned char*)"Waiting answer...");
            
            scores_updated = 1;
            update_score_display();
            
            // Wait for button release
            while (!(LPC_GPIO2->FIOPIN & PLAYER2_BUZZER));
            delay_ms(20);
        }
    }
}

void process_keypad_input(void) {
    // Only scan keypad if a player is active
    if (active_player == 0) return;
    
    // Scan keypad
    for (row = 0; row < 4; row++) {
        LPC_GPIO2->FIOCLR = 0x00003C00;  // Clear P2.10-13
        LPC_GPIO2->FIOSET = (1 << (KEYPAD_ROW_START + row));  // Set one row at a time
        
        delay_ms(1);  // Small delay for stability
        
        for (col = 0; col < 4; col++) {
            if (!(LPC_GPIO1->FIOPIN & (1 << (KEYPAD_COL_START + col)))) {
                key = scan_code[row][col];
                
                // Process special keys
                if (key == KEY_CORRECT) {
                    // Correct answer
                    process_answer(1);
                } else if (key == KEY_WRONG) {
                    // Wrong answer
                    process_answer(0);
                } else if (key == KEY_RESET) {
                    // Reset game state
                    reset_game_state();
                }
                
                // Wait for key release
                while (!(LPC_GPIO1->FIOPIN & (1 << (KEYPAD_COL_START + col))));
                delay_ms(20);  // Debounce
                return;
            }
        }
    }
}

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();
    
    // Initialize timer
    init_timer();
    
    // Configure GPIO pins
    // LCD Control
    LPC_GPIO0->FIODIR |= DT_CTRL | EN_CTRL | RS_CTRL;
    
    // Keypad
    LPC_GPIO2->FIODIR |= 0x00003C00;                // Row lines as output (P2.10-13)
    LPC_GPIO1->FIODIR &= ~(0x0F << KEYPAD_COL_START);  // Column lines as input (P1.23-26)
    
    // Player Buzzers (input) and LEDs (output)
    LPC_GPIO2->FIODIR &= ~(PLAYER1_BUZZER | PLAYER2_BUZZER);  // Buzzers as input
    LPC_GPIO2->FIODIR |= PLAYER1_LED | PLAYER2_LED | BUZZER_SOUND;  // LEDs and sound buzzer as output
    
    // Initialize LCD
    lcd_init();
    
    // Initialize game state
    reset_game_state();
    
    // Main loop
    while (1) {
        // Check player buzzers
        check_player_buzzers();
        
        // Process keypad input from moderator
        process_keypad_input();
        
        // Update display if needed
        if (scores_updated) {
            update_score_display();
        }
        
        // Small delay to prevent CPU hogging
        delay_ms(5);
    }
}
