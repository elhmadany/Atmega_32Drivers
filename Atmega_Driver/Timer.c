/*
 * Timer.c
 *
 *  Created on: ٠٨‏/٠٧‏/٢٠١٨
 *      Author: mohammed
 */
#include "Timer.h"
#include "Timer_Cfg.h"
#include "GPIO.h"
#define DDRB   *((volatile uint8_t* const)0X37)  /*OC0 PIN Direction*/
typedef volatile uint8_t* const TIMER_RegAddType;
#define OCR0        *(TIMER_RegAddType)(0X5C)
#define TCCR0       *(TIMER_RegAddType)(0X53)
#define TCNT0       *(TIMER_RegAddType)(0X52)
#define TIMSK       *(TIMER_RegAddType)(0X59)
#define SFIOR       *(TIMER_RegAddType)(0X50)
#define TCCR1A      *(TIMER_RegAddType)(0X4F)
#define TCCR1B      *(TIMER_RegAddType)(0X4E)
#define TCNT1H      *(TIMER_RegAddType)(0X4D)
#define TCNT1L      *(TIMER_RegAddType)(0X4C)
#define OCR1AH      *(TIMER_RegAddType)(0X4B)
#define OCR1AL      *(TIMER_RegAddType)(0X4A)
#define OCR1BH      *(TIMER_RegAddType)(0X49)
#define OCR1BL      *(TIMER_RegAddType)(0X48)
#define ICR1H       *(TIMER_RegAddType)(0X47)
#define ICR1L       *(TIMER_RegAddType)(0X46)
#define TCCR2       *(TIMER_RegAddType)(0X45)
#define TCNT2       *(TIMER_RegAddType)(0X44)
#define OCR2        *(TIMER_RegAddType)(0X43)
/**********************TCCR0_ControlRegister************************/
#define FOC0 	    7u
#define WGM00    	6u
#define COM01 	    5u
#define COM00 		4u
#define WGM01		3u
#define CS02 		2u
#define CS01		1u
#define CS00	    0u
/**********************TCCR1A_ControlRegister************************/
#define COM1A1 	    7u
#define COM1A0    	6u
#define COM1B1 	    5u
#define COM1B0 		4u
#define FOC1A		3u
#define FOC1B 		2u
#define WGM11		1u
#define WGM10	    0u
/**********************TCCR1B_ControlRegister************************/
#define ICNC1 	    7u
#define ICES1    	6u
#define WGM13 		4u
#define WGM12		3u
#define CS12 		2u
#define CS11		1u
#define CS10	    0u
/**********************TCCR2_ControlRegister************************/
#define FOC2 	    7u
#define WGM20    	6u
#define COM21 	    5u
#define COM20 		4u
#define WGM21		3u
#define CS22 		2u
#define CS21		1u
#define CS20	    0u
/**********************TIMSK_ControlRegister************************/
#define OCIE0		1u
#define TOIE0	    0u
/*********************************************************************/

#define TIMERS_MAX_NUM                  3u
#define TIMER_STATE_INIT                0x01
static uint8_t TIMER_State[TIMERS_NUM] = {0};
TIMER_ChkType TIMER0_Init(void){
	TIMER_ChkType RetVar;
	uint8_t LoopIndex;
	uint8_t Error = 0;
	const TIMER_ConfigType* CfgPtr;
	for(LoopIndex = 0; (LoopIndex < TIMERS_NUM) && (Error == 0); LoopIndex ++)
		{
		CfgPtr =&TIMER_ConfigParam[LoopIndex];
		 if(TIMER_ConfigParam[LoopIndex].TimerNUM <= TIMERS_MAX_NUM &&CfgPtr->TimerNUM==Timer0)
		 {
			if(TIMER_ConfigParam[LoopIndex].Timer0Mode == NormalMode){
				TCCR0|=(0<<WGM01)|(0<<WGM00)|(CfgPtr->PRESCALAR);

			}
		    if(TIMER_ConfigParam[LoopIndex].Timer0Mode == CTCmode){
				TCCR0|=(1<<WGM01)|(0<<WGM00)|(CfgPtr->PRESCALAR);

		    }
			if(TIMER_ConfigParam[LoopIndex].Timer0Mode == FastPWM){
				TCCR0|=(1<<WGM01)|(1<<WGM00)|(CfgPtr->PRESCALAR);

			}
			if(TIMER_ConfigParam[LoopIndex].Timer0Mode == PhaseCorrectPWM){
				TCCR0|=(0<<WGM01)|(1<<WGM00)|(CfgPtr->PRESCALAR);
              }


         }


		 else
		 		{
		 					/*Invalid ModeOperate*/
		 				RetVar = TIMER_NOK;
		 				Error = 1;
		 		}
		 TIMER_State[LoopIndex] = TIMER_STATE_INIT;
		}

return RetVar;

}
TIMER_ChkType StartTimer0OneShot(uint8_t TimeNUM){
	TIMER_ChkType RetVar;
		const TIMER_ConfigType* CfgPtr;
			CfgPtr = &TIMER_ConfigParam[TimeNUM];
if(TimeNUM==0&&TIMER_State[TimeNUM]==TIMER_STATE_INIT ){
	if(TimeNUM==0){
if(TIMER_USE_INT_OVERFLOW_TO_HANDLE==1&&CfgPtr->Timer0Mode==NormalMode)
	TIMSK|=CfgPtr->OverFlowInt_EnableTimer0<<TOIE0;

if(TIMER_USE_INT_COMPARE_TO_HANDLE==1&&CfgPtr->Timer0Mode==CTCmode){
TIMSK|=CfgPtr->CompareInt_EnableTimer0<<OCIE0;
		TCNT0 = 0;
		OCR0&=CfgPtr->Compare_ValueTimer0;
	    TCCR0|=CfgPtr->Action_OC0pin<<COM00;
	    DDRB |=1<<3;
	}
	//if(FastPWM)

	//	if( PhaseCorrectPWM)




	}
}
	else
			{
			 /*Invalid ModeOperate*/
			 RetVar = TIMER_NOK;
			 }
	return RetVar;
}


void InterruptServiceCompare_TIMER0(void){
	GPIO_Write(0,0xff);
}
void InterruptServiceOverFlow_TIMER0(void){
	GPIO_Write(1,0xff);
}
void Set_ISR_CallbackCompare_TIMER0(void(*ptr)(void)){
	Timer0OutputCompare_CallBackPtrType=ptr;
}
void Set_ISR_CallbackOverFlow_TIMER0(void(*ptr)(void)){
	Timer0OverFlow_CallBackPtrType=ptr;
}

