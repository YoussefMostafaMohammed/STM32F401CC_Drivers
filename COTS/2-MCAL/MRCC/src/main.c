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

#include "../../../0-STD_LIBS/inc/BIT_MATH.h"
#include "../../../0-STD_LIBS/inc/STD_TYPES.h"
#include "../include/RCC.h"
#include "../include/RCC_CFG.h"
#include <libopencm3/stm32/gpio.h>

#ifdef STM32F0
	#define RCCLEDPORT (RCC_GPIOE)
	#define LEDPORT (GPIOE)
	#define LEDPIN (GPIO8)
#elif STM32F1
	#define RCCLEDPORT (RCC_GPIOC)
	#define LEDPORT (GPIOC)
	#define LEDPIN (GPIO13)
	#define GPIO_MODE_OUTPUT GPIO_MODE_OUTPUT_2_MHZ
	#define GPIO_PUPD_NONE GPIO_CNF_OUTPUT_PUSHPULL
#elif STM32F2
	#define RCCLEDPORT (RCC_GPIOE)
	#define LEDPORT (GPIOE)
	#define LEDPIN (GPIO8)
#elif STM32F3
	#define RCCLEDPORT (RCC_GPIOE)
	#define LEDPORT (GPIOE)
	#define LEDPIN (GPIO8)
#elif STM32F4
	#define RCCLEDPORT (RCC_GPIOA)
	#define LEDPORT (GPIOA)
	#define LEDPIN (GPIO0)
#elif STM32F7
	#define RCCLEDPORT (RCC_GPIOD)
	#define LEDPORT (GPIOD)
	#define LEDPIN (GPIO12)
#elif STM32G0
	#define RCCLEDPORT (RCC_GPIOD)
	#define LEDPORT (GPIOD)
	#define LEDPIN (GPIO12)
#elif STM32G4
	#define RCCLEDPORT (RCC_GPIOD)
	#define LEDPORT (GPIOD)
	#define LEDPIN (GPIO12)
#elif STM32H7
	#define RCCLEDPORT (RCC_GPIOD)
	#define LEDPORT (GPIOD)
	#define LEDPIN (GPIO12)
#elif STM32L0
	#define RCCLEDPORT (RCC_GPIOB)
	#define LEDPORT (GPIOB)
	#define LEDPIN (GPIO6)
#elif STM32L1
	#define RCCLEDPORT (RCC_GPIOB)
	#define LEDPORT (GPIOB)
	#define LEDPIN (GPIO6)
#elif STM32L4
	#define RCCLEDPORT (RCC_GPIOB)
	#define LEDPORT (GPIOB)
	#define LEDPIN (GPIO6)
#else
    #error "This example doesn't support this target!"
#endif

static void gpio_setup(void)
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

	/* Set pin to 'output push-pull'. */
	/* Using API functions: */
#ifdef STM32F1
    gpio_set_mode(LEDPORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LEDPIN);
#else
	gpio_mode_setup(LEDPORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LEDPIN);
#endif
}

int main(void)
{
	int i;
	gpio_setup();
	/* Blink the LED on the board. */
	while (1) {
		/* Using API function gpio_toggle(): */
		gpio_toggle(LEDPORT, LEDPIN);	/* LED on/off */
		for (i = 0; i < 1000000; i++) {	/* Wait a bit. */
			__asm__("nop");
		}
	}

	return 0;
}
