#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "MGPIO.h"
#include "HLED_CFG.h"
#include "HLED.h"


HLED_structConfigrations_t HLED_enuArrLedConfig[NUMBER_OF_LEDS]={

    [LED_START] = {
        .HLED_PORT = MGPIO_ENU_PORT_A,
        .HLED_Pin = MGPIO_ENU_PIN_0,
        .HLED_Connection = HLED_ACTIVE_HIGH,
        .HLED_PinState = MGPIO_PIN_LOW,
        .HLED_PinType = HLED_PUSH_PULL   
    },
    [LED_STOP] = {
        .HLED_PORT = MGPIO_ENU_PORT_A,
        .HLED_Pin = MGPIO_ENU_PIN_1,
        .HLED_Connection = HLED_ACTIVE_HIGH,
        .HLED_PinState = MGPIO_PIN_LOW,
        .HLED_PinType = HLED_PUSH_PULL
    },
    [LED_ALERT] = {
        .HLED_PORT = MGPIO_ENU_PORT_A,
        .HLED_Pin = MGPIO_ENU_PIN_2,
        .HLED_Connection = HLED_ACTIVE_HIGH,
        .HLED_PinState = MGPIO_PIN_LOW,
        .HLED_PinType = HLED_PUSH_PULL
    }
};