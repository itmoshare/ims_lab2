#include <aduc812.h>

#include <timer.h>

void main()
{
	//Алгоритм: внешнее прерывание - больший приоритет, устанавливеь глабольну переменную
	// режима работы.
	// прерыания таймера в зависимости от режима зажигает диоды
	InitTimer();

	while (1);
}