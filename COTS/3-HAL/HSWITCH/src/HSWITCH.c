#include "MGPIO.h"
#include "MGPIO_CFG.h"
#include "HSCHED.h"
#include "HSCHED_CFG.h"
#include "HSWITCH.h"
#include "HLED_CFG.h"
#include "HSWITCH_CFG.h"

extern HSWITCH_structConfigrations_t HSWITCH_enuArrSwitchConfig[NUMBER_OF_SWITCHS];
static uint32_t HSWITCH_uint32State[NUMBER_OF_SWITCHS]={0};
static uint32_t HSWITCH_uint32PreviousState[NUMBER_OF_SWITCHS]={0};


static HSWITCH_enuErrorStatus_t HSWITCH_enuRunnable(void){
    HSWITCH_enuErrorStatus_t Ret_enuErrorStatus = HSWITCH_ENU_OK;
    static  uint64_t Local_uint32SwitchCounter[NUMBER_OF_SWITCHS];
    uint32_t Local_uint32State=0;
    for(uint32_t switchItr=0;switchItr<NUMBER_OF_SWITCHS;switchItr++){
        Ret_enuErrorStatus=MGPIO_enuGetPinValue(HSWITCH_enuArrSwitchConfig[switchItr].HSWITCH_PORT,HSWITCH_enuArrSwitchConfig[switchItr].HSWITCH_Pin,&Local_uint32State);        
        if(Local_uint32State==HSWITCH_uint32PreviousState[switchItr]){
            Local_uint32SwitchCounter[switchItr]++;
        }else{
            Local_uint32SwitchCounter[switchItr]=0;
        }
        if(Local_uint32SwitchCounter[switchItr]==5){
            HSWITCH_uint32State[switchItr]=Local_uint32State;
        }
        HSWITCH_uint32PreviousState[switchItr]=Local_uint32State;
    }
    return Ret_enuErrorStatus;
}

static HSCHED_RunnableCfg_t Local_structRunnableConfigration={
    .Priority=0,
    .CyclicTime=5,
    .CallBack=HSWITCH_enuRunnable,
    .FirstDelay=0,
};

HSWITCH_enuErrorStatus_t HSWITCH_enuInit(void){
    HSWITCH_enuErrorStatus_t Ret_enuErrorStatus = HSWITCH_ENU_OK;
    MGPIO_enuPort_t Local_enuCurrentPort;
    MGPIO_enuPin_t Local_enuCurrentPin;
    HSWITCH_enuConnectioOpt_t Local_enuCurrentConnection;

    for(uint8_t itr=0;(itr<NUMBER_OF_SWITCHS);itr++){
        Local_enuCurrentPort=HSWITCH_enuArrSwitchConfig[itr].HSWITCH_PORT;
        Local_enuCurrentPin=HSWITCH_enuArrSwitchConfig[itr].HSWITCH_Pin;
        Local_enuCurrentConnection=HSWITCH_enuArrSwitchConfig[itr].HSWITCH_Connection;
        if((Local_enuCurrentPort > MGPIO_ENU_PORT_E) && (Local_enuCurrentPort != MGPIO_ENU_PORT_H)){
            Ret_enuErrorStatus= HSWITCH_ENU_INVALID_PORT_NUM;
        }
        else if(Local_enuCurrentPin < MGPIO_ENU_PIN_0 ||  Local_enuCurrentPin > MGPIO_ENU_PIN_15){
            Ret_enuErrorStatus= HSWITCH_ENU_INVALID_PIN_NUM;
        }
        else{
           /* Shhhhhhhhhhhhhh */
        }

        HSWITCH_uint32PreviousState[itr]=100;

            /*Set the Led to lzow*/
        if(Local_enuCurrentConnection==HSWITCH_PULLUP){
            MGPIO_PinCfg_t Local_structPinConfigration={
                .Port=Local_enuCurrentPort,
                .Pin=Local_enuCurrentPin,
                .Mode=MGPIO_ENU_PIN_MODE_INPUT,
                .OutputType=MGPIO_ENU_PIN_OUTPUT_PUSH_PULL,
                .PullUpDownType=MGPIO_ENU_PIN_PULLUP,
            };
            Ret_enuErrorStatus=MGPIO_enuPinCfg(&Local_structPinConfigration);
        }else if(Local_enuCurrentConnection==HSWITCH_PULLDOWN){

            MGPIO_PinCfg_t Local_structPinConfigration={
                .Port=Local_enuCurrentPort,
                .Pin=Local_enuCurrentPin,
                .Mode=MGPIO_ENU_PIN_MODE_INPUT,
                .OutputType=MGPIO_ENU_PIN_OUTPUT_PUSH_PULL,
                .PullUpDownType=MGPIO_ENU_PIN_PULLDOWN,
            };
    
            Ret_enuErrorStatus=MGPIO_enuPinCfg(&Local_structPinConfigration);

        }else{
            /* Shhhhhhhhhhhhhhhhhhhhh */
        }

        HSCHED_enuRegisterRunnable(&Local_structRunnableConfigration);
    }
    return Ret_enuErrorStatus;
}

HSWITCH_enuErrorStatus_t HSWITCH_enuGetSwitchState(HSWITCH_enuSwitches Copy_enuSwitchNum,HSWITCH_enuState_t* Copy_enuState){
    HSWITCH_enuErrorStatus_t Ret_enuErrorStatus = HSWITCH_ENU_OK;
    if(Copy_enuSwitchNum>NUMBER_OF_SWITCHS){
        Ret_enuErrorStatus=HSWITCH_ENU_INVALID_SWITCH_NUM;   
    }else if(Copy_enuState==NULL)
    {   
        Ret_enuErrorStatus=HSWITCH_ENU_NULL_PTR;   
    }else{
        *Copy_enuState=HSWITCH_uint32State[Copy_enuSwitchNum];
    }
    return Ret_enuErrorStatus;
}


