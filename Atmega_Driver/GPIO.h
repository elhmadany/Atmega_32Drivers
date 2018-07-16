#ifndef GPIO_H
#define GPIO_H
#include <stdint.h>
#include "GPIO_Cfg.h"
/*interrupt call back pointer*/
 void(*GPIO_FnPtrType)(void);

typedef struct
{
uint8_t PortId;
uint8_t PortMask;
uint8_t PortDirection;
/*Mode Control*/
/*analog -> 0
 *Digital-> 1
 */
uint8_t SetPinType;
}GPIO_CfgType;
typedef enum {GPIO_OK =1, GPIO_NOK=0} GPIO_CheckType;
typedef enum {INTERRUPT2  =5, INTERRUPT0 , INTERRUPT1} GPIO_ExternalInterruptSlect;
typedef enum {TheLowLevel  = 0, AnyLogicalChange , TheFallingEdge,TheRisingEdge} GPIO_IntEventType;
typedef enum {INT_PENDING = 0xff,INT_INACTIVE = 0x00}GPIO_IntStatus;
extern const GPIO_CfgType GPIO_ConfigParam [GPIO_GROUPS_NUMBER];

/*A function to initialize all the GPIO Groups in the configurations*/
GPIO_CheckType GPIO_Init(void);
/*A function to Digital Write data to a specific group*/
GPIO_CheckType GPIO_Write(uint8_t GroupId,uint8_t GroupData);
/*A function to Digital read data from a specific group*/
GPIO_CheckType GPIO_Read(uint8_t GroupId,uint8_t* GroupDataPtr);
/*A function to Select the interrupt event for a specific GPIO Group*/
GPIO_CheckType GPIO_SetInterruptEvent(uint8_t GroupId,GPIO_IntEventType IntEvent,GPIO_ExternalInterruptSlect ExternalInterruptSlect );
/*A function to clear a specific pin interrupt flag*/
GPIO_CheckType GPIO_ClrInterruptFlag(uint8_t GroupId,GPIO_ExternalInterruptSlect ExternalInterruptSlect);
/*A function to Get a specific pin interrupt status*/
GPIO_CheckType GPIO_GetInterruptStatus(uint8_t GroupId,GPIO_IntStatus *IntStatusPtr,GPIO_ExternalInterruptSlect ExternalInterruptSlect);
void Set_ISR_Callback(void(*ptr)(void));
void InterruptService(void);

#endif
