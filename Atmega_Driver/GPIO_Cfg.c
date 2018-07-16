/*
 * GPIO_Cfg.c
 *
 *  Created on: ١٨‏/٠٨‏/٢٠١٧
 *      Author: mohammed
 */

#include "GPIO.h"
#include "GPIO_Cfg.h"
const GPIO_CfgType GPIO_ConfigParam [GPIO_GROUPS_NUMBER] ={


		        {
						0x03,1<<5,0xff,0x00

				},
				{
										0x03,1<<6,0xff,0x00

								},
				{
						0x03,1<<4,0x00,0xff

				 }

};
/*{
		 {
		  0x01,1<<1,0xff,0x00
		 },
		 {
          0x01,1<<2,0xff,0x00
		 },
		 {
	      0x01,1<<3,0xff,0x00
		 },
		 {
	      0x00,0xf0,0xff,0x00
		 }
		};*/
