/*
 * ADC.h
 *
 *  Created on: ٠٥‏/٠٧‏/٢٠١٨
 *      Author: mohammed
 */
#include "ADC_Cfg.h"
#include <stdint.h>

#ifndef ADC_H_
#define ADC_H_
void (*ADC_FnPtr)(void);
typedef enum {ADC_OK=0,ADC_NOK}ADC_ChkType;

typedef struct
{
	/*Analog Pin ADC Channel*/
	uint8_t  ChannelId;
	/*Trigger control */
		uint8_t TriggerSource;
    /*Average number of samples for conversion result*/
		uint8_t Prescaler;
	/*SelectModeSingleEndededorDifferential*/
		uint8_t ModeOperate;
	/*Read_Adjusted*/
		uint8_t ReadFrom;
	/*Voltage Reference Source*/
		uint8_t  Vref_Source;
	}ADC_ConfigType;
 enum
	{
		FreeRunning,AnalogComparator,ExternalInterruptRequest0,TimerCounter0CompareMatch,TimerCounter0Overflow,
		TimerCounter1CompareMatchB,TimerCounter1Overflow,TimerCounter1CaptureEvent
	};
	 enum
		{SingleEndeded,Differential};
	/*ADC Prescaler values and clock frequecy must be more than 50kHz and less than 200kHz */
	 enum{_2CLK,_2_CLK,_4_CLK,_8_CLK,_16_CLK,_32_CLK,_64_CLK,_128_CLK};
	/*Left Adjusted OR Right Adjusted*/
	 enum{RIGHT,LEFT};
	/*Voltage Reference Source*/
	 enum{AREFpin,AVCCwithExternalCapacitoratAREFpin,Internal2_56V};
	extern const ADC_ConfigType ADC_ConfigParam [ADC_CHANNELS_NUM];

/*A function to initialize module with config parameters*/
ADC_ChkType ADC_Init(void);
/*A Function to request start single conversion*/
ADC_ChkType ADC_StartSingleConversion(uint8_t ChannelID);
/*A function to Read Conversion result*/
ADC_ChkType ADC_GetConversionResult(uint16_t* ResultPtr,uint8_t ChannelId);
/* A function to stop an ongoing conversion*/
ADC_ChkType ADC_StopCrtConversion(uint8_t ChannelId);

#endif /* ADC_H_ */
