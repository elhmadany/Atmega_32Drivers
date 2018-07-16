#ifndef UART_H
#define UART_H
#include <stdint.h>
#include "UART_cfg.h"

typedef void (*UART_CallBackPtrType)(void);
/*A structure type to contain all the required configuration*/
typedef struct
{
	uint32_t BaudRate;
	/*Channel ParityType*/
	uint8_t Parity;
	/*Channel Stop Bits*/
	uint8_t StopBits;
	/*Channel frame Data Size*/
	uint8_t DataSize;
	/*ModeSelect*/
	uint8_t ModeSelect;
	/*Channel TxDone Callback function*/
	//UART_CallBackPtrType TxDoneCallBackPtr;
	/*Channel RxDone CallBack function*/
	//UART_CallBackPtrType RxDoneCallBackPtr;
}UART_ConfigType;
extern const UART_ConfigType UART_ConfigParam[UART_CHANNELS_NUM];
typedef enum {UART_OK =0, UART_NOK}UART_ChkType;
typedef enum {Asynchronous =0, Synchronous}ModeSelect;
typedef enum {PARITY_DISABLED =0, PARITY_ODD = 2, PARITY_EVEN = 3} UART_ParityType;
typedef enum {STOP_ONE =0, STOP_TWO}UART_StopBitsType;
typedef enum {DATA_5=0,DATA_6=1,DATA_7=2,DATA_8=3,DATA_9=7}UART_DataSizeType;
/*A function to initialize all the UART channels with the configuration set in UART_Cfg*/
UART_ChkType UART_Init(void);
/*A function to Request a silent transmission*/
UART_ChkType UART_StartTransmission ( uint8_t TxBuff,uint8_t ChannelId);
/*A function to Request a silent transmission*/
void UART_Start_StringTransmission ( char *Tx_String,uint8_t ChannelId);
/*A function to stop a current transmission*/
UART_ChkType UART_StopCrntTransmission(uint8_t ChannelId);
/*A function to get number of transmitted bytes in an ongoing transmission*/
UART_ChkType UART_GetNumberOfTxBytes(uint8_t ChannelID,uint8_t* NumPtr);
/*A function to Request a silent Reception*/
UART_ChkType UART_StartReception(uint8_t* RxBuffPtr,uint8_t ChannelId);
/*A function to stop a current Reception*/
UART_ChkType UART_StopCrntReception(uint8_t ChannelId);
/*A function to Get the number of received bytes in an ongoing reception*/
UART_ChkType UART_GetNumberOfRxBytes(uint8_t ChannelID,uint8_t* NumPtr);

#endif



