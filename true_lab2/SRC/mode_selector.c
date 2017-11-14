#include "mode_selector.h"

//���������� ������ INT0 ��� ������������ ������

u8 mode;

void INT0_ISR( void ) __interrupt ( 5 ); //1

void initialize_mode_selector() {
	IT1=1;//INT0 (1)
	EX1=1;
	SetVector( 0x2013, (void *)INT0_ISR ); // 1
}

void INT0_ISR( void ) __interrupt ( 5 ){
	if( mode==MODE_COUNTER ) {
		mode=MODE_ANIMATION;
	}else{
		mode=MODE_COUNTER;
	}
}