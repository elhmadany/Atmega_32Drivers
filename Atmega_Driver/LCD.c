/*
 * LCD.c
 *
 *  Created on: ٠٦‏/٠٧‏/٢٠١٨
 *      Author: mohammed
 */
#include <stdint.h>
#include "GPIO.h"
#include "LCD_Cfg.h"
static void Delay_US(uint16_t DealyUs);
static void SendCmd(uint8_t CmdID);
static void SendData(uint8_t DataID);
void LCD_Init(void)
{
    SendCmd(0x33);
    SendCmd(0x32);
    SendCmd(0x28);
    SendCmd(0x0E);
    SendCmd(0x06);
    SendCmd(0x01);

}
void LCD_Clear(void)
{
    SendCmd(0x01);

}
void LCD_DispChar(char Data)
{
    SendData(Data);
}
void LCD_DispString(char* StrPtr)
{
while(*StrPtr !='\0'){
    SendData(*StrPtr);
      StrPtr++;}
}
void LCD_GotoXy(int x,int y)
{
if(y==0)
    //SendCmd(0x80+x);

    SendCmd(0x80+x);
if(y==1)
   // SendCmd(0xc0+x);

    SendCmd(0xc0+x);

}
static void Delay_US(uint16_t DelayUs)
{
   volatile uint8_t InnerLoop;
    volatile uint16_t Loop;
    for(Loop = 0; Loop < DelayUs; Loop++)
    {
    for (InnerLoop =0; InnerLoop < 4; InnerLoop++);
    }
}
static void SendCmd(uint8_t CmdID)
{
    uint8_t Temp;
    GPIO_Write(RS_GROUP_ID, 0x00);
    GPIO_Write(RW_GROUP_ID, 0x00);
    Temp = CmdID >> 4;
    GPIO_Write(DATA_GROUP_ID, Temp );
    GPIO_Write(EN_GROUP_ID, 0xff);
    Delay_US(100);
    GPIO_Write(EN_GROUP_ID, 0x00);
    Temp = CmdID & 0x0f;
    GPIO_Write(DATA_GROUP_ID, Temp << DATA_PIN_NUM);
    GPIO_Write(EN_GROUP_ID, 0xff);
    Delay_US(100);
    GPIO_Write(EN_GROUP_ID, 0x00);
    Delay_US(2000);

}
static void SendData(uint8_t DataID)
{
    uint8_t Temp;
        GPIO_Write(RS_GROUP_ID, 0xff);
        GPIO_Write(RW_GROUP_ID, 0x00);
        Temp = DataID >> 4;
        GPIO_Write(DATA_GROUP_ID, Temp << DATA_PIN_NUM);
        GPIO_Write(EN_GROUP_ID, 0xff);
        Delay_US(100);
        GPIO_Write(EN_GROUP_ID, 0x00);
        Temp = DataID & 0x0f;
        GPIO_Write(DATA_GROUP_ID, Temp << DATA_PIN_NUM);
        GPIO_Write(EN_GROUP_ID, 0xff);
        Delay_US(100);
        GPIO_Write(RS_GROUP_ID, 0xff);
        GPIO_Write(EN_GROUP_ID, 0x00);
        Delay_US(100);
}











