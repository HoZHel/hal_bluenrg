/**
  ******************************************************************************
  * @file    rf_driver_ll_lcd.h
  * @author  RF Application Team
  * @brief   Header file of LCD Controller LL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef RF_DRIVER_LL_LCD_H
#define RF_DRIVER_LL_LCD_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#if defined(CONFIG_DEVICE_SPIRIT3)
#include "spirit3.h"
#endif
  
/** @addtogroup RF_Driver_LL_Driver
  * @{
  */

#if defined(LCD)

/** @addtogroup LCD
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup LCD_Exported_Types LCD Exported Types
  * @{
  */

#if defined(USE_FULL_LL_DRIVER)
/** @defgroup LCD_LL_INIT LCD Exported Init structure
  * @{
  */
  
/**
  * @brief LCD structure definition
  */

typedef struct
{
  uint32_t Prescaler;       /*!< Configures the LCD Prescaler.
                                 This parameter can be one value of @ref LCD_Prescaler */
  uint32_t Divider;         /*!< Configures the LCD Divider.
                                 This parameter can be one value of @ref LCD_Divider */
  uint32_t Duty;            /*!< Configures the LCD Duty.
                                 This parameter can be one value of @ref LCD_Duty */
  uint32_t Bias;            /*!< Configures the LCD Bias.
                                 This parameter can be one value of @ref LCD_Bias */
  uint32_t VoltageSource;   /*!< Selects the LCD Voltage source.
                                 This parameter can be one value of @ref LCD_Voltage_Source */
  uint32_t Contrast;        /*!< Configures the LCD Contrast.
                                 This parameter can be one value of @ref LCD_Contrast */
  uint32_t DeadTime;        /*!< Configures the LCD Dead Time.
                                 This parameter can be one value of @ref LCD_DeadTime */
  uint32_t PulseOnDuration; /*!< Configures the LCD Pulse On Duration.
                                 This parameter can be one value of @ref LCD_PulseOnDuration */
  uint32_t HighDrive;       /*!< Enable or disable the low resistance divider.
                                 This parameter can be one value of @ref LCD_HighDrive */
  uint32_t BlinkMode;       /*!< Configures the LCD Blink Mode.
                                 This parameter can be one value of @ref LCD_BlinkMode */
  uint32_t BlinkFrequency;  /*!< Configures the LCD Blink frequency.
                                 This parameter can be one value of @ref LCD_BlinkFrequency */
#ifdef LCD_CR_MUX_SEG
  uint32_t MuxSegment;      /*!< Enable or disable mux segment.
                                 This parameter can be one value of @ref LCD_MuxSegment */
#endif
} LCD_InitTypeDef;


/**
  * @}
  */
#endif /* USE_FULL_LL_DRIVER */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup LCD_Exported_Constants LCD Exported Constants
  * @{
  */

/** @defgroup  LCD_ErrorCode LCD Error Code
  * @{
  */
#define LL_LCD_FLAG_NONE       (0x00000000U)    /*!< No error */
#define LL_LCD_FLAG_FCRSF      LCD_SR_FCRSF     /*!< Synchro flag timeout */
#define LL_LCD_FLAG_UDR        LCD_SR_UDR       /*!< Update display request flag timeout */
#define LL_LCD_FLAG_UDD        LCD_SR_UDD       /*!< Update display done flag timeout */
#define LL_LCD_FLAG_ENS        LCD_SR_ENS       /*!< LCD enabled status flag timeout */
#define LL_LCD_FLAG_RDY        LCD_SR_RDY       /*!< LCD Booster ready timeout */
#define LL_LCD_FLAG_SOF        LCD_SR_SOF       /*!< Start of frame flag */
/**
  * @}
  */

/** @defgroup LCD_Prescaler LCD Prescaler
  * @{
  */
#define LL_LCD_PRESCALER_1        (0x00000000U)  /*!< CLKPS = LCDCLK        */
#define LL_LCD_PRESCALER_2        (0x00400000U)  /*!< CLKPS = LCDCLK/2      */
#define LL_LCD_PRESCALER_4        (0x00800000U)  /*!< CLKPS = LCDCLK/4      */
#define LL_LCD_PRESCALER_8        (0x00C00000U)  /*!< CLKPS = LCDCLK/8      */
#define LL_LCD_PRESCALER_16       (0x01000000U)  /*!< CLKPS = LCDCLK/16     */
#define LL_LCD_PRESCALER_32       (0x01400000U)  /*!< CLKPS = LCDCLK/32     */
#define LL_LCD_PRESCALER_64       (0x01800000U)  /*!< CLKPS = LCDCLK/64     */
#define LL_LCD_PRESCALER_128      (0x01C00000U)  /*!< CLKPS = LCDCLK/128    */
#define LL_LCD_PRESCALER_256      (0x02000000U)  /*!< CLKPS = LCDCLK/256    */
#define LL_LCD_PRESCALER_512      (0x02400000U)  /*!< CLKPS = LCDCLK/512    */
#define LL_LCD_PRESCALER_1024     (0x02800000U)  /*!< CLKPS = LCDCLK/1024   */
#define LL_LCD_PRESCALER_2048     (0x02C00000U)  /*!< CLKPS = LCDCLK/2048   */
#define LL_LCD_PRESCALER_4096     (0x03000000U)  /*!< CLKPS = LCDCLK/4096   */
#define LL_LCD_PRESCALER_8192     (0x03400000U)  /*!< CLKPS = LCDCLK/8192   */
#define LL_LCD_PRESCALER_16384    (0x03800000U)  /*!< CLKPS = LCDCLK/16384  */
#define LL_LCD_PRESCALER_32768    (0x03C00000U)  /*!< CLKPS = LCDCLK/32768  */
/**
  * @}
  */

/** @defgroup LCD_Divider LCD Divider
  * @{
  */
#define LL_LCD_DIVIDER_16         (0x00000000U)  /*!< LCD frequency = CLKPS/16 */
#define LL_LCD_DIVIDER_17         (0x00040000U)  /*!< LCD frequency = CLKPS/17 */
#define LL_LCD_DIVIDER_18         (0x00080000U)  /*!< LCD frequency = CLKPS/18 */
#define LL_LCD_DIVIDER_19         (0x000C0000U)  /*!< LCD frequency = CLKPS/19 */
#define LL_LCD_DIVIDER_20         (0x00100000U)  /*!< LCD frequency = CLKPS/20 */
#define LL_LCD_DIVIDER_21         (0x00140000U)  /*!< LCD frequency = CLKPS/21 */
#define LL_LCD_DIVIDER_22         (0x00180000U)  /*!< LCD frequency = CLKPS/22 */
#define LL_LCD_DIVIDER_23         (0x001C0000U)  /*!< LCD frequency = CLKPS/23 */
#define LL_LCD_DIVIDER_24         (0x00200000U)  /*!< LCD frequency = CLKPS/24 */
#define LL_LCD_DIVIDER_25         (0x00240000U)  /*!< LCD frequency = CLKPS/25 */
#define LL_LCD_DIVIDER_26         (0x00280000U)  /*!< LCD frequency = CLKPS/26 */
#define LL_LCD_DIVIDER_27         (0x002C0000U)  /*!< LCD frequency = CLKPS/27 */
#define LL_LCD_DIVIDER_28         (0x00300000U)  /*!< LCD frequency = CLKPS/28 */
#define LL_LCD_DIVIDER_29         (0x00340000U)  /*!< LCD frequency = CLKPS/29 */
#define LL_LCD_DIVIDER_30         (0x00380000U)  /*!< LCD frequency = CLKPS/30 */
#define LL_LCD_DIVIDER_31         (0x003C0000U)  /*!< LCD frequency = CLKPS/31 */
/**
  * @}
  */


/** @defgroup LCD_Duty LCD Duty
  * @{
  */
#define LL_LCD_DUTY_STATIC                 (                                0x00000000U )                   /*!< Static duty */
#define LL_LCD_DUTY_1_2                    (                              LCD_CR_DUTY_0 )                   /*!< 1/2 duty    */
#define LL_LCD_DUTY_1_3                    (              LCD_CR_DUTY_1                 )                   /*!< 1/3 duty    */
#define LL_LCD_DUTY_1_4                    (              LCD_CR_DUTY_1 | LCD_CR_DUTY_0 )                   /*!< 1/4 duty    */
#define LL_LCD_DUTY_1_8                    ( LCD_CR_DUTY_2                              )                   /*!< 1/8 duty    */
/**
  * @}
  */


/** @defgroup LCD_Bias LCD Bias
  * @{
  */
#define LL_LCD_BIAS_1_4                    (0x00000000U) /*!< 1/4 Bias */
#define LL_LCD_BIAS_1_2                    LCD_CR_BIAS_0 /*!< 1/2 Bias */
#define LL_LCD_BIAS_1_3                    LCD_CR_BIAS_1 /*!< 1/3 Bias */
/**
  * @}
  */

/** @defgroup LCD_Voltage_Source LCD Voltage Source
  * @{
  */
#define LL_LCD_VOLTAGESOURCE_INTERNAL      (0x00000000U)  /*!< Internal voltage source for the LCD */
#define LL_LCD_VOLTAGESOURCE_EXTERNAL      LCD_CR_VSEL    /*!< External voltage source for the LCD */
/**
  * @}
  */

/** @defgroup LCD_Interrupts LCD Interrupts
  * @{
  */
#define LL_LCD_IT_SOF                      LCD_FCR_SOFIE
#define LL_LCD_IT_UDD                      LCD_FCR_UDDIE
/**
  * @}
  */

/** @defgroup LCD_PulseOnDuration LCD Pulse On Duration
  * @{
  */
#define LL_LCD_PULSEONDURATION_0           (                                 0x00000000U  )                 /*!< Pulse ON duration = 0 pulse  */
#define LL_LCD_PULSEONDURATION_1           (                                LCD_FCR_PON_0 )                 /*!< Pulse ON duration = 1/CK_PS  */
#define LL_LCD_PULSEONDURATION_2           (                LCD_FCR_PON_1                 )                 /*!< Pulse ON duration = 2/CK_PS  */
#define LL_LCD_PULSEONDURATION_3           (                LCD_FCR_PON_1 | LCD_FCR_PON_0 )                 /*!< Pulse ON duration = 3/CK_PS  */
#define LL_LCD_PULSEONDURATION_4           (LCD_FCR_PON_2                                 )                 /*!< Pulse ON duration = 4/CK_PS  */
#define LL_LCD_PULSEONDURATION_5           (LCD_FCR_PON_2 |                 LCD_FCR_PON_0 )                 /*!< Pulse ON duration = 5/CK_PS  */
#define LL_LCD_PULSEONDURATION_6           (LCD_FCR_PON_2 | LCD_FCR_PON_1                 )                 /*!< Pulse ON duration = 6/CK_PS  */
#define LL_LCD_PULSEONDURATION_7           (LCD_FCR_PON                                   )                 /*!< Pulse ON duration = 7/CK_PS  */
/**
  * @}
  */


/** @defgroup LCD_DeadTime LCD Dead Time
  * @{
  */
#define LL_LCD_DEADTIME_0                  (                                     0x00000000U)                /*!< No dead Time                                  */
#define LL_LCD_DEADTIME_1                  (                                  LCD_FCR_DEAD_0)                /*!< One Phase between different couple of Frame   */
#define LL_LCD_DEADTIME_2                  (                 LCD_FCR_DEAD_1                 )                /*!< Two Phase between different couple of Frame   */
#define LL_LCD_DEADTIME_3                  (                 LCD_FCR_DEAD_1 | LCD_FCR_DEAD_0)                /*!< Three Phase between different couple of Frame */
#define LL_LCD_DEADTIME_4                  (LCD_FCR_DEAD_2                                  )                /*!< Four Phase between different couple of Frame  */
#define LL_LCD_DEADTIME_5                  (LCD_FCR_DEAD_2 |                  LCD_FCR_DEAD_0)                /*!< Five Phase between different couple of Frame  */
#define LL_LCD_DEADTIME_6                  (LCD_FCR_DEAD_2 | LCD_FCR_DEAD_1                 )                /*!< Six Phase between different couple of Frame   */
#define LL_LCD_DEADTIME_7                  (LCD_FCR_DEAD                                    )                /*!< Seven Phase between different couple of Frame */
/**
  * @}
  */

/** @defgroup LCD_HighDrive LCD High Drive
  * @{
  */

#define LL_LCD_HIGHDRIVE_DISABLE             ((uint32_t)0x00000000)        /*!< High drive disabled */
#define LL_LCD_HIGHDRIVE_ENABLE              (LCD_FCR_HD)                  /*!< High drive enabled  */
/**
  * @}
  */

/** @defgroup LCD_BlinkMode LCD Blink Mode
  * @{
  */
#define LL_LCD_BLINKMODE_OFF               (0x00000000U)           /*!< Blink disabled                                     */
#define LL_LCD_BLINKMODE_SEG0_COM0         (LCD_FCR_BLINK_0)       /*!< Blink enabled on SEG[0], COM[0] (1 pixel)          */
#define LL_LCD_BLINKMODE_SEG0_ALLCOM       (LCD_FCR_BLINK_1)       /*!< Blink enabled on SEG[0], all COM (up to
                                                                        8 pixels according to the programmed duty)         */
#define LL_LCD_BLINKMODE_ALLSEG_ALLCOM     (LCD_FCR_BLINK)         /*!< Blink enabled on all SEG and all COM (all pixels)  */
/**
  * @}
  */

/** @defgroup LCD_BlinkFrequency LCD Blink Frequency
  * @{
  */
#define LL_LCD_BLINKFREQUENCY_DIV8         (                                           0x00000000U) /*!< The Blink frequency = fLCD/8    */
#define LL_LCD_BLINKFREQUENCY_DIV16        (                                      LCD_FCR_BLINKF_0) /*!< The Blink frequency = fLCD/16   */
#define LL_LCD_BLINKFREQUENCY_DIV32        (                   LCD_FCR_BLINKF_1                   ) /*!< The Blink frequency = fLCD/32   */
#define LL_LCD_BLINKFREQUENCY_DIV64        (                   LCD_FCR_BLINKF_1 | LCD_FCR_BLINKF_0) /*!< The Blink frequency = fLCD/64   */
#define LL_LCD_BLINKFREQUENCY_DIV128       (LCD_FCR_BLINKF_2                                      ) /*!< The Blink frequency = fLCD/128  */
#define LL_LCD_BLINKFREQUENCY_DIV256       (LCD_FCR_BLINKF_2 |                    LCD_FCR_BLINKF_0)  /*!< The Blink frequency = fLCD/256  */
#define LL_LCD_BLINKFREQUENCY_DIV512       (LCD_FCR_BLINKF_2 | LCD_FCR_BLINKF_1                   )  /*!< The Blink frequency = fLCD/512  */
#define LL_LCD_BLINKFREQUENCY_DIV1024      (LCD_FCR_BLINKF                                        )  /*!< The Blink frequency = fLCD/1024 */
/**
  * @}
  */

/** @defgroup LCD_Contrast LCD Contrast
  * @{
  */
#define LL_LCD_CONTRASTLEVEL_0               (0x00000000U)                
#define LL_LCD_CONTRASTLEVEL_1               (LCD_FCR_CC_0)               
#define LL_LCD_CONTRASTLEVEL_2               (LCD_FCR_CC_1)               
#define LL_LCD_CONTRASTLEVEL_3               (LCD_FCR_CC_1 | LCD_FCR_CC_0)
#define LL_LCD_CONTRASTLEVEL_4               (LCD_FCR_CC_2)               
#define LL_LCD_CONTRASTLEVEL_5               (LCD_FCR_CC_2 | LCD_FCR_CC_0)
#define LL_LCD_CONTRASTLEVEL_6               (LCD_FCR_CC_2 | LCD_FCR_CC_1)
#define LL_LCD_CONTRASTLEVEL_7               (LCD_FCR_CC)     
/**
  * @}
  */

/** @defgroup LCD_RAMRegister LCD RAMRegister
  * @{
  */
#define LL_LCD_RAM_REGISTER0               (0x00000000U) /*!< LCD RAM Register 0  */
#define LL_LCD_RAM_REGISTER1               (0x00000001U) /*!< LCD RAM Register 1  */
#define LL_LCD_RAM_REGISTER2               (0x00000002U) /*!< LCD RAM Register 2  */
#define LL_LCD_RAM_REGISTER3               (0x00000003U) /*!< LCD RAM Register 3  */
#define LL_LCD_RAM_REGISTER4               (0x00000004U) /*!< LCD RAM Register 4  */
#define LL_LCD_RAM_REGISTER5               (0x00000005U) /*!< LCD RAM Register 5  */
#define LL_LCD_RAM_REGISTER6               (0x00000006U) /*!< LCD RAM Register 6  */
#define LL_LCD_RAM_REGISTER7               (0x00000007U) /*!< LCD RAM Register 7  */
/**
  * @}
  */

/** @defgroup LCD_RAMRegister LCD RAMSegmentIndex
  * @{
  */
#define LL_LCD_RAM_SEGMENT_0               LCD_RAM_COM0_SEGMENT_DATA_0  /*!< LCD RAM Segment 0  */
#define LL_LCD_RAM_SEGMENT_1               LCD_RAM_COM0_SEGMENT_DATA_1  /*!< LCD RAM Segment 1  */
#define LL_LCD_RAM_SEGMENT_2               LCD_RAM_COM0_SEGMENT_DATA_2  /*!< LCD RAM Segment 2  */
#define LL_LCD_RAM_SEGMENT_3               LCD_RAM_COM0_SEGMENT_DATA_3  /*!< LCD RAM Segment 3  */
#define LL_LCD_RAM_SEGMENT_4               LCD_RAM_COM0_SEGMENT_DATA_4  /*!< LCD RAM Segment 4  */
#define LL_LCD_RAM_SEGMENT_5               LCD_RAM_COM0_SEGMENT_DATA_5  /*!< LCD RAM Segment 5  */
#define LL_LCD_RAM_SEGMENT_6               LCD_RAM_COM0_SEGMENT_DATA_6  /*!< LCD RAM Segment 6  */
#define LL_LCD_RAM_SEGMENT_7               LCD_RAM_COM0_SEGMENT_DATA_7  /*!< LCD RAM Segment 7  */
#define LL_LCD_RAM_SEGMENT_8               LCD_RAM_COM0_SEGMENT_DATA_8  /*!< LCD RAM Segment 8  */
#define LL_LCD_RAM_SEGMENT_9               LCD_RAM_COM0_SEGMENT_DATA_9  /*!< LCD RAM Segment 9  */
#define LL_LCD_RAM_SEGMENT_10              LCD_RAM_COM0_SEGMENT_DATA_10 /*!< LCD RAM Segment 10 */
#define LL_LCD_RAM_SEGMENT_11              LCD_RAM_COM0_SEGMENT_DATA_11 /*!< LCD RAM Segment 11 */
#define LL_LCD_RAM_SEGMENT_12              LCD_RAM_COM0_SEGMENT_DATA_12 /*!< LCD RAM Segment 12 */
#define LL_LCD_RAM_SEGMENT_13              LCD_RAM_COM0_SEGMENT_DATA_13 /*!< LCD RAM Segment 13 */
#define LL_LCD_RAM_SEGMENT_14              LCD_RAM_COM0_SEGMENT_DATA_14 /*!< LCD RAM Segment 14 */
#define LL_LCD_RAM_SEGMENT_15              LCD_RAM_COM0_SEGMENT_DATA_15 /*!< LCD RAM Segment 15 */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup LCD_Exported_Macros LCD Exported Macros
  * @{
  */

/**
  * @brief  Enable the voltage output buffer for higher driving capability.
  * @rmtoll CR          BUFEN            LL_LCD_EnableVoltageOutputBuffer
  * @param  LCDx LCD Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCD_EnableVoltageOutputBuffer(LCD_TypeDef *LCDx)
{
  SET_BIT(LCDx->CR, LCD_CR_BUFEN);
}

/**
  * @brief  Disable Enable the voltage output buffer for higher driving capability.
  * @rmtoll CR          BUFEN            LL_LCD_DisableVoltageOutputBuffer
  * @param  LCDx LCD Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCD_DisableVoltageOutputBuffer(LCD_TypeDef *LCDx)
{ 
  CLEAR_BIT(LCDx->CR, LCD_CR_BUFEN);
}

/**
  * @brief  Indicate if BUFEN is enabled
  * @rmtoll CR          BUFEN            LL_LCD_IsEnabledVoltageOutputBuffer  
  * @param  LCDx LCD Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCD_IsEnabledVoltageOutputBuffer(LCD_TypeDef *LCDx)
{
  return ((READ_BIT(LCDx->CR, LCD_CR_BUFEN) == (LCD_CR_BUFEN)) ? 1U : 0U);
}

/**
  * @brief  Set the Bias selector
  * @note   These bits determine the bias used. Value 11 is forbidden.
  * @rmtoll CR          BIAS       LL_LCD_SetBias
  * @param  LCDx LCD Instance
  * @param  value can be one of the following values:
  * @arg LL_LCD_BIAS_1_4
  * @arg LL_LCD_BIAS_1_2
  * @arg LL_LCD_BIAS_1_3
  * @retval None
  */
__STATIC_INLINE void LL_LCD_SetBias(LCD_TypeDef *LCDx, uint32_t value)
{
  MODIFY_REG(LCDx->CR, LCD_CR_BIAS, value);
}

/**
  * @brief  Get the Bias selector
  * @rmtoll CR          BIAS       LL_LCD_GetBias
  * @retval retval value can be one of the following values:
  * @arg LL_LCD_BIAS_1_4
  * @arg LL_LCD_BIAS_1_2
  * @arg LL_LCD_BIAS_1_3
  */
__STATIC_INLINE uint32_t LL_LCD_GetBias(LCD_TypeDef *LCDx)
{
  return (uint32_t)(READ_BIT(LCDx->CR, LCD_CR_BIAS));
}


/**
  * @brief  Set the Duty selection
  * @note   These bits determine the duty cycle. Values 101, 110 and 111 are forbidden.
  * @rmtoll CR          DUTY       LL_LCD_SetDuty
  * @note   When the display is disabled, the update is performed for all
  *         LCD_DISPLAY locations.
  *         When the display is enabled, the update is performed only for locations
  *         for which commons are active (depending on DUTY). For example if
  *         DUTY = 1/2, only the LCD_DISPLAY of COM0 and COM1 will be updated.
  * @note  Refer to the User Manual for all COMx/SEGx specifications into 
  *        the section Remapping capability.
  *        Remapping capability
  *        DUTY     Capability     Function
  *        1/8      12x8           COM[0:7] SEG[0:11]
  *        1/4      16x4           COM[0:3] SEG[0:15]
  *        1/3      16x3           COM[0:2] SEG[0:15]
  *        1/2      16x2           COM[0:1] SEG[0:15]
  *        STATIC   16x1           COM[0]   SEG[0:15]
  * @param  LCDx LCD Instance
  * @param  value can be one of the following values:
  * @arg LL_LCD_DUTY_STATIC
  * @arg LL_LCD_DUTY_1_2
  * @arg LL_LCD_DUTY_1_3
  * @arg LL_LCD_DUTY_1_4
  * @arg LL_LCD_DUTY_1_8
  * @retval None
  */
__STATIC_INLINE void LL_LCD_SetDuty(LCD_TypeDef *LCDx, uint32_t value)
{
  MODIFY_REG(LCDx->CR, LCD_CR_DUTY, value);
}

/**
  * @brief  Get the Duty selected
  * @rmtoll CR          DUTY       LL_LCD_GetDuty
  * @retval Duty selected can be one of the following values:
  * @arg LL_LCD_DUTY_STATIC
  * @arg LL_LCD_DUTY_1_2
  * @arg LL_LCD_DUTY_1_3
  * @arg LL_LCD_DUTY_1_4
  * @arg LL_LCD_DUTY_1_8
  */
__STATIC_INLINE uint32_t LL_LCD_GetDuty(LCD_TypeDef *LCDx)
{
  return (uint32_t)(READ_BIT(LCDx->CR, LCD_CR_DUTY));
}

/**
  * @brief  Set the Voltage source
  * @note   The VSEL bit determines the voltage source for the LCD.
  * @rmtoll CR          VSEL       LL_LCD_SetVoltageSource
  * @param  LCDx LCD Instance
  * @param  value can be one of the following values:
  * @arg LL_LCD_VOLTAGESOURCE_INTERNAL
  * @arg LL_LCD_VOLTAGESOURCE_EXTERNAL
  * @retval None
  */
__STATIC_INLINE void LL_LCD_SetVoltageSource(LCD_TypeDef *LCDx, uint32_t value)
{
  MODIFY_REG(LCDx->CR, LCD_CR_VSEL, value);
}

/**
  * @brief  Get the Voltage source
  * @rmtoll CR          VSEL       LL_LCD_GetVoltageSource
  * @retval can be one of the following values:
  * @arg LL_LCD_VOLTAGESOURCE_INTERNAL
  * @arg LL_LCD_VOLTAGESOURCE_EXTERNAL
  */
__STATIC_INLINE uint32_t LL_LCD_GetVoltageSource(LCD_TypeDef *LCDx)
{
  return (uint32_t)(READ_BIT(LCDx->CR, LCD_CR_VSEL));
}

/**
  * @brief  Enable LCD peripheral
  * @rmtoll CR          LCDEN           LL_LCD_Enable
  * @param  LCDx LCD Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCD_Enable(LCD_TypeDef *LCDx)
{
  SET_BIT(LCDx->CR, LCD_CR_LCDEN);
}

/**
  * @brief  Disable LCD peripheral
  * @note   When disabling the LCD, follow the procedure described in the Reference Manual.
  * @rmtoll CR          LCDEN           LL_LCD_Disable
  * @param  LCDx LCD Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCD_Disable(LCD_TypeDef *LCDx)
{
  CLEAR_BIT(LCDx->CR, LCD_CR_LCDEN);
}

/**
  * @brief  Check if LCD peripheral is enabled
  * @rmtoll CR          LCDEN           LL_LCD_IsEnabled
  * @param  LCDx LCD Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCD_IsEnabled(LCD_TypeDef *LCDx)
{
  return ((READ_BIT(LCDx->CR, LCD_CR_LCDEN) == (LCD_CR_LCDEN)) ? 1UL : 0UL);
}
   
/**
  * @brief  Set the PS 16-bit prescaler
  * @note   These bits are written by software to define the division factor of the PS 16-bit prescaler.
  * @rmtoll FCR          PS       LL_LCD_SetPrescaler
  * @param  LCDx LCD Instance
  * @param  value can be one of the following values:
  * @arg LL_LCD_PRESCALER_1
  * @arg LL_LCD_PRESCALER_2
  * @arg LL_LCD_PRESCALER_4
  * @arg LL_LCD_PRESCALER_8
  * @arg LL_LCD_PRESCALER_16
  * @arg LL_LCD_PRESCALER_32
  * @arg LL_LCD_PRESCALER_64
  * @arg LL_LCD_PRESCALER_128
  * @arg LL_LCD_PRESCALER_256
  * @arg LL_LCD_PRESCALER_512
  * @arg LL_LCD_PRESCALER_1024
  * @arg LL_LCD_PRESCALER_2048
  * @arg LL_LCD_PRESCALER_4096
  * @arg LL_LCD_PRESCALER_8192
  * @arg LL_LCD_PRESCALER_16384
  * @arg LL_LCD_PRESCALER_32768
  * @retval None
  */
__STATIC_INLINE void LL_LCD_SetPrescaler(LCD_TypeDef *LCDx, uint32_t value)
{
  MODIFY_REG(LCDx->FCR, LCD_FCR_PS, value);
}

/**
  * @brief  Get the PS 16-bit prescaler
  * @note   These bits are written by software to define the division factor of the PS 16-bit prescaler.
  * @rmtoll FCR          PS       LL_LCD_GetPrescaler
  * @retval can be one of the following values:
  * @arg LL_LCD_PRESCALER_1
  * @arg LL_LCD_PRESCALER_2
  * @arg LL_LCD_PRESCALER_4
  * @arg LL_LCD_PRESCALER_8
  * @arg LL_LCD_PRESCALER_16
  * @arg LL_LCD_PRESCALER_32
  * @arg LL_LCD_PRESCALER_64
  * @arg LL_LCD_PRESCALER_128
  * @arg LL_LCD_PRESCALER_256
  * @arg LL_LCD_PRESCALER_512
  * @arg LL_LCD_PRESCALER_1024
  * @arg LL_LCD_PRESCALER_2048
  * @arg LL_LCD_PRESCALER_4096
  * @arg LL_LCD_PRESCALER_8192
  * @arg LL_LCD_PRESCALER_16384
  * @arg LL_LCD_PRESCALER_32768
  */
__STATIC_INLINE uint32_t LL_LCD_GetPrescaler(LCD_TypeDef *LCDx)
{
  return (uint32_t)(READ_BIT(LCDx->FCR, LCD_FCR_PS));
}

/**
  * @brief  Set the DIV clock divider
  * @note   These bits are written by software to define the division factor of the DIV divider.
  * @rmtoll FCR          DIV       LL_LCD_SetClockDivider
  * @param  LCDx LCD Instance
  * @param  value can be one of the following values:
  * @arg LL_LCD_DIVIDER_16
  * @arg LL_LCD_DIVIDER_17
  * @arg LL_LCD_DIVIDER_18
  * @arg LL_LCD_DIVIDER_19
  * @arg LL_LCD_DIVIDER_20
  * @arg LL_LCD_DIVIDER_21
  * @arg LL_LCD_DIVIDER_22
  * @arg LL_LCD_DIVIDER_23
  * @arg LL_LCD_DIVIDER_24
  * @arg LL_LCD_DIVIDER_25
  * @arg LL_LCD_DIVIDER_26
  * @arg LL_LCD_DIVIDER_27
  * @arg LL_LCD_DIVIDER_28
  * @arg LL_LCD_DIVIDER_29
  * @arg LL_LCD_DIVIDER_30
  * @arg LL_LCD_DIVIDER_31
  * @retval None
  */
__STATIC_INLINE void LL_LCD_SetClockDivider(LCD_TypeDef *LCDx, uint32_t value)
{
  MODIFY_REG(LCDx->FCR, LCD_FCR_DIV, value);
}

/**
  * @brief  Get the DIV clock divider
  * @note   These bits are written by software to define the division factor of the DIV divider.
  * @rmtoll FCR          DIV       LL_LCD_GetClockDivider
  * @retval can be one of the following values:
  * @arg LL_LCD_DIVIDER_16
  * @arg LL_LCD_DIVIDER_17
  * @arg LL_LCD_DIVIDER_18
  * @arg LL_LCD_DIVIDER_19
  * @arg LL_LCD_DIVIDER_20
  * @arg LL_LCD_DIVIDER_21
  * @arg LL_LCD_DIVIDER_22
  * @arg LL_LCD_DIVIDER_23
  * @arg LL_LCD_DIVIDER_24
  * @arg LL_LCD_DIVIDER_25
  * @arg LL_LCD_DIVIDER_26
  * @arg LL_LCD_DIVIDER_27
  * @arg LL_LCD_DIVIDER_28
  * @arg LL_LCD_DIVIDER_29
  * @arg LL_LCD_DIVIDER_30
  * @arg LL_LCD_DIVIDER_31
  */
__STATIC_INLINE uint32_t LL_LCD_GetClockDivider(LCD_TypeDef *LCDx)
{
  return (uint32_t)(READ_BIT(LCDx->FCR, LCD_FCR_DIV));
}

/**
  * @brief  Set the Blink mode selection
  * @note   Configure the LCD Blink mode.
  * @rmtoll FCR          BLINK       LL_LCD_SetBlinkMode
  * @param  LCDx LCD Instance
  * @param  value can be one of the following values:
  * @arg LL_LCD_BLINKMODE_OFF
  * @arg LL_LCD_BLINKMODE_SEG0_COM0
  * @arg LL_LCD_BLINKMODE_SEG0_ALLCOM
  * @arg LL_LCD_BLINKMODE_ALLSEG_ALLCOM
  * @retval None
  */
__STATIC_INLINE void LL_LCD_SetBlinkMode(LCD_TypeDef *LCDx, uint32_t value)
{
  MODIFY_REG(LCDx->FCR, LCD_FCR_BLINK, value);
}

/**
  * @brief  Get the Blink mode selection
  * @note   Configure the LCD Blink mode.
  * @rmtoll FCR          BLINK       LL_LCD_GetBlinkMode
  * @retval can be one of the following values:
  * @arg LL_LCD_BLINKMODE_OFF
  * @arg LL_LCD_BLINKMODE_SEG0_COM0
  * @arg LL_LCD_BLINKMODE_SEG0_ALLCOM
  * @arg LL_LCD_BLINKMODE_ALLSEG_ALLCOM
  */
__STATIC_INLINE uint32_t LL_LCD_GetBlinkMode(LCD_TypeDef *LCDx)
{
  return (uint32_t)(READ_BIT(LCDx->FCR, LCD_FCR_BLINK));
}

/**
  * @brief  Set the Blink frequency selection
  * @note   Configure the LCD Blink frequency.
  * 
  *                                 ck_div frequency (with LCDCLK frequency of 32.768 kHz)
  *         BLINKF[2:0]                   32Hz     64Hz     128Hz    256Hz
  * LL_LCD_BLINKFREQUENCY_DIV8            4.0  Hz  N/A      N/A      N/A      
  * LL_LCD_BLINKFREQUENCY_DIV16           2.0  Hz  4.0  Hz  N/A      N/A
  * LL_LCD_BLINKFREQUENCY_DIV32           1.0  Hz  2.0  Hz  4.0  Hz  N/A
  * LL_LCD_BLINKFREQUENCY_DIV64           0.5  Hz  1.0  Hz  2.0  Hz  4.0  Hz
  * LL_LCD_BLINKFREQUENCY_DIV128          0.25 Hz  0.5  Hz  1.0  Hz  2.0  Hz
  * LL_LCD_BLINKFREQUENCY_DIV256          N/A      0.25 Hz  0.5  Hz  1.0  Hz
  * LL_LCD_BLINKFREQUENCY_DIV512          N/A      N/A      0.25 Hz  0.5  Hz
  * LL_LCD_BLINKFREQUENCY_DIV1024         N/A      N/A      N/A      0.25 Hz
  *
  * @rmtoll FCR          BLINKF       LL_LCD_SetBlinkFrequency
  * @param  LCDx LCD Instance
  * @param  value can be one of the following values:
  * @arg LL_LCD_BLINKFREQUENCY_DIV8    
  * @arg LL_LCD_BLINKFREQUENCY_DIV16   
  * @arg LL_LCD_BLINKFREQUENCY_DIV32   
  * @arg LL_LCD_BLINKFREQUENCY_DIV64   
  * @arg LL_LCD_BLINKFREQUENCY_DIV128  
  * @arg LL_LCD_BLINKFREQUENCY_DIV256  
  * @arg LL_LCD_BLINKFREQUENCY_DIV512  
  * @arg LL_LCD_BLINKFREQUENCY_DIV1024 
  * @retval None
  */
__STATIC_INLINE void LL_LCD_SetBlinkFrequency(LCD_TypeDef *LCDx, uint32_t value)
{
  MODIFY_REG(LCDx->FCR, LCD_FCR_BLINKF, value);
}

/**
  * @brief  Get the Blink frequency selection
  * @note   Get the configured LCD Blink frequency.
  * @rmtoll FCR          BLINKF       LL_LCD_GetBlinkFrequency
  * @retval can be one of the following values:
  * @arg LL_LCD_BLINKFREQUENCY_DIV8    
  * @arg LL_LCD_BLINKFREQUENCY_DIV16   
  * @arg LL_LCD_BLINKFREQUENCY_DIV32   
  * @arg LL_LCD_BLINKFREQUENCY_DIV64   
  * @arg LL_LCD_BLINKFREQUENCY_DIV128  
  * @arg LL_LCD_BLINKFREQUENCY_DIV256  
  * @arg LL_LCD_BLINKFREQUENCY_DIV512  
  * @arg LL_LCD_BLINKFREQUENCY_DIV1024 
  */
__STATIC_INLINE uint32_t LL_LCD_GetBlinkFrequency(LCD_TypeDef *LCDx)
{
  return (uint32_t)(READ_BIT(LCDx->FCR, LCD_FCR_BLINKF));
}


/**
  * @brief  Set the Contrast control
  * @note   These bits specify one of the VLCD maximum voltages (independent of VDD). It ranges from 2.60 V to 3.51V.
  * @rmtoll FCR          CC       LL_LCD_SetContrastControl
  * @param  LCDx LCD Instance
  * @param  value can be one of the following values:
  * @arg LL_LCD_CONTRASTLEVEL_0
  * @arg LL_LCD_CONTRASTLEVEL_1
  * @arg LL_LCD_CONTRASTLEVEL_2
  * @arg LL_LCD_CONTRASTLEVEL_3
  * @arg LL_LCD_CONTRASTLEVEL_4
  * @arg LL_LCD_CONTRASTLEVEL_5
  * @arg LL_LCD_CONTRASTLEVEL_6
  * @arg LL_LCD_CONTRASTLEVEL_7
  * @retval None
  */
__STATIC_INLINE void LL_LCD_SetContrastControl(LCD_TypeDef *LCDx, uint32_t value)
{
  MODIFY_REG(LCDx->FCR, LCD_FCR_CC, value);
}

/**
  * @brief  Get the Contrast control
  * @note   These bits specify one of the VLCD maximum voltages (independent of VDD). It ranges from 2.60 V to 3.51V.
  * @rmtoll FCR          CC       LL_LCD_GetContrastControl
  * @retval can be one of the following values:
  * @arg LL_LCD_CONTRASTLEVEL_0
  * @arg LL_LCD_CONTRASTLEVEL_1
  * @arg LL_LCD_CONTRASTLEVEL_2
  * @arg LL_LCD_CONTRASTLEVEL_3
  * @arg LL_LCD_CONTRASTLEVEL_4
  * @arg LL_LCD_CONTRASTLEVEL_5
  * @arg LL_LCD_CONTRASTLEVEL_6
  * @arg LL_LCD_CONTRASTLEVEL_7
  */
__STATIC_INLINE uint32_t LL_LCD_GetContrastControl(LCD_TypeDef *LCDx)
{
  return (uint32_t)(READ_BIT(LCDx->FCR, LCD_FCR_CC));
}

/**
  * @brief  Set the Dead time duration
  * @note   These bits are written by software to configure the length of the dead time between frames. During the dead time the COM and SEG voltage levels are held at 0 V to reduce the contrast without modifying the frame rate.
  * @rmtoll FCR          DEAD       LL_LCD_SetDeadTimeDuration
  * @param  LCDx LCD Instance
  * @param  value can be one of the following values:
  * @arg LL_LCD_DEADTIME_0
  * @arg LL_LCD_DEADTIME_1
  * @arg LL_LCD_DEADTIME_2
  * @arg LL_LCD_DEADTIME_3
  * @arg LL_LCD_DEADTIME_4
  * @arg LL_LCD_DEADTIME_5
  * @arg LL_LCD_DEADTIME_6
  * @arg LL_LCD_DEADTIME_7
  * @retval None
  */
__STATIC_INLINE void LL_LCD_SetDeadTimeDuration(LCD_TypeDef *LCDx, uint32_t value)
{
  MODIFY_REG(LCDx->FCR, LCD_FCR_DEAD, value);
}

/**
  * @brief  Get the Dead time duration
  * @note   These bits are written by software to configure the length of the dead time between frames. During the dead time the COM and SEG voltage levels are held at 0 V to reduce the contrast without modifying the frame rate.
  * @rmtoll FCR          DEAD       LL_LCD_GetDeadTimeDuration
  * @retval can be one of the following values:
  * @arg LL_LCD_DEADTIME_0
  * @arg LL_LCD_DEADTIME_1
  * @arg LL_LCD_DEADTIME_2
  * @arg LL_LCD_DEADTIME_3
  * @arg LL_LCD_DEADTIME_4
  * @arg LL_LCD_DEADTIME_5
  * @arg LL_LCD_DEADTIME_6
  * @arg LL_LCD_DEADTIME_7
  */
__STATIC_INLINE uint32_t LL_LCD_GetDeadTimeDuration(LCD_TypeDef *LCDx)
{
  return (uint32_t)(READ_BIT(LCDx->FCR, LCD_FCR_DEAD));
}

/**
  * @brief  Set the pulse duration in terms of ck_ps pulses.
  * @note   A short pulse will lead to lower power consumption, but displays with high internal resistance may need a longer pulse to achieve satisfactory contrast. Note that the pulse will never be longer than one half prescaled LCD clock period.
  * @rmtoll FCR          PON       LL_LCD_SetPulseONDuration
  * @param  LCDx LCD Instance
  * @param  value can be one of the following values:
  * @arg LL_LCD_PULSEONDURATION_0
  * @arg LL_LCD_PULSEONDURATION_1
  * @arg LL_LCD_PULSEONDURATION_2
  * @arg LL_LCD_PULSEONDURATION_3
  * @arg LL_LCD_PULSEONDURATION_4
  * @arg LL_LCD_PULSEONDURATION_5
  * @arg LL_LCD_PULSEONDURATION_6
  * @arg LL_LCD_PULSEONDURATION_7
  * @retval None
  */
__STATIC_INLINE void LL_LCD_SetPulseONDuration(LCD_TypeDef *LCDx, uint32_t value)
{
  MODIFY_REG(LCDx->FCR, LCD_FCR_PON, value);
}

/**
  * @brief  Get the pulse duration in terms of ck_ps pulses.
  * @note   A short pulse will lead to lower power consumption, but displays with high internal resistance may need a longer pulse to achieve satisfactory contrast. Note that the pulse will never be longer than one half prescaled LCD clock period.
  * @rmtoll FCR          PON       LL_LCD_GetPulseONDuration
  * @retval can be one of the following values:
  * @arg LL_LCD_PULSEONDURATION_0
  * @arg LL_LCD_PULSEONDURATION_1
  * @arg LL_LCD_PULSEONDURATION_2
  * @arg LL_LCD_PULSEONDURATION_3
  * @arg LL_LCD_PULSEONDURATION_4
  * @arg LL_LCD_PULSEONDURATION_5
  * @arg LL_LCD_PULSEONDURATION_6
  * @arg LL_LCD_PULSEONDURATION_7
  */
__STATIC_INLINE uint32_t LL_LCD_GetPulseONDuration(LCD_TypeDef *LCDx)
{
  return (uint32_t)(READ_BIT(LCDx->FCR, LCD_FCR_PON));
}

/**
  * @brief  LCD Enable Update display done interrupt
  * @rmtoll FCR          UDDIE            LL_LCD_EnableUpdateDisplayDoneInterrupt
  * @param  LCDx LCD Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCD_EnableUpdateDisplayDoneInterrupt(LCD_TypeDef *LCDx)
{
  SET_BIT(LCDx->FCR, LCD_FCR_UDDIE);
}

/**
  * @brief  LCD Disable Update display done interrupt
  * @rmtoll FCR          UDDIE            LL_LCD_DisableUpdateDisplayDoneInterrupt
  * @param  LCDx LCD Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCD_DisableUpdateDisplayDoneInterrupt(LCD_TypeDef *LCDx)
{ 
  CLEAR_BIT(LCDx->FCR, LCD_FCR_UDDIE);
}

/**
  * @brief  Indicate if UDDIE is enabled. Update display done interrupt
  * @rmtoll FCR          UDDIE            LL_LCD_IsEnabledUpdateDisplayDoneInterrupt  
  * @param  LCDx LCD Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCD_IsEnabledUpdateDisplayDoneInterrupt(LCD_TypeDef *LCDx)
{
  return ((READ_BIT(LCDx->FCR, LCD_FCR_UDDIE) == (LCD_FCR_UDDIE)) ? 1U : 0U);
}

/**
  * @brief  LCD Enable Start of frame interrupt
  * @rmtoll FCR          SOFIE            LL_LCD_EnableStartOfFrameInterrupt
  * @param  LCDx LCD Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCD_EnableStartOfFrameInterrupt(LCD_TypeDef *LCDx)
{
  SET_BIT(LCDx->FCR, LCD_FCR_SOFIE);
}

/**
  * @brief  LCD Disable Start of frame interrupt
  * @rmtoll FCR          SOFIE            LL_LCD_DisableStartOfFrameInterrupt
  * @param  LCDx LCD Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCD_DisableStartOfFrameInterrupt(LCD_TypeDef *LCDx)
{ 
  CLEAR_BIT(LCDx->FCR, LCD_FCR_SOFIE);
}

/**
  * @brief  Indicate if SOFIE is enabled. Start of frame interrupt
  * @rmtoll FCR          SOFIE            LL_LCD_IsEnabledStartOfFrameInterrupt  
  * @param  LCDx LCD Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCD_IsEnabledStartOfFrameInterrupt(LCD_TypeDef *LCDx)
{
  return ((READ_BIT(LCDx->FCR, LCD_FCR_SOFIE) == (LCD_FCR_SOFIE)) ? 1U : 0U);
}

/**
  * @brief  LCD Set High Drive
  * @note   This bit is written by software to enable a low resistance divider. Displays with high internal resistance may need a longer drive time to achieve satisfactory contrast.
  * @rmtoll FCR          HD            LL_LCD_SetHighDrive
  * @param  LCDx LCD Instance
  * @param  value can be one of the following values:
  * @arg LL_LCD_HIGHDRIVE_DISABLE
  * @arg LL_LCD_HIGHDRIVE_ENABLE
  * @retval None
  */
__STATIC_INLINE void LL_LCD_SetHighDrive(LCD_TypeDef *LCDx, uint32_t value)
{
  MODIFY_REG(LCDx->FCR, LCD_FCR_HD, value);
}

/**
  * @brief  LCD Get High Drive
  * @note   This bit is written by software to enable a low resistance divider. Displays with high internal resistance may need a longer drive time to achieve satisfactory contrast.
  * @rmtoll FCR          HD            LL_LCD_GetHighDrive
  * @param  LCDx LCD Instance
  * @retval can be one of the following values:
  * @arg LL_LCD_HIGHDRIVE_DISABLE
  * @arg LL_LCD_HIGHDRIVE_ENABLE
  */
__STATIC_INLINE uint32_t LL_LCD_GetHighDrive(LCD_TypeDef *LCDx)
{ 
  return (uint32_t)(READ_BIT(LCDx->FCR, LCD_FCR_HD));
}


/**
  * @brief  Indicate if FCRSF is enabled. LCD Frame Control Register Synchronization flag
  * @note   This bit is set by hardware each time the LCD_FCR register is updated in the LCDCLK domain. It is cleared by hardware when writing to the LCD_FCR register.
  * @rmtoll SR          FCRSF            LL_LCD_IsActiveFlag_FCRSF  
  * @param  LCDx LCD Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCD_IsActiveFlag_FCRSF(LCD_TypeDef *LCDx)
{
  return ((READ_BIT(LCDx->SR, LCD_SR_FCRSF) == (LCD_SR_FCRSF)) ? 1U : 0U);
}

/**
  * @brief  Indicate if RDY is enabled. It indicates the status of the step-up converter.
  * @note   This bit is set and cleared by hardware.
  * @rmtoll SR          RDY            LL_LCD_IsActiveFlag_RDY  
  * @param  LCDx LCD Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCD_IsActiveFlag_RDY(LCD_TypeDef *LCDx)
{
  return ((READ_BIT(LCDx->SR, LCD_SR_RDY) == (LCD_SR_RDY)) ? 1U : 0U);
}

/**
  * @brief  Indicate if UDD is enabled. Update Display Request done
  * @note   It is cleared by writing 1 to the UDDC bit in the LCD_CLR register.
  * @rmtoll SR          UDD            LL_LCD_IsActiveFlag_UDD  
  * @param  LCDx LCD Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCD_IsActiveFlag_UDD(LCD_TypeDef *LCDx)
{
  return ((READ_BIT(LCDx->SR, LCD_SR_UDD) == (LCD_SR_UDD)) ? 1U : 0U);
}

/**
  * @brief  LCD Enable Each time software modifies the LCD_RAM it must set the UDR bit to transfer the updated data to the second level buffer.
  * @note   The UDR bit stays set until the end of the update and during this time the LCD_RAM is write protected.
  * @rmtoll SR          UDR            LL_LCD_EnableUpdateDisplayRequest
  * @param  LCDx LCD Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCD_EnableUpdateDisplayRequest(LCD_TypeDef *LCDx)
{
  SET_BIT(LCDx->SR, LCD_SR_UDR);
}

/**
  * @brief  Indicate if UDR is enabled. Each time software modifies the LCD_RAM it must set the UDR bit to transfer the updated data to the second level buffer.
  * @note   The UDR bit stays set until the end of the update and during this time the LCD_RAM is write protected.
  * @rmtoll SR          UDR            LL_LCD_IsActiveFlag_UDR  
  * @param  LCDx LCD Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCD_IsActiveFlag_UDR(LCD_TypeDef *LCDx)
{
  return ((READ_BIT(LCDx->SR, LCD_SR_UDR) == (LCD_SR_UDR)) ? 1U : 0U);
}

/**
  * @brief  Indicate if SOF is enabled. This bit is set by hardware at the beginning of a new frame, at the same time as the display data is updated.
  * @note   It is cleared by writing a 1 to the SOFC bit in the LCD_CLR register.
  * @rmtoll SR          SOF            LL_LCD_IsActiveFlag_SOF  
  * @param  LCDx LCD Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCD_IsActiveFlag_SOF(LCD_TypeDef *LCDx)
{
  return ((READ_BIT(LCDx->SR, LCD_SR_SOF) == (LCD_SR_SOF)) ? 1U : 0U);
}

/**
  * @brief  Indicate if ENS is enabled. It indicates the LCD controller status.
  * @note   This bit is set and cleared by hardware.
  * @rmtoll SR          ENS            LL_LCD_IsActiveFlag_ENS  
  * @param  LCDx LCD Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCD_IsActiveFlag_ENS(LCD_TypeDef *LCDx)
{
  return ((READ_BIT(LCDx->SR, LCD_SR_ENS) == (LCD_SR_ENS)) ? 1U : 0U);
}

/**
  * @brief  This bit is written by software to clear the UDD flag in the LCD_SR register.
  * @note   
  * @rmtoll CLR          UDDC            LL_LCD_ClearFlag_UDDC 
  * @param  LCDx LCD Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE void LL_LCD_ClearFlag_UDDC(LCD_TypeDef *LCDx)
{
  SET_BIT(LCDx->CLR, LCD_CLR_UDDC);
}

/**
  * @brief  This bit is written by software to clear the SOF flag in the LCD_SR register.
  * @note   
  * @rmtoll CLR          SOFC            LL_LCD_ClearFlag_SOFC 
  * @param  LCDx LCD Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE void LL_LCD_ClearFlag_SOFC(LCD_TypeDef *LCDx)
{
  SET_BIT(LCDx->CLR, LCD_CLR_SOFC);
}


/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */
/** @addtogroup LCD_Exported_Functions
  * @{
  */
                    
#if defined(USE_FULL_LL_DRIVER)
/** @defgroup RTC_LL_EF_Init Initialization and de-initialization functions
  * @{
  */
/* Initialization/de-initialization methods  **********************************/
/** @addtogroup LCD_Exported_Functions_Group1
  * @{
  */
ErrorStatus LL_LCD_DeInit(LCD_TypeDef *LCDx);
ErrorStatus LL_LCD_Init(LCD_TypeDef *LCDx, LCD_InitTypeDef *hlcd);
void LL_LCD_StructInit(LCD_InitTypeDef *hlcd);
/**
  * @}
  */

/* IO operation methods *******************************************************/
/** @addtogroup LCD_Exported_Functions_Group2
  * @{
  */
void LL_LCD_SetRAM(volatile uint32_t *LCD_COMx, uint32_t SegmentIndex, uint32_t value);
void LL_LCD_UpdateDisplayRequest(LCD_TypeDef *LCDx);

/**
  * @}
  */

/* Peripheral State methods  **************************************************/
/** @addtogroup LCD_Exported_Functions_Group3
  * @{
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* USE_FULL_LL_DRIVER */

/**
  * @}
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/** @defgroup LCD_Private_Macros LCD Private Macros
  * @{
  */


#define LL_LCD_DISABLE_SEGMENT                                            0U
#define LL_LCD_ENABLE_SEGMENT                                             1U  

#define IS_LCD_PRESCALER(__PRESCALER__) (((__PRESCALER__) == LL_LCD_PRESCALER_1)     || \
                                         ((__PRESCALER__) == LL_LCD_PRESCALER_2)     || \
                                         ((__PRESCALER__) == LL_LCD_PRESCALER_4)     || \
                                         ((__PRESCALER__) == LL_LCD_PRESCALER_8)     || \
                                         ((__PRESCALER__) == LL_LCD_PRESCALER_16)    || \
                                         ((__PRESCALER__) == LL_LCD_PRESCALER_32)    || \
                                         ((__PRESCALER__) == LL_LCD_PRESCALER_64)    || \
                                         ((__PRESCALER__) == LL_LCD_PRESCALER_128)   || \
                                         ((__PRESCALER__) == LL_LCD_PRESCALER_256)   || \
                                         ((__PRESCALER__) == LL_LCD_PRESCALER_512)   || \
                                         ((__PRESCALER__) == LL_LCD_PRESCALER_1024)  || \
                                         ((__PRESCALER__) == LL_LCD_PRESCALER_2048)  || \
                                         ((__PRESCALER__) == LL_LCD_PRESCALER_4096)  || \
                                         ((__PRESCALER__) == LL_LCD_PRESCALER_8192)  || \
                                         ((__PRESCALER__) == LL_LCD_PRESCALER_16384) || \
                                         ((__PRESCALER__) == LL_LCD_PRESCALER_32768))

#define IS_LCD_DIVIDER(__DIVIDER__)      (((__DIVIDER__) == LL_LCD_DIVIDER_16) || \
                                          ((__DIVIDER__) == LL_LCD_DIVIDER_17) || \
                                          ((__DIVIDER__) == LL_LCD_DIVIDER_18) || \
                                          ((__DIVIDER__) == LL_LCD_DIVIDER_19) || \
                                          ((__DIVIDER__) == LL_LCD_DIVIDER_20) || \
                                          ((__DIVIDER__) == LL_LCD_DIVIDER_21) || \
                                          ((__DIVIDER__) == LL_LCD_DIVIDER_22) || \
                                          ((__DIVIDER__) == LL_LCD_DIVIDER_23) || \
                                          ((__DIVIDER__) == LL_LCD_DIVIDER_24) || \
                                          ((__DIVIDER__) == LL_LCD_DIVIDER_25) || \
                                          ((__DIVIDER__) == LL_LCD_DIVIDER_26) || \
                                          ((__DIVIDER__) == LL_LCD_DIVIDER_27) || \
                                          ((__DIVIDER__) == LL_LCD_DIVIDER_28) || \
                                          ((__DIVIDER__) == LL_LCD_DIVIDER_29) || \
                                          ((__DIVIDER__) == LL_LCD_DIVIDER_30) || \
                                          ((__DIVIDER__) == LL_LCD_DIVIDER_31))

#define IS_LCD_DUTY(__DUTY__) (((__DUTY__) == LL_LCD_DUTY_STATIC)  || \
                               ((__DUTY__) == LL_LCD_DUTY_1_2)     || \
                               ((__DUTY__) == LL_LCD_DUTY_1_3)     || \
                               ((__DUTY__) == LL_LCD_DUTY_1_4)     || \
                               ((__DUTY__) == LL_LCD_DUTY_1_8))

#define IS_LCD_BIAS(__BIAS__) (((__BIAS__) == LL_LCD_BIAS_1_4) || \
                               ((__BIAS__) == LL_LCD_BIAS_1_2) || \
                               ((__BIAS__) == LL_LCD_BIAS_1_3))

#define IS_LCD_VOLTAGE_SOURCE(SOURCE) (((SOURCE) == LL_LCD_VOLTAGESOURCE_INTERNAL) || \
                                       ((SOURCE) == LL_LCD_VOLTAGESOURCE_EXTERNAL))


#define IS_LCD_PULSE_ON_DURATION(__DURATION__) (((__DURATION__) == LL_LCD_PULSEONDURATION_0) || \
                                                ((__DURATION__) == LL_LCD_PULSEONDURATION_1) || \
                                                ((__DURATION__) == LL_LCD_PULSEONDURATION_2) || \
                                                ((__DURATION__) == LL_LCD_PULSEONDURATION_3) || \
                                                ((__DURATION__) == LL_LCD_PULSEONDURATION_4) || \
                                                ((__DURATION__) == LL_LCD_PULSEONDURATION_5) || \
                                                ((__DURATION__) == LL_LCD_PULSEONDURATION_6) || \
                                                ((__DURATION__) == LL_LCD_PULSEONDURATION_7))

#define IS_LCD_DEAD_TIME(__TIME__) (((__TIME__) == LL_LCD_DEADTIME_0) || \
                                    ((__TIME__) == LL_LCD_DEADTIME_1) || \
                                    ((__TIME__) == LL_LCD_DEADTIME_2) || \
                                    ((__TIME__) == LL_LCD_DEADTIME_3) || \
                                    ((__TIME__) == LL_LCD_DEADTIME_4) || \
                                    ((__TIME__) == LL_LCD_DEADTIME_5) || \
                                    ((__TIME__) == LL_LCD_DEADTIME_6) || \
                                    ((__TIME__) == LL_LCD_DEADTIME_7))

#define IS_LCD_BLINK_MODE(__MODE__) (((__MODE__) == LL_LCD_BLINKMODE_OFF)            || \
                                     ((__MODE__) == LL_LCD_BLINKMODE_SEG0_COM0)      || \
                                     ((__MODE__) == LL_LCD_BLINKMODE_SEG0_ALLCOM)    || \
                                     ((__MODE__) == LL_LCD_BLINKMODE_ALLSEG_ALLCOM))

#define IS_LCD_BLINK_FREQUENCY(__FREQUENCY__) (((__FREQUENCY__) == LL_LCD_BLINKFREQUENCY_DIV8)   || \
                                               ((__FREQUENCY__) == LL_LCD_BLINKFREQUENCY_DIV16)  || \
                                               ((__FREQUENCY__) == LL_LCD_BLINKFREQUENCY_DIV32)  || \
                                               ((__FREQUENCY__) == LL_LCD_BLINKFREQUENCY_DIV64)  || \
                                               ((__FREQUENCY__) == LL_LCD_BLINKFREQUENCY_DIV128) || \
                                               ((__FREQUENCY__) == LL_LCD_BLINKFREQUENCY_DIV256) || \
                                               ((__FREQUENCY__) == LL_LCD_BLINKFREQUENCY_DIV512) || \
                                               ((__FREQUENCY__) == LL_LCD_BLINKFREQUENCY_DIV1024))

#define IS_LCD_CONTRAST(__CONTRAST__) (((__CONTRAST__) == LL_LCD_CONTRASTLEVEL_0) || \
                                       ((__CONTRAST__) == LL_LCD_CONTRASTLEVEL_1) || \
                                       ((__CONTRAST__) == LL_LCD_CONTRASTLEVEL_2) || \
                                       ((__CONTRAST__) == LL_LCD_CONTRASTLEVEL_3) || \
                                       ((__CONTRAST__) == LL_LCD_CONTRASTLEVEL_4) || \
                                       ((__CONTRAST__) == LL_LCD_CONTRASTLEVEL_5) || \
                                       ((__CONTRAST__) == LL_LCD_CONTRASTLEVEL_6) || \
                                       ((__CONTRAST__) == LL_LCD_CONTRASTLEVEL_7))

#define IS_LCD_RAM_REGISTER(__REGISTER__) (((__REGISTER__) == LL_LCD_RAM_REGISTER0)  || \
                                           ((__REGISTER__) == LL_LCD_RAM_REGISTER1)  || \
                                           ((__REGISTER__) == LL_LCD_RAM_REGISTER2)  || \
                                           ((__REGISTER__) == LL_LCD_RAM_REGISTER3)  || \
                                           ((__REGISTER__) == LL_LCD_RAM_REGISTER4)  || \
                                           ((__REGISTER__) == LL_LCD_RAM_REGISTER5)  || \
                                           ((__REGISTER__) == LL_LCD_RAM_REGISTER6)  || \
                                           ((__REGISTER__) == LL_LCD_RAM_REGISTER7))


/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @addtogroup LCD_Private_Functions
  * @{
  */

ErrorStatus     LCD_WaitForSynchro(LCD_TypeDef *hlcd);

/**
  * @}
  */

/**
  * @}
  */

#endif /* LCD */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* RF_DRIVER_LL_LCD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
