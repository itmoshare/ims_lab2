#include "animation.h"
#include "led.h"
#include "system_timer.h"

//»спользует таймер T1 дл¤ Ў»ћ

//¤ркость светодиодов
u8 power=0;
//LEVEL_ON
u8 power_l1=0;
u8 power_h1=0;
//LEVEL_OFF
u8 power_l2=0;
u8 power_h2=0;
u8 cur = 0x01;
u8 counter;
u8 level;
//величивать или уменьшать ¤ркость
u8 direction;
//врем¤ следующего кадра
time next_frame;

const u8 LEVEL_ON=0xF0;
const u8 LEVEL_OFF=0x0F;

const u8 UP=0xFF;
const u8 DOWN=0x00;
const time FRAME_DELAY=9000;

void T1_ISR( void ) __interrupt ( 1 );

void initialize_animation() {
	TMOD|=0b00010000; //Timer1, внутренний генератор частоты, 16- бит счетчик
	SetVector( 0x201B, (void *)T1_ISR );
}

void enable_animation(){
	direction=UP;
	next_frame=get_ms()+FRAME_DELAY;
	//set_power(0);
	cur=1;
	level=LEVEL_ON;
	//leds( level );
	counter = 0;
	TH1 = 0xF8;
	TL1= 0xFF;
	
	// TH1=power_h1;
	// TL1=power_l1;
	
	ET1=1;       //включаем прерывание от таймера 1
	TR1=1;      //разрешаем счет таймера 1
}

void disable_animation(){
	ET1=0;
	TR1=0;
}



u8 powers[8] ={0,0,6,5,4,3,2,1}; //{ 0,0,1,2,4,6,8,10};

void movePower() {
	u8 last = powers[7];
	u8 i;
    for(i=7; i > 0; i--) 
    {
        powers[i] = powers[i-1];
    }
    powers[0] = last;
}

void set_power(u8 k, u8 power){
	//u16 power1,power2;
	
	TR1=0;//дабы не сработал T1_ISR и не записал неправильные данные
	
	powers[k]=power;
	
	
	// power=new_power;
	// power1=-(100*power+1);
	// power2=-(100*(100-power)+1);
	
	// power_l1=power1&0xFF;
	// power_h1=power1>>8;
	
	// power_l2=power2&0xFF;
	// power_h2=power2>>8;
	
	TR1=1;
}

u8 get_power() {
	return power;
}



void updateLeds() 
{
	u8 i =0;
	u8 lights = 0;
	u8 mask;
	for(i=0; i<8; i++)
	{
		if(counter % powers[i] == 0) {
			lights = lights | (1 << i);
		}
	}
	leds(lights);
	//leds(counter);
}

void T1_ISR( void ) __interrupt ( 1 ){

	TR1=0;
	if(get_ms()>=next_frame)
	{
		// leds(cur);
		// cur = cur << 1;
		movePower();
		counter = 0;
		next_frame=get_ms()+FRAME_DELAY;
	}
	counter++;
	updateLeds();
	TH1 = 0xF8;
	TL1= 0xFF;
	TR1=1;
	
	// if( level==LEVEL_ON ){
		// level=LEVEL_OFF;
		// leds( level );
		
		// TH1=power_h2;
		// TL1=power_l2;
	// }else{
		// level=LEVEL_ON;
		// leds( level );
		
		// TH1=power_h1;
		// TL1=power_l1;
	// }
	
	// if(get_ms()>=next_frame) {//пора сменить кадр анимации
		// next_frame=get_ms()+FRAME_DELAY;
		
		// if( direction==UP ){
			// set_power(get_power()+10);
			
			// if( get_power()==100 ){
				// direction=DOWN;
			// }
		// }else{
			// set_power(get_power()-10);
					
			// if( get_power()==0 ){
				// direction=UP;
			// }
		// }
	// }
}


// #include "animation.h"
// #include "led.h"
// #include "system_timer.h"

// //���������� ������ T1 ��� ���

// //������� �����������
// u8 power=0;
// //LEVEL_ON
// u8 power_l1=0;
// u8 power_h1=0;
// //LEVEL_OFF
// u8 power_l2=0;
// u8 power_h2=0;

// u8 level;
// u8 ledsState;
// u8 stepCounter;
// //���������� ��� ��������� �������
// u8 direction;
// //����� ���������� �����
// time next_frame;

// u8 i;//global counter

// const u8 LEVEL_ON=0xF0;
// const u8 LEVEL_OFF=0x0F;

// const u8 UP=0xFF;
// const u8 DOWN=0x00;
// const time FRAME_DELAY=1000;

// void T1_ISR( void ) __interrupt ( 1 );

// void initialize_animation() {
	// TMOD|=0b00010000; 
	// SetVector( 0x201B, (void *)T1_ISR );
// }

// void enable_animation(){
	// direction=UP;
	// next_frame=get_ms()+FRAME_DELAY;
	// set_power(0);
	// ledsState = 0x01;
	// level=LEVEL_ON;
	// leds( ledsState );
	
	// TH1=power_h1;
	// TL1=power_l1;
	
	// ET1=1;       
	// TR1=1;      
// }

// void disable_animation(){
	// ET1=0;
	// TR1=0;
// }

// void set_power(u8 new_power){
	// u16 power1,power2;
	// TR1=0;
	
	// power=new_power;
	// power1=-(100*power+1);
	// power2=-(100*(100-power)+1);
	
	// power_l1=power1&0xFF;
	// power_h1=power1>>8;
	
	// power_l2=power2&0xFF;
	// power_h2=power2>>8;
	
	// TR1=1;
// }

// u8 get_power() {
	// return power;
// }

// u8 current_step[8] = {10, 0, 0, 2, 4, 6, 8, 9};
// u8 current_position[8] = {10, 0, 0, 2, 4, 6, 8, 9};
// void process_step()
// {
    // for(i=0; i<8; i++)
        // if(current_step[i] > 0)
            // current_step[i]--;
// }

// void organize_step() 
// {
    // u8 last = current_position[7];
    // for(i=7; i >= 0; i--) 
    // {
        // current_position[i] = current_position[i-1];
        // current_step[i] = current_position[i];
    // }
    // current_position[0] = last;
    // current_step[0] = current_position[0];
// }

// void T1_ISR( void ) __interrupt ( 1 ){


	// if( level==LEVEL_ON ){ //light up delay
		// level=LEVEL_OFF;
		// leds( ledsState ); // медленно зажигаем 
		
		// TH1=power_h2;
		// TL1=power_l2;
	// }else{//turn off delay
		// level=LEVEL_ON;
		// //leds( ledsState ); 
		
		// TH1=power_h1;
		// TL1=power_l1;
	// }
	
	// if(get_ms()>=next_frame) {
		// next_frame=get_ms()+FRAME_DELAY;
		// set_power(get_power()+10);
		// process_step();
		// ledsState = 0;
		// for(i=0; i<8; i++)
		// {
			// if(current_step[i]>0)
				// ledsState = ledsState | (1 << i);
		// }
		// if(get_power()==100)
		// {
			// set_power(0);
			// organize_step();
		// }
	// }	
	// // 	if( direction==UP ){
	// // 		set_power(get_power()+10);
			
	// // 		if( get_power()==100 ){
	// // 			direction=DOWN;
	// // 		}
	// // 	}else{
	// // 		set_power(get_power()-10);
					
	// // 		if( get_power()==0 ){
	// // 			direction=UP;
	// // 		}
	// // 	}
	// // }
// }