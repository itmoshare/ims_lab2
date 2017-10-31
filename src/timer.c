#include <timer.h>
#include <aduc812.h>
#include <stdint.h>
#include <led.h>
#include <max.h>
#include <external.h>
#include <counter.h>

unsigned long ms_count;
unsigned long last_output_time;

void delay ( unsigned long ms ) {
    volatile unsigned long i, j;
    for( j = 0; j < ms; j++ ) {
        for( i = 0; i < 50; i++ );
    }
}

unsigned long GetMsCounter(void)
{
	return ms_count;
}

unsigned long DTimeMs(unsigned long t0)
{
	return ms_count - t0;
}

void DelayMs(unsigned long t)
{
	unsigned long target = t + ms_count;
	while(ms_count != target);
}

void set_vector(uint8_t xdata* address, void* vector)
{
	unsigned char xdata* tmp;
	
	*address = 0x02;
	
	tmp = (unsigned char xdata*)(address + 1);
	*tmp = (unsigned char)((unsigned short)vector >> 8);
	++tmp;
	
	*tmp = (unsigned char)vector;
}



void animation_handler() interrupt(3)
{
	ms_count++; //увеличиваем счетчик милисекунд
	
	if(current_mode == COUNT)
	{
		leds(TH0);
		//Без делэя
		TH1 = 0xFF;
		TH0 = 0xFF;
	} else {
		if(!is_step_doone())
			process_step(); 
		else 
			organize_step();
		//небольшой делэй
		TH1 = 0xF0;
		TL1 = 0xF0;
	}
}

void InitTimer()
{
	ms_count = 0;
	//очищаем счетчик
	TH0 = 0x00;
	TL0 = 0x00;
	//таймер в максимум, чтобы произошло прерывание
	TH1 = 0xF0;
	TL1 = 0xF0;
	//настройка режимов таймеров 
	TCON = 0x51; //+edge sensetive внешенего прерывания (to clear interupt flag)
	TMOD = 0x51;
	//устанавливаем векторы прерываний
	set_vector(0x201B, (void*)animation_handler);
	set_vector(0x2003, (void*)external_handler);

	//enable interupts IE = 0x80
	EA = 1;
	ET1 = 1;
	EX0 = 1;

	//priority int0 - high, t1 -low IP
	PX0 = 1;
}