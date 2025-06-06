#ifndef HCLCD_H_
#define HCLCD_H_

#include "MGPIO.h"
#include "STD_TYPES.h"

/* Basic LCD Commands (8-bit) */
#define CLEAR_DISPLAY              0x01    // Clear display and return home
#define RETURN_HOME                0x02    // Return cursor to home position
#define ENTRY_MODE_SET             0x04    // Configure cursor move direction
#define DISPLAY_CONTROL            0x08    // Control display/cursor/blink
#define CURSOR_DISPLAY_SHIFT       0x10    // Shift display/cursor position
#define FUNCTION_SET               0x20    // Interface/line/font configuration
#define SET_CGRAM_ADDR             0x40    // Set CGRAM address
#define SET_DDRAM_ADDR             0x80    // Set DDRAM address

/* Entry Mode Set Options (use with ENTRY_MODE_SET) */
#define ENTRY_INCREMENT            0x02    // Cursor moves right (I/D=1)
#define ENTRY_DECREMENT            0x00    // Cursor moves left (I/D=0)
#define ENTRY_SHIFT_ENABLE         0x01    // Display shifts on write (S=1)
#define ENTRY_SHIFT_DISABLE        0x00    // Display doesn't shift (S=0)

/* Display Control Options (use with DISPLAY_CONTROL) */
#define DISPLAY_ON                 0x04    // Turn display on (D=1)
#define DISPLAY_OFF                0x00    // Turn display off (D=0)
#define CURSOR_ON                  0x02    // Turn cursor on (C=1)
#define CURSOR_OFF                 0x00    // Turn cursor off (C=0)
#define BLINK_ON                   0x01    // Enable cursor blink (B=1)
#define BLINK_OFF                  0x00    // Disable cursor blink (B=0)

/* Cursor/Display Shift Options (use with CURSOR_DISPLAY_SHIFT) */
#define SHIFT_DISPLAY              0x08    // Shift entire display (S/C=1)
#define SHIFT_CURSOR               0x00    // Shift cursor only (S/C=0)
#define SHIFT_RIGHT                0x04    // Shift right (R/L=1)
#define SHIFT_LEFT                 0x00    // Shift left (R/L=0)

/* Function Set Options (use with FUNCTION_SET) */
#define DATA_LENGTH_8BIT           0x10    // 8-bit interface (DL=1)
#define DATA_LENGTH_4BIT           0x00    // 4-bit interface (DL=0)
#define TWO_LINES                  0x08    // 2-line display (N=1)
#define ONE_LINE                   0x00    // 1-line display (N=0)
#define FONT_5X10                  0x04    // 5x10 dot font (F=1)
#define FONT_5X8                   0x00    // 5x8 dot font (F=0)

/* Status Masks */
#define BUSY_FLAG_MASK             0x80    // Mask for busy flag (BF)
#define ADDRESS_COUNTER_MASK       0x7F    // Mask for address counter value

/* Common Command Combinations */
// Entry modes
#define ENABLE_AUTO_SHIFT_RIGHT    (ENTRY_MODE_SET | ENTRY_DECREMENT | ENTRY_SHIFT_ENABLE)  // 0x05
#define ENABLE_AUTO_SHIFT_LEFT     (ENTRY_MODE_SET | ENTRY_INCREMENT | ENTRY_SHIFT_ENABLE)  // 0x07
#define DISABLE_AUTO_SHIFT         (ENTRY_MODE_SET | ENTRY_INCREMENT | ENTRY_SHIFT_DISABLE) // 0x06

// Display control
#define DISPLAY_ON_CURSOR_OFF      (DISPLAY_CONTROL | DISPLAY_ON | CURSOR_OFF | BLINK_OFF)  // 0x0C
#define DISPLAY_ON_CURSOR_ON       (DISPLAY_CONTROL | DISPLAY_ON | CURSOR_ON  | BLINK_OFF)   // 0x0E
#define DISPLAY_ON_CURSOR_BLINK    (DISPLAY_CONTROL | DISPLAY_ON | CURSOR_ON  | BLINK_ON)    // 0x0F

// Shift commands
#define SHIFT_DISPLAY_RIGHT                (CURSOR_DISPLAY_SHIFT | SHIFT_DISPLAY | SHIFT_RIGHT)     // 0x1C
#define SHIFT_DISPLAY_LEFT                 (CURSOR_DISPLAY_SHIFT | SHIFT_DISPLAY | SHIFT_LEFT)      // 0x18
#define MOVE_CURSOR_RIGHT                  (CURSOR_DISPLAY_SHIFT | SHIFT_CURSOR  | SHIFT_RIGHT)      // 0x14
#define MOVE_CURSOR_LEFT                   (CURSOR_DISPLAY_SHIFT | SHIFT_CURSOR  | SHIFT_LEFT)       // 0x10

#define FUNCTION_SET_8BIT_2_LINES_5x8      (FUNCTION_SET | DATA_LENGTH_8BIT | TWO_LINES | FONT_5X8)
#define FUNCTION_SET_8BIT_2_LINES_5x10     (FUNCTION_SET | DATA_LENGTH_8BIT | TWO_LINES | FONT_5X10)
#define FUNCTION_SET_8BIT_1_LINES_5x8      (FUNCTION_SET | DATA_LENGTH_8BIT | ONE_LINE  | FONT_5X8)
#define FUNCTION_SET_8BIT_1_LINES_5x10     (FUNCTION_SET | DATA_LENGTH_8BIT | ONE_LINE  | FONT_5X10)
#define FUNCTION_SET_4BIT_2_LINES_5x8      (FUNCTION_SET | DATA_LENGTH_4BIT | TWO_LINES | FONT_5X8)
#define FUNCTION_SET_4BIT_2_LINES_5x10     (FUNCTION_SET | DATA_LENGTH_4BIT | TWO_LINES | FONT_5X10)
#define FUNCTION_SET_4BIT_1_LINES_5x8      (FUNCTION_SET | DATA_LENGTH_4BIT | ONE_LINE  | FONT_5X8)
#define FUNCTION_SET_4BIT_1_LINES_5x10     (FUNCTION_SET | DATA_LENGTH_4BIT | ONE_LINE  | FONT_5X10)

typedef enum{
        HCLCD_ENU_OK,
        HCLCD_ENU_LCD_IS_BUSY,
        HCLCD_ENU_INVALID_CLCD_MODE,
        HCLCD_ENU_INVALID_ROW,
        HCLCD_ENU_INVALID_COLUMN,
        HCLCD_ENU_INVALID_PATTERN_NUM,
        HCLCD_ENU_INVALID_CLCD_NUM,
        HCLCD_ENU_INVALID_PIN_NUM,
        HCLCD_ENU_INVALID_PORT_NUM,  
        HCLCD_ENU_INVALID_PIN_CONFIG,  
        HCLCD_ENU_INVALID_PORT_CONFIG,
        HCLCD_ENU_INVALID_PIN_STATE,  
        HCLCD_ENU_INVALID_PORT_STATE,
        HCLCD_ENU_NULL_PTR,
        HCLCD_ENU_NOK
}HCLCD_enuErrorStatus_t;

typedef enum{
    HCLCD_ENU_PIN_D0,
    HCLCD_ENU_PIN_D1,
    HCLCD_ENU_PIN_D2,
    HCLCD_ENU_PIN_D3,
    HCLCD_ENU_PIN_D4,
    HCLCD_ENU_PIN_D5,
    HCLCD_ENU_PIN_D6,
    HCLCD_ENU_PIN_D7
}HCLCD_enuDataPins_t;

typedef enum{
    HCLCD_ENU_PIN_RS,
    HCLCD_ENU_PIN_RW,
    HCLCD_ENU_PIN_E     
}HCLCD_enuPowPins_t;

typedef enum{
        HCLCD_4_BIT_MODE=4,
        HCLCD_8_BIT_MODE=8
}HCLCD_enuLcdMode_t;

typedef enum{
        HCLCD_2x16,
        HCLCD_4x20
}HCLCD_enuLcdType_t;

typedef enum
{
    HCLCD_FIRST_ROW=0,
    HCLCD_SECOND_ROW
} HCLCD_enuRow_t;

typedef struct
{
        MGPIO_enuPort_t portNum;
        MGPIO_enuPin_t pinNum;
}HCLCD_PORT_PIN;

typedef struct{
        HCLCD_enuLcdType_t ClcdType;
        HCLCD_enuLcdMode_t ClcdMode;
        HCLCD_PORT_PIN DataPins[8];
        HCLCD_PORT_PIN PowerPins[3];
}HCLCD_structLcdCfg_t;

/* Usage Example:
 * lcd_command(FUNCTION_SET | DATA_LENGTH_8BIT | TWO_LINES | FONT_5X8); // 8-bit, 2 lines, 5x8 font
 * lcd_command(DISPLAY_ON_CURSOR_BLINK); // Display on, cursor on, blink on
 * lcd_command(SHIFT_DISPLAY_LEFT);      // Shift entire display left
*/
HCLCD_enuErrorStatus_t HCLCD_enuAsyncInit(void);
HCLCD_enuErrorStatus_t HCLCD_enuAsyncWriteData(uint8_t Copyu8Data, uint8_t Copy_uint8LcdNum);
HCLCD_enuErrorStatus_t HCLCD_enuReadData(uint8_t* Adduint8_Data, uint8_t Copy_uint8LcdNum);
HCLCD_enuErrorStatus_t HCLCD_enuAsyncWriteCommand(uint8_t Copyu8Command, uint8_t Copy_uint8LcdNum);
HCLCD_enuErrorStatus_t HCLCD_enuReadCommand(uint8_t* Adduint8_Data, uint8_t Copy_uint8LcdNum);
HCLCD_enuErrorStatus_t HCLCD_enuAsyncWriteString(sint8_t *Addu8_string, uint8_t Copy_uint8LcdNum);
HCLCD_enuErrorStatus_t HCLCD_enuWriteNumber(float64_t Copyfloat64_number, uint8_t Copy_uint8LcdNum);
HCLCD_enuErrorStatus_t HCLCD_enuGotoRowColumn(HCLCD_enuRow_t Copyenu_Row, uint8_t Copyenu_Column,uint8_t Copy_uint8LcdNum);
HCLCD_enuErrorStatus_t HCLCD_enuStoreSpecialChar(uint8_t *Addu8_pattern, uint8_t Copyu8_BlockNumber, uint8_t Copy_uint8LcdNum);
HCLCD_enuErrorStatus_t HCLCD_enuDisplaySpecialChar(uint8_t Copyu8_PatternNumber, HCLCD_enuRow_t Copyenu_Row, uint8_t Copyenu_Column, uint8_t Copy_uint8LcdNum);

#endif



