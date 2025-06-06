#include "MNVIC_CFG.h"
#include "MNVIC.h"

#define NVIC_BASE_ADDRESS 0xE000E100
#define MSCB_BASE_ADDRESS 0xE000E008

typedef struct{
    volatile uint32_t ISER[8]; // set enable
    const uint32_t RESERVED0[24]; // 96 bytes in-between
    volatile uint32_t ICER[8]; // clear enable
    const uint32_t RESERVED1[24]; // 96 bytes in-between
    volatile uint32_t ISPR[8]; // set pending
    const uint32_t RESERVED2[24]; // 96 bytes in-between
    volatile uint32_t ICPR[8]; // clear pending
    const uint32_t RESERVED3[24]; // 96 bytes in-between
    volatile uint32_t IABR[8]; // active bit
    const uint32_t RESERVED4[56]; // 224 bytes in-between
    volatile uint8_t IPR[240]; // priority
    const uint32_t RESERVED5[644]; // 2576 bytes in-between
    volatile uint32_t STIR; // software trigger
}MNVIC_StructREGS_t;

typedef struct{
    volatile uint32_t ACTLR;
    const uint32_t RESERVED0[829];
    volatile uint32_t CPUID;
    volatile uint32_t ICSR;
    volatile uint32_t VTOR;
    volatile uint32_t AIRCR;
    volatile uint32_t SCR;
    volatile uint32_t CCR;
    volatile uint32_t SHPR1; 
    volatile uint32_t SHPR2;
    volatile uint32_t SHPR3;
    volatile uint32_t SHCSR;
    volatile uint32_t CFSR;
    volatile uint32_t MMSR; 
    volatile uint32_t BFSR; 
    volatile uint32_t UFSR;
    volatile uint32_t HFSR;
    volatile uint32_t MMAR;
    volatile uint32_t BFAR; 
    volatile uint32_t AFSR;
}MSCB_StructREGS_t;

#define MNVIC_REG (( volatile MNVIC_StructREGS_t*)NVIC_BASE_ADDRESS)
#define MSCB_REG (( volatile MSCB_StructREGS_t*)MSCB_BASE_ADDRESS)

#define IS_PRIORITY_GROUPING(PG) ((PG==Group_priorities_16)||(PG==Group_priorities_8)||(PG==Group_priorities_4)||(PG==Group_priorities_2)||(PG==Group_priorities_NONE))
#define PRIORITY_GROUPING_MASK 0x00000700
#define SYSTEM_RESET_MASK 0x00000004

#if STM == STM32F401CC
    #define MAX_IRQ_NUMBER NVIC_enuSPI4
#else
    #define MAX_IRQ_NUMBER 239
#endif 

MNVIC_enuErrorStatus_t MNVIC_EnableIRQ(MNVIC_enuIRQn_t Copy_enuIRQNumber){
    MNVIC_enuErrorStatus_t Ret_enuErrorStatus=MNVIC_ENU_OK;
    volatile MNVIC_StructREGS_t* NVIC_REG = MNVIC_REG;
    uint32_t Locau_uint32RegisterNumber=(Copy_enuIRQNumber/32);
    uint32_t Locau_uint32BitNumber = (Copy_enuIRQNumber%32);
    uint32_t Locau_uint32BitStatus = ((NVIC_REG->ISER[Locau_uint32RegisterNumber]>>Locau_uint32BitNumber)&1);
    if(Copy_enuIRQNumber>MAX_IRQ_NUMBER){
        Ret_enuErrorStatus = MNVIC_ENU_INVALID_IRQ_NUMBER;
    }else if(Locau_uint32BitStatus){
        Ret_enuErrorStatus = MNVIC_ENU_IRQ_NUMBER_INTERRUPT_IS_ENABLED;
    }else{
        NVIC_REG->ICPR[Locau_uint32RegisterNumber]|= (1<<Locau_uint32BitNumber);
        NVIC_REG->ISER[Locau_uint32RegisterNumber]|= (1<<Locau_uint32BitNumber);
    }
    return Ret_enuErrorStatus;
}

MNVIC_enuErrorStatus_t MNVIC_DisableIRQ(MNVIC_enuIRQn_t Copy_enuIRQNumber){
    MNVIC_enuErrorStatus_t Ret_enuErrorStatus=MNVIC_ENU_OK;
    volatile MNVIC_StructREGS_t* NVIC_REG = MNVIC_REG;
    if(Copy_enuIRQNumber>MAX_IRQ_NUMBER){
        Ret_enuErrorStatus=MNVIC_ENU_INVALID_IRQ_NUMBER;
    }else{
        uint32_t Locau_uint32RegisterNumber=(Copy_enuIRQNumber/32);
        uint32_t Locau_uint32BitNumber=(Copy_enuIRQNumber%32);
        NVIC_REG->ICER[Locau_uint32RegisterNumber]|= (1<<Locau_uint32BitNumber);
    }
    return Ret_enuErrorStatus;
}

MNVIC_enuErrorStatus_t MNVIC_SetPendingIRQ(MNVIC_enuIRQn_t Copy_enuIRQNumber){
    MNVIC_enuErrorStatus_t Ret_enuErrorStatus=MNVIC_ENU_OK;
    volatile MNVIC_StructREGS_t* NVIC_REG = MNVIC_REG;
    if(Copy_enuIRQNumber>MAX_IRQ_NUMBER){
        Ret_enuErrorStatus=MNVIC_ENU_INVALID_IRQ_NUMBER;
    }else{
        uint32_t Locau_uint32RegisterNumber=(Copy_enuIRQNumber/32);
        uint32_t Locau_uint32BitNumber=(Copy_enuIRQNumber%32);
        NVIC_REG->ISPR[Locau_uint32RegisterNumber]|= (1<<Locau_uint32BitNumber);
    }
    return Ret_enuErrorStatus;   
}

MNVIC_enuErrorStatus_t MNVIC_GetPendingIRQ(MNVIC_enuIRQn_t Copy_enuIRQNumber, MNVIC_enuBitStatus_t* Addr_uint32PendingIRQ ){
    MNVIC_enuErrorStatus_t Ret_enuErrorStatus=MNVIC_ENU_OK;
    volatile MNVIC_StructREGS_t* NVIC_REG = MNVIC_REG;
    if(Copy_enuIRQNumber>MAX_IRQ_NUMBER){
        Ret_enuErrorStatus=MNVIC_ENU_INVALID_IRQ_NUMBER;
    }else if(Addr_uint32PendingIRQ==NULL){
        Ret_enuErrorStatus=MNVIC_ENU_NULLPTR;
    }else{
        uint32_t Locau_uint32RegisterNumber=(Copy_enuIRQNumber/32);
        uint32_t Locau_uint32BitNumber=(Copy_enuIRQNumber%32);
        *Addr_uint32PendingIRQ=((NVIC_REG->ISPR[Locau_uint32RegisterNumber]>>Locau_uint32BitNumber)&1);
    }
    return Ret_enuErrorStatus;

}

MNVIC_enuErrorStatus_t MNVIC_ClearPendingIRQ (MNVIC_enuIRQn_t Copy_enuIRQNumber){
    MNVIC_enuErrorStatus_t Ret_enuErrorStatus=MNVIC_ENU_OK;
    volatile MNVIC_StructREGS_t* NVIC_REG = MNVIC_REG;
    if(Copy_enuIRQNumber>MAX_IRQ_NUMBER){
        Ret_enuErrorStatus=MNVIC_ENU_INVALID_IRQ_NUMBER;
    }else{
        uint32_t Locau_uint32RegisterNumber=(Copy_enuIRQNumber/32);
        uint32_t Locau_uint32BitNumber=(Copy_enuIRQNumber%32);
        NVIC_REG->ICPR[Locau_uint32RegisterNumber]|= (1<<Locau_uint32BitNumber);
    }
    return Ret_enuErrorStatus;  
}

MNVIC_enuErrorStatus_t MNVIC_GetActive(MNVIC_enuIRQn_t Copy_enuIRQNumber, uint32_t* Addr_uint32ActiveIRQ ){
    MNVIC_enuErrorStatus_t Ret_enuErrorStatus=MNVIC_ENU_OK;
    volatile MNVIC_StructREGS_t* NVIC_REG = MNVIC_REG;
    if(Copy_enuIRQNumber>MAX_IRQ_NUMBER){
        Ret_enuErrorStatus=MNVIC_ENU_INVALID_IRQ_NUMBER;
    }else if(Addr_uint32ActiveIRQ==NULL){
        Ret_enuErrorStatus=MNVIC_ENU_NULLPTR;
    }else{
        uint32_t Locau_uint32RegisterNumber=(Copy_enuIRQNumber/32);
        uint32_t Locau_uint32BitNumber=(Copy_enuIRQNumber%32);
        *Addr_uint32ActiveIRQ=((NVIC_REG->IABR[Locau_uint32RegisterNumber]>>Locau_uint32BitNumber)&1);
    }
    return Ret_enuErrorStatus;
}

MNVIC_enuErrorStatus_t MNVIC_SetPriority(MNVIC_enuIRQn_t Copy_enuIRQNumber, uint8_t Copy_uint32priority){
    MNVIC_enuErrorStatus_t Ret_enuErrorStatus=MNVIC_ENU_OK;
    volatile MNVIC_StructREGS_t* NVIC_REG = MNVIC_REG;
    if(Copy_enuIRQNumber>MAX_IRQ_NUMBER){
        Ret_enuErrorStatus=MNVIC_ENU_INVALID_IRQ_NUMBER;
    }else if(Copy_uint32priority>15){
        Ret_enuErrorStatus=MNVIC_ENU_INVALID_PRIORITY;
    }else{
        NVIC_REG->IPR[Copy_enuIRQNumber]= (Copy_uint32priority<<4);
    }
    return Ret_enuErrorStatus;  
}


MNVIC_enuErrorStatus_t MNVIC_GetPriority(MNVIC_enuIRQn_t Copy_enuIRQNumber, uint8_t* Addr_uint32Priority){
    MNVIC_enuErrorStatus_t Ret_enuErrorStatus=MNVIC_ENU_OK;
    volatile MNVIC_StructREGS_t* NVIC_REG = MNVIC_REG;
    if(Copy_enuIRQNumber>MAX_IRQ_NUMBER){
        Ret_enuErrorStatus=MNVIC_ENU_INVALID_IRQ_NUMBER;
    }else if(Addr_uint32Priority==NULL){
        Ret_enuErrorStatus=MNVIC_ENU_NULLPTR;
    }else{
        *Addr_uint32Priority=((NVIC_REG->IPR[Copy_enuIRQNumber]>>4));
    }
    return Ret_enuErrorStatus;
}

MNVIC_enuErrorStatus_t MNVIC_SetPriorityGrouping(MNVIC_enuPriority_Grouping priority_grouping){
    MNVIC_enuErrorStatus_t Ret_enuErrorStatus=MNVIC_ENU_OK;
    volatile MSCB_StructREGS_t* SCB_REG = MSCB_REG;
    if(!IS_PRIORITY_GROUPING(priority_grouping)){
        Ret_enuErrorStatus = MNVIC_ENU_INVALID_PRIORITY_GROUPING;
    }else{
        uint32_t Local_uint32AIRCR = SCB_REG->AIRCR;
        Local_uint32AIRCR &= (~PRIORITY_GROUPING_MASK);
        Local_uint32AIRCR |= (priority_grouping << 8);
        SCB_REG->AIRCR=Local_uint32AIRCR;
    }
    return Ret_enuErrorStatus;
}

MNVIC_enuErrorStatus_t MNVIC_SystemReset(void){
    MNVIC_enuErrorStatus_t Ret_enuErrorStatus=MNVIC_ENU_OK;
    volatile MSCB_StructREGS_t* SCB_REG = MSCB_REG;
    SCB_REG->AIRCR |= SYSTEM_RESET_MASK;
    return Ret_enuErrorStatus;
}
