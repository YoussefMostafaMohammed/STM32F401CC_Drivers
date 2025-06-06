#include "MGPIO.h"
#include "MGPIO_CFG.h"
#include "HSCHED.h"
#include "HSCHED_CFG.h"
#include "HLED_CFG.h"
#include "HCLCD_CFG.h"
#include "HCLCD.h"

// use pins from HCLCD_ENU_PIN_D4 to HCLCD_ENU_PIN_D7 in 4 bit mode
HCLCD_structLcdCfg_t HCLCD_enuArrLcdConfig[NUMBER_OF_LCDS]={
    [LCD_ONE] = {
        .ClcdType = HCLCD_2x16,
        .ClcdMode = HCLCD_4_BIT_MODE,
        .DataPins = {
            [HCLCD_ENU_PIN_D0] = {MGPIO_ENU_PORT_A, MGPIO_ENU_PIN_0},
            [HCLCD_ENU_PIN_D1] = {MGPIO_ENU_PORT_A, MGPIO_ENU_PIN_1},
            [HCLCD_ENU_PIN_D2] = {MGPIO_ENU_PORT_A, MGPIO_ENU_PIN_2},
            [HCLCD_ENU_PIN_D3] = {MGPIO_ENU_PORT_A, MGPIO_ENU_PIN_3},
            [HCLCD_ENU_PIN_D4] = {MGPIO_ENU_PORT_A, MGPIO_ENU_PIN_4},
            [HCLCD_ENU_PIN_D5] = {MGPIO_ENU_PORT_A, MGPIO_ENU_PIN_5},
            [HCLCD_ENU_PIN_D6] = {MGPIO_ENU_PORT_A, MGPIO_ENU_PIN_6},
            [HCLCD_ENU_PIN_D7] = {MGPIO_ENU_PORT_A, MGPIO_ENU_PIN_7},
        },
        .PowerPins={
            [HCLCD_ENU_PIN_RS] = {MGPIO_ENU_PORT_B, MGPIO_ENU_PIN_0},
            [HCLCD_ENU_PIN_RW] = {MGPIO_ENU_PORT_B, MGPIO_ENU_PIN_1},
            [HCLCD_ENU_PIN_E]  = {MGPIO_ENU_PORT_B, MGPIO_ENU_PIN_2},
        }
    }
};

