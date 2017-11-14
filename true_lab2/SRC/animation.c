#include "animation.h"
#include "led.h"
#include "system_timer.h"

//���������� ������ T1 ��� ���

//������� �����������
u8 power=0;
//LEVEL_ON
u8 power_l1=0;
u8 power_h1=0;
//LEVEL_OFF
u8 power_l2=0;
u8 power_h2=0;

u8 level;
u8 ledsState;
u8 stepCounter;
//���������� ��� ��������� �������
u8 direction;
//����� ���������� �����
time next_frame;

u8 i;//global counter

const u8 LEVEL_ON=0xF0;
const u8 LEVEL_OFF=0x0F;

const u8 UP=0xFF;
const u8 DOWN=0x00;
const time FRAME_DELAY=1000;

void T1_ISR( void ) __interrupt ( 1 );

void initialize_animation() {
	TMOD|=0b00010000; 
	SetVector( 0x201B, (void *)T1_ISR );
}

void enable_animation(){
	direction=UP;
	next_frame=get_ms()+FRAME_DELAY;
	set_power(0);
	ledsState = 0x01;
	level=LEVEL_ON;
	leds( ledsState );
	
	TH1=power_h1;
	TL1=power_l1;
	
	ET1=1;       
	TR1=1;      
}

void disable_animation(){
	ET1=0;
	TR1=0;
}

void set_power(u8 new_power){
	u16 power1,power2;
	TR1=0;
	
	power=new_power;
	power1=-(100*power+1);
	power2=-(100*(100-power)+1);
	
	power_l1=power1&0xFF;
	power_h1=power1>>8;
	
	power_l2=power2&0xFF;
	power_h2=power2>>8;
	
	TR1=1;
}

u8 get_power() {
	return power;
}

u8 current_step[8] = {6, 0, 0, 1, 2, 3, 4, 5};
u8 current_position[8] = {6, 0, 0, 1, 2, 3, 4, 5};
void process_step()
{
    for(i=0; i<8; i++)
        if(current_step[i] > 0)
            current_step[i]--;
}

void organize_step() 
{
    u8 last = current_position[7];
    for(i=7; i >= 0; i--) 
    {
        current_position[i] = current_position[i-1];
        current_step[i] = current_position[i];
    }
    current_position[0] = last;
    current_step[0] = current_position[0];
}

void T1_ISR( void ) __interrupt ( 1 ){


	if( level==LEVEL_ON ){ //light up delay
		level=LEVEL_OFF;
		leds( ledsState ); // медленно зажигаем 
		
		TH1=power_h2;
		TL1=power_l2;
	}else{//turn off delay
		level=LEVEL_ON;
		//leds( ledsState ); 
		
		TH1=power_h1;
		TL1=power_l1;
	}
	
	if(get_ms()>=next_frame) {
		next_frame=get_ms()+FRAME_DELAY;
		set_power(get_power()+10);
		process_step();
		ledsState = 0;
		for(i=0; i<8; i++)
		{
			if(current_step[i]>0)
				ledsState = ledsState | (1 << i);
		}
		if(get_power==100)
		{
			set_power(0);
			organize_step();
		}
	}	
	// 	if( direction==UP ){
	// 		set_power(get_power()+10);
			
	// 		if( get_power()==100 ){
	// 			direction=DOWN;
	// 		}
	// 	}else{
	// 		set_power(get_power()-10);
					
	// 		if( get_power()==0 ){
	// 			direction=UP;
	// 		}
	// 	}
	// }
}