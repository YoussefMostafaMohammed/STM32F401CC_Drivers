#ifndef MSYSTICK_H_
#define MSYSTICK_H_
#include "STD_TYPES.h"

typedef void (*systickcbf_t) (void);

typedef enum
{
    MSYSTICK_ENU_OK=0,
    MSYSTICK_ENU_NOK,
    MSYSTICK_ENU_NULLPTR
}MSYSTICK_enuErrorStatus_t;

typedef enum {
    MSYSTICK_DISABLE_EXCEPTION_REQUEST,
    MSYSTICK_ENABLE_EXCEPTION_REQUEST
}MSYSTICK_enuExceptionRequestStatus_t;

typedef enum {
    MSYSTICK_DISABLE_COUNTER,
    MSYSTICK_ENABLE_COUNTER
}MSYSTICK_enuCounterStatus_t;

typedef enum {
    MSYSTICK_CLOCK_SOURCE_AHB_BY_8,
    MSYSTICK_CLOCK_SOURCE_AHB
}MSYSTICK_enuClockSourceSelection_t;

MSYSTICK_enuErrorStatus_t MSYSTICK_enuInit(void);
MSYSTICK_enuErrorStatus_t MSYSTICK_enuDelay_ms(uint32_t Copy_uint32DelayMs);
MSYSTICK_enuErrorStatus_t MSYSTICK_enuSetCyclicFunction(systickcbf_t Addr_CallBackFunction, uint32_t Copy_uint32CyclicTime);
MSYSTICK_enuErrorStatus_t SYSTICK_enuStart(void);
MSYSTICK_enuErrorStatus_t SYSTICK_enuStop(void);
void SysTick_Handler(void);

#endif