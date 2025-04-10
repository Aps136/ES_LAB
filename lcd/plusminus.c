#include <LPC17xx.h>

#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

unsigned long int init_command[] = {0x30,0x30,0x30,0x20,0x28,0x0C,0x06,0x01,0x80};
unsigned char msg[] = {'0','1','2','3','4','5','6','7','8','9','+','=','C','-',' ',' '};
unsigned char scan_code[4][4] = {
  {0,1,2,3},
  {4,5,6,7},
  {8,9,10,11},
  {12,13,14,15}
};
unsigned int r, r1;

unsigned char row, col, key;
unsigned int temp1 = 0, temp2 = 0, i;
unsigned char flag1 = 0, flag2 = 0;
int A = -1, B = -1, result = 0;
char op = '+';  // current operator

void delay_lcd(int r1) {
  int r;
  for (r = 0; r < r1; r++);
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
  delay_lcd(500000);
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

void display_number(int num) {
  display_char(num + '0');  // only for 0–9
}

void scan_keypad(void) {
  char ch;
  while (1) {
    for (row = 0; row < 4; row++) {
      LPC_GPIO2->FIOCLR = 0x00003C00;
      LPC_GPIO2->FIOSET = (1 << (10 + row));

      for (col = 0; col < 4; col++) {
        if (!(LPC_GPIO1->FIOPIN & (1 << (23 + col)))) {
          key = scan_code[row][col];
          ch = msg[key];

          if (ch >= '0' && ch <= '9') {
            if (A == -1) {
              A = ch - '0';
              display_char(ch);
              display_char(op);  // display operator immediately
            } else if (B == -1) {
              B = ch - '0';
              display_char(ch);
              display_char('=');  // show '=' immediately

              if (op == '+')
                result = A + B;
              else if (op == '-')
                result = A - B;

              display_number(result);

              A = B = -1;
              result = 0;
              op = '+';  // reset to default operator
            }
          } else if (ch == '+' || ch == '-') {
            op = ch;  // update operator (for future input)
          }

          while (!(LPC_GPIO1->FIOPIN & (1 << (23 + col))));
          delay_lcd(100000);
        }
      }
    }
  }
}

int main(void) {
  SystemInit();
  SystemCoreClockUpdate();

  LPC_GPIO0->FIODIR = DT_CTRL | EN_CTRL | RS_CTRL;
  LPC_GPIO2->FIODIR = 0x00003C00;
  LPC_GPIO1->FIODIR &= ~(0x07800000);

  flag1 = 0;
  for (i = 0; i < 9; i++) {
    temp1 = init_command[i];
    lcd_write();
  }

  scan_keypad();
}
