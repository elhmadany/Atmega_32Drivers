/*
 * LCD.h
 *
 *  Created on: ٠٦‏/٠٧‏/٢٠١٨
 *      Author: mohammed
 */
#include "stdint.h"

#ifndef LCD_H_
#define LCD_H_

void LCD_Init(void);
void LCD_Clear(void);
void LCD_DispChar(char Data);
void LCD_DispString(char* StrPtr);
void LCD_GotoXy(uint8_t Postion,uint8_t Line);


#endif /* LCD_H_ */
