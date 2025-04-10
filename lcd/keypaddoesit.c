#include <LPC17xx.h>

#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

unsigned char msg1[] = "COUNT:";

unsigned char scan_code[4][4] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

unsigned int r, c;
unsigned char row, col, key;
unsigned int count = 0;
unsigned long int temp1, temp2;
unsigned char flag1 = 0, flag2 = 0;

void delay_lcd(unsigned int r1) {
  for (int r = 0; r < r1; r++);
}

void port_write(void) {
  LPC_GPIO0->FIOPIN = temp2;
  if (flag1 == 0)
    LPC_GPIO0->FIOCLR = RS_CTRL;
  else
    LPC_GPIO0->FIOSET = RS_CTRL;
  LPC_GPIO0->FIOSET = EN_CTRL;
  delay_lcd(25);
  LPC_GPIO0->FIOCLR = EN_CTRL;
  delay_lcd(300000);
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

void display_char(char ch) {
  flag1 = 1;
  temp1 = ch;
  lcd_write();
}

void display_number(unsigned int num) {
  if (num >= 10)
    display_char((num / 10) + '0');
  else
    display_char('0');
  display_char((num % 10) + '0');
}

void display_count() {
  temp1 = 0x80; flag1 = 0; lcd_write(); // 1st line
  flag1 = 1;
  for (int i = 0; msg1[i] != '\0'; i++) {
    temp1 = msg1[i];
    lcd_write();
  }
  display_number(count);
}

char scan_keypad(void) {
  for (row = 0; row < 4; row++) {
    LPC_GPIO2->FIOCLR = 0x00003C00;
    LPC_GPIO2->FIOSET = (1 << (10 + row));

    for (col = 0; col < 4; col++) {
      if (!(LPC_GPIO1->FIOPIN & (1 << (23 + col)))) {
        char key = scan_code[row][col];
        while (!(LPC_GPIO1->FIOPIN & (1 << (23 + col)))); // wait for release
        delay_lcd(100000);
        return key;
      }
    }
  }
  return 0;
}

void lcd_init(void) {
  unsigned long int init_cmds[] = { 0x30, 0x30, 0x30, 0x20, 0x28, 0x0C, 0x06, 0x01, 0x80 };
  flag1 = 0;
  for (int i = 0; i < 9; i++) {
    temp1 = init_cmds[i];
    lcd_write();
  }
}

int main(void) {
  SystemInit();
  SystemCoreClockUpdate();

  LPC_GPIO0->FIODIR = RS_CTRL | EN_CTRL | DT_CTRL;
  LPC_GPIO2->FIODIR = 0x00003C00;       // Rows output
  LPC_GPIO1->FIODIR &= ~(0x07800000);   // Cols input

  lcd_init();
  display_count();

  while (1) {
    key = scan_keypad();

    if (key == '1') {
      count = (count + 1) % 100;
      display_count();
    } else if (key == '2') {
      if (count == 0) count = 99;
      else count--;
      display_count();
    }
  }
}
