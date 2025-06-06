#include "MGPIO.h"
#include "MGPIO_CFG.h"
#include "HSCHED.h"
#include "HSCHED_CFG.h"
#include "HLED_CFG.h"
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "HCLCD_CFG.h"
#include "HCLCD.h"

#define DDRAM_BASE_COMMAND 0b10000000
#define CGRAM_BASE_COMMAND 0b01000000

/* array of LCDs */
extern HCLCD_structLcdCfg_t HCLCD_enuArrLcdConfig[NUMBER_OF_LCDS];

/* Static functions */
static float64_t HCLCD_enuWriteNumberHelperPowerFunction(float64_t base, float64_t power);
static HCLCD_enuErrorStatus_t HCLCD_enuWriteNumberHelper(uint64_t Copyfloat64_number, uint8_t Copy_uint8LcdNum);
static HCLCD_enuErrorStatus_t HCLCD_enuAsyncInitHelper(uint8_t Copyu8Command, uint8_t Copy_uint8LcdNum);
static void HCLCD_enuRunnable(void);
static void HCLCD_enuRunnableInit(void);
static void HCLCD_enuRunnableWrite(void);
static void HCLCD_enuRunnableInitHelper(void);
static HCLCD_enuErrorStatus_t HCLCD_enuAsyncWriteStringHelper(uint8_t Copyu8Data, uint8_t Copy_uint8LcdNum);
static void HCLCD_enuRunnableWriteStringHelper(void);

/* Runnable Configration */
static HSCHED_RunnableCfg_t Global_structRunnableConfigration={
    .Priority=0,
    .CyclicTime=2,
    .CallBack=HCLCD_enuRunnable,
    .FirstDelay=0,
};

typedef enum
{
    INIT_STATE,
    INIT_POWER_ON = 20,
    INIT_FUNC_SET,
    INIT_DISPLAY_ON,
    INIT_DISPLAY_CLEAR,
    INIT_ENTRY_MODE,
    INIT_FINISH_STATE
}HLCD_enuInitState_t;

typedef enum
{
    WRITE_SEND_8BIT,
    WRITE_SEND_FIRST_4_BIT,
    WRITE_SEND_SECOND_4_BIT,
    WRITE_ENABLE_PULSE,
    WRITE_DISABLE_PULSE,
    WRITE_DISABLE_PULSE_4BIT,
    WRITE_SEND_STRING,
    WRITE_FINISH_STATE,
}HLCD_enuWriteState_t;

typedef enum
{
    MODE_NONE=0,
    MODE_INIT,
    MODE_INIT_HELPER,
    MODE_WRITE,
    MODE_WRITE_STRING,
    MODE_WRITE_STRING_HELPER,
    MODE_WRITE_NUMBER,
    MODE_READ
} HCLCDMode_t;

static HLCD_enuWriteState_t Global_enuLcdWriteState[NUMBER_OF_LCDS];
static HLCD_enuInitState_t Global_enuLcdInitState[NUMBER_OF_LCDS];
static HCLCDMode_t Global_enuLcdMode[NUMBER_OF_LCDS];
static bool flag_isBusy[NUMBER_OF_LCDS];
static uint8_t Global_uint8CommandData[NUMBER_OF_LCDS];
static uint8_t Global_uint8LcdNum=0;
static uint8_t Global_uint8First4BitSent=0;
static sint8_t *Globaluint8_string=NULL;

static void HCLCD_enuRunnableInit(void){ 
        if(Global_enuLcdMode[Global_uint8LcdNum]==MODE_INIT){
            switch (Global_enuLcdInitState[Global_uint8LcdNum])
            {
                case INIT_POWER_ON:
                    Global_enuLcdInitState[Global_uint8LcdNum]=INIT_FUNC_SET;
                    break;
                case INIT_FUNC_SET:
                        if(HCLCD_enuArrLcdConfig[Global_uint8LcdNum].ClcdMode == HCLCD_4_BIT_MODE){
                            HCLCD_enuAsyncInitHelper(FUNCTION_SET_4BIT_2_LINES_5x8, Global_uint8LcdNum);
                        }else{
                            HCLCD_enuAsyncInitHelper(FUNCTION_SET_8BIT_2_LINES_5x8, Global_uint8LcdNum);
                        }
                        Global_enuLcdInitState[Global_uint8LcdNum]=INIT_DISPLAY_ON;
                    break;
                case INIT_DISPLAY_ON:
                        HCLCD_enuAsyncInitHelper(DISPLAY_ON_CURSOR_BLINK, Global_uint8LcdNum);
                        Global_enuLcdInitState[Global_uint8LcdNum]=INIT_DISPLAY_CLEAR;
                    break;
                case INIT_DISPLAY_CLEAR:
                        HCLCD_enuAsyncInitHelper(CLEAR_DISPLAY, Global_uint8LcdNum);
                        Global_enuLcdInitState[Global_uint8LcdNum]=INIT_ENTRY_MODE;
                    break;
                case INIT_ENTRY_MODE:
                        HCLCD_enuAsyncInitHelper(DISABLE_AUTO_SHIFT, Global_uint8LcdNum);
                        Global_enuLcdInitState[Global_uint8LcdNum]=INIT_FINISH_STATE;
                    break;
                case INIT_FINISH_STATE:
                    if(Global_uint8LcdNum>=NUMBER_OF_LCDS){
                        Global_enuLcdMode[Global_uint8LcdNum]=MODE_NONE;
                        flag_isBusy[Global_uint8LcdNum]=0;
                    }else{
                        flag_isBusy[Global_uint8LcdNum]=0;
                        Global_uint8LcdNum++;
                    }
                    break;
                default:
                    Global_enuLcdInitState[Global_uint8LcdNum]++;
                break;
            } 
    }
}

static void HCLCD_enuRunnableInitHelper(void){
        switch (Global_enuLcdWriteState[Global_uint8LcdNum])
        {
            case WRITE_SEND_8BIT:{
                for(uint8_t itr=0; itr<HCLCD_enuArrLcdConfig[Global_uint8LcdNum].ClcdMode; itr++){
                    MGPIO_enuPort_t Local_enuPortNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr].portNum;
                    MGPIO_enuPin_t Local_enuPinNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr].pinNum;
                    MGPIO_enuSetPinValue(Local_enuPortNum,Local_enuPinNum,(Global_uint8CommandData[Global_uint8LcdNum]>>itr)&1);
                }
                Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_ENABLE_PULSE;
                break;
            }
            case WRITE_SEND_FIRST_4_BIT:{
                for(uint8_t itr=0; itr<HCLCD_enuArrLcdConfig[Global_uint8LcdNum].ClcdMode; itr++){
                    MGPIO_enuPort_t Local_enuPortNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr+4].portNum;
                    MGPIO_enuPin_t Local_enuPinNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr+4].pinNum;
                    MGPIO_enuSetPinValue(Local_enuPortNum,Local_enuPinNum,(Global_uint8CommandData[Global_uint8LcdNum]>>(itr+4))&1);
                }
                Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_ENABLE_PULSE;
                    break;
            }
            case WRITE_SEND_SECOND_4_BIT:{
                for(uint8_t itr=0; itr<HCLCD_enuArrLcdConfig[Global_uint8LcdNum].ClcdMode; itr++){
                    MGPIO_enuPort_t Local_enuPortNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr+4].portNum;
                    MGPIO_enuPin_t Local_enuPinNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr+4].pinNum;
                    MGPIO_enuSetPinValue(Local_enuPortNum,Local_enuPinNum,(Global_uint8CommandData[Global_uint8LcdNum]>>(itr))&1);
                }
                Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_ENABLE_PULSE;
                break;
            }
            case WRITE_ENABLE_PULSE:{
                MGPIO_enuPort_t Local_enuPortNumE=HCLCD_enuArrLcdConfig[Global_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].portNum;
                MGPIO_enuPin_t Local_enuPinNumE=HCLCD_enuArrLcdConfig[Global_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].pinNum;
                MGPIO_enuSetPinValue(Local_enuPortNumE,Local_enuPinNumE,MGPIO_PIN_HIGH);
                Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_DISABLE_PULSE;
                break;
            }
            case WRITE_DISABLE_PULSE:{
                MGPIO_enuPort_t Local_enuPortNumE=HCLCD_enuArrLcdConfig[Global_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].portNum;
                MGPIO_enuPin_t Local_enuPinNumE=HCLCD_enuArrLcdConfig[Global_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].pinNum;
                MGPIO_enuSetPinValue(Local_enuPortNumE,Local_enuPinNumE,MGPIO_PIN_LOW);
                if(HCLCD_enuArrLcdConfig[Global_uint8LcdNum].ClcdMode == HCLCD_4_BIT_MODE && !Global_uint8First4BitSent){
                    Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_SEND_SECOND_4_BIT;
                    Global_uint8First4BitSent=1;
                }else{
                    Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_FINISH_STATE;
                }
                break;
            }
            case WRITE_FINISH_STATE:{
                Global_enuLcdMode[Global_uint8LcdNum]=MODE_INIT;
                break;
            }
            default:
                break;
        }
}

static void HCLCD_enuRunnableWrite(void){
    switch (Global_enuLcdWriteState[Global_uint8LcdNum])
    {
        case WRITE_SEND_8BIT:{
            for(uint8_t itr=0; itr<HCLCD_enuArrLcdConfig[Global_uint8LcdNum].ClcdMode; itr++){
                MGPIO_enuPort_t Local_enuPortNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr].portNum;
                MGPIO_enuPin_t Local_enuPinNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr].pinNum;
                MGPIO_enuSetPinValue(Local_enuPortNum,Local_enuPinNum,(Global_uint8CommandData[Global_uint8LcdNum]>>itr)&1);
            }
            Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_ENABLE_PULSE;
            break;
        }
        case WRITE_SEND_FIRST_4_BIT:{
            for(uint8_t itr=0; itr<HCLCD_enuArrLcdConfig[Global_uint8LcdNum].ClcdMode; itr++){
                MGPIO_enuPort_t Local_enuPortNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr+4].portNum;
                MGPIO_enuPin_t Local_enuPinNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr+4].pinNum;
                MGPIO_enuSetPinValue(Local_enuPortNum,Local_enuPinNum,(Global_uint8CommandData[Global_uint8LcdNum]>>(itr+4))&1);
            }
            Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_ENABLE_PULSE;
                break;
        }
        case WRITE_SEND_SECOND_4_BIT:{
            for(uint8_t itr=0; itr<HCLCD_enuArrLcdConfig[Global_uint8LcdNum].ClcdMode; itr++){
                MGPIO_enuPort_t Local_enuPortNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr+4].portNum;
                MGPIO_enuPin_t Local_enuPinNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr+4].pinNum;
                MGPIO_enuSetPinValue(Local_enuPortNum,Local_enuPinNum,(Global_uint8CommandData[Global_uint8LcdNum]>>(itr))&1);
            }
            Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_ENABLE_PULSE;
            break;
        }

        case WRITE_ENABLE_PULSE:{
            MGPIO_enuPort_t Local_enuPortNumE=HCLCD_enuArrLcdConfig[Global_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].portNum;
            MGPIO_enuPin_t Local_enuPinNumE=HCLCD_enuArrLcdConfig[Global_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].pinNum;
            MGPIO_enuSetPinValue(Local_enuPortNumE,Local_enuPinNumE,MGPIO_PIN_HIGH);
            Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_DISABLE_PULSE;
            break;
        }
        case WRITE_DISABLE_PULSE:{
            MGPIO_enuPort_t Local_enuPortNumE=HCLCD_enuArrLcdConfig[Global_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].portNum;
            MGPIO_enuPin_t Local_enuPinNumE=HCLCD_enuArrLcdConfig[Global_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].pinNum;
            MGPIO_enuSetPinValue(Local_enuPortNumE,Local_enuPinNumE,MGPIO_PIN_LOW);
            if(HCLCD_enuArrLcdConfig[Global_uint8LcdNum].ClcdMode == HCLCD_4_BIT_MODE && !Global_uint8First4BitSent){
                Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_SEND_SECOND_4_BIT;
                Global_uint8First4BitSent=1;
            }else{
                Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_FINISH_STATE;
            }
            break;
        }
        case WRITE_FINISH_STATE:{
            Global_enuLcdMode[Global_uint8LcdNum]=MODE_NONE;
            flag_isBusy[Global_uint8LcdNum]=false;
            break;
        }
        default:
            break;
    }
}

static void HCLCD_enuRunnableWriteStringHelper(void){
    switch (Global_enuLcdWriteState[Global_uint8LcdNum])
    {
        case WRITE_SEND_8BIT:{
            for(uint8_t itr=0; itr<HCLCD_enuArrLcdConfig[Global_uint8LcdNum].ClcdMode; itr++){
                MGPIO_enuPort_t Local_enuPortNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr].portNum;
                MGPIO_enuPin_t Local_enuPinNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr].pinNum;
                MGPIO_enuSetPinValue(Local_enuPortNum,Local_enuPinNum,(Global_uint8CommandData[Global_uint8LcdNum]>>itr)&1);
            }
            Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_ENABLE_PULSE;
            break;
        }
        case WRITE_SEND_FIRST_4_BIT:{
            for(uint8_t itr=0; itr<HCLCD_enuArrLcdConfig[Global_uint8LcdNum].ClcdMode; itr++){
                MGPIO_enuPort_t Local_enuPortNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr+4].portNum;
                MGPIO_enuPin_t Local_enuPinNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr+4].pinNum;
                MGPIO_enuSetPinValue(Local_enuPortNum,Local_enuPinNum,(Global_uint8CommandData[Global_uint8LcdNum]>>(itr+4))&1);
            }
            Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_ENABLE_PULSE;
                break;
        }
        case WRITE_SEND_SECOND_4_BIT:{
            for(uint8_t itr=0; itr<HCLCD_enuArrLcdConfig[Global_uint8LcdNum].ClcdMode; itr++){
                MGPIO_enuPort_t Local_enuPortNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr+4].portNum;
                MGPIO_enuPin_t Local_enuPinNum = HCLCD_enuArrLcdConfig[Global_uint8LcdNum].DataPins[itr+4].pinNum;
                MGPIO_enuSetPinValue(Local_enuPortNum,Local_enuPinNum,(Global_uint8CommandData[Global_uint8LcdNum]>>(itr))&1);
            }
            Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_ENABLE_PULSE;
            break;
        }

        case WRITE_ENABLE_PULSE:{
            MGPIO_enuPort_t Local_enuPortNumE=HCLCD_enuArrLcdConfig[Global_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].portNum;
            MGPIO_enuPin_t Local_enuPinNumE=HCLCD_enuArrLcdConfig[Global_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].pinNum;
            MGPIO_enuSetPinValue(Local_enuPortNumE,Local_enuPinNumE,MGPIO_PIN_HIGH);
            Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_DISABLE_PULSE;
            break;
        }
        case WRITE_DISABLE_PULSE:{
            MGPIO_enuPort_t Local_enuPortNumE=HCLCD_enuArrLcdConfig[Global_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].portNum;
            MGPIO_enuPin_t Local_enuPinNumE=HCLCD_enuArrLcdConfig[Global_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].pinNum;
            MGPIO_enuSetPinValue(Local_enuPortNumE,Local_enuPinNumE,MGPIO_PIN_LOW);
            if(HCLCD_enuArrLcdConfig[Global_uint8LcdNum].ClcdMode == HCLCD_4_BIT_MODE && !Global_uint8First4BitSent){
                Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_SEND_SECOND_4_BIT;
                Global_uint8First4BitSent=1;
            }else{
                Global_enuLcdWriteState[Global_uint8LcdNum]=WRITE_FINISH_STATE;
            }
            break;
        }
        case WRITE_FINISH_STATE:{
            Global_enuLcdMode[Global_uint8LcdNum]=MODE_WRITE_STRING;
            break;
        }
        default:
            break;
    }

}

static void HCLCD_enuRunnableWriteString(){
    if(*Globaluint8_string){
        HCLCD_enuAsyncWriteStringHelper(*(Globaluint8_string++),Global_uint8LcdNum);
    }else{
        flag_isBusy[Global_uint8LcdNum]=false;
    }
}

static void HCLCD_enuRunnable(void){
    if(Global_enuLcdMode[Global_uint8LcdNum]==MODE_INIT){
        Global_uint8LcdNum=0;
        HCLCD_enuRunnableInit();
    }   
    else if(Global_enuLcdMode[Global_uint8LcdNum]==MODE_INIT_HELPER){
        HCLCD_enuRunnableInitHelper();
    }else if (Global_enuLcdMode[Global_uint8LcdNum]==MODE_WRITE){
        HCLCD_enuRunnableWrite();
    }else if (Global_enuLcdMode[Global_uint8LcdNum]==MODE_WRITE_STRING){
        HCLCD_enuRunnableWriteString();
    }else if(Global_enuLcdMode[Global_uint8LcdNum]==MODE_WRITE_STRING_HELPER){
        HCLCD_enuRunnableWriteStringHelper();
    }else{

    }
}

static HCLCD_enuErrorStatus_t HCLCD_enuAsyncInitHelper(uint8_t Copyu8Command,uint8_t Copy_uint8LcdNum){
    HCLCD_enuErrorStatus_t Ret_enuErrorStatus=HCLCD_ENU_OK;
    Global_uint8LcdNum=Copy_uint8LcdNum;
    MGPIO_enuPort_t Local_enuPortNumRS = HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_RS].portNum;
    MGPIO_enuPin_t Local_enuPinNumRS = HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_RS].pinNum;
    MGPIO_enuPort_t Local_enuPortNumRW =  HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_RW].portNum;
    MGPIO_enuPin_t Local_enuPinNumRW =  HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_RW].pinNum;
    MGPIO_enuPort_t Local_enuPortNumE =  HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].portNum;
    MGPIO_enuPin_t Local_enuPinNumE =  HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].pinNum;
    
    MGPIO_enuSetPinValue(Local_enuPortNumRS,Local_enuPinNumRS,MGPIO_PIN_LOW);
    MGPIO_enuSetPinValue(Local_enuPortNumRW,Local_enuPinNumRW,MGPIO_PIN_LOW);
    MGPIO_enuSetPinValue(Local_enuPortNumE,Local_enuPinNumE,MGPIO_PIN_LOW);
    
    Global_enuLcdMode[Copy_uint8LcdNum]=MODE_INIT_HELPER;
    Global_uint8CommandData[Copy_uint8LcdNum]=Copyu8Command;

    if(HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].ClcdMode == HCLCD_8_BIT_MODE){
        Global_enuLcdWriteState[Copy_uint8LcdNum]=WRITE_SEND_8BIT;
    }else if(HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].ClcdMode == HCLCD_4_BIT_MODE){
        Global_enuLcdWriteState[Copy_uint8LcdNum]=WRITE_SEND_FIRST_4_BIT;
    }else{
        Ret_enuErrorStatus=HCLCD_ENU_INVALID_CLCD_MODE;
    }
    return Ret_enuErrorStatus;
}

static HCLCD_enuErrorStatus_t HCLCD_enuAsyncWriteStringHelper(uint8_t Copyu8Data, uint8_t Copy_uint8LcdNum){
    HCLCD_enuErrorStatus_t Ret_enuErrorStatus=HCLCD_ENU_OK;
    Global_uint8LcdNum=Copy_uint8LcdNum;
    flag_isBusy[Copy_uint8LcdNum]=true;
    MGPIO_enuPort_t Local_enuPortNumRS = HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_RS].portNum;
    MGPIO_enuPin_t Local_enuPinNumRS = HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_RS].pinNum;
    MGPIO_enuPort_t Local_enuPortNumRW =  HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_RW].portNum;
    MGPIO_enuPin_t Local_enuPinNumRW =  HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_RW].pinNum;
    MGPIO_enuPort_t Local_enuPortNumE =  HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].portNum;
    MGPIO_enuPin_t Local_enuPinNumE =  HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].pinNum;
    MGPIO_enuSetPinValue(Local_enuPortNumRS,Local_enuPinNumRS,MGPIO_PIN_HIGH);
    MGPIO_enuSetPinValue(Local_enuPortNumRW,Local_enuPinNumRW,MGPIO_PIN_LOW);
    MGPIO_enuSetPinValue(Local_enuPortNumE,Local_enuPinNumE,MGPIO_PIN_LOW);
    Global_uint8CommandData[Copy_uint8LcdNum]=Copyu8Data;
    Global_enuLcdMode[Copy_uint8LcdNum]=MODE_WRITE_STRING_HELPER;
    if(HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].ClcdMode == HCLCD_8_BIT_MODE){
        Global_enuLcdWriteState[Copy_uint8LcdNum]=WRITE_SEND_8BIT;
    }else if(HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].ClcdMode == HCLCD_4_BIT_MODE){
        Global_enuLcdWriteState[Copy_uint8LcdNum]=WRITE_SEND_FIRST_4_BIT;
    }else{
        Ret_enuErrorStatus=HCLCD_ENU_INVALID_CLCD_MODE;
    }
    return Ret_enuErrorStatus;
}

HCLCD_enuErrorStatus_t HCLCD_enuAsyncInit(void){
    HCLCD_enuErrorStatus_t Ret_enuErrorStatus=HCLCD_ENU_OK;
    for(uint8_t itr=0; itr < NUMBER_OF_LCDS; itr++){
        for(uint8_t pin=0; pin<HCLCD_enuArrLcdConfig[itr].ClcdMode; pin++){
            if(HCLCD_enuArrLcdConfig[itr].ClcdMode == HCLCD_4_BIT_MODE){
                MGPIO_PinCfg_t Local_structPinConfigration={
                    .Port=HCLCD_enuArrLcdConfig[itr].DataPins[pin + 4].portNum,
                    .Pin=HCLCD_enuArrLcdConfig[itr].DataPins[pin + 4].pinNum,
                    .Mode=MGPIO_ENU_PIN_MODE_OUTPUT,
                    .OutputType=MGPIO_ENU_PIN_OUTPUT_PUSH_PULL,
                    .PullUpDownType=MGPIO_ENU_PIN_PULLDOWN,
                };
                Ret_enuErrorStatus=MGPIO_enuPinCfg(&Local_structPinConfigration);
            }else{
                MGPIO_PinCfg_t Local_structPinConfigration={
                    .Port=HCLCD_enuArrLcdConfig[itr].DataPins[pin].portNum,
                    .Pin=HCLCD_enuArrLcdConfig[itr].DataPins[pin].pinNum,
                    .Mode=MGPIO_ENU_PIN_MODE_OUTPUT,
                    .OutputType=MGPIO_ENU_PIN_OUTPUT_PUSH_PULL,
                    .PullUpDownType=MGPIO_ENU_PIN_PULLDOWN,
                };
                Ret_enuErrorStatus=MGPIO_enuPinCfg(&Local_structPinConfigration);
            }
        }
        for(int pin=0; pin <3; pin++){
            MGPIO_PinCfg_t Local_structPinConfigration={
                .Port=HCLCD_enuArrLcdConfig[itr].PowerPins[pin].portNum,
                .Pin=HCLCD_enuArrLcdConfig[itr].PowerPins[pin].pinNum,
                .Mode=MGPIO_ENU_PIN_MODE_OUTPUT,
                .OutputType=MGPIO_ENU_PIN_OUTPUT_PUSH_PULL,
                .PullUpDownType=MGPIO_ENU_PIN_PULLDOWN,
            };
            Ret_enuErrorStatus=MGPIO_enuPinCfg(&Local_structPinConfigration);
        }
        flag_isBusy[itr]=true;
        Global_enuLcdMode[itr]=MODE_INIT;
        Global_enuLcdInitState[itr]=INIT_STATE;
    }
    HSCHED_enuRegisterRunnable(&Global_structRunnableConfigration);
    HSCHED_enuStart();
    return Ret_enuErrorStatus;
}

HCLCD_enuErrorStatus_t HCLCD_enuAsyncWriteCommand(uint8_t Copyu8Command,uint8_t Copy_uint8LcdNum){
    HCLCD_enuErrorStatus_t Ret_enuErrorStatus=HCLCD_ENU_OK;
    if(flag_isBusy[Copy_uint8LcdNum]){
        Ret_enuErrorStatus=HCLCD_ENU_LCD_IS_BUSY;
    }else{ 
        Global_uint8LcdNum=Copy_uint8LcdNum;
        flag_isBusy[Copy_uint8LcdNum]=true;
        MGPIO_enuPort_t Local_enuPortNumRS = HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_RS].portNum;
        MGPIO_enuPin_t Local_enuPinNumRS = HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_RS].pinNum;
        MGPIO_enuPort_t Local_enuPortNumRW =  HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_RW].portNum;
        MGPIO_enuPin_t Local_enuPinNumRW =  HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_RW].pinNum;
        MGPIO_enuPort_t Local_enuPortNumE =  HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].portNum;
        MGPIO_enuPin_t Local_enuPinNumE =  HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].pinNum;
        
        MGPIO_enuSetPinValue(Local_enuPortNumRS,Local_enuPinNumRS,MGPIO_PIN_LOW);
        MGPIO_enuSetPinValue(Local_enuPortNumRW,Local_enuPinNumRW,MGPIO_PIN_LOW);
        MGPIO_enuSetPinValue(Local_enuPortNumE,Local_enuPinNumE,MGPIO_PIN_LOW);
        
        Global_enuLcdMode[Copy_uint8LcdNum]=MODE_WRITE;
        Global_uint8CommandData[Copy_uint8LcdNum]=Copyu8Command;

        if(HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].ClcdMode == HCLCD_8_BIT_MODE){
            Global_enuLcdWriteState[Copy_uint8LcdNum]=WRITE_SEND_8BIT;
        }else if(HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].ClcdMode == HCLCD_4_BIT_MODE){
            Global_enuLcdWriteState[Copy_uint8LcdNum]=WRITE_SEND_FIRST_4_BIT;
        }else{
            Ret_enuErrorStatus=HCLCD_ENU_INVALID_CLCD_MODE;
        }
    }
    return Ret_enuErrorStatus;
}

HCLCD_enuErrorStatus_t HCLCD_enuAsyncWriteData(uint8_t Copyu8Data,uint8_t Copy_uint8LcdNum){
    HCLCD_enuErrorStatus_t Ret_enuErrorStatus=HCLCD_ENU_OK;
    if(flag_isBusy[Copy_uint8LcdNum]){
        Ret_enuErrorStatus=HCLCD_ENU_LCD_IS_BUSY;
    }else{ 
        Global_uint8LcdNum=Copy_uint8LcdNum;
        flag_isBusy[Copy_uint8LcdNum]=true;
        MGPIO_enuPort_t Local_enuPortNumRS = HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_RS].portNum;
        MGPIO_enuPin_t Local_enuPinNumRS = HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_RS].pinNum;
        MGPIO_enuPort_t Local_enuPortNumRW =  HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_RW].portNum;
        MGPIO_enuPin_t Local_enuPinNumRW =  HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_RW].pinNum;
        MGPIO_enuPort_t Local_enuPortNumE =  HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].portNum;
        MGPIO_enuPin_t Local_enuPinNumE =  HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].PowerPins[HCLCD_ENU_PIN_E].pinNum;
        
        MGPIO_enuSetPinValue(Local_enuPortNumRS,Local_enuPinNumRS,MGPIO_PIN_HIGH);
        MGPIO_enuSetPinValue(Local_enuPortNumRW,Local_enuPinNumRW,MGPIO_PIN_LOW);
        MGPIO_enuSetPinValue(Local_enuPortNumE,Local_enuPinNumE,MGPIO_PIN_LOW);

        Global_uint8CommandData[Copy_uint8LcdNum]=Copyu8Data;
        Global_enuLcdMode[Copy_uint8LcdNum]=MODE_WRITE;

        if(HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].ClcdMode == HCLCD_8_BIT_MODE){
            Global_enuLcdWriteState[Copy_uint8LcdNum]=WRITE_SEND_8BIT;
        }else if(HCLCD_enuArrLcdConfig[Copy_uint8LcdNum].ClcdMode == HCLCD_4_BIT_MODE){
            Global_enuLcdWriteState[Copy_uint8LcdNum]=WRITE_SEND_FIRST_4_BIT;
        }else{
            Ret_enuErrorStatus=HCLCD_ENU_INVALID_CLCD_MODE;
        }
    }
    return Ret_enuErrorStatus;
}

HCLCD_enuErrorStatus_t HCLCD_enuAsyncWriteString(sint8_t *Addu8_string ,uint8_t Copy_uint8LcdNum){
    HCLCD_enuErrorStatus_t Ret_enuErrorStatus=HCLCD_ENU_OK;
    if(flag_isBusy[Copy_uint8LcdNum]){
        Ret_enuErrorStatus=HCLCD_ENU_LCD_IS_BUSY;
    }else{ 
        Globaluint8_string=Addu8_string;
        Global_uint8LcdNum=Copy_uint8LcdNum;
        Global_enuLcdMode[Copy_uint8LcdNum]=MODE_WRITE_STRING;
    }   
    return Ret_enuErrorStatus;
}

/*****************************************************************************************/

static float64_t HCLCD_enuWriteNumberHelperPowerFunction(float64_t base, float64_t power) {
    if (power == 0) return 1.0;
    float64_t result = HCLCD_enuWriteNumberHelperPowerFunction(base, power / 2);
    result *= result;
    if ((uint64_t)power % 2 == 1) {
        result *= base;
    }
    return result;
}

static HCLCD_enuErrorStatus_t HCLCD_enuWriteNumberHelper(uint64_t Copy_uint64Number, uint8_t Copy_uint8LcdNum) {
    HCLCD_enuErrorStatus_t Ret_enuErrorStatus = HCLCD_ENU_OK;
    if (!Copy_uint64Number) return Ret_enuErrorStatus;
    HCLCD_enuWriteNumberHelper(Copy_uint64Number / 10, Copy_uint8LcdNum);
    HCLCD_enuAsyncWriteData((Copy_uint64Number % 10) + '0', Copy_uint8LcdNum);
    return Ret_enuErrorStatus;
}

HCLCD_enuErrorStatus_t HCLCD_enuWriteNumber(float64_t Copyfloat64_number, uint8_t Copy_uint8LcdNum) {
    HCLCD_enuErrorStatus_t Ret_enuErrorStatus = HCLCD_ENU_OK;

    if (Copyfloat64_number < 0) {
        HCLCD_enuAsyncWriteData('-', Copy_uint8LcdNum);
        Copyfloat64_number = -Copyfloat64_number;
    }

    uint64_t Local_uint64IntgerValue = (uint64_t)Copyfloat64_number;
    float64_t Local_float64Fraction = Copyfloat64_number - Local_uint64IntgerValue;

    if (Local_uint64IntgerValue == 0) {
        HCLCD_enuAsyncWriteData('0', Copy_uint8LcdNum);
    } else {
        HCLCD_enuWriteNumberHelper(Local_uint64IntgerValue, Copy_uint8LcdNum);
    }
    uint64_t Local_uint64FractionValue = (uint64_t)((Local_float64Fraction * HCLCD_enuWriteNumberHelperPowerFunction(10, HCLCD_NUMBER_FRACTION_RESOLUTION)));
    if(Local_uint64FractionValue!=0){
        HCLCD_enuAsyncWriteData('.', Copy_uint8LcdNum);
        HCLCD_enuWriteNumberHelper(Local_uint64FractionValue, Copy_uint8LcdNum);
    }

    return Ret_enuErrorStatus;
}

HCLCD_enuErrorStatus_t HCLCD_enuGotoRowColumn(HCLCD_enuRow_t Copyenu_Row, uint8_t Copyenu_Column, uint8_t Copy_uint8LcdNum){
    HCLCD_enuErrorStatus_t Ret_enuErrorStatus = HCLCD_ENU_OK;
    if(Copyenu_Column>39){
        Ret_enuErrorStatus=HCLCD_ENU_INVALID_COLUMN;
    }else if(Copy_uint8LcdNum>NUMBER_OF_LCDS){
        Ret_enuErrorStatus=HCLCD_ENU_INVALID_CLCD_NUM;
    }else if(Copyenu_Row<0 || Copyenu_Row>1){
        Ret_enuErrorStatus=HCLCD_ENU_INVALID_ROW;
    }else{
        uint8_t Localu8_AddressDDRAM=(Copyenu_Row==HCLCD_FIRST_ROW)?Copyenu_Column:0x40+Copyenu_Column;
        HCLCD_enuAsyncWriteCommand(DDRAM_BASE_COMMAND | Localu8_AddressDDRAM,Copy_uint8LcdNum);
    }

    return Ret_enuErrorStatus;
}        

static HCLCD_enuErrorStatus_t HCLCD_enuGotoCGRAM(uint8_t Copyu8_PatternNumber, uint8_t Copy_uint8LcdNum){
    HCLCD_enuErrorStatus_t Ret_enuErrorStatus = HCLCD_ENU_OK;
      if(Copyu8_PatternNumber>8){
        Ret_enuErrorStatus=HCLCD_ENU_INVALID_PATTERN_NUM;
    }else if(Copy_uint8LcdNum>NUMBER_OF_LCDS){
        Ret_enuErrorStatus=HCLCD_ENU_INVALID_CLCD_NUM;
    }else{
        uint8_t Localu8_AddressCGRAM=Copyu8_PatternNumber*8;
        HCLCD_enuAsyncWriteCommand(CGRAM_BASE_COMMAND | Localu8_AddressCGRAM,Copy_uint8LcdNum);
    }
    return Ret_enuErrorStatus;
}

static HCLCD_enuErrorStatus_t HCLCD_enuWriteSpecialChar(uint8_t * Addu8_pattern, uint8_t Copy_uint8LcdNum ){
    HCLCD_enuErrorStatus_t Ret_enuErrorStatus = HCLCD_ENU_OK;
    if(Addu8_pattern == NULL){
        Ret_enuErrorStatus  =HCLCD_ENU_NULL_PTR;
    }else if(Copy_uint8LcdNum>NUMBER_OF_LCDS){
        Ret_enuErrorStatus= HCLCD_ENU_INVALID_CLCD_NUM;
    }else{
        for(uint8_t itr =0; itr<8; itr++){
            HCLCD_enuAsyncWriteData(Addu8_pattern[itr],Copy_uint8LcdNum);
        }
    }
    return Ret_enuErrorStatus;
}

HCLCD_enuErrorStatus_t HCLCD_enuStoreSpecialChar(uint8_t *Addu8_pattern, uint8_t Copyu8_PatternNumber, uint8_t Copy_uint8LcdNum){
    HCLCD_enuErrorStatus_t Ret_enuErrorStatus = HCLCD_ENU_OK;
    if(Addu8_pattern == NULL){
        Ret_enuErrorStatus  =HCLCD_ENU_NULL_PTR;
    }else if( Copyu8_PatternNumber>8){
        Ret_enuErrorStatus= HCLCD_ENU_INVALID_PATTERN_NUM;
    }else if(Copy_uint8LcdNum>NUMBER_OF_LCDS){
        Ret_enuErrorStatus= HCLCD_ENU_INVALID_CLCD_NUM;
    }else{
        Ret_enuErrorStatus = HCLCD_enuGotoCGRAM(Copyu8_PatternNumber,Copy_uint8LcdNum);
        Ret_enuErrorStatus = HCLCD_enuWriteSpecialChar(Addu8_pattern,Copy_uint8LcdNum);
        HCLCD_enuGotoRowColumn(HCLCD_FIRST_ROW,0,Copy_uint8LcdNum);
    }
    return Ret_enuErrorStatus;
}

HCLCD_enuErrorStatus_t HCLCD_enuDisplaySpecialChar(uint8_t Copyu8_PatternNumber, HCLCD_enuRow_t Copyenu_Row, uint8_t Copyenu_Column, uint8_t Copy_uint8LcdNum){
    HCLCD_enuErrorStatus_t Ret_enuErrorStatus = HCLCD_ENU_OK;
    if( Copyu8_PatternNumber>8){
        Ret_enuErrorStatus= HCLCD_ENU_INVALID_PATTERN_NUM;
    }else if(Copy_uint8LcdNum>NUMBER_OF_LCDS){
        Ret_enuErrorStatus= HCLCD_ENU_INVALID_CLCD_NUM;
    }else{
        Ret_enuErrorStatus = HCLCD_enuGotoRowColumn(Copyenu_Row,Copyenu_Column,Copy_uint8LcdNum);
        Ret_enuErrorStatus = HCLCD_enuAsyncWriteData(Copyu8_PatternNumber, Copy_uint8LcdNum);
    }
    return Ret_enuErrorStatus;
}