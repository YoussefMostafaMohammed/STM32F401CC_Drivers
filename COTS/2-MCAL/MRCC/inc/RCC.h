#ifndef RCC_H_
#define RCC_H_

#define MRCC_CTRL_HSI  (0x01ULL)
#define MRCC_CTRL_HSE  (0x00010000ULL)
#define MRCC_CTRL_PLL  (0x01000000ULL)

#define MRCC_PLL_EN_MSK ((uint32_t)0x3000000)

#define RCC_AHP1_BUS (0x0UL)
#define RCC_AHP2_BUS (0x1UL)
#define RCC_APB1_BUS (0x2UL)
#define RCC_APB2_BUS (0x3UL)

/* AHB1 */
#define RCC_GPIOA  (0x000000001ULL)
#define RCC_GPIOB  (0x000000002ULL)
#define RCC_GPIOC  (0x000000004ULL)
#define RCC_GPIOD  (0x000000008ULL)
#define RCC_GPIOE  (0x000000010ULL)
#define RCC_GPIOH  (0x000000080ULL)
#define RCC_CRC    (0x000001000ULL)
#define RCC_DMA1   (0x000200000ULL)
#define RCC_DMA2   (0x000400000ULL)

/* AHB2 */
#define RCC_OTGFS  (0x100000080ULL)

/* ABP1 */
#define RCC_TIM2   (0x2000000001ULL)
#define RCC_TIM3   (0x2000000002ULL)
#define RCC_TIM4   (0x2000000004ULL)
#define RCC_TIM5   (0x2000000008ULL)
#define RCC_WWDG   (0x2000000800ULL)
#define RCC_SPI2   (0x2000004000ULL)
#define RCC_SPI3   (0x2000008000ULL)
#define RCC_USART2 (0x2000020000ULL)
#define RCC_I2C1   (0x2000200000ULL)
#define RCC_I2C2   (0x2000400000ULL)
#define RCC_I2C3   (0x2000800000ULL)
#define RCC_PWR    (0x2010000000ULL)

/* ABP2 */
#define RCC_TIM1   (0x3000000001ULL)
#define RCC_USART1 (0x3000000010ULL)
#define RCC_USART6 (0x3000000020ULL)
#define RCC_ADC1   (0x3000000100ULL)
#define RCC_SDIO   (0x3000000800ULL)
#define RCC_SPI1   (0x3000001000ULL)
#define RCC_SPI4   (0x3000002000ULL)
#define RCC_SYSCFG (0x3000004000ULL)
#define RCC_TIM9   (0x3000010000ULL)
#define RCC_TIM10  (0x3000020000ULL)
#define RCC_TIM11  (0x3000040000ULL)

typedef enum{
    RCC_enu_PLL_P_PRESCALER_2=0,
    RCC_enu_PLL_P_PRESCALER_4,
    RCC_enu_PLL_P_PRESCALER_6,
    RCC_enu_PLL_P_PRESCALER_8
}RCC_enuPLL_P_CFG_t;

typedef enum
{
    MRCC_enu_OK=0,
    MRCC_enu_NOK,
    MRCC_enu_INVALID_CLOCK_CONFIGRATION,
    MRCC_enu_CLOCK_NOT_READY,
    MRCC_enu_NULLPTR,
    MRCC_enu_INVALID_PERIPHERAL,
    MRCC_enu_INVALID_BUS,
    MRCC_enu_PLL_IS_ENABLED,
    MRCC_enu_PLL_INVALID_CLOCK_SOURCE,
    MRCC_enu_PLL_INVALID_M,
    MRCC_enu_PLL_INVALID_N,
    MRCC_enu_PLL_INVALID_P,
    MRCC_enu_PLL_INVALID_Q,
    MRCC_enu_PLL_INVALID_VCO,
    MRCC_enu_PLL_INVALID_PLL_CLOCK,
    MRCC_enu_PLL_INVALID_USB_CLOCK,
    MRCC_enu_PLLON
}MRCC_enuErrorStatus_t;

typedef enum
{
    MRCC_enu_HSI=0,
    MRCC_enu_HSE,
    MRCC_enu_PLL
}MRCC_enuClockCfg_t;

typedef enum{
    MRCC_enu_PLLCFG_HSI=0,
    MRCC_enu_PLLCFG_HSE
}RCC_PLL_SRC_t;


typedef union {
    struct {
        uint32_t M_Prescaler             : 6;          // 6 bits for M
        uint32_t N_Multiplier            : 9;         // 9 bits for N
        uint32_t                         : 1;        // 1 reserved bit
        RCC_enuPLL_P_CFG_t P_Prescaler   : 2;       // 2 bits for P
        uint32_t                         : 4;      // 4 reserved bits
        RCC_PLL_SRC_t CLK_SRC            : 1;     // 1 bit for SRC
        uint32_t                         : 1;    // 1 reserved bit
        uint32_t Q_Prescaler             : 4;   // 4 bits for Q
        uint32_t                         : 4;  // 4 reserved bits
    } Bits;                                   // Access individual fields
    uint32_t All_Reg;                        // Access the entire register
} MRCC_unionPLL_CFG_t;


uint8_t Binary_Search(uint64_t Copy_uint64Target);
MRCC_enuErrorStatus_t MRCC_enuEnableClock(MRCC_enuClockCfg_t Copy_enuClockCfg);
MRCC_enuErrorStatus_t MRCC_enuDisableClock(MRCC_enuClockCfg_t Copy_enuClockCfg);
MRCC_enuErrorStatus_t MRCC_enuGetClkStatus(MRCC_enuClockCfg_t Copy_enuClockCfg,uint32_t* Add_ret);
MRCC_enuErrorStatus_t MRCC_enuGetSystemClock(uint64_t* Addr_uint64Clock);
MRCC_enuErrorStatus_t MRCC_enuSelectSysCLK(MRCC_enuClockCfg_t Copy_enuClockCfg);

MRCC_enuErrorStatus_t MRCC_enuEnablePeripheral(uint64_t Copy_uint64Peripheral);
MRCC_enuErrorStatus_t MRCC_enuDisablePeripheral(uint64_t Copy_uint64Peripheral);

MRCC_enuErrorStatus_t MRCC_enuCfgPLL(MRCC_unionPLL_CFG_t* Add_enuPllCfg);

// MRCC_enuErrorStatus_t MRCC_enuControlCLK();
#endif /* MRCC_H_ */
