/*
 * ADC.c
 *
 *  Created on: ٠٥‏/٠٧‏/٢٠١٨
 *      Author: mohammed
 */
#include "ADC.h"
#include "ADC_Cfg.h"
#include "stdint.h"
typedef volatile uint8_t* const ADC_RegAddType;
#define ADMUX   *(ADC_RegAddType)(0x27)
#define ADCSRA  *(ADC_RegAddType)(0x26)
#define ADCH    *(ADC_RegAddType)(0x25)
#define ADCL    *(ADC_RegAddType)(0x24)
#define SFIOR   *(ADC_RegAddType)(0x50)
/**********************ADMUX_ControlRegister************************/
#define REFS1 	7u
#define REFS0 	6u
#define ADLAR 	5u
#define MUX4 	4u
#define MUX3	3u
#define MUX2 	2u
#define MUX1	1u
#define MUX0	0u
/**********************ADCSRA_ControlRegister************************/
#define ADEN 	7u
#define ADSC 	6u
#define ADATE 	5u
#define ADIF 	4u
#define ADIE	3u
#define ADPS2 	2u
#define ADPS1	1u
#define ADPS0	0u
/**********************SFIOR_ControlRegister************************/
#define ADTS2 	7u
#define ADTS1 	6u
#define ADTS0 	5u
/****************************************************************/
#define ADC_MAX_CH_NUM  7u
#define ADC_STATE_INIT               0x01
#define ADC_STATE_COMPLETE_CONVERGE  0x03
static uint8_t ADC_ChannelState[ADC_CHANNELS_NUM] = {0};

ADC_ChkType ADC_Init(void)
{
	ADC_ChkType RetVar;
	uint8_t LoopIndex;
	uint8_t Error = 0;
	const ADC_ConfigType* CfgPtr;
	for(LoopIndex = 0; (LoopIndex < ADC_CHANNELS_NUM) && (Error == 0); LoopIndex ++)
		{
		CfgPtr =&ADC_ConfigParam[LoopIndex];
			    if(ADC_ConfigParam[LoopIndex].ChannelId<= ADC_MAX_CH_NUM)
			    {
				/*DATA CONTROL*/
							ADMUX|=(CfgPtr->Vref_Source<<REFS0)|(CfgPtr->ReadFrom<<ADLAR);
							if(CfgPtr->ModeOperate==SingleEndeded)
							{
								ADMUX|=(CfgPtr->ChannelId);
/*to prescale the input frequcene /8>>ADC must operate in range 50-200Khz to get the maximun accuracy  we used freq 1M so 1M/8=125kHz*/
							ADCSRA|=(1<<ADEN)|(1<<ADATE)|(CfgPtr->Prescaler);
								SFIOR|=(CfgPtr->TriggerSource<<ADTS0);
								RetVar = ADC_OK;
						   }
							else if(CfgPtr->ModeOperate==Differential){
								/*write your code here*/
							                                          }
#if(ADC_USE_INT_TO_HANDLE==1)
							ADCSRA|=(1<<ADIE);
#endif

			    }
			     else
						{
							/*Invalid ModeOperate*/
							RetVar = ADC_NOK;
							 Error = 1;
						}
				ADC_ChannelState[LoopIndex] = ADC_STATE_INIT;

			}
	return RetVar;}

ADC_ChkType ADC_GetConversionResult(uint16_t* ResultPtr,uint8_t ChannelId){
		ADC_ChkType RetVar;
			const ADC_ConfigType* CfgPtr;
					CfgPtr = &ADC_ConfigParam[ChannelId];
	if (((ADC_ChannelState[ChannelId] == ADC_STATE_INIT )|(ADC_ChannelState[ChannelId]== ADC_STATE_COMPLETE_CONVERGE))
			&& (ChannelId <= ADC_MAX_CH_NUM)){
		ADCSRA |=(1<<ADSC);
		while(!(ADCSRA &(1<<ADIF)));
			ADCSRA |=(1<<ADIF);
			/*clear ADIF by write'1' to it */

	switch (CfgPtr->ReadFrom)
									{
										case RIGHT:
										{
											* ResultPtr = (ADCH<<8|(CfgPtr->ChannelId));

										}break;

										case LEFT:
										{
											* ResultPtr = (ADCH<<2|(CfgPtr->ChannelId)>>6);

										}break;
									}


				}
				else
				{
				/*Invalid ChannelId*/
				RetVar = ADC_NOK;
				}
	ADC_ChannelState[ChannelId]=ADC_STATE_COMPLETE_CONVERGE;
		return RetVar;
	}







