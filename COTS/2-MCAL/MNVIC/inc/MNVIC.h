#ifndef MNVIC_H_H
#define MNVIC_H_H

#include "STD_TYPES.h"
#include "BIT_MATH.h"

typedef enum
{
    MNVIC_ENU_OK=0,
    MNVIC_ENU_NOK,
    MNVIC_ENU_NULLPTR,
    MNVIC_ENU_INVALID_IRQ_NUMBER,
    MNVIC_ENU_IRQ_NUMBER_INTERRUPT_IS_ENABLED,
    MNVIC_ENU_INVALID_PRIORITY,
    MNVIC_ENU_INVALID_PRIORITY_GROUPING,
}MNVIC_enuErrorStatus_t;

typedef enum {
    NVIC_enuWWDG,
    NVIC_enuEXTI16_PVD,
    NVIC_enuEXTI21_TAMP_STAMP,
    NVIC_enuEXTI22_RTC_WKUP,
    NVIC_enuFLASH,
    NVIC_enuRCC,
    NVIC_enuEXTI0,
    NVIC_enuEXTI1,
    NVIC_enuEXTI2,
    NVIC_enuEXTI3,
    NVIC_enuEXTI4,
    NVIC_enuDMA1_Stream0,
    NVIC_enuDMA1_Stream1,
    NVIC_enuDMA1_Stream2,
    NVIC_enuDMA1_Stream3,
    NVIC_enuDMA1_Stream4,
    NVIC_enuDMA1_Stream5,
    NVIC_enuDMA1_Stream6,
    NVIC_enuADC ,
    NVIC_enuEXTI9_5=23,
    NVIC_enuTIM1_BRK_TIM9,
    NVIC_enuTIM1_UP_TIM10,
    NVIC_enuTIM1_TRG_COM_TIM11,
    NVIC_enuTIM1_CC,
    NVIC_enuTIM2,
    NVIC_enuTIM3,
    NVIC_enuTIM4,
    NVIC_enuI2C1_EV,
    NVIC_enuI2C1_ER,
    NVIC_enuI2C2_EV,
    NVIC_enuI2C2_ER,
    NVIC_enuSPI1,
    NVIC_enuSPI2,
    NVIC_enuUSART1,
    NVIC_enuUSART2,
    NVIC_enuEXTI15_10=40,
    NVIC_enuEXTI17_RTC_Alarm,
    NVIC_enuEXTI18_OTG_FS_WKUP,
    NVIC_enuDMA1_Stream7=47,
    NVIC_enuSDIO=49,
    NVIC_enuTIM5,
    NVIC_enuSPI3,
    NVIC_enuDMA2_Stream0=56,
    NVIC_enuDMA2_Stream1,
    NVIC_enuDMA2_Stream2,
    NVIC_enuDMA2_Stream3,
    NVIC_enuDMA2_Stream4,
    NVIC_enuOTG_FS=67,
    NVIC_enuDMA2_Stream5,
    NVIC_enuDMA2_Stream6,
    NVIC_enuDMA2_Stream7,
    NVIC_enuUSART6,
    NVIC_enuI2C3_EV ,
    NVIC_enuI2C3_ER,
    NVIC_enuFPU=81,
    NVIC_enuSPI4=84 
} MNVIC_enuIRQn_t;

typedef enum 
{
    MNVIC_BIT_LOW = 0,
    MNVIC_BIT_HIGH,

} MNVIC_enuBitStatus_t;

typedef enum{
    Group_priorities_16= 0b000,
    Group_priorities_8= 0b100,
    Group_priorities_4= 0b101,
    Group_priorities_2= 0b110,
    Group_priorities_NONE= 0b111,
}MNVIC_enuPriority_Grouping;

MNVIC_enuErrorStatus_t MNVIC_EnableIRQ(MNVIC_enuIRQn_t Copy_enuIRQNumber);
MNVIC_enuErrorStatus_t MNVIC_DisableIRQ(MNVIC_enuIRQn_t Copy_enuIRQNumber);
MNVIC_enuErrorStatus_t MNVIC_GetPendingIRQ(MNVIC_enuIRQn_t Copy_enuIRQNumber, MNVIC_enuBitStatus_t* Addr_uint32PendingIRQ );
MNVIC_enuErrorStatus_t MNVIC_SetPendingIRQ(MNVIC_enuIRQn_t Copy_enuIRQNumber);
MNVIC_enuErrorStatus_t MNVIC_ClearPendingIRQ (MNVIC_enuIRQn_t Copy_enuIRQNumber);
MNVIC_enuErrorStatus_t MNVIC_GetActive(MNVIC_enuIRQn_t Copy_enuIRQNumber, uint32_t* Addr_uint32ActiveIRQ );
MNVIC_enuErrorStatus_t MNVIC_SystemReset(void);

MNVIC_enuErrorStatus_t MNVIC_SetPriority(MNVIC_enuIRQn_t Copy_enuIRQNumber, uint8_t Copy_uint32priority);
MNVIC_enuErrorStatus_t MNVIC_GetPriority(MNVIC_enuIRQn_t Copy_enuIRQNumber, uint8_t* Addr_uint32Priority);
MNVIC_enuErrorStatus_t MNVIC_SetPriorityGrouping(MNVIC_enuPriority_Grouping priority_grouping);

#endif
