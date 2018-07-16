/*
 * Timer_Cfg.c
 *
 *  Created on: ٠٨‏/٠٧‏/٢٠١٨
 *      Author: mohammed
 */
#include "Timer.h"
#include "Timer_Cfg.h"
const TIMER_ConfigType TIMER_ConfigParam [TIMERS_NUM] ={
		{Timer0,CTCmode,PRESCALING_CLK1024,254,OutputCompareInterruptEnable,OverflowInterruptEnable,ToggleOC0onCompareMatch}
};
