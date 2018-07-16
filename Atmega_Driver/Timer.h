/*
 * Timer.h
 *
 *  Created on: ٠٨‏/٠٧‏/٢٠١٨
 *      Author: mohammed
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "stdint.h"
#include "Timer_Cfg.h"


 void (*Timer0OverFlow_CallBackPtrType)(void);
 void (*Timer0OutputCompare_CallBackPtrType)(void);

/*A structure type to contain all the required configuration*/
typedef struct
{
uint8_t TimerNUM;                   /*Timer0,Timer1,Timer2*/
uint8_t Timer0Mode;                /*NormalMode,PhaseCorrectPWM,CTCmode,FastPWM This is for Timer 0*/
uint8_t PRESCALAR;
uint8_t Compare_ValueTimer0;             /*This is for Timer 0 only*/
uint8_t CompareInt_EnableTimer0;         /*This is for Timer 0 only*/
uint8_t OverFlowInt_EnableTimer0;        /*This is for Timer 0 only*/
uint8_t Action_OC0pin;             /*This is for Timer 0 only*/


}TIMER_ConfigType;
typedef enum {TIMER_OK =0, TIMER_NOK}TIMER_ChkType;
enum{NormalMode,PhaseCorrectPWM,CTCmode,FastPWM};
enum{Timer0,Timer1,Timer2};
enum{OutputCompareInterruptEnable=1,OverflowInterruptEnable=1};       /*Interrupt Enable This is for Timer 0 only*/
enum{OC0disconnected,ToggleOC0onCompareMatch,ClearOC0onCompareMatch,SetOC0onCompareMatch};      /*OC0 Pin This is for Timer 0 only*/

enum
{
	NO_CLK_SRC = 0, //Timer Disable
	NO_PRESCALING = 1,
	PRESCALING_CLK8 = 2,
	PRESCALING_CLK64 = 3,
	PRESCALING_CLK256 = 4,
	PRESCALING_CLK1024 = 5,
	EXT_CLK_FALLING=6,
	EXT_CLK_RISING = 7
};
extern const TIMER_ConfigType TIMER_ConfigParam[TIMERS_NUM];
/*A function to initialize module with config parameters*/
TIMER_ChkType TIMER0_Init(void);
TIMER_ChkType StartTimer0OneShot(uint8_t TimeNUM);
void Set_ISR_CallbackCompare_TIMER0(void(*ptr)(void));
void Set_ISR_CallbackOverFlow_TIMER0(void(*ptr)(void));
void InterruptServiceCompare_TIMER0(void);
void InterruptServiceOverFlow_TIMER0(void);
#endif /* TIMER_H_ */
