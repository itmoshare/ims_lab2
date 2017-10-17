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
	//disable timer 1 enable timer 0 and oposite

	if(current_mode = COUNT)
	{
		current_mode = ANIM;
		TCON = set_bit(TCON, 6);
		TCON = clear_bit(TCON, 4);
	}
	else
	{	
		current_mode = COUNT;
		TCON = set_bit(TCON, 4);
		TCON = clear_bit(TCON, 6);
	}
}