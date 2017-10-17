#include <timer.h>
#include <aduc812.h>
#include <stdint.h>
#include <led.h>
#include <max.h>

void count_handler() interrupt(1)
{
	while(true)
		leds(TH0); //output counter value	
}
