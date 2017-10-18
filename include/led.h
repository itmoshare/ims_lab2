#ifndef __LED__H
#define __LED__H

void led( unsigned char n, unsigned char on );
void leds( unsigned char on );
bool is_step_doone();
void process_step(char step);
void organize_step();

#endif //__LED__H
