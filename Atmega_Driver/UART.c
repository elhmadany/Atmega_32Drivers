/*
 * CFile1.c
 *
 * Created: 4/8/2017 9:19:27 AM
 *  Author: 2017
 */ 
#include "UART.h"
#include "UART_cfg.h"
#include <stdint.h>
//#define  F_CPU 8000000ul

//#include <avr/interrupt.h>
typedef volatile uint8_t* const UART_RegAddType;
#define UDR   *(UART_RegAddType)(0x2C)
#define UCSRA *(UART_RegAddType)(0x2B)
#define UCSRB *(UART_RegAddType)(0x2A)
#define UCSRC *(UART_RegAddType)(0x40)
#define UBRRH *(UART_RegAddType)(0x40)
#define UBRRL *(UART_RegAddType)(0x29)

/**********************UCSRA_ControlRegisterA************************/
#define RXC 	7u
#define TXC 	6u
#define UDRE 	5u
#define FE 		4u
#define DOR		3u
#define PE 		2u
#define U2X		1u
#define MPCM	0u

/**********************UCSRB_ControlRegisterB************************/
#define RXCIE 	7u
#define TXCIE	6u
#define UDRIE	5u
#define RXEN 	4u
#define TXEN	3u
#define UCSZ2	2u
#define RXB8	1u
#define TXB8	0u

/**********************UCSRC_ControlRegisterC************************/
#define URSEL	7u
#define UMSEL	6u
#define UPM1	5u
#define UPM0	4u
#define USBS	3u
#define UCSZ1	2u
#define UCSZ0	1u
#define UCPOL	0u
/********************************************************************/


static uint8_t UartDriverState[UART_CHANNELS_NUM] = {0};
//static uint8_t UartTxCount[UART_MAX_CH_NUM];
//static uint8_t UartRxLength[UART_MAX_CH_NUM];
//static uint8_t UartRxCount[UART_MAX_CH_NUM];
//static uint8_t* UartRxBuffPtr[UART_MAX_CH_NUM];
#define UART_TXRIS 5u
#define UART_RXRIS 4u
#define UART_STATE_UNINIT        0x00
#define UART_STATE_INIT          0x01
#define UART_STATE_TX_INPROGRESS 0x02
#define UART_STATE_RX_INPROGRESS 0x04
#define UART_STATE_TX_DONE       0x08
#define UART_STATE_RX_DONE       0x10
#define F_CPU 8000000ul

UART_ChkType UART_Init(void)
{
	UART_ChkType RetVar;
	const UART_ConfigType* CfgPtr;
	uint8_t LoopIndex;
	uint8_t TempVar;
	uint8_t Baud;
	uint8_t Error=0;

		/*Checking for a valid configuration*/
		for(LoopIndex = 0; (LoopIndex < UART_CHANNELS_NUM) && (Error== 0); LoopIndex ++){
		CfgPtr = &UART_ConfigParam[LoopIndex];

		if((CfgPtr->DataSize <= DATA_9) &&(CfgPtr->Parity <= PARITY_EVEN )&& (CfgPtr->StopBits <=STOP_TWO))
		{

			Baud = (F_CPU/16/CfgPtr->BaudRate)-1;
			UBRRL= (uint8_t)Baud ;
			UBRRH = (uint8_t)(Baud>>8) ;


				/*init the UART Setting*/
				TempVar = 0x00;
				TempVar |= (CfgPtr-> DataSize<<UCSZ0);
				TempVar |= (CfgPtr-> StopBits<<USBS);
				TempVar |= (CfgPtr-> Parity <<UPM0);
				TempVar |= (CfgPtr->ModeSelect<<UMSEL);
				UCSRC |= TempVar;
		/*UCSRC |=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);*/
				/*Enable interrupt masks*/
                #if(UART_USE_INT_TO_HANDLE == 1)
				UCSRB |= (1 << TXCIE) |(1 <<RXCIE) ;
				#endif
				UartDriverState[LoopIndex] = UART_STATE_INIT;

				RetVar = UART_OK;

			    }
			else
			    {
				RetVar = UART_NOK;
				Error=1;
			    }
	  	        }
	return RetVar;
}


/*A function to Request a silent transmission*/
//const uint8_t* Temp_TxBuffPtr;
//uint8_t TempTxLength;
//static uint8_t TxCounter =0 ;
UART_ChkType UART_StartTransmission (  uint8_t  TxBuff,uint8_t ChannelId)
{
	UART_ChkType RetVar;

	const UART_ConfigType* CfgPtr;
		CfgPtr = &UART_ConfigParam[ChannelId];

	if (((UartDriverState[ChannelId] == 0x01 )|(UartDriverState[ChannelId]== UART_STATE_TX_INPROGRESS))&& (ChannelId < UART_CHANNELS_NUM))
	{
		UCSRB |= (1 << TXEN) ;
		/* Wait for empty transmit buffer */
			while (!( UCSRA & (1 << UDRE)));
		if ((CfgPtr->DataSize) ==DATA_9 )
			{
				UCSRB |= ((TxBuff >> 8) & 1);
			}
		UDR =TxBuff ;
		UartDriverState[ChannelId]= UART_STATE_TX_INPROGRESS;
		RetVar = UART_OK;
   }
		else
		{
			RetVar = UART_NOK;
		}
		return RetVar;
	}
void UART_Start_StringTransmission ( char * Tx_String,uint8_t ChannelId){
while(*Tx_String){
	UART_StartTransmission(*(Tx_String++),ChannelId);
}


}

/*A function to stop a current transmission*/
UART_ChkType UART_StopCrntTransmission(uint8_t ChannelId)
{
	UART_ChkType RetVar;
	if(ChannelId < UART_CHANNELS_NUM){
		if((UartDriverState[ChannelId] & UART_STATE_TX_INPROGRESS) == UART_STATE_TX_INPROGRESS){

	              UCSRB &= ~(1 << TXEN);
	              UartDriverState[ChannelId] &= ~(UART_STATE_TX_INPROGRESS);
			    	RetVar = UART_OK;

		}
				else
				{
					RetVar = UART_NOK;
				}
			}
			else
			{
				RetVar = UART_NOK;
			}
			return RetVar;
		}

/*A function to Request a silent Reception*/
UART_ChkType UART_StartReception(uint8_t* RxBuffPtr,uint8_t ChannelId)
{
	const UART_ConfigType* CfgPtr;

	CfgPtr = &UART_ConfigParam[ChannelId];

	UART_ChkType RetVar;
		/*Check the validity of channel ID*/
	if ((UartDriverState[ChannelId] == 0x01 )&& (ChannelId < UART_CHANNELS_NUM))
		{
			/* Wait for complete receiver buffer */
				while (!( UCSRA & (1 << RXC)));
				UCSRB |= (1 << RXEN) ;				/*Init the global parameters*/
				if ((CfgPtr->DataSize) ==DATA_9)
							{
								*RxBuffPtr = (((UCSRB >> 1)& 1) << 8) ;
							}
				*RxBuffPtr|= UDR;
				UartDriverState[ChannelId] |= UART_STATE_RX_INPROGRESS;
				RetVar = UART_OK;
		}
		else
		{
			RetVar = UART_NOK;
		}
		return RetVar;

}

/*A function to stop a current Reception*/
UART_ChkType UART_StopCrntReception(uint8_t ChannelId)
{
	UART_ChkType RetVar;
		/*Check the validity of channel ID*/
		if(ChannelId < UART_MAX_CH_NUM)
		{
			/*Check if the transmission is already in progress*/
			if((UartDriverState[ChannelId] & UART_STATE_RX_INPROGRESS) == UART_STATE_RX_INPROGRESS)
			{
				UCSRB &= ~(1 << RXEN) ;
				UartDriverState[ChannelId] &= ~(UART_STATE_RX_INPROGRESS);
				RetVar = UART_OK;
			}
			else
			{
				RetVar = UART_NOK;
			}


		}
		else
		{
			RetVar = UART_NOK;
		}
		return RetVar;
	}

