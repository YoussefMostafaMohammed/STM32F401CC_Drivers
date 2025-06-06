#ifndef HSWITCH_H_
#define HSWITCH_H_

#include "HSWITCH_CFG.h"
typedef void (*switchcbf_t) (void);

typedef enum
{
    HSWITCH_ENU_OK = 0,
    HSWITCH_ENU_NOK,
    HSWITCH_ENU_NULL_PTR,
    HSWITCH_ENU_NULL_PTR_CALLBACK,
    HSWITCH_ENU_INVALID_PRIORITY,
    HSWITCH_ENU_INVALID_PORT_NUM,
    HSWITCH_ENU_INVALID_PIN_NUM,
    HSWITCH_ENU_INVALID_SWITCH_NUM
} HSWITCH_enuErrorStatus_t;

typedef enum{
    HSWITCH_PULLUP,
    HSWITCH_PULLDOWN
}HSWITCH_enuConnectioOpt_t;

typedef enum{
    HSWITCH_ENU_HIGH,
    HSWITCH_ENU_LOW
}HSWITCH_enuState_t;

typedef struct {
    MGPIO_enuPort_t HSWITCH_PORT;
    MGPIO_enuPin_t HSWITCH_Pin;
    HSWITCH_enuConnectioOpt_t HSWITCH_Connection;
}HSWITCH_structConfigrations_t;

HSWITCH_enuErrorStatus_t HSWITCH_enuInit(void);
HSWITCH_enuErrorStatus_t HSWITCH_enuGetSwitchState(HSWITCH_enuSwitches Copy_enuSwitchNum,HSWITCH_enuState_t* Copy_enuState);

#endif
