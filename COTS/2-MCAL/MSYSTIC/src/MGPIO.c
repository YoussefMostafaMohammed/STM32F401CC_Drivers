#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include "RCC_CFG.h"
#include "RCC.h"
#include "MGPIO_CFG.h"
#include "MGPIO.h"

typedef struct {
    volatile uint32_t GPIO_MODER;
    volatile uint32_t GPIO_OTYPER;
    volatile uint32_t GPIO_OSPEEDR;
    volatile uint32_t GPIO_PUPDR;
    volatile uint32_t GPIO_IDR;
    volatile uint32_t GPIO_ODR;
    volatile uint32_t GPIO_BSRR;
    volatile uint32_t GPIO_LCKR;
    volatile uint32_t GPIO_AFRL;
    volatile uint32_t GPIO_AFRH;
}MGPIO_structREGS_t;



typedef struct {
    volatile uint32_t PWR_CR;
    volatile uint32_t PWR_CSR;
}MPWR_structREGS_t;


#define MGPIO_BASE_ADDRESS  (0x40020000UL)



#define MGPIO_OFFSET        (0x400UL)
#define MGPIOA_BASE_ADDRESS (0x40020000UL)
#define MGPIOB_BASE_ADDRESS (0x40020400UL)
#define MGPIOC_BASE_ADDRESS (0x40020800UL)
#define MGPIOD_BASE_ADDRESS (0x40020C00UL)
#define MGPIOE_BASE_ADDRESS (0x40021000UL)
#define MGPIOH_BASE_ADDRESS (0x40021C00UL)

// #define MGPIOA_REG ((volatile MGPIO_structREGS_t*)MGPIOA_BASE_ADDRESS) 
// volatile MGPIO_structREGS_t* GPIOA_REG = MGPIOA_REG;

// #define MGPIOB_REG ((volatile MGPIO_structREGS_t*)MGPIOB_BASE_ADDRESS) 
// volatile MGPIO_structREGS_t* GPIOB_REG = MGPIOB_REG;

// #define MGPIOC_REG ((volatile MGPIO_structREGS_t*)MGPIOC_BASE_ADDRESS)
// volatile MGPIO_structREGS_t* GPIOC_REG = MGPIOC_REG;

// #define MGPIOD_REG ((volatile MGPIO_structREGS_t*)MGPIOD_BASE_ADDRESS) 
// volatile MGPIO_structREGS_t* GPIOD_REG = MGPIOD_REG;

// #define MGPIOE_REG ((volatile MGPIO_structREGS_t*)MGPIOE_BASE_ADDRESS) 
// volatile MGPIO_structREGS_t* GPIOE_REG = MGPIOE_REG;

// #define MGPIOH_REG ((volatile MGPIO_structREGS_t*)MGPIOH_BASE_ADDRESS) 
// volatile MGPIO_structREGS_t* GPIOH_REG = MGPIOH_REG;

#define GPIOx_GET_BASE_ADD(GPIOx) (MGPIO_structREGS_t*)((uint32_t)MGPIO_BASE_ADDRESS + ((uint32_t)MGPIO_OFFSET * (MGPIO_enuPort_t)GPIOx)) 

MGPIO_enuErrorStatus_t MGPIO_enuPinCfg(  MGPIO_PinCfg_t* Local_enuPinConfig){
    MGPIO_enuErrorStatus_t Local_enuErrorStatus=MGPIO_ENU_OK;
    if(Local_enuPinConfig==NULL){
        Local_enuErrorStatus = MGPIO_ENU_NULLPTR;
    }else if((Local_enuPinConfig->Port > MGPIO_ENU_PORT_E) && (Local_enuPinConfig->Port != MGPIO_ENU_PORT_H)){
        Local_enuErrorStatus = MGPIO_ENU_INVALID_PORT_NUM;
    }else if((Local_enuPinConfig->Pin > MGPIO_ENU_PIN_15)){
        Local_enuErrorStatus = MGPIO_ENU_INVALID_PIN_NUM;
    }else if((Local_enuPinConfig->Mode > MGPIO_ENU_PIN_MODE_ANALOG)){
        Local_enuErrorStatus = MGPIO_ENU_INVALID_PIN_MODE;
    }else if((Local_enuPinConfig->OutputType > MGPIO_ENU_PIN_OUTPUT_OPEN_DRAIN)){
        Local_enuErrorStatus = MGPIO_ENU_INVALID_PIN_OUTPUT_TYPE;
    }else if((Local_enuPinConfig->Speed > MGPIO_ENU_PIN_VERY_HIGH_SPPED)){
        Local_enuErrorStatus = MGPIO_ENU_INVALID_PIN_SPEED;
    }else if((Local_enuPinConfig->PullUpDownType > MGPIO_ENU_PIN_PULLDOWN)){
        Local_enuErrorStatus = MGPIO_ENU_INVALID_PIN_CONFIGRATION;
    }else if((Local_enuPinConfig->AlternateFunction > MGPIO_ENU_AF16 )){
        Local_enuErrorStatus = MGPIO_ENU_INVALID_PIN_ALTERNATIVE_FUNCTION;
    }else{
        volatile MGPIO_structREGS_t* GPIO_REG = GPIOx_GET_BASE_ADD(Local_enuPinConfig->Port);

        GPIO_REG->GPIO_MODER &= ~((Local_enuPinConfig->Mode) << (2*Local_enuPinConfig->Pin));
        GPIO_REG->GPIO_OTYPER &= ~((Local_enuPinConfig->OutputType) << (Local_enuPinConfig->Pin));
        GPIO_REG->GPIO_OSPEEDR &= ~((Local_enuPinConfig->Speed) << (2*Local_enuPinConfig->Pin));
        GPIO_REG->GPIO_PUPDR &= ~((Local_enuPinConfig->PullUpDownType) << (2*Local_enuPinConfig->Pin));
        
        GPIO_REG->GPIO_MODER |= ((Local_enuPinConfig->Mode) << (2*Local_enuPinConfig->Pin));
        GPIO_REG->GPIO_OTYPER |= ((Local_enuPinConfig->OutputType) << (Local_enuPinConfig->Pin));
        GPIO_REG->GPIO_OSPEEDR |= ((Local_enuPinConfig->Speed) << (2*Local_enuPinConfig->Pin));
        GPIO_REG->GPIO_PUPDR |= ((Local_enuPinConfig->PullUpDownType) << (2*Local_enuPinConfig->Pin));
        if(Local_enuPinConfig->Mode==MGPIO_ENU_PIN_MODE_ALTERFUNCTION){
            if(Local_enuPinConfig->Pin<8){
               GPIO_REG->GPIO_AFRL &= ~((Local_enuPinConfig->AlternateFunction) << (4*Local_enuPinConfig->Pin));
               GPIO_REG->GPIO_AFRL |= ((Local_enuPinConfig->AlternateFunction) << (4*Local_enuPinConfig->Pin));
            }else{
               GPIO_REG->GPIO_AFRH  &= ~((Local_enuPinConfig->AlternateFunction) << (4*(Local_enuPinConfig->Pin-8)));
               GPIO_REG->GPIO_AFRH  |= ((Local_enuPinConfig->AlternateFunction) << (4*(Local_enuPinConfig->Pin-8)));
            }
        }else{
            // DO nothing
        }
    }
    return Local_enuErrorStatus;
}

extern MGPIO_enuErrorStatus_t MGPIO_enuSetPinValue(MGPIO_enuPort_t Copy_enuPort,MGPIO_enuPin_t Copy_enuPin,MGPIO_PinValue_t Copy_enuPinValue){
    MGPIO_enuErrorStatus_t Local_enuErrorStatus = MGPIO_ENU_OK;
    if((Copy_enuPort > MGPIO_ENU_PORT_E) && (Copy_enuPort != MGPIO_ENU_PORT_H)){
        Local_enuErrorStatus = MGPIO_ENU_INVALID_PORT_NUM;
    }else if(Copy_enuPin > MGPIO_ENU_PIN_15){
        Local_enuErrorStatus = MGPIO_ENU_INVALID_PIN_NUM;
    }else if(Copy_enuPinValue>MGPIO_PIN_HIGH){
        Local_enuErrorStatus = MGPIO_ENU_INVALID_PIN_VALUE;
    }else{
        volatile MGPIO_structREGS_t* GPIO_REG = GPIOx_GET_BASE_ADD(Copy_enuPort);
        GPIO_REG->GPIO_ODR &= ~(!Copy_enuPinValue<<Copy_enuPin);
        GPIO_REG->GPIO_ODR |= (Copy_enuPinValue<<Copy_enuPin);
    }
    return Local_enuErrorStatus;
}

 MGPIO_enuErrorStatus_t MGPIO_enuGetPinValue(MGPIO_enuPort_t Copy_enuPort,MGPIO_enuPin_t Copy_enuPin,uint32_t* Add_u8PinValue)
{
    volatile MGPIO_structREGS_t* GPIO_REG = GPIOx_GET_BASE_ADD(Copy_enuPort);
    MGPIO_enuErrorStatus_t Local_enuErrorStatus = MGPIO_ENU_OK;
    if((Copy_enuPort > MGPIO_ENU_PORT_E) && (Copy_enuPort != MGPIO_ENU_PORT_H)){
        Local_enuErrorStatus = MGPIO_ENU_INVALID_PORT_NUM;
    }else if(Copy_enuPin > MGPIO_ENU_PIN_15){
        Local_enuErrorStatus = MGPIO_ENU_INVALID_PIN_NUM;
    }else if(Add_u8PinValue==NULL){
        Local_enuErrorStatus=MGPIO_ENU_NULLPTR;
    }else
    {
        *Add_u8PinValue = (GPIO_REG->GPIO_ODR >> Copy_enuPin) & 1;
    }
    return Local_enuErrorStatus;
}
