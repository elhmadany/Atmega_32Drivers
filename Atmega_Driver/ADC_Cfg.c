/*
 * ADC_Cfg.c
 *
 *  Created on: ٠٥‏/٠٧‏/٢٠١٨
 *      Author: mohammed
 */
#include "ADC.h"
#include "ADC_Cfg.h"

const ADC_ConfigType ADC_ConfigParam [ADC_CHANNELS_NUM] ={
		{0x00,FreeRunning,_4_CLK,SingleEndeded,LEFT,AREFpin}
};
