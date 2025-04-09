
	while(1){
		delay();
		dig_count+=1;
		if(dig_count==4){
			dig_count=0;
			
		}
		display();
		
	}
}
void display(void){
	LPC_GPIO1->FIOPIN= select_segment[dig_count];
	LPC_GPIO1->FIOPIN= seven_seg[digit_value[dig_count]]<<4;
	//for(i=0;i<500;i++);
	LPC_GPIO0->FIOCLR = 0X00000FF0;//clear display before next update
		
}
void delay(void){
	for(i=0;i<500;i++){
		if(count==N){
			flag=0xFF;
			count=0;
		}else{
			count+=1;
			if(flag==0xFF){
				flag=0;
				digit_value[0]+=1;
				if(digit_value[0]==10){
					digit_value[0]=0;
					digit_value[1]+=1;
					if(digit_value[1]==10){
						digit_value[1]=0;
						digit_value[2]+=1;
						if(digit_value[2]==10){
							digit_value[2]=0;
							digit_value[3]+=1;
						if(digit_value[3]==10){
							digit_value[3]=0;
							
						}
					}
				}
			}
		}
	}
}
	}

