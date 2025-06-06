#ifndef MGPIO_ENU_H_
#define MGPIO_ENU_H_
#include "STD_TYPES.h"
typedef enum
{
    MGPIO_ENU_OK=0,
    MGPIO_ENU_NOK,
    MGPIO_ENU_NULLPTR,
    MGPIO_ENU_INVALID_PORT_NUM,
    MGPIO_ENU_INVALID_PIN_NUM,
    MGPIO_ENU_INVALID_PIN_MODE,
    MGPIO_ENU_INVALID_PIN_OUTPUT_TYPE,
    MGPIO_ENU_INVALID_PIN_SPEED,
    MGPIO_ENU_INVALID_PIN_CONFIGRATION,
    MGPIO_ENU_INVALID_PIN_ALTERNATIVE_FUNCTION,
    MGPIO_ENU_INVALID_PIN_VALUE
}MGPIO_enuErrorStatus_t;

typedef enum{
    MGPIO_ENU_PORT_A=0,
    MGPIO_ENU_PORT_B,
    MGPIO_ENU_PORT_C,
    MGPIO_ENU_PORT_D,
    MGPIO_ENU_PORT_E,
    MGPIO_ENU_PORT_H=7,
}MGPIO_enuPort_t;

typedef enum{
    MGPIO_ENU_PIN_0=0,
    MGPIO_ENU_PIN_1,
    MGPIO_ENU_PIN_2,
    MGPIO_ENU_PIN_3,
    MGPIO_ENU_PIN_4,
    MGPIO_ENU_PIN_5,
    MGPIO_ENU_PIN_6,
    MGPIO_ENU_PIN_7,
    MGPIO_ENU_PIN_8,
    MGPIO_ENU_PIN_9,
    MGPIO_ENU_PIN_10,
    MGPIO_ENU_PIN_11,
    MGPIO_ENU_PIN_12,
    MGPIO_ENU_PIN_13,
    MGPIO_ENU_PIN_14,
    MGPIO_ENU_PIN_15
}MGPIO_enuPin_t;

typedef enum{
    MGPIO_ENU_PIN_MODE_INPUT=0,
    MGPIO_ENU_PIN_MODE_OUTPUT,
    MGPIO_ENU_PIN_MODE_ALTERFUNCTION,
    MGPIO_ENU_PIN_MODE_ANALOG
}MGPIO_enuPinMode_t;


typedef enum{
    MGPIO_ENU_PIN_OUTPUT_PUSH_PULL=0,
    MGPIO_ENU_PIN_OUTPUT_OPEN_DRAIN
}MGPIO_enuPinOutputType_t;

typedef enum{
    MGPIO_ENU_PIN_LOW_SPPED=0,
    MGPIO_ENU_PIN_MEDIUM_SPPED,
    MGPIO_ENU_PIN_HIGH_SPPED,
    MGPIO_ENU_PIN_VERY_HIGH_SPPED
}MGPIO_enuPinSpeedConfigration_t;

typedef enum{
    MGPIO_ENU_PIN_NO_PULLUP_PULLDOWN=0,
    MGPIO_ENU_PIN_PULLUP,
    MGPIO_ENU_PIN_PULLDOWN
}MGPIO_enuPinConfiguration_t;

typedef enum 
{
    MGPIO_PIN_LOW = 0,
    MGPIO_PIN_HIGH,
} MGPIO_PinValue_t;

typedef enum 
{
    MGPIO_ENU_AF1 = 0,
    MGPIO_ENU_AF2,
    MGPIO_ENU_AF3,
    MGPIO_ENU_AF4,
    MGPIO_ENU_AF5,
    MGPIO_ENU_AF6,
    MGPIO_ENU_AF7,
    MGPIO_ENU_AF8,
    MGPIO_ENU_AF9,
    MGPIO_ENU_AF10,
    MGPIO_ENU_AF11,
    MGPIO_ENU_AF12,
    MGPIO_ENU_AF13,
    MGPIO_ENU_AF14,
    MGPIO_ENU_AF15,
    MGPIO_ENU_AF16
} MGPIO_enuAlternateFunction_t;

typedef union {
    struct {
        uint32_t AFRL0              : 4;          // 6 bits for M
        uint32_t AFRL1              : 4;         // 9 bits for N
        uint32_t AFRL2              : 4;        // 1 reserved bit
        uint32_t AFRL3              : 4;       // 2 bits for P
        uint32_t AFRL4              : 4;      // 4 reserved bits
        uint32_t AFRL5              : 4;     // 1 bit for SRC
        uint32_t AFRL6              : 4;    // 1 reserved bit
        uint32_t AFRL7              : 4;   // 4 bits for Q
        uint32_t AFRL8              : 4;  // 4 reserved bits
    } Bits;                                   // Access individual fields
    uint32_t All_Reg;                        // Access the entire register
} MRCC_union_t;

typedef struct 
{
    MGPIO_enuPort_t Port;
    MGPIO_enuPin_t Pin;
    MGPIO_enuPinMode_t Mode;
    MGPIO_enuPinOutputType_t OutputType;
    MGPIO_enuPinSpeedConfigration_t Speed;
    MGPIO_enuPinConfiguration_t PullUpDownType;
    MGPIO_enuAlternateFunction_t AlternateFunction;
} MGPIO_PinCfg_t;


MGPIO_enuErrorStatus_t MGPIO_enuSetPinValue(MGPIO_enuPort_t Copy_enuPort,MGPIO_enuPin_t Copy_enuPin,MGPIO_PinValue_t Copy_enuPinValue);
 
MGPIO_enuErrorStatus_t MGPIO_enuGetPinValue(MGPIO_enuPort_t Copy_enuPort,MGPIO_enuPin_t Copy_enuPin,uint32_t* Add_u8PinValue);

MGPIO_enuErrorStatus_t MGPIO_enuPinCfg( MGPIO_PinCfg_t* Local_enuPinConfig);

#endif
