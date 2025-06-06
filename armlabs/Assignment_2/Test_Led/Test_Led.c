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
#include "HLED_CFG.h"
#include "HLED.h"

int main(void) 
{
 /* Enable GPIO clock. */
    /* Using API functions: */
    MRCC_enuDisableClock(MRCC_enu_PLL);
    //MRCC_enuDisableClock(MRCC_enu_HSI);
    //MRCC_enuDisableClock(MRCC_enu_HSE);

    MRCC_unionPLL_CFG_t PLL_CFG;
    PLL_CFG.Bits.M_Prescaler = 16;
    PLL_CFG.Bits.N_Multiplier = 320;
    PLL_CFG.Bits.P_Prescaler = RCC_enu_PLL_P_PRESCALER_8;
    PLL_CFG.Bits.CLK_SRC = MRCC_enu_HSI;
    PLL_CFG.Bits.Q_Prescaler = 15;
    MRCC_enuCfgPLL(&PLL_CFG);

    MRCC_enuEnableClock(MRCC_enu_PLL);
    //MRCC_enuEnableClock(MRCC_enu_HSE);
    //MRCC_enuEnableClock(MRCC_enu_HSI);
    MRCC_enuSelectSysCLK(MRCC_enu_PLL); 
    MRCC_enuEnablePeripheral(RCC_GPIOA);


	MGPIO_PinCfg_t Pin0_Cfg = {
    	.Port = MGPIO_ENU_PORT_A,
    	.Pin = MGPIO_ENU_PIN_0,
		.Mode = MGPIO_ENU_PIN_MODE_OUTPUT,
	    //.OutputType = GPIO_PUSH_PULL
	};
	MGPIO_PinCfg_t Pin1_Cfg = {
	    .Port = MGPIO_ENU_PORT_A,
	    .Pin = MGPIO_ENU_PIN_1,
	    .Mode = MGPIO_ENU_PIN_MODE_OUTPUT,
	    //.OutputType = GPIO_PUSH_PULL
	};
	MGPIO_PinCfg_t Pin2_Cfg = {
	    .Port = MGPIO_ENU_PORT_A,
	    .Pin = MGPIO_ENU_PIN_2,
	    .Mode = MGPIO_ENU_PIN_MODE_OUTPUT,
	    //.OutputType = GPIO_PUSH_PULL
	};
	volatile uint32_t Loc_PinValue = 10;
	MGPIO_enuPinCfg(&Pin0_Cfg);
	MGPIO_enuPinCfg(&Pin1_Cfg);
	MGPIO_enuPinCfg(&Pin2_Cfg);
    HLED_enuInit();
    for (volatile uint32_t i = 0; i < 1000000; i++);
    while(1)
    {
        HLED_enuSetLedState(LED_START,HLED_ENU_HIGH);
        for (volatile uint32_t i = 0; i < 1000000; i++);
        HLED_enuSetLedState(LED_START,HLED_ENU_LOW);
        HLED_enuSetLedState(LED_STOP,HLED_ENU_HIGH);
        for (volatile uint32_t i = 0; i < 1000000; i++);
        HLED_enuSetLedState(LED_STOP,HLED_ENU_LOW);
        HLED_enuSetLedState(LED_ALERT,HLED_ENU_HIGH);
        for (volatile uint32_t i = 0; i < 1000000; i++);
        HLED_enuSetLedState(LED_ALERT,HLED_ENU_LOW);
        for (volatile uint32_t i = 0; i < 1000000; i++);
    }

    return 0;
}