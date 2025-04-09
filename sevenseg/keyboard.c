#include <LPC17xx.h>
#define FIRST_SEG 0xFFF87FFF
void scan(void);
unsigned char col, row, flag;
unsigned long int i, var1, temp, temp3, temp2;
unsigned char SEVEN_CODE[4][4]={{0X3F, 0X06, 0X5B, 0X4F},{0X66, 0X6D, 0X7D, 0X07},{0X7F, 0X6F,0X77, 0X7C},{0X58, 0X5E, 0X79, 0X71}};
int main(void){
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL0=0;
	LPC_GPIO0->FIODIR=0XFFFFFFFF;//port 0 output
	LPC_PINCON->PINSEL3=0;
	LPC_PINCON->PINSEL4=0;
	LPC_GPIO2->FIODIR=0X00003C00;//output p2.10 to p2.13 rows
	LPC_GPIO1->FIODIR=0;//input: p1.23 to p1.26 cols
	while(1){
		for(row=0;row<4;row++){
			if(row==0) temp= 1<<10;
			else if(row==1) temp= 1<<11;
			else if(row==2) temp= 1<<12;
			else if(row==3) temp= 1<<13;
			LPC_GPIO2->FIOPIN=temp;// activates one row at a time
			flag=0;//reset the detection flag before scanning
			scan();
			if(flag==1){//if key press is detected
				temp2=SEVEN_CODE[row][col];//get the code fr key row, col
				LPC_GPIO0->FIOMASK= 0XFFF87FFF;//mask all except select pins ; as 15: 11 ->digit/select segment. 4:7 ->data lines
				LPC_GPIO0->FIOPIN = FIRST_SEG;//select the first segment 
				temp2<<=4;//shift 7 seg code to align with 4 to 11
				LPC_GPIO0->FIOMASK = 0xFFFFF00F;//mask all except 7 seg data points
				LPC_GPIO0->FIOPIN = temp2;//send pattern to 7 seg display
				break;//exit loop - key press handled 
				
			}
			
		}
	}
	
	
}
void scan(void){

	unsigned long temp3;
	temp3=LPC_GPIO1->FIOPIN;
	temp3&=0x07800000;
	if(temp3!=0){
		flag=1;
		if(temp3==1<<23) col=0;
		else if(temp3==1<<24) col=1;
		else if(temp3==1<<25) col=2;
		else if(temp3==1<<26) col=3;
	}
}
