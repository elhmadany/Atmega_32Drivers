/*
 * GPIO.c
 *
 * Created: 4/7/2017 4:05:42 PM
 *  Author: 2017
 */
#include "GPIO_Cfg.h"
#include "GPIO.h"
#include "UART.h"
#define PORTS_NUMBER 4u
typedef volatile uint8_t* const GPIO_RegAddType;
#define PORTD_BASE_ADDRESS (GPIO_RegAddType)(0x30)
#define PORTC_BASE_ADDRESS (GPIO_RegAddType)(0x33)
#define PORTB_BASE_ADDRESS (GPIO_RegAddType)(0x36)
#define PORTA_BASE_ADDRESS (GPIO_RegAddType)(0x39)

static const uint8_t PortsBaseAddressLut[PORTS_NUMBER] =
{
	PORTA_BASE_ADDRESS,
	PORTB_BASE_ADDRESS,
	PORTC_BASE_ADDRESS,
	PORTD_BASE_ADDRESS

};
#define GPIO_REG_ADDRESS(CHANNEL_ID,REG_OFFSET)\
(PortsBaseAddressLut[CHANNEL_ID] + REG_OFFSET)
/*Port Control*/
#define GPIODATA_WRITE(PORT_ID)  *((GPIO_RegAddType)(GPIO_REG_ADDRESS(PORT_ID,0x02)))
#define GPIODATA_READ(PORT_ID)   *((GPIO_RegAddType)(GPIO_REG_ADDRESS(PORT_ID,0x00)))
#define GPIODIR_REG(PORT_ID)     *((GPIO_RegAddType)(GPIO_REG_ADDRESS(PORT_ID,0x01)))
/*Interrupt Control*/
#define GeneralInterruptControlRegister           *(GPIO_RegAddType)(0x5B)
#define MCUControlRegister                        *(GPIO_RegAddType)(0x55)
#define MCUControlandStatusRegister               *(GPIO_RegAddType)(0x54)
#define GeneralInterruptFlagRegister              *(GPIO_RegAddType)(0x5A)
static uint8_t GPIO_GroupState[GPIO_GROUPS_NUMBER] = {0};

/*A function to initialize all the GPIO Groups in the configurations*/
GPIO_CheckType GPIO_Init(void)
{
uint8_t LoopIndex;
uint8_t ErrorFlag = 0;
GPIO_CheckType RetVar;
const GPIO_CfgType * CfgPtr;
	for(LoopIndex = 0; (LoopIndex < GPIO_GROUPS_NUMBER) && (ErrorFlag == 0); LoopIndex ++)
	{
		if(GPIO_ConfigParam[LoopIndex].PortId <= PORTS_NUMBER)
		{
              /*DATA CONTROL*/
			CfgPtr =&GPIO_ConfigParam[LoopIndex];
			GPIODIR_REG(CfgPtr->PortId) |= (CfgPtr->PortMask & CfgPtr->PortDirection);
		    //GPIODATA_WRITE(CfgPtr->PortId) |= ( CfgPtr->PortMask & CfgPtr->PortDirection);

			GPIO_GroupState[LoopIndex] = 1;
			RetVar = GPIO_OK;
		}
		else
		{
			/*Invalid GroupId*/
			RetVar = GPIO_NOK;
			 ErrorFlag = 1;
		}
	}
	return RetVar;
}
/*A function to Digital Write data to a specific group*/
GPIO_CheckType GPIO_Write(uint8_t GroupId,uint8_t GroupData)
{
   	GPIO_CheckType RetVar;
   	const GPIO_CfgType * CfgPtr;
   	if(GroupId < GPIO_GROUPS_NUMBER  )
   	{
	   	if(GPIO_GroupState[GroupId] == 0x01)
	   	{
		   	CfgPtr = &GPIO_ConfigParam[GroupId];
		   	if((CfgPtr->PortDirection != 0x00))
		   	{

			   	GPIODATA_WRITE(CfgPtr->PortId) = (GroupData) & CfgPtr->PortMask;

			   	RetVar = GPIO_OK;
		   	}
		   	else
		   	{
			   	/*invalid initialization*/
			   	RetVar = GPIO_NOK;
		   	}
	   	}
	   	else
	   	{
		   	/*Init function not called*/
		   	RetVar = GPIO_NOK;
	   	}
   	}
   	else
   	{
	   	/*Invalid GroupId*/
	   	RetVar = GPIO_NOK;
   	}
   	return RetVar;
}
/*A function to Digital read data from a specific group*/
GPIO_CheckType GPIO_Read(uint8_t GroupId,uint8_t* GroupDataPtr)
{
		GPIO_CheckType RetVar;
		const GPIO_CfgType * CfgPtr;
		if(GroupId < GPIO_GROUPS_NUMBER)
		{
			if(GPIO_GroupState[GroupId] == 0x01)
			{
				CfgPtr = &GPIO_ConfigParam[GroupId];
				if((CfgPtr->PortDirection == 0x00))

				{

					*GroupDataPtr =  (GPIODATA_READ(CfgPtr->PortId)>>(CfgPtr->PortMask-1)&1) ;

					RetVar = GPIO_OK;
				}
				else
				{
					/*invalid initialization*/
					RetVar = GPIO_NOK;
				}

			}
			else
			{
				/*Init function not called*/
				RetVar = GPIO_NOK;
			}

		}
		else
		{
			/*Invalid GroupId*/
			RetVar = GPIO_NOK;
		}
		return RetVar;
	}
GPIO_CheckType GPIO_SetInterruptEvent(uint8_t GroupId,GPIO_IntEventType IntEvent,GPIO_ExternalInterruptSlect ExternalInterruptSlect )
{
	GPIO_CheckType RetVar;
	const GPIO_CfgType * CfgPtr;
	if(GroupId < GPIO_GROUPS_NUMBER)
	{
	 if(GPIO_GroupState[GroupId] == 0x01)
		{
			CfgPtr = &GPIO_ConfigParam[GroupId];
			if(CfgPtr->SetPinType  != 0x00)
			{
				GeneralInterruptControlRegister |=(1<<ExternalInterruptSlect);
				if(ExternalInterruptSlect==INTERRUPT0)
					MCUControlRegister|=IntEvent;
				else if(ExternalInterruptSlect==INTERRUPT1)
					MCUControlRegister|=IntEvent<<2;
				else
				{
				/*invalid ExternalInterruptSlect*/
				RetVar = GPIO_NOK;
				}
				RetVar = GPIO_OK;
			}
			else
			{
				/*invalid initialization*/
				RetVar = GPIO_NOK;
			}
		}
		else
		{
			/*Init function not called*/
			RetVar = GPIO_NOK;
		}
	}
	else
	{
		/*Invalid GroupId*/
		RetVar = GPIO_NOK;
	}
	return RetVar;
}
GPIO_CheckType GPIO_ClrInterruptFlag(uint8_t GroupId,GPIO_ExternalInterruptSlect ExternalInterruptSlect)
{
	GPIO_CheckType RetVar;
	const GPIO_CfgType * CfgPtr;
	if(GroupId < GPIO_GROUPS_NUMBER)
	{
		if(GPIO_GroupState[GroupId] == 0x01)
		{
			CfgPtr = &GPIO_ConfigParam[GroupId];
			if(CfgPtr->SetPinType  != 0x00)
			{
				if(ExternalInterruptSlect==INTERRUPT0)
			GeneralInterruptFlagRegister|=(1<<INTERRUPT0);
			else if(ExternalInterruptSlect==INTERRUPT1)
				GeneralInterruptFlagRegister|=(1<<INTERRUPT1);
			else
			{
			/*invalid ExternalInterruptSlect*/
			RetVar = GPIO_NOK;
			}
			RetVar = GPIO_OK;
			}
			else
			{
				/*invalid initialization*/
				RetVar = GPIO_NOK;
			}
		}
		else
		{
			/*Init function not called*/
			RetVar = GPIO_NOK;
		}
	}
	else
	{
		/*Invalid GroupId*/
		RetVar = GPIO_NOK;
	}
	return RetVar;
}
/*A function to Get a specific pin interrupt status*/
GPIO_CheckType GPIO_GetInterruptStatus(uint8_t GroupId,GPIO_IntStatus *IntStatusPtr,GPIO_ExternalInterruptSlect ExternalInterruptSlect)
{
	GPIO_CheckType RetVar;
	const GPIO_CfgType * CfgPtr;
	if(GroupId < GPIO_GROUPS_NUMBER)
	{
	if(GPIO_GroupState[GroupId] == 0x01)
		{
			CfgPtr = &GPIO_ConfigParam[GroupId];
		if(CfgPtr->SetPinType  != 0x00)
			{
			 if(ExternalInterruptSlect==INTERRUPT0)
		*IntStatusPtr=(GeneralInterruptFlagRegister&(1<<INTERRUPT0))!= 0x00 ? INT_PENDING : INT_INACTIVE;
			 else if(ExternalInterruptSlect==INTERRUPT1)
		*IntStatusPtr=(GeneralInterruptFlagRegister&(1<<INTERRUPT1))!= 0x00 ? INT_PENDING : INT_INACTIVE;
		      else
			{
				/*invalid initialization*/
				RetVar = GPIO_NOK;
			}
			}
		 else
			{
				/*invalid initialization*/
				RetVar = GPIO_NOK;
			}
		}
	else
		{
			/*Init function not called*/
			RetVar = GPIO_NOK;
		}
	}
	else
	{
		/*Invalid GroupId*/
		RetVar = GPIO_NOK;
	}
	return RetVar;
}

void InterruptService(void){
	 static   uint8_t Time=1;
	 Time++;
	 if(Time%2==0){
		 GPIO_Write(0,0xff);
		// UART_Start_StringTransmission("PRESSED_LED ON",0);
	 }
	 else {
		 GPIO_Write(0,0x00);
		 UART_Start_StringTransmission("PRESSED_LED OFF\r",0);
	 }

}
void Set_ISR_Callback(void(*ptr)(void)){
	GPIO_FnPtrType=ptr;
}
