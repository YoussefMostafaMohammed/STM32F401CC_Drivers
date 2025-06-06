/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 * Copyright (C) 2012 Karl Palsson <karlp@tweak.net.au>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include "RCC_CFG.h"
#include "RCC.h"
#include "MGPIO_CFG.h"
#include "MGPIO.h"

#define MPWR_BASE_ADDRESS    (0x40007000UL)
#define MFLASH_BASE_ADDRESS  (0x40023C00UL)


typedef struct {
    volatile uint32_t PWR_CR;
    volatile uint32_t PWR_CSR;
}MPWR_structREGS_t;

typedef struct {
    volatile uint32_t FLASH_ACR;
    volatile uint32_t FLASH_KEYR;
    volatile uint32_t FLASH_SR;
    volatile uint32_t FLASH_CR;
    volatile uint32_t FLASH_OPTCR; 
}MFLASH_structREGS_t;

#define MPWR_REG ((volatile MPWR_structREGS_t*)MPWR_BASE_ADDRESS) 
volatile MPWR_structREGS_t* PWR_REG = MPWR_REG;

#define MFLASH_REG ((volatile MFLASH_structREGS_t*)MFLASH_BASE_ADDRESS) 
volatile MFLASH_structREGS_t* FLASH_REG = MFLASH_REG;

void ConfigrationsFor84MHZPLL(void){
    PWR_REG->PWR_CR&=~(0xC000);    // clear pin 14 & 15  
    PWR_REG->PWR_CR|=(0x8000);     // set pin 15
    FLASH_REG->FLASH_ACR&=~(0xF);  // clear FLASH_ACR_LATENCY_Pos bits
    FLASH_REG->FLASH_ACR|=0x2;     // Set 2 wait states for 84MHz FLASH_ACR_LATENCY_Pos
    FLASH_REG->FLASH_ACR|=0x100;   // Enable prefetch buffer FLASH_ACR_PRFTEN
    FLASH_REG->FLASH_ACR|=0x200;   // Enable instruction cache FLASH_ACR_ICEN
    FLASH_REG->FLASH_ACR|=0x400;   // Enable data cache FLASH_ACR_DCEN
}

int main(void) 
{    
    MRCC_enuDisableClock(MRCC_enu_PLL);
    MRCC_enuEnablePeripheral(RCC_PWR);  
    ConfigrationsFor84MHZPLL();

    MRCC_unionPLL_CFG_t PLL_CFG;
    PLL_CFG.Bits.M_Prescaler = 16;
    PLL_CFG.Bits.N_Multiplier = 336;
    PLL_CFG.Bits.P_Prescaler = RCC_enu_PLL_P_PRESCALER_4;
    PLL_CFG.Bits.CLK_SRC = MRCC_enu_HSI;
    PLL_CFG.Bits.Q_Prescaler = 15;
    MRCC_enuCfgPLL(&PLL_CFG);

    MRCC_enuEnableClock(MRCC_enu_HSI);
    MRCC_enuEnableClock(MRCC_enu_HSE);
    MRCC_enuEnableClock(MRCC_enu_PLL);
    MRCC_enuSelectSysCLK(MRCC_enu_HSE); 
    MRCC_enuEnablePeripheral(RCC_GPIOA);
    
	MGPIO_PinCfg_t Pin0_Cfg = {
    	.Port = MGPIO_ENU_PORT_A,
    	.Pin = MGPIO_ENU_PIN_0,
		.Mode = MGPIO_ENU_PIN_MODE_OUTPUT,
	};
	MGPIO_PinCfg_t Pin1_Cfg = {
	    .Port = MGPIO_ENU_PORT_A,
	    .Pin = MGPIO_ENU_PIN_1,
	    .Mode = MGPIO_ENU_PIN_MODE_OUTPUT,
	};
	MGPIO_PinCfg_t Pin2_Cfg = {
	    .Port = MGPIO_ENU_PORT_A,
	    .Pin = MGPIO_ENU_PIN_2,
	    .Mode = MGPIO_ENU_PIN_MODE_OUTPUT,
	};
	volatile uint32_t Loc_PinValue = 10;
	MGPIO_enuPinCfg(&Pin0_Cfg);
	MGPIO_enuPinCfg(&Pin1_Cfg);
	MGPIO_enuPinCfg(&Pin2_Cfg);

    while(1)
    {
        MGPIO_enuSetPinValue(MGPIO_ENU_PORT_A,MGPIO_ENU_PIN_0,MGPIO_PIN_HIGH);
        MGPIO_enuGetPinValue(MGPIO_ENU_PORT_A,MGPIO_ENU_PIN_0,&Loc_PinValue);
        if(Loc_PinValue == MGPIO_PIN_HIGH)
        {
            MGPIO_enuSetPinValue(MGPIO_ENU_PORT_A,MGPIO_ENU_PIN_1,MGPIO_PIN_HIGH);
            MGPIO_enuSetPinValue(MGPIO_ENU_PORT_A,MGPIO_ENU_PIN_2,MGPIO_PIN_LOW);
        }
        for (volatile uint32_t i = 0; i < 1000000; i++);
        
        MGPIO_enuSetPinValue(MGPIO_ENU_PORT_A,MGPIO_ENU_PIN_0,MGPIO_PIN_LOW);
        MGPIO_enuGetPinValue(MGPIO_ENU_PORT_A,MGPIO_ENU_PIN_0,&Loc_PinValue);
        if(Loc_PinValue == MGPIO_PIN_LOW)
        {
            MGPIO_enuSetPinValue(MGPIO_ENU_PORT_A,MGPIO_ENU_PIN_2,MGPIO_PIN_HIGH);
            MGPIO_enuSetPinValue(MGPIO_ENU_PORT_A,MGPIO_ENU_PIN_1,MGPIO_PIN_LOW);
        }
        for (volatile uint32_t i = 0; i < 1000000; i++);
    }

    return 0;
}