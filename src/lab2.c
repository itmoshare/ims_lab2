#include "aduc812.h"
#include "led.h"
#include "max.h"

#define ANIM1_DIP_VALUE 0xBB
#define ANIM2_DIP_VALUE 0x44

#define ANIM1_START 0x07
#define ANIM1_END 0xE0
#define ANIM2_START 0x81
#define ANIM2_END 0x18

unsigned char cur_animation = 0;
unsigned char cur_animation_state = 0;
unsigned char direction = 0;

void delay ( unsigned long ms ) {
	volatile unsigned long i, j;
	for( j = 0; j < ms; j++ ) {
		for( i = 0; i < 50; i++ );
	}
}

unsigned char get_dip() {
	return read_max(EXT_LO);
}

static void anim1_step() {
    if (direction)
        cur_animation_state >>= 1;
	else
		cur_animation_state <<= 1;
}

static void anim2_step() {
    if (direction)
        cur_animation_state = ((cur_animation_state & 0xF0) >> 1) | ((cur_animation_state & 0x0F) << 1);
    else
        cur_animation_state = ((cur_animation_state & 0xF0) << 1) | ((cur_animation_state & 0x0F) >> 1);
}

void main (void) {
	while(1) {
		unsigned char dip_value = get_dip();
		
		switch (dip_value)
		{
			case ANIM1_DIP_VALUE:
			    if (cur_animation != 1) {
			        cur_animation = 1;
			        cur_animation_state = ANIM1_START;
			    }
			    else {
			        if (!(cur_animation_state & ANIM1_END)) {
			            direction = 1;
			        } 
			        else if (!(cur_animation_state & ANIM1_START)) {
			            direction = 0;
			        }
			        anim1_step();
			    }
			    break;
			case ANIM2_DIP_VALUE:
			    if (cur_animation != 2) {
			        cur_animation = 2;
			        cur_animation_state = ANIM2_START;
			    }
			    else {
			        if (!(cur_animation_state & ANIM2_END)) {
			            direction = 1;
			        } 
			        else if (!(cur_animation_state & ANIM2_START)) {
			            direction = 0;
			        }
			        anim2_step();
			    }
			    break;
			default:
			    if (cur_animation != 0) {
			        cur_animation = 0;
			        cur_animation_state = dip_value;
			    }
			    break;
		}
		leds(cur_animation_state);
		delay(100);
	}
}
