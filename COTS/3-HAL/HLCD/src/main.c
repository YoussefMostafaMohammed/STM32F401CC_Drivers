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
#include "HLED.h"
#include "HLED_CFG.h"
#include "HCLCD.h"
#include "HCLCD_CFG.h"
#include "MSYSTICK.h"
#include "MSYSTICK_CFG.h"
#include "HSCHED.h"
#include "HSCHED_CFG.h"

extern uint64_t Global_uint64CurrentSystemClOCK;
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

void ToggleLed1();
void ToggleLed2();

void ConfigrationsFor84MHZPLL(void){
    PWR_REG->PWR_CR&=~(0xC000);    // clear pin 14 & 15  
    PWR_REG->PWR_CR|=(0x8000);     // set pin 15
    FLASH_REG->FLASH_ACR&=~(0xF);  // clear FLASH_ACR_LATENCY_Pos bits
    FLASH_REG->FLASH_ACR|=0x2;     // Set 2 wait states for 84MHz FLASH_ACR_LATENCY_Pos
    FLASH_REG->FLASH_ACR|=0x100;   // Enable prefetch buffer FLASH_ACR_PRFTEN
    FLASH_REG->FLASH_ACR|=0x200;   // Enable instruction cache FLASH_ACR_ICEN
    FLASH_REG->FLASH_ACR|=0x400;   // Enable data cache FLASH_ACR_DCEN
}
HSCHED_RunnableCfg_t Led1={
    .CallBack=ToggleLed1,
    .CyclicTime=1000,
    .FirstDelay=10000,
    .Priority=0,
};



HSCHED_RunnableCfg_t Led2={
    .CallBack=ToggleLed2,
    .CyclicTime=1000,
    .FirstDelay=0,
    .Priority=1,
};

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
    MRCC_enuSelectSysCLK(MRCC_enu_HSI);
    MRCC_enuEnablePeripheral(RCC_GPIOA);
    MRCC_enuEnablePeripheral(RCC_GPIOB);

    HLED_enuInit();
    HSCHED_enuInit();
    //HSCHED_enuRegisterRunnable(&Led1);
    //HSCHED_enuRegisterRunnable(&Led2);
    HCLCD_enuAsyncInit();    
    HSCHED_enuStart();
    while(1)
    {           
         //HCLCD_enuAsyncWriteString("adaB",0);
        HCLCD_enuAsyncWriteData('B',0);
        //HCLCD_enuAsyncWriteCommand(0x01,0);
    }
    return 0;
}

void ToggleLed1(){
    static HLED_enuPinState_t Local_enuLedState=HLED_ACTIVE_HIGH;
    HLED_enuSetLedState(LED_START ,Local_enuLedState);
    Local_enuLedState= !Local_enuLedState;
}


void ToggleLed2(){
    static HLED_enuPinState_t Local_enuLedState=HLED_ACTIVE_HIGH;
    HLED_enuSetLedState(LED_STOP ,Local_enuLedState);
    Local_enuLedState= !Local_enuLedState;
}