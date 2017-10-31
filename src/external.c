#include <timer.h>
#include <aduc812.h>
#include <stdint.h>
#include <led.h>
#include <max.h>


int current_mode = ANIM;


void external_handler() interrupt(1)
{
	if(current_mode = COUNT)
		current_mode = ANIM;
	else
		current_mode = COUNT;
}