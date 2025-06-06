#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "MGPIO.h"
#include "HLED_CFG.h"
#include "HLED.h"

extern HLED_structConfigrations_t HLED_enuArrLedConfig[NUMBER_OF_LEDS];

HLED_enuErrorStatus_t HLED_enuInit(void){
    HLED_enuErrorStatus_t Ret_enuErrorStatus = HLED_ENU_OK;
    MGPIO_enuPort_t Local_enuCurrentPort;
    MGPIO_enuPin_t Local_enuCurrentPin;
    HLED_enuConnectioOpt_t Local_enuCurrentConnection;
    for(uint8_t itr=0; (itr<NUMBER_OF_LEDS); itr++){
        Local_enuCurrentPort=HLED_enuArrLedConfig[itr].HLED_PORT;
        Local_enuCurrentPin=HLED_enuArrLedConfig[itr].HLED_Pin;
        Local_enuCurrentConnection=HLED_enuArrLedConfig[itr].HLED_Connection;        
        if((Local_enuCurrentPort > MGPIO_ENU_PORT_E) && (Local_enuCurrentPort != MGPIO_ENU_PORT_H)){
            Ret_enuErrorStatus= MGPIO_ENU_INVALID_PORT_NUM;
        }
        else if(Local_enuCurrentPin <MGPIO_ENU_PIN_0 ||  Local_enuCurrentPin >MGPIO_ENU_PIN_15 ){
            Ret_enuErrorStatus= MGPIO_ENU_INVALID_PIN_NUM;
        }
        else{
           /* Shhhhhhhhhhhhhh */
        }
         /* Set the Led to High */
	    MGPIO_PinCfg_t Pin_Cfg = {
    	    .Port = HLED_enuArrLedConfig[itr].HLED_PORT, 
    	    .Pin  = HLED_enuArrLedConfig[itr].HLED_Pin,
		    .Mode = MGPIO_ENU_PIN_MODE_OUTPUT,
	        .OutputType = HLED_enuArrLedConfig[itr].HLED_PinType,
	    };

        Ret_enuErrorStatus=MGPIO_enuPinCfg(&Pin_Cfg);
        if(Local_enuCurrentConnection==HLED_ACTIVE_HIGH){
            if(HLED_enuArrLedConfig[itr].HLED_PinState==HLED_ENU_HIGH){
                Ret_enuErrorStatus=MGPIO_enuSetPinValue(Local_enuCurrentPort,Local_enuCurrentPin,MGPIO_PIN_HIGH);
            }else{
                Ret_enuErrorStatus=MGPIO_enuSetPinValue(Local_enuCurrentPort,Local_enuCurrentPin,MGPIO_PIN_LOW);
            }
        }
        else if(Local_enuCurrentConnection==HLED_ACTIVE_LOW){   
            if(HLED_enuArrLedConfig[itr].HLED_PinState==HLED_ENU_HIGH){
                Ret_enuErrorStatus=MGPIO_enuSetPinValue(Local_enuCurrentPort,Local_enuCurrentPin,MGPIO_PIN_LOW);
            }else{
                Ret_enuErrorStatus=MGPIO_enuSetPinValue(Local_enuCurrentPort,Local_enuCurrentPin,MGPIO_PIN_HIGH);
            }        
        }
        else{
            /* Shhhhhhhhhhhhhhhhhhhhh */
        }
    }
    return Ret_enuErrorStatus;
}

HLED_enuErrorStatus_t HLED_enuSetLedState(uint8_t Copy_u8LedName, HLED_enuPinState_t Copy_u8State){
    HLED_enuErrorStatus_t Ret_enuErrorStatus = HLED_ENU_OK;
    MGPIO_enuPort_t Local_enuCurrentPort=HLED_enuArrLedConfig[Copy_u8LedName].HLED_PORT;
    MGPIO_enuPin_t Local_enuCurrentPin=HLED_enuArrLedConfig[Copy_u8LedName].HLED_Pin;
    HLED_enuConnectioOpt_t Local_enuCurrentConnection=HLED_enuArrLedConfig[Copy_u8LedName].HLED_Connection;
    if((Local_enuCurrentPort > MGPIO_ENU_PORT_E) && (Local_enuCurrentPort != MGPIO_ENU_PORT_H)){
        Ret_enuErrorStatus= MGPIO_ENU_INVALID_PORT_NUM;
    }else if(Local_enuCurrentPin <MGPIO_ENU_PIN_0 ||  Local_enuCurrentPin >MGPIO_ENU_PIN_15 ){
        Ret_enuErrorStatus= MGPIO_ENU_INVALID_PIN_NUM;
    }else if((Local_enuCurrentConnection!=HLED_ACTIVE_HIGH) && (Local_enuCurrentConnection != HLED_ACTIVE_LOW)){
        Ret_enuErrorStatus= HLED_ENU_INVALID_LED_CONNECTION;
    }else if((Copy_u8State!=HLED_ENU_HIGH) && (Copy_u8State!=HLED_ENU_LOW)){
        Ret_enuErrorStatus=HLED_ENU_INVALID_LED_STATE;
    }else{
        if(Local_enuCurrentConnection==HLED_ACTIVE_HIGH){
            Ret_enuErrorStatus=MGPIO_enuSetPinValue(Local_enuCurrentPort,Local_enuCurrentPin,Copy_u8State);
        }else if(Local_enuCurrentConnection==HLED_ACTIVE_LOW){
            Ret_enuErrorStatus=MGPIO_enuSetPinValue(Local_enuCurrentPort,Local_enuCurrentPin,!Copy_u8State);
        }else{
                /* Shhhhhhhhhhhhhhhhhhhhh */
        } 
    }
    return Ret_enuErrorStatus;
}