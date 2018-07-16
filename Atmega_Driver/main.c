/*
 * main.c
 *
 *  Created on: ١٨‏/٠٨‏/٢٠١٧
 *      Author: mohammed
 */

//#include "LCD.h"
#include "GPIO.h"
#include "GPIO_Cfg.h"
#include "Timer.h"
#include "Timer_Cfg.h"
#include "avr/interrupt.h"

	int main(void){
	    GPIO_Init();
	    //LCD_Init();
	    TIMER0_Init();
		//UART_Init();
		//GPIO_SetInterruptEvent(1,TheRisingEdge,INTERRUPT0);
		//Set_ISR_Callback(InterruptService);
		StartTimer0OneShot(0);

		Set_ISR_CallbackCompare_TIMER0(InterruptServiceCompare_TIMER0);
		Set_ISR_CallbackOverFlow_TIMER0(InterruptServiceOverFlow_TIMER0);

		sei();
	  //  LCD_GotoXy(5,1);
	//	LCD_DispString("Hello World fGTGHWT");

		while(1){

	            }
	   return 0;
	            }

/*ISR(INT0_vect)
{

GPIO_FnPtrType();
}
*/
	ISR(TIMER0_COMP_vect){
		Timer0OutputCompare_CallBackPtrType();
	}
	ISR(TIMER0_OVF_vect){
		Timer0OverFlow_CallBackPtrType();
	}
