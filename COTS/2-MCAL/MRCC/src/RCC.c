#include "../../../0-STD_LIBS/inc/BIT_MATH.h"
#include "../../../0-STD_LIBS/inc/STD_TYPES.h"
#include "../inc/RCC.h"
#include "../inc/RCC_CFG.h"

#define MRCC_HSI_ON_MASK  (0x00000001ULL)
#define MRCC_HSE_ON_MASK  (0x00010000ULL)
#define MRCC_PLL_ON_MASK  (0x01000000ULL)

#define MRCC_HSI_RDY_MASK (0x00000002ULL)
#define MRCC_HSE_RDY_MASK (0x00020000ULL)
#define MRCC_PLL_RDY_MASK (0x02000000ULL)

#define MRCC_PLL_ON_CHECK_MASK (0x03000000UL)
#define MRCC_SWITCH_CLOCK_MASK (0xFFFFFFFCUL)

#define MRCC_PLLCFG_CLEAR_MASK (0xF0BC8000UL)
#define MRCC_HSI_RDY_BIT 1
#define MRCC_HSE_RDY_BIT 17
#define MRCC_PLL_RDY_BIT 25

#define MRCC_HSI_ON_BIT 0
#define MRCC_HSE_ON_BIT 16
#define MRCC_PLL_ON_BIT 24

#define NUMBER_OF_PERIPHERAL 33

static uint64_t Global_uint64CurrentPLLCLOCK=16000000;
uint64_t Global_uint64CurrentSystemClOCK=16000000;

typedef struct
{
    volatile uint32_t RCC_CR;
    volatile uint32_t RCC_PLLCFGR;
    volatile uint32_t RCC_CFGR;
    volatile uint32_t RCC_CIR;
    volatile uint32_t RCC_AHB1RSTR;
    volatile uint32_t RCC_AHB2RSTR;
    uint32_t Reserved0[2];
    volatile uint32_t RCC_APB1RSTR;
    volatile uint32_t RCC_APB2RSTR;
    uint32_t Reserved1[2];
    volatile uint32_t RCC_AHB1ENR;
    volatile uint32_t RCC_AHB2ENR;
    uint32_t Reserved2[2];
    volatile uint32_t RCC_APB1ENR;
    volatile uint32_t RCC_APB2ENR;
    uint32_t Reserved3[2];
    volatile uint32_t RCC_AHB1LPENR;
    volatile uint32_t RCC_AHB2LPENR;
    uint32_t Reserved4[2];
    volatile uint32_t RCC_APB1LPENR;
    volatile uint32_t RCC_APB2LPENR;
    uint32_t Reserved5[2];
    volatile uint32_t RCC_BDCR;
    volatile uint32_t RCC_CSR;
    uint32_t Reserved6[2];
    volatile uint32_t RCC_SSCGR;
    volatile uint32_t RCC_PLLI2SCFGR;
    uint32_t Reserved7;
    volatile uint32_t RCC_DCKCFGR;
} MRCC_structREGS_t;


uint64_t RCC_PeripheralArr[NUMBER_OF_PERIPHERAL]={
    /************ AHB1 **************/
    RCC_GPIOA,
    RCC_GPIOB,
    RCC_GPIOC,
    RCC_GPIOD,
    RCC_GPIOE,
    RCC_GPIOH,
    RCC_CRC,
    RCC_DMA1,
    RCC_DMA2,
    /************  AHB2 **************/   
    RCC_OTGFS,
    /************  ABP1 **************/  
    RCC_TIM2,
    RCC_TIM3,
    RCC_TIM4,
    RCC_TIM5,
    RCC_WWDG,
    RCC_SPI2,
    RCC_SPI3,
    RCC_USART2,
    RCC_I2C1,
    RCC_I2C2,
    RCC_I2C3,
    RCC_PWR,
    /************ ABP2 **************/
    RCC_TIM1,
    RCC_USART1,
    RCC_USART6,
    RCC_ADC1,
    RCC_SDIO,
    RCC_SPI1,
    RCC_SPI4,
    RCC_SYSCFG,
    RCC_TIM9,
    RCC_TIM10,
    RCC_TIM11
};


#define MRCC_BASE_ADD (0x40023800UL)
#define MRCC_REG (( volatile MRCC_structREGS_t*)MRCC_BASE_ADD) // or create local var
volatile MRCC_structREGS_t* RCC_REG = MRCC_REG;




MRCC_enuErrorStatus_t MRCC_enuEnableClock(MRCC_enuClockCfg_t Copy_enuClockCfg ){
    MRCC_enuErrorStatus_t Local_enuErrorStatus=MRCC_enu_OK;
    uint32_t Local_uint32TimeOut = 1000;
    uint32_t Local_uint32ClockStatus=TRUE;
    switch (Copy_enuClockCfg)
    {
        case MRCC_enu_HSI:
            RCC_REG->RCC_CR |= MRCC_HSI_ON_MASK;
            Local_uint32ClockStatus = (RCC_REG->RCC_CR & MRCC_HSI_RDY_MASK);
            while((Local_uint32TimeOut--) && (!Local_uint32ClockStatus)){
                Local_uint32ClockStatus = (RCC_REG->RCC_CR & MRCC_HSI_RDY_MASK);
            }
            break;
        case MRCC_enu_HSE:
            RCC_REG->RCC_CR |= MRCC_HSE_ON_MASK;
            Local_uint32ClockStatus = (RCC_REG->RCC_CR & MRCC_HSE_RDY_MASK);
            while((Local_uint32TimeOut--) && (!Local_uint32ClockStatus)){
                Local_uint32ClockStatus = (RCC_REG->RCC_CR & MRCC_HSE_RDY_MASK);
            }
            break;
        case MRCC_enu_PLL:
            RCC_REG->RCC_CR |= MRCC_PLL_ON_MASK;
            Local_uint32ClockStatus = (RCC_REG->RCC_CR & MRCC_PLL_RDY_MASK);
            while((Local_uint32TimeOut--) && (!Local_uint32ClockStatus)){
                Local_uint32ClockStatus = (RCC_REG->RCC_CR & MRCC_PLL_RDY_MASK);
            }
            break;
        default:
            Local_enuErrorStatus = MRCC_enu_INVALID_CLOCK_CONFIGRATION;
            break;
    }
    if(Local_uint32ClockStatus == 0)
        Local_enuErrorStatus = MRCC_enu_CLOCK_NOT_READY;

    return Local_enuErrorStatus;
}

MRCC_enuErrorStatus_t MRCC_enuDisableClock(MRCC_enuClockCfg_t Copy_enuClockCfg){
    MRCC_enuErrorStatus_t Local_enuErrorStatus=MRCC_enu_OK;
    switch (Copy_enuClockCfg)
    {
        case MRCC_enu_HSI:
            RCC_REG->RCC_CR &= ~(MRCC_HSI_ON_MASK);
            break;
        case MRCC_enu_HSE:
            RCC_REG->RCC_CR &= ~(MRCC_HSE_ON_MASK);
            break;
        case MRCC_enu_PLL:
            RCC_REG->RCC_CR &= ~(MRCC_PLL_ON_MASK);
            break;
        default:
            Local_enuErrorStatus = MRCC_enu_INVALID_CLOCK_CONFIGRATION;
            break;
    }
    return Local_enuErrorStatus;
}

MRCC_enuErrorStatus_t MRCC_enuGetClkStatus(MRCC_enuClockCfg_t Copy_enuClockCfg, uint32_t* Add_ret){
    MRCC_enuErrorStatus_t Local_enuErrorStatus = MRCC_enu_OK;
    if(Add_ret == NULL){
        Local_enuErrorStatus = MRCC_enu_NULLPTR;
    }
    else
    {
        switch (Copy_enuClockCfg)
        {
            case MRCC_enu_HSI:
                *Add_ret = ((RCC_REG->RCC_CR & (MRCC_HSI_RDY_MASK)) >> MRCC_HSI_RDY_BIT);
                break;
            case MRCC_enu_HSE:
                *Add_ret = ((RCC_REG->RCC_CR & (MRCC_HSE_RDY_MASK)) >> MRCC_HSE_RDY_BIT);
                break;
            case MRCC_enu_PLL:
                *Add_ret = ((RCC_REG->RCC_CR & (MRCC_PLL_RDY_MASK)) >> MRCC_PLL_RDY_BIT);
                break;
            default:
                Local_enuErrorStatus = MRCC_enu_INVALID_CLOCK_CONFIGRATION;
                break;
        }
    }
    return Local_enuErrorStatus;    
}

uint8_t Binary_Search(uint64_t Copy_uint64Target){
    uint16_t low = 0;
    uint16_t high = NUMBER_OF_PERIPHERAL-1;
    uint16_t mid = (low+high)/2;
    while(low<=high){
        mid = (low+high)/2;
        if(RCC_PeripheralArr[mid]==Copy_uint64Target){
            return TRUE;
        }else if(RCC_PeripheralArr[mid]>Copy_uint64Target){
            high=mid-1;
        }else{
            low=mid+1;
        }
    }
    return FALSE;
}

MRCC_enuErrorStatus_t MRCC_enuEnablePeripheral(uint64_t Copy_uint64Peripheral){
    MRCC_enuErrorStatus_t Local_enuErrorStatus = MRCC_enu_OK;
    uint32_t Local_uint32Bus = (uint32_t)(Copy_uint64Peripheral >> 32) ;
    uint32_t Local_uint32PeripheralMask = (uint32_t)Copy_uint64Peripheral ;
    if(!Binary_Search(Copy_uint64Peripheral)){
            Local_enuErrorStatus=MRCC_enu_INVALID_PERIPHERAL;
    }else{
        switch (Local_uint32Bus)
        {
        case RCC_AHP1_BUS:    
            RCC_REG->RCC_AHB1ENR |= Local_uint32PeripheralMask;
            break;
        case RCC_AHP2_BUS:
            RCC_REG->RCC_AHB2ENR |= Local_uint32PeripheralMask;
            break;
        case RCC_APB1_BUS:        
            RCC_REG->RCC_APB1ENR |= Local_uint32PeripheralMask;
            break;
        case RCC_APB2_BUS:
            RCC_REG->RCC_APB2ENR |= Local_uint32PeripheralMask;
            break;
        default:
            Local_enuErrorStatus=MRCC_enu_INVALID_BUS;
            break;
        }
    }
    return Local_enuErrorStatus;
}

MRCC_enuErrorStatus_t MRCC_enuDisablePeripheral(uint64_t Copy_uint64Peripheral){
    MRCC_enuErrorStatus_t Local_enuErrorStatus = MRCC_enu_OK;
    uint32_t Local_uint32Bus = (uint32_t)(Copy_uint64Peripheral >> 32) ;
    uint32_t Local_uint32PeripheralMask = (uint32_t)Copy_uint64Peripheral ;
    if(!Binary_Search(Copy_uint64Peripheral)){
            Local_enuErrorStatus=MRCC_enu_INVALID_PERIPHERAL;
    }else{
        switch (Local_uint32Bus)
        {
        case RCC_AHP1_BUS:    
            RCC_REG->RCC_AHB1ENR &= ~(Local_uint32PeripheralMask);
            break;

        case RCC_AHP2_BUS:
            RCC_REG->RCC_AHB2ENR &= ~(Local_uint32PeripheralMask);
            break;

        case RCC_APB1_BUS:        
            RCC_REG->RCC_APB1ENR &= ~(Local_uint32PeripheralMask);
            break;

        case RCC_APB2_BUS:
            RCC_REG->RCC_APB2ENR &= ~(Local_uint32PeripheralMask);
            break;
        
        default:
            Local_enuErrorStatus=MRCC_enu_INVALID_BUS;
            break;
        }
    }
    return Local_enuErrorStatus;
}

MRCC_enuErrorStatus_t MRCC_enuGetSystemClock(uint64_t* Addr_uint64Clock){
    MRCC_enuErrorStatus_t Ret_enuErrorStatus= MRCC_enu_OK;
    if(Addr_uint64Clock==NULL){
        Ret_enuErrorStatus=MRCC_enu_NULLPTR;
    }else{
        *Addr_uint64Clock = Global_uint64CurrentSystemClOCK;
    }return Ret_enuErrorStatus;
}

MRCC_enuErrorStatus_t MRCC_enuSelectSysCLK(MRCC_enuClockCfg_t Copy_enuClockCfg){
    MRCC_enuErrorStatus_t Local_enuErrorStatus = MRCC_enu_OK;
    if(Copy_enuClockCfg> MRCC_enu_PLL || Copy_enuClockCfg < MRCC_enu_HSI){
        Local_enuErrorStatus = MRCC_enu_INVALID_CLOCK_CONFIGRATION;
    }else{
        uint32_t Local_uint32ClockStatus;
        Local_enuErrorStatus = MRCC_enuGetClkStatus(Copy_enuClockCfg,&Local_uint32ClockStatus);
        if(Local_enuErrorStatus == MRCC_enu_OK ){
            if(!Local_uint32ClockStatus){
                Local_enuErrorStatus = MRCC_enu_CLOCK_NOT_READY;
            }
            else{
                 uint32_t Local_uint32TempReg = ((RCC_REG->RCC_CFGR&MRCC_SWITCH_CLOCK_MASK) | ((uint32_t) Copy_enuClockCfg));
                 RCC_REG->RCC_CFGR |= Local_uint32TempReg;
                 if(Copy_enuClockCfg==MRCC_enu_HSI){
                    Global_uint64CurrentSystemClOCK=MRCC_HSI_CLK_HZ;
                 }else if (Copy_enuClockCfg==MRCC_enu_HSE){
                    Global_uint64CurrentSystemClOCK=MRCC_HSE_CLK_HZ;
                 }else{
                    Global_uint64CurrentSystemClOCK=Global_uint64CurrentPLLCLOCK;
                 }
            }
        }
    }
    return Local_enuErrorStatus;
}



/*
    •f(VCO clock) = f(PLL clock input) × (PLLN / PLLM)
    •f(PLL general clock output) = f(VCO clock) / PLLP
    •f(USB OTG FS, SDIO, RNG clock output) = f(VCO clock) / PLLQ
*/
MRCC_enuErrorStatus_t MRCC_enuCfgPLL(MRCC_unionPLL_CFG_t* Add_unionPllCfg){
    MRCC_enuErrorStatus_t Local_enuErrorStatus = MRCC_enu_OK;
    uint32_t Local_uint32VCO=0;
    uint32_t Local_uint32PLLClock=0;
    uint32_t Local_uint32USBClock=0;

    if(Add_unionPllCfg==NULL){
        Local_enuErrorStatus=MRCC_enu_NULLPTR;
    }else if(((RCC_REG->RCC_CR & MRCC_PLL_ON_CHECK_MASK))){
        Local_enuErrorStatus = MRCC_enu_PLL_IS_ENABLED;
    }else if(Add_unionPllCfg->Bits.CLK_SRC < MRCC_enu_PLLCFG_HSI || Add_unionPllCfg->Bits.CLK_SRC > MRCC_enu_PLLCFG_HSE){
        Local_enuErrorStatus = MRCC_enu_PLL_INVALID_CLOCK_SOURCE;
    }else if(Add_unionPllCfg->Bits.M_Prescaler < 2 || Add_unionPllCfg->Bits.M_Prescaler > 63){
        Local_enuErrorStatus = MRCC_enu_PLL_INVALID_M;
    }else if(Add_unionPllCfg->Bits.N_Multiplier < 192 || Add_unionPllCfg->Bits.N_Multiplier > 432){
        Local_enuErrorStatus = MRCC_enu_PLL_INVALID_N;
    }else if(Add_unionPllCfg->Bits.P_Prescaler < RCC_enu_PLL_P_PRESCALER_2 || Add_unionPllCfg->Bits.P_Prescaler > RCC_enu_PLL_P_PRESCALER_8){
        Local_enuErrorStatus = MRCC_enu_PLL_INVALID_P;
    }else if(Add_unionPllCfg->Bits.Q_Prescaler < 2 || Add_unionPllCfg->Bits.Q_Prescaler > 15){
        Local_enuErrorStatus = MRCC_enu_PLL_INVALID_Q;
    }else{
        if(Add_unionPllCfg->Bits.CLK_SRC == MRCC_enu_PLLCFG_HSI){
            Local_uint32VCO =(MRCC_HSI_CLK_HZ/(Add_unionPllCfg->Bits.M_Prescaler));
        }else{
            Local_uint32VCO =(MRCC_HSE_CLK_HZ/(Add_unionPllCfg->Bits.M_Prescaler));
        }
        if(Local_uint32VCO<1000000 || Local_uint32VCO>2000000){
            Local_enuErrorStatus=MRCC_enu_PLL_INVALID_VCO;
        }
        else{
            Local_uint32VCO*=(Add_unionPllCfg->Bits.N_Multiplier);
            if((Local_uint32VCO < 192000000) || (Local_uint32VCO > 432000000)){
                Local_enuErrorStatus=MRCC_enu_PLL_INVALID_VCO;
            }else{
                Local_uint32PLLClock=Local_uint32VCO/((Add_unionPllCfg->Bits.P_Prescaler+1)*2);
                Local_uint32USBClock=Local_uint32VCO/(Add_unionPllCfg->Bits.Q_Prescaler);
                if(Local_uint32PLLClock > 84000000){
                    Local_enuErrorStatus = MRCC_enu_PLL_INVALID_PLL_CLOCK;
                }else if(Local_uint32USBClock > 48000000){
                    Local_enuErrorStatus = MRCC_enu_PLL_INVALID_USB_CLOCK;
                }
            }
        }
        if(Local_enuErrorStatus==MRCC_enu_OK){
            Global_uint64CurrentPLLCLOCK=Local_uint32PLLClock;
            uint32_t Local_uint32TempRCC_PLLCFG = RCC_REG->RCC_PLLCFGR ;
            Local_uint32TempRCC_PLLCFG &= MRCC_PLLCFG_CLEAR_MASK;
            Local_uint32TempRCC_PLLCFG |= Add_unionPllCfg->All_Reg;
            RCC_REG->RCC_PLLCFGR = Local_uint32TempRCC_PLLCFG;
        }
    }
    return Local_enuErrorStatus;
}

//MRCC_enuErrorStatus_t MRCC_enuControlCLK(){}
    
