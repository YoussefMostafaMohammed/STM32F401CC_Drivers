#include "HSCHED.h"
#include "HSCHED_CFG.h"
#include "MSYSTICK.h"
#include "MSYSTICK_CFG.h"

static HSCHED_RunnableCfg_t* HSCHED_structRunnablesArray[HSCHED_NUMBER_OF_RUNABLES];

static HSCHED_enuErrorStatus HSCHED_enuCyclicRunnables(){
    MSYSTICK_enuErrorStatus_t Ret_enuErrorStatus=HSCHED_ENU_OK;
    static uint64_t Local_uint64Counter=0;
    for(uint32_t Runnableitr=0; Runnableitr<HSCHED_NUMBER_OF_RUNABLES; Runnableitr++){
        if(HSCHED_structRunnablesArray[Runnableitr]!=NULL){
            if(Local_uint64Counter%HSCHED_structRunnablesArray[Runnableitr]->FirstDelay==0 && HSCHED_structRunnablesArray[Runnableitr]->FirstDelay!=0 && Local_uint64Counter!=0){
                HSCHED_structRunnablesArray[Runnableitr]->FirstDelay=0;
                HSCHED_structRunnablesArray[Runnableitr]->CallBack();
            }
            else if (HSCHED_structRunnablesArray[Runnableitr]->FirstDelay==0 ){
                if(Local_uint64Counter%HSCHED_structRunnablesArray[Runnableitr]->CyclicTime==0 ){
                    HSCHED_structRunnablesArray[Runnableitr]->CallBack();
                }
            }
        }
    }
    Local_uint64Counter++;
    return Ret_enuErrorStatus;
}

HSCHED_enuErrorStatus HSCHED_enuInit(void){
    MSYSTICK_enuErrorStatus_t Ret_enuErrorStatus=HSCHED_ENU_OK;
    for(uint32_t itr=0;itr<HSCHED_NUMBER_OF_RUNABLES;itr++){
        HSCHED_structRunnablesArray[itr]=NULL;
    }
    // init systich  
    Ret_enuErrorStatus=MSYSTICK_enuInit();
    // set call back finction cyclic 
    MSYSTICK_enuSetCyclicFunction(&HSCHED_enuCyclicRunnables,1);
    return Ret_enuErrorStatus;
}   

HSCHED_enuErrorStatus HSCHED_enuRegisterRunnable(HSCHED_RunnableCfg_t* Add_HSCHED_cfg){
    MSYSTICK_enuErrorStatus_t Ret_enuErrorStatus=HSCHED_ENU_OK;
    if(Add_HSCHED_cfg==NULL){
        Ret_enuErrorStatus=HSCHED_ENU_NULL_PTR;
    }else if(Add_HSCHED_cfg->CallBack==NULL){
        Ret_enuErrorStatus=HSCHED_ENU_NULL_PTR_CALLBACK;
    }else if(Add_HSCHED_cfg->Priority >HSCHED_NUMBER_OF_RUNABLES){
        Ret_enuErrorStatus=HSCHED_ENU_INVALID_PRIORITY;
    }else if(HSCHED_structRunnablesArray[Add_HSCHED_cfg->Priority]!=NULL){
        Ret_enuErrorStatus=HSCHED_ENU_INVALID_PRIORITY;
    }else{
        HSCHED_structRunnablesArray[Add_HSCHED_cfg->Priority]=Add_HSCHED_cfg;
    }
    return Ret_enuErrorStatus;
}

HSCHED_enuErrorStatus HSCHED_enuStart(){
    MSYSTICK_enuErrorStatus_t Ret_enuErrorStatus=HSCHED_ENU_OK;
    Ret_enuErrorStatus=SYSTICK_enuStart();
    return Ret_enuErrorStatus;
}

HSCHED_enuErrorStatus HSCHED_enuStop(){
    MSYSTICK_enuErrorStatus_t Ret_enuErrorStatus=HSCHED_ENU_OK;
    Ret_enuErrorStatus=HSCHED_enuStop();
    return Ret_enuErrorStatus;
}