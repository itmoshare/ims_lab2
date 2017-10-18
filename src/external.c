#include <timer.h>
#include <aduc812.h>
#include <stdint.h>
#include <led.h>
#include <max.h>

#define ANIM 1
#define COUNT 0

int current_mode = ANIM;

char set_bit(char* value, int bit_number) 
{
	value ^= 1 << bit_number;
	return value;
}

char clear_bit(char* value, int bit_number) 
{
	value &= ~(1 << bit_number);
	return value;
}

void external_handler() interrupt(2)
{
	//отключаем таймер - выводим значение счетчика, или наоборот по внешнему прерыванию

	if(current_mode = COUNT)
	{
		current_mode = ANIM;
		TCON = set_bit(TCON, 6);
	}
	else
	{	
		current_mode = COUNT;
		TCON = clear_bit(TCON, 6);
		while(true)
			leds(TH0);
	}
}