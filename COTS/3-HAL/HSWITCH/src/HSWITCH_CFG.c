#include "MGPIO.h"
#include "MGPIO_CFG.h"
#include "HSCHED.h"
#include "HSCHED_CFG.h"
#include "HSWITCH.h"
#include "HLED_CFG.h"
#include "HSWITCH_CFG.h"

HSWITCH_structConfigrations_t HSWITCH_enuArrSwitchConfig[NUMBER_OF_SWITCHS] = {
    [SWITCH_START] = {
        .HSWITCH_PORT = MGPIO_ENU_PORT_A,
        .HSWITCH_Pin = MGPIO_ENU_PIN_0,
        .HSWITCH_Connection = HSWITCH_PULLUP
    },
    [SWITCH_STOP] = {
        .HSWITCH_PORT = MGPIO_ENU_PORT_A,
        .HSWITCH_Pin = MGPIO_ENU_PIN_1,
        .HSWITCH_Connection = HSWITCH_PULLUP
    },
    [SWITCH_ALERT] = {
        .HSWITCH_PORT = MGPIO_ENU_PORT_A,
        .HSWITCH_Pin = MGPIO_ENU_PIN_2,
        .HSWITCH_Connection = HSWITCH_PULLUP
    }
};
