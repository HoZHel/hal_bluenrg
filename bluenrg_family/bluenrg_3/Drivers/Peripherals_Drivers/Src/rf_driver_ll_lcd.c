/**
  ******************************************************************************
  * @file    rf_driver_ll_lcd.c
  * @author  RF Application Team
  * @brief   LCD Controller LL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the LCD Controller (LCD) peripheral:
  *           + Initialization/de-initialization methods
  *           + I/O operation methods
  *           + Peripheral State methods
  *
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
      [..] The LCD LL driver can be used as follows:

      (#) Declare a LCD_TypeDef handle structure.

      -@- The frame frequency must be selected to be within a range of around ~30 Hz to ~100 Hz
          and is a compromise between power consumption and the acceptable refresh rate.

      -@- When the display is enabled, the update is performed only for locations
          for which commons are active (depending on DUTY). For example if
          DUTY = 1/2, only the LCD_DISPLAY of COM0 and COM1 will be updated.
                  DUTY     Capability     Function
                  1/8      12x8           COM[0:7] SEG[0:11]
                  1/4      16x4           COM[0:3] SEG[0:15]
                  1/3      16x3           COM[0:2] SEG[0:15]
                  1/2      16x2           COM[0:1] SEG[0:15]
                  STATIC   16x1           COM[0]   SEG[0:15]
      -@- Follow table gives examples of different blink frequencies (as a function of ck_div frequency).
                                          ck_div frequency (with LCDCLK frequency of 32.768 kHz)
                  BLINKF[2:0]                   32Hz     64Hz     128Hz    256Hz
          LL_LCD_BLINKFREQUENCY_DIV8            4.0  Hz  N/A      N/A      N/A      
          LL_LCD_BLINKFREQUENCY_DIV16           2.0  Hz  4.0  Hz  N/A      N/A
          LL_LCD_BLINKFREQUENCY_DIV32           1.0  Hz  2.0  Hz  4.0  Hz  N/A
          LL_LCD_BLINKFREQUENCY_DIV64           0.5  Hz  1.0  Hz  2.0  Hz  4.0  Hz
          LL_LCD_BLINKFREQUENCY_DIV128          0.25 Hz  0.5  Hz  1.0  Hz  2.0  Hz
          LL_LCD_BLINKFREQUENCY_DIV256          N/A      0.25 Hz  0.5  Hz  1.0  Hz
          LL_LCD_BLINKFREQUENCY_DIV512          N/A      N/A      0.25 Hz  0.5  Hz
          LL_LCD_BLINKFREQUENCY_DIV1024         N/A      N/A      N/A      0.25 Hz

      (#) Initialize the LCD low level resources API:

          (++) Enable the LCDCLK 
		  
          (++) LCD pins configuration:
              (+++) Enable the clock for the LCD GPIOs.
              (+++) Configure these LCD pins as alternate function no-pull.
			  
          (++) Enable the LCD interface clock.

      (#) Program the Prescaler, Divider, Blink mode, Blink Frequency Duty, Bias,
          Voltage Source, Dead Time, Pulse On Duration, Contrast and High drive in the Init structure 
          of the LCD handle.

      (#) Initialize the LCD registers by calling the LL_LCD_Init() API.

      (#) Optionally you can update the LCD configuration using these function:
              (++) Enable, disable or check the voltage output buffer for higher driving capability LL_LCD_EnableVoltageOutputBuffer(), LL_LCD_DisableVoltageOutputBuffer(), LL_LCD_IsEnabledVoltageOutputBuffer()
              (++) Set or get the Bias selector LL_LCD_SetBias() and () LL_LCD_GetBias()
              (++) Set or get the Duty selection LL_LCD_SetDuty() and () LL_LCD_GetDuty()
              (++) Set the Voltage source LL_LCD_SetVoltageSource() and LL_LCD_GetVoltageSource()
              (++) Enable, disable or check Enable LCD peripheral LL_LCD_Enable(), LL_LCD_Disable() and LL_LCD_IsEnabled()
              (++) Set or get 16-bit prescaler LL_LCD_SetPrescaler() and LL_LCD_GetPrescaler()
              (++) Set or get the DIV clock divider LL_LCD_SetClockDivider() and LL_LCD_GetClockDivider()
              (++) Set or get the Blink mode selection LL_LCD_SetBlinkMode() and LL_LCD_GetBlinkMode()
              (++) Set or get the Blink frequency selection LL_LCD_SetBlinkFrequency() and LL_LCD_GetBlinkFrequencyand()
              (++) Set or get the Contrast control LL_LCD_SetContrastControl() and LL_LCD_GetContrastControl()
              (++) Set or get the Dead time duration LL_LCD_SetDeadTimeDuration() and LL_LCD_GetDeadTimeDuration()
              (++) Set or get the Pulse duration LL_LCD_SetPulseONDuration() and LL_LCD_GetPulseONDuration()
              (++) Enable, disable or check Update display done interrupt LL_LCD_EnableUpdateDisplayDoneInterrupt(), LL_LCD_DisableUpdateDisplayDoneInterrupt() and LL_LCD_IsEnabledUpdateDisplayDoneInterrupt()
              (++) Enable, disable or check Start of frame interrupt LL_LCD_EnableStartOfFrameInterrupt(), LL_LCD_DisableStartOfFrameInterrupt() and LL_LCD_IsEnabledStartOfFrameInterrupt()
              (++) Set or get the High Drive LL_LCD_SetHighDrive() and LL_LCD_GetHighDrive()
              (++) Indicate if FCRSF is enabled  LL_LCD_IsActiveFlag_FCRSF()
              (++) Indicate if RDY is enabled  LL_LCD_IsActiveFlag_RDY()
              (++) Indicate if UDD is enabled  LL_LCD_IsActiveFlag_UDD()
              (++) Indicate if SOF is enabled  LL_LCD_IsActiveFlag_SOF()
              (++) Indicate if ENS is enabled  LL_LCD_IsActiveFlag_ENS()
              (++) Enable or check  LL_LCD_EnableUpdateDisplayRequest() and LL_LCD_IsActiveFlag_UDR()
              (++) Clear the UDD flag  LL_LCD_ClearFlag_UDDC()
              (++) Clear the SOFC flag  LL_LCD_ClearFlag_SOFC()

      (#) Write to the LCD RAM memory using the LL_LCD_SetRAM() API, this API can be called
          more time to update the different LCD RAM registers before calling
          LL_LCD_UpdateDisplayRequest() API.

      (#) When LCD RAM memory is updated enable the update display request using
          the LL_LCD_UpdateDisplayRequest() API.

      [..] LCD and low power modes:  //todo mm1mm do a test and check this lines.
           (#) The LCD remain active during Sleep, Low Power run, Low Power Sleep and
               STOP modes.

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rf_driver_ll_lcd.h"

/** @addtogroup RF_Driver_LL_Driver
* @{
*/

#if defined(USE_FULL_LL_DRIVER)

#if defined (LCD)

/** @defgroup LCD LCD
* @brief LCD LL module driver
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup LCD_Private_Defines LCD Private Defines
* @{
*/

/**
* @}
*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup LCD_Exported_Functions LCD Exported Functions
* @{
*/

/** @defgroup LCD_Exported_Functions_Group1 Initialization/de-initialization methods
*  @brief    Initialization and Configuration functions
*
@verbatim
===============================================================================
##### Initialization and Configuration functions #####
===============================================================================
[..]

@endverbatim
* @{
*/

/**
* @brief  Initialize the LCD peripheral according to the specified parameters
*         in the LCD_InitTypeDef and initialize the associated handle.
* @note   This function can be used only when the LCD is disabled.
* @param  LCDx LCD handle
* @param  hlcd LCD init structure handle
* @retval None
*/
ErrorStatus LL_LCD_Init(LCD_TypeDef *LCDx, LCD_InitTypeDef *hlcd)
{
  /* Set the Voltage source */
  LL_LCD_SetVoltageSource(LCDx, hlcd->VoltageSource);
  
  /* Wait for the external capacitor CEXT to be charged (approximately 2 ms for CEXT = 1 uF) */
  //LL_mDelay(2);
  
  /* Program desired DUTY in LCD_CR */
  LL_LCD_SetDuty(LCDx, hlcd->Duty);
  
  /* Program desired BIAS in LCD_CR */
  LL_LCD_SetBias(LCDx, hlcd->Bias);
  
  /* set the UDR bit in the LCD_SR register */
  LL_LCD_EnableUpdateDisplayRequest(LCDx);
  
  /* Program the desired frame rate (PS and DIV bits in LCD_FCR) */
  LL_LCD_SetPrescaler(LCDx, hlcd->Prescaler);
  LL_LCD_SetClockDivider(LCDx, hlcd->Divider);
  
  /* Program the contrast (CC bits in LCD_FCR register) */
  LL_LCD_SetContrastControl(LCDx, hlcd->Contrast);
  
  /* Program optional features (BLINK, BLINKF, PON, DEAD, HD) */
  LL_LCD_SetBlinkMode(LCDx, hlcd->BlinkMode);
  LL_LCD_SetBlinkFrequency(LCDx, hlcd->BlinkFrequency);
  LL_LCD_SetDeadTimeDuration(LCDx, hlcd->DeadTime);
  LL_LCD_SetPulseONDuration(LCDx, hlcd->PulseOnDuration);
  LL_LCD_SetHighDrive(LCDx, hlcd->HighDrive);
  
  /* Enable the display (LCDEN bit in LCD_CR register) */
  LL_LCD_Enable(LCDx);
  
  // Wait until the LCD FCR register is synchronized
  while(!LL_LCD_IsActiveFlag_FCRSF(LCDx));
  
  // Wait until the LCD status is ready
  while(!LL_LCD_IsActiveFlag_ENS(LCDx));
  
  // Wait until the LCD BOOSTER is ready
  while(!LL_LCD_IsActiveFlag_RDY(LCDx));
    
  return SUCCESS;
}

/**
* @brief  DeInitialize the LCD peripheral.
* @param  LCDx LCD handle
* @retval LL status
*/
ErrorStatus LL_LCD_DeInit(LCD_TypeDef *LCDx)
{
  /* Disable the display (LCDEN bit in LCD_CR register) */
  LL_LCD_Disable(LCDx);
  return SUCCESS;
}


/**
* @brief Set each @ref LCD_InitTypeDef field to default value.
* @param hlcd LCD_InitTypeDef pointer to a @ref LCD_InitTypeDef structure
*        whose fields will be set to default values.
* @retval None
*/
WEAK_FUNCTION(void LL_LCD_StructInit(LCD_InitTypeDef *hlcd))
{
  hlcd->Prescaler = LL_LCD_PRESCALER_1;
  hlcd->Divider = LL_LCD_DIVIDER_16;
  hlcd->Duty = LL_LCD_DUTY_STATIC;
  hlcd->Bias = LL_LCD_BIAS_1_4;
  hlcd->VoltageSource = LL_LCD_VOLTAGESOURCE_INTERNAL;
  hlcd->Contrast = LL_LCD_CONTRASTLEVEL_0;
  hlcd->DeadTime = LL_LCD_DEADTIME_0;
  hlcd->PulseOnDuration = LL_LCD_PULSEONDURATION_0;
  hlcd->HighDrive = LL_LCD_HIGHDRIVE_DISABLE;
  hlcd->BlinkMode = LL_LCD_BLINKMODE_OFF;
  hlcd->BlinkFrequency = LL_LCD_BLINKFREQUENCY_DIV8;
}

/**
* @}
*/

/** @defgroup LCD_Exported_Functions_Group2 IO operation methods
*  @brief LCD RAM functions
* @endverbatim
* @{
*/

/**
* @brief Write a word in the specific LCD RAM.
* @note  Refer to the User Manual for all COMx/SEGx specifications into 
*        the section Remapping capability.
*        Remapping capability
*        DUTY     Capability     Function
*        1/8      12x8           COM[0:7] SEG[0:11]
*        1/4      16x4           COM[0:3] SEG[0:15]
*        1/3      16x3           COM[0:2] SEG[0:15]
*        1/2      16x2           COM[0:1] SEG[0:15]
*        STATIC   16x1           COM[0]   SEG[0:15]
* @param LCD_COMx specifies the LCD RAM Register.
*   This parameter can be one of the following values:
*     @arg LCD->RAM_COM0
*     @arg LCD->RAM_COM1
*     @arg LCD->RAM_COM2
*     @arg LCD->RAM_COM3
*     @arg LCD->RAM_COM4
*     @arg LCD->RAM_COM5
*     @arg LCD->RAM_COM6
*     @arg LCD->RAM_COM7
* @param SegmentIndex specifies LCD Data Segment to be written.
*   This parameter can be one of the following values:
*     @arg LL_LCD_RAM_SEGMENT_0 
*     @arg LL_LCD_RAM_SEGMENT_1 
*     @arg LL_LCD_RAM_SEGMENT_2 
*     @arg LL_LCD_RAM_SEGMENT_3 
*     @arg LL_LCD_RAM_SEGMENT_4 
*     @arg LL_LCD_RAM_SEGMENT_5 
*     @arg LL_LCD_RAM_SEGMENT_6 
*     @arg LL_LCD_RAM_SEGMENT_7 
*     @arg LL_LCD_RAM_SEGMENT_8 
*     @arg LL_LCD_RAM_SEGMENT_9 
*     @arg LL_LCD_RAM_SEGMENT_10
*     @arg LL_LCD_RAM_SEGMENT_11
*     @arg LL_LCD_RAM_SEGMENT_12
*     @arg LL_LCD_RAM_SEGMENT_13
*     @arg LL_LCD_RAM_SEGMENT_14
*     @arg LL_LCD_RAM_SEGMENT_15
* @param value specifies LCD Data Segment to be written.
*   This parameter can be one of the following values:
*     @arg value LL_LCD_DISABLE_SEGMENT
*     @arg value LL_LCD_ENABLE_SEGMENT
* @retval None
*/
void LL_LCD_SetRAM(volatile uint32_t *LCD_COMx, uint32_t SegmentIndex, uint32_t value)
{
    if(value == LL_LCD_ENABLE_SEGMENT)
    {
      SET_BIT(*LCD_COMx, SegmentIndex);
    }
    else
    {
      CLEAR_BIT(*LCD_COMx, SegmentIndex);
    }
}


/**
* @brief  Enable the Update Display Request.
* @param  LCDx LCD handle
* @note   Each time software modifies the LCD_RAM it must set the UDR bit to
*         transfer the updated data to the second level buffer.
*         The UDR bit stays set until the end of the update and during this
*         time the LCD_RAM is write protected.
* @note   When the display is disabled, the update is performed for all
*         LCD_DISPLAY locations.
*         When the display is enabled, the update is performed only for locations
*         for which commons are active (depending on DUTY). For example if
*         DUTY = 1/2, only the LCD_DISPLAY of COM0 and COM1 will be updated.
* @note   Refer to the User Manual for all COMx/SEGx specifications into 
*         the section Remapping capability.
*         Remapping capability
*         DUTY     Capability     Function
*         1/8      12x8           COM[0:7] SEG[0:11]
*         1/4      16x4           COM[0:3] SEG[0:15]
*         1/3      16x3           COM[0:2] SEG[0:15]
*         1/2      16x2           COM[0:1] SEG[0:15]
*         STATIC   16x1           COM[0]   SEG[0:15]
* @retval None
*/
void LL_LCD_UpdateDisplayRequest(LCD_TypeDef *LCDx)
{
  /* Clear the Update Display Done flag before starting the update display request */
  LL_LCD_ClearFlag_UDDC(LCDx);
  
  LL_LCD_EnableUpdateDisplayRequest(LCDx);
  
  LCD_WaitForSynchro(LCDx);
}

/**
* @}
*/

/** @defgroup LCD_Exported_Functions_Group3 Peripheral State methods
*  @brief   LCD State functions
*
@verbatim
===============================================================================
##### Peripheral State functions #####
===============================================================================


/**
* @}
*/

/** @defgroup LCD_Private_Functions LCD Private Functions
* @{
*/

/**
* @brief  Wait until the LCD FCR register is synchronized in the LCDCLK domain.
*         This function must be called after any write operation to LCD_FCR register.
* @param  LCDx LCD handle
* @retval None
*/
ErrorStatus LCD_WaitForSynchro(LCD_TypeDef *LCDx)
{
  while(LL_LCD_IsActiveFlag_UDR(LCDx));
  
  return SUCCESS;
}



/**
* @}
*/

/**
* @}
*/

#endif /* LCD */

#endif /* USE_FULL_LL_DRIVER */

/**
* @}
*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
