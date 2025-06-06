#ifndef HLED_H_
#define HLED_H_
#include "MGPIO.h"

typedef enum{
        HLED_ENU_OK,
        HLED_ENU_INVALID_PARAM,
        HLED_ENU_INVALID_PIN_NUM,
        HLED_ENU_INVALID_PORT_NUM,
        HLED_ENU_INVALID_LED_CONNECTION,
        HLED_ENU_INVALID_MODE,
        HLED_ENU_INVALID_LED_STATE,  
        HLED_ENU_NULL_PTR,
        HLED_ENU_NOK
}HLED_enuErrorStatus_t;

typedef enum{
   HLED_ENU_LOW,
   HLED_ENU_HIGH,
}HLED_enuPinState_t;

typedef enum{
    HLED_ACTIVE_LOW,
    HLED_ACTIVE_HIGH
}HLED_enuConnectioOpt_t;
 
typedef enum{
    HLED_PUSH_PULL,
    HLED_OPEN_DRAIN
}HLED_enuPinType;

typedef struct {
    MGPIO_enuPort_t HLED_PORT; // Port number
    MGPIO_enuPin_t HLED_Pin;  // Pin number
    HLED_enuConnectioOpt_t HLED_Connection;
    HLED_enuPinState_t HLED_PinState;
    HLED_enuPinType HLED_PinType;
}HLED_structConfigrations_t;

HLED_enuErrorStatus_t HLED_enuInit(void);
HLED_enuErrorStatus_t HLED_enuSetLedState(uint8_t Copy_u8LedName,HLED_enuPinState_t Copy_u8State);

#endif