#include "MSYSTICK.h"
#include "MSYSTICK_CFG.h"

volatile uint64_t global_uint64CyclicTIme=0;
volatile uint64_t global_uint64DelayCounter=0;
systickcbf_t global_CallBackFunction=NULL;

#define MSYSTICK_BASE_ADDRESS (0xE000E010)

#define MSYSTICK_CLKSOURCE_MASK 0x00000004
#define MSYSTICK_EXCEPTION_REQUEST_MASK 0x00000002
#define MSYSTICK_COUNTER_MASK 0x00000001
#define MSYSTICK_LOAD_REGISTER_MASK 0x00FFFFFF

typedef struct{
    volatile uint32_t STK_CTRL;
    volatile uint32_t STK_LOAD;
    volatile uint32_t STK_VAL;
    volatile uint32_t STK_CALIB;
}MSYSTICK_StructREGS_t;

#define MSYSTICK_REG ((volatile MSYSTICK_StructREGS_t*)MSYSTICK_BASE_ADDRESS)

MSYSTICK_enuErrorStatus_t MSYSTICK_enuInit(void){
    MSYSTICK_enuErrorStatus_t Ret_enuErrorStatus=MSYSTICK_ENU_OK;
    volatile MSYSTICK_StructREGS_t* SYSTICK_REG = MSYSTICK_REG;

    #if MSYSTIC_CLOCK_SOURCE == MSYSTICK_CLOCK_SOURCE_AHB
        SYSTICK_REG->STK_CTRL |= MSYSTICK_CLKSOURCE_MASK;
    #elif MSYSTIC_CLOCK_SOURCE == MSYSTICK_CLOCK_SOURCE_AHB_BY_8 
        SYSTICK_REG->STK_CTRL &= ~MSYSTICK_CLKSOURCE_MASK;
    #endif

    #if MSYSTICK_TICKINT == MSYSTICK_ENABLE_EXCEPTION_REQUEST
        SYSTICK_REG->STK_CTRL |= MSYSTICK_EXCEPTION_REQUEST_MASK;
    #elif MSYSTICK_TICKINT == MSYSTICK_DISABLE_EXCEPTION_REQUEST 
        SYSTICK_REG->STK_CTRL &= ~MSYSTICK_EXCEPTION_REQUEST_MASK;
    #endif

    volatile uint64_t Local_uint64Ticks=0,Local_uint64Clock=0;
    Ret_enuErrorStatus=MRCC_enuGetSystemClock(&Local_uint64Clock);
    Local_uint64Ticks=((Local_uint64Clock)/(1e3))+1; 
    SYSTICK_REG->STK_VAL=0;
    SYSTICK_REG->STK_LOAD&=~MSYSTICK_LOAD_REGISTER_MASK;

    SYSTICK_REG->STK_LOAD|=Local_uint64Ticks;
    return Ret_enuErrorStatus;
}

MSYSTICK_enuErrorStatus_t SYSTICK_enuStart(void){
    MSYSTICK_enuErrorStatus_t Ret_enuErrorStatus=MSYSTICK_ENU_OK;
    volatile MSYSTICK_StructREGS_t* SYSTICK_REG = MSYSTICK_REG;
    SYSTICK_REG->STK_CTRL |= MSYSTICK_COUNTER_MASK;
    return Ret_enuErrorStatus;
}

MSYSTICK_enuErrorStatus_t SYSTICK_enuStop(void){
    MSYSTICK_enuErrorStatus_t Ret_enuErrorStatus=MSYSTICK_ENU_OK;
    volatile MSYSTICK_StructREGS_t* SYSTICK_REG = MSYSTICK_REG;
    SYSTICK_REG->STK_CTRL &= ~MSYSTICK_COUNTER_MASK;
    return Ret_enuErrorStatus;
}

MSYSTICK_enuErrorStatus_t MSYSTICK_enuDelay_ms(uint32_t Copy_uint32DelayMs){
    MSYSTICK_enuErrorStatus_t Ret_enuErrorStatus=MSYSTICK_ENU_OK;
    global_uint64DelayCounter=0;
    while(global_uint64DelayCounter!=Copy_uint32DelayMs);
    return Ret_enuErrorStatus;
}

MSYSTICK_enuErrorStatus_t MSYSTICK_enuSetCyclicFunction(systickcbf_t Addr_CallBackFunction, uint32_t Copy_uint32CyclicTime){
    MSYSTICK_enuErrorStatus_t Ret_enuErrorStatus=MSYSTICK_ENU_OK;
    if(Addr_CallBackFunction==NULL){
        Ret_enuErrorStatus=MSYSTICK_ENU_NULLPTR;
    }else{
        global_uint64CyclicTIme=Copy_uint32CyclicTime;
        global_CallBackFunction=Addr_CallBackFunction;
    }
    return Ret_enuErrorStatus;
}

SysTick_Handler(void){
    static uint64_t Local_uint64CyclicCounter=0;
    if(global_CallBackFunction==NULL){

    }else{
        if(Local_uint64CyclicCounter==global_uint64CyclicTIme){
            global_CallBackFunction();
            Local_uint64CyclicCounter=0;
        }
    }
    global_uint64DelayCounter++;
    Local_uint64CyclicCounter++;
    return;
}
