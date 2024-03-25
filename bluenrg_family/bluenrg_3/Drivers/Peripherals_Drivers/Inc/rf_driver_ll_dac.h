/**
  ******************************************************************************
  * @file    rf_driver_ll_dac.h
  * @author  RF Application Team
  * @brief   Header file of DAC LL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#ifndef RF_DRIVER_LL_DAC_H
#define RF_DRIVER_LL_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#if defined(CONFIG_DEVICE_SPIRIT3)
#include "spirit3.h"
#endif

/** @addtogroup RF_DRIVER_LL_Driver
  * @{
  */

#if defined(DAC)

/** @defgroup DAC_LL DAC
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
#if defined(USE_FULL_LL_DRIVER)
/** @defgroup GPIO_LL_ES_INIT GPIO Exported Init structures
  * @{
  */

/**
  * @brief LL GPIO Init Structure definition
  */
typedef struct
{
////////  uint32_t Pin;          /*!< Specifies the GPIO pins to be configured.
////////                              This parameter can be any value of @ref GPIO_LL_EC_PIN */
////////
////////  uint32_t Mode;         /*!< Specifies the operating mode for the selected pins.
////////                              This parameter can be a value of @ref GPIO_LL_EC_MODE.
////////
////////                              GPIO HW configuration can be modified afterwards using unitary function @ref LL_GPIO_SetPinMode().*/
////////
////////  uint32_t Speed;        /*!< Specifies the speed for the selected pins.
////////                              This parameter can be a value of @ref GPIO_LL_EC_SPEED.
////////
////////                              GPIO HW configuration can be modified afterwards using unitary function @ref LL_GPIO_SetPinSpeed().*/
uint32_t Speed;
} LL_DAC_InitTypeDef;

/**
  * @}
  */
#endif /* USE_FULL_LL_DRIVER */

/* Exported constants --------------------------------------------------------*/
/** @defgroup DAC_LL_Exported_Constants DAC Exported Constants
  * @{
  */

 /** @defgroup DAC_LL_TSEL Values
  * @{
  */
#define LL_DAC_CR_TSEL_TIM16_TRGO                                               (0U) 
#define LL_DAC_CR_TSEL_PA8                                                      (                                 DAC_CR_TSEL_0  ) 
#define LL_DAC_CR_TSEL_SOFTWARE                                                 ( DAC_CR_TSEL_2 | DAC_CR_TSEL_1 | DAC_CR_TSEL_0  ) 
/**                                                                       
  * @}
  */
 /** @defgroup DAC_LL_WAVE Values
  * @{
  */
#define LL_DAC_CR_WAVE_GENERATION_DISABLED                                      (0U) 
#define LL_DAC_CR_NOISE_WAVE_GENERATION_ENABLED                                 (                 DAC_CR_WAVE_0  ) 
#define LL_DAC_CR_TRIANGLE_WAVE_GENERATION_ENABLED                              ( DAC_CR_WAVE_1                  ) 
/**
  * @}
  */

 /** @defgroup DAC_LL_MAMP Values
  * @{
  */
#define LL_DAC_CR_MAMP_UNMASK_BIT_0                                              (0U) 
#define LL_DAC_CR_MAMP_UNMASK_BITS_1_0                                           (                                                 DAC_CR_MAMP_0  ) 
#define LL_DAC_CR_MAMP_UNMASK_BITS_2_0                                           (                                 DAC_CR_MAMP_1                  ) 
#define LL_DAC_CR_MAMP_UNMASK_BITS_3_0                                           (                                 DAC_CR_MAMP_1 | DAC_CR_MAMP_0  ) 
#define LL_DAC_CR_MAMP_UNMASK_BITS_4_0                                           (                 DAC_CR_MAMP_2                                  ) 
#define LL_DAC_CR_MAMP_UNMASK_BITS_5_0                                           (                 DAC_CR_MAMP_2 |                 DAC_CR_MAMP_0  ) 
/**
  * @}
  */

 /** @defgroup DAC_LL_VCMON Values
  * @{
  */
#define LL_DAC_CR_VCMON_OFF                                                          (0U) 
#define LL_DAC_CR_VCMON_ON                                                          ( DAC_CR_VCMON_0  ) 
/**
  * @}
  */

 /** @defgroup DAC_LL_DMAUDR Values
  * @{
  */
#define LL_DAC_SR_DMAUDR_NO                                                         (0U) 
#define LL_DAC_SR_DMAUDR_YES                                                         ( DAC_SR_DMAUDR_0  ) 
#define LL_DAC_SR_DMAUDR_CLEAR_BIT                                                 ( DAC_SR_DMAUDR_0  ) 
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup DAC_LL_Exported_Macros DAC Exported Macros
  * @{
  */

/** @defgroup DAC_LL_EM_WRITE_READ Common Write and read registers Macros
  * @{
  */

/**
  * @brief  DAC Enable channel enable This bit is set and cleared by software to enable/disable DAC channel.
  * @note   0: DAC channel disabled
  *         1: DAC channel enabled
  * @rmtoll CR          EN            LL_DAC_EnableDacChannel
  * @param  DACx DAC Instance
  * @retval None
  */
__STATIC_INLINE void LL_DAC_EnableDacChannel(DAC_TypeDef *DACx)
{
  SET_BIT(DACx->CR, DAC_CR_EN);
}

/**
  * @brief  DAC Disable channel enable This bit is set and cleared by software to enable/disable DAC channel.
  * @note   0: DAC channel disabled
  *         1: DAC channel enabled
  * @rmtoll CR          EN            LL_DAC_DisableDacChannel
  * @param  DACx DAC Instance
  * @retval None
  */
__STATIC_INLINE void LL_DAC_DisableDacChannel(DAC_TypeDef *DACx)
{ 
  CLEAR_BIT(DACx->CR, DAC_CR_EN);
}

/**
  * @brief  Indicate if EN is enabled. DAC channel enable This bit is set and cleared by software to enable/disable DAC channel.
  * @note   0: DAC channel disabled
  *         1: DAC channel enabled
  * @rmtoll CR          EN            LL_DAC_IsEnabledDacChannel 
  * @param  DACx DAC Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DAC_IsEnabledDacChannel(DAC_TypeDef *DACx)
{
  return ((READ_BIT(DACx->CR, DAC_CR_EN) == (DAC_CR_EN)) ? 1U : 0U);
}

/**
  * @brief  DAC Enable channel output buffer enable.
  * @note   This bit is set and cleared by software to enable/disable DAC channel output buffer.
  *         0: DAC channel output buffer disabled
  *         1: DAC channel output buffer enabled
  * @rmtoll CR          BON            LL_DAC_EnableBon
  * @param  DACx DAC Instance
  * @retval None
  */
__STATIC_INLINE void LL_DAC_EnableBon(DAC_TypeDef *DACx)
{
  SET_BIT(DACx->CR, DAC_CR_BON);
}

/**
  * @brief  DAC Disable channel output buffer enable.
  * @note   This bit is set and cleared by software to enable/disable DAC channel output buffer.
  *         0: DAC channel output buffer disabled
  *         1: DAC channel output buffer enabled
  * @rmtoll CR          BON            LL_DAC_DisableBon
  * @param  DACx DAC Instance
  * @retval None
  */
__STATIC_INLINE void LL_DAC_DisableBon(DAC_TypeDef *DACx)
{ 
  CLEAR_BIT(DACx->CR, DAC_CR_BON);
}

/**
  * @brief  Indicate if BON is enabled. DAC channel output buffer enable.
  * @note   This bit is set and cleared by software to enable/disable DAC channel output buffer.
  *         0: DAC channel output buffer disabled
  *         1: DAC channel output buffer enabled
  * @rmtoll CR          BON            LL_DAC_IsEnabledBon 
  * @param  DACx DAC Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DAC_IsEnabledBon(DAC_TypeDef *DACx)
{
  return ((READ_BIT(DACx->CR, DAC_CR_BON) == (DAC_CR_BON)) ? 1U : 0U);
}

/**
  * @brief  DAC Enable channel trigger enable This bit is set and cleared by software to enable/disable DAC channel trigger.
  * @note   0: DAC channel trigger disabled and data written into the DAC_DHR register are transferred one APB0 clock cycle later to the DAC_DOR register
  *         1: DAC channel trigger enabled and data from the DAC_DHR register are transferred three APB0 clock cycles later to the DAC_DOR register
  *         When software trigger is selected, the transfer from the DAC_DHR register to the DAC_DOR register takes only one APB0 clock cycle.
  * @rmtoll CR          TEN            LL_DAC_EnableTen
  * @param  DACx DAC Instance
  * @retval None
  */
__STATIC_INLINE void LL_DAC_EnableTen(DAC_TypeDef *DACx)
{
  SET_BIT(DACx->CR, DAC_CR_TEN);
}

/**
  * @brief  DAC Disable channel trigger enable This bit is set and cleared by software to enable/disable DAC channel trigger.
  * @note   0: DAC channel trigger disabled and data written into the DAC_DHR register are transferred one APB0 clock cycle later to the DAC_DOR register
  *         1: DAC channel trigger enabled and data from the DAC_DHR register are transferred three APB0 clock cycles later to the DAC_DOR register
  *         When software trigger is selected, the transfer from the DAC_DHR register to the DAC_DOR register takes only one APB0 clock cycle.
  * @rmtoll CR          TEN            LL_DAC_DisableTen
  * @param  DACx DAC Instance
  * @retval None
  */
__STATIC_INLINE void LL_DAC_DisableTen(DAC_TypeDef *DACx)
{ 
  CLEAR_BIT(DACx->CR, DAC_CR_TEN);
}

/**
  * @brief  Indicate if TEN is enabled. DAC channel trigger enable This bit is set and cleared by software to enable/disable DAC channel trigger.
  * @note   0: DAC channel trigger disabled and data written into the DAC_DHR register are transferred one APB0 clock cycle later to the DAC_DOR register
  *         1: DAC channel trigger enabled and data from the DAC_DHR register are transferred three APB0 clock cycles later to the DAC_DOR register
  *         When software trigger is selected, the transfer from the DAC_DHR register to the DAC_DOR register takes only one APB0 clock cycle.
  * @rmtoll CR          TEN            LL_DAC_IsEnabledTen 
  * @param  DACx DAC Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DAC_IsEnabledTen(DAC_TypeDef *DACx)
{
  return ((READ_BIT(DACx->CR, DAC_CR_TEN) == (DAC_CR_TEN)) ? 1U : 0U);
}

/**
  * @brief  Set the DAC channel trigger selection These bits select the external event used to trigger DAC channel.
  * @note   000: Timer 16 TRGO event
  *         001: PA8 pin event from SYSCFG
  *         010 to 011: Reserved
  *         111: Software trigger
  *         Only used if bit TEN = 1 (DAC channel trigger enabled).
  * @rmtoll CR          TSEL       LL_DAC_SetTsel
  * @param  DACx DAC Instance
  * @param  value can be one of the following values:
  * @arg LL_DAC_CR_TSEL_TIM16_TRGO
  * @arg LL_DAC_CR_TSEL_PA8
  * @arg LL_DAC_CR_TSEL_SOFTWARE
  * @retval None
  */
__STATIC_INLINE void LL_DAC_SetTsel(DAC_TypeDef *DACx, uint32_t value)
{
  MODIFY_REG(DACx->CR, DAC_CR_TSEL, value);
}

/**
  * @brief  Get the DAC channel trigger selection These bits select the external event used to trigger DAC channel.
  * @note   000: Timer 16 TRGO event
  *         001: PA8 pin event from SYSCFG
  *         010 to 011: Reserved
  *         111: Software trigger
  *         Only used if bit TEN = 1 (DAC channel trigger enabled).
  * @rmtoll CR          TSEL       LL_DAC_GetTsel
  * @retval can be one of the following values
  * @arg LL_DAC_CR_TSEL_TIM16_TRGO
  * @arg LL_DAC_CR_TSEL_PA8
  * @arg LL_DAC_CR_TSEL_SOFTWARE
  */
__STATIC_INLINE uint32_t LL_DAC_GetTsel(DAC_TypeDef *DACx)
{
  return (uint32_t)(READ_BIT(DACx->CR, DAC_CR_TSEL));
}

/**
  * @brief  Set the DAC channel noise/triangle wave generation enable These bits are set and cleared by software.
  * @note   00: wave generation disabled
  *         01: Noise wave generation enabled
  *         1x: Triangle wave generation enabled
  *         Only used if bit TEN = 1 (DAC channel trigger enabled).
  * @rmtoll CR          WAVE       LL_DAC_SetWave
  * @param  DACx DAC Instance
  * @param  value can be one of the following values:
  * @arg LL_DAC_CR_WAVE_GENERATION_DISABLED
  * @arg LL_DAC_CR_NOISE_WAVE_GENERATION_ENABLED
  * @arg LL_DAC_CR_TRIANGLE_WAVE_GENERATION_ENABLED
  * @retval None
  */
__STATIC_INLINE void LL_DAC_SetWave(DAC_TypeDef *DACx, uint32_t value)
{
  MODIFY_REG(DACx->CR, DAC_CR_WAVE, value);
}

/**
  * @brief  Get the DAC channel noise/triangle wave generation enable These bits are set and cleared by software.
  * @note   00: wave generation disabled
  *         01: Noise wave generation enabled
  *         1x: Triangle wave generation enabled
  *         Only used if bit TEN = 1 (DAC channel trigger enabled).
  * @rmtoll CR          WAVE       LL_DAC_GetWave
  * @retval can be one of the following values
  * @arg LL_DAC_CR_WAVE_GENERATION_DISABLED
  * @arg LL_DAC_CR_NOISE_WAVE_GENERATION_ENABLED
  * @arg LL_DAC_CR_TRIANGLE_WAVE_GENERATION_ENABLED
  */
__STATIC_INLINE uint32_t LL_DAC_GetWave(DAC_TypeDef *DACx)
{
  return (uint32_t)(READ_BIT(DACx->CR, DAC_CR_WAVE));
}

/**
  * @brief  Set the DAC channel mask/amplitude selector These bits are written by software to select mask in wave generation mode or amplitude in triangle generation mode.
  * @note   0000: Unmask bit0 of LFSR triangle amplitude equal to 1
  *         0001: Unmask bits[1:0] of LFSR triangle amplitude equal to 3
  *         0010: Unmask bits[2:0] of LFSR triangle amplitude equal to 7
  *         0011: Unmask bits[3:0] of LFSR triangle amplitude equal to 15
  *         0100: Unmask bits[4:0] of LFSR triangle amplitude equal to 31
  *         >= 0101: Unmask bits[5:0] of LFSR triangle amplitude equal to 63
  * @rmtoll CR          MAMP       LL_DAC_SetMamp
  * @param  DACx DAC Instance
  * @param  value can be one of the following values:
  * @arg LL_DAC_CR_MAMP_UNMASK_BIT_0
  * @arg LL_DAC_CR_MAMP_UNMASK_BITS_1_0
  * @arg LL_DAC_CR_MAMP_UNMASK_BITS_2_0
  * @arg LL_DAC_CR_MAMP_UNMASK_BITS_3_0
  * @arg LL_DAC_CR_MAMP_UNMASK_BITS_4_0
  * @arg LL_DAC_CR_MAMP_UNMASK_BITS_5_0
  * @retval None
  */
__STATIC_INLINE void LL_DAC_SetMamp(DAC_TypeDef *DACx, uint32_t value)
{
  MODIFY_REG(DACx->CR, DAC_CR_MAMP, value);
}

/**
  * @brief  Get the DAC channel mask/amplitude selector These bits are written by software to select mask in wave generation mode or amplitude in triangle generation mode.
  * @note   0000: Unmask bit0 of LFSR triangle amplitude equal to 1
  *         0001: Unmask bits[1:0] of LFSR triangle amplitude equal to 3
  *         0010: Unmask bits[2:0] of LFSR triangle amplitude equal to 7
  *         0011: Unmask bits[3:0] of LFSR triangle amplitude equal to 15
  *         0100: Unmask bits[4:0] of LFSR triangle amplitude equal to 31
  *         >= 0101: Unmask bits[5:0] of LFSR triangle amplitude equal to 63
  * @rmtoll CR          MAMP       LL_DAC_GetMamp
  * @retval can be one of the following values
  * @arg LL_DAC_CR_MAMP_UNMASK_BIT_0
  * @arg LL_DAC_CR_MAMP_UNMASK_BITS_1_0
  * @arg LL_DAC_CR_MAMP_UNMASK_BITS_2_0
  * @arg LL_DAC_CR_MAMP_UNMASK_BITS_3_0
  * @arg LL_DAC_CR_MAMP_UNMASK_BITS_4_0
  * @arg LL_DAC_CR_MAMP_UNMASK_BITS_5_0
  */
__STATIC_INLINE uint32_t LL_DAC_GetMamp(DAC_TypeDef *DACx)
{
  return (uint32_t)(READ_BIT(DACx->CR, DAC_CR_MAMP));
}

/**
  * @brief  DAC Enable channel DMA enable This bit is set and cleared by software.
  * @note   0: DAC channel DMA mode disabled
  *         1: DAC channel DMA mode enabled
  * @rmtoll CR          DMAEN            LL_DAC_EnableDMA
  * @param  DACx DAC Instance
  * @retval None
  */
__STATIC_INLINE void LL_DAC_EnableDMA(DAC_TypeDef *DACx)
{
  SET_BIT(DACx->CR, DAC_CR_DMAEN);
}

/**
  * @brief  DAC Disable channel DMA enable This bit is set and cleared by software.
  * @note   0: DAC channel DMA mode disabled
  *         1: DAC channel DMA mode enabled
  * @rmtoll CR          DMAEN            LL_DAC_DisableDMA
  * @param  DACx DAC Instance
  * @retval None
  */
__STATIC_INLINE void LL_DAC_DisableDMA(DAC_TypeDef *DACx)
{ 
  CLEAR_BIT(DACx->CR, DAC_CR_DMAEN);
}

/**
  * @brief  Indicate if DMAEN is enabled. DAC channel DMA enable This bit is set and cleared by software.
  * @note   0: DAC channel DMA mode disabled
  *         1: DAC channel DMA mode enabled
  * @rmtoll CR          DMAEN            LL_DAC_IsEnabledDMA 
  * @param  DACx DAC Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DAC_IsEnabledDMA(DAC_TypeDef *DACx)
{
  return ((READ_BIT(DACx->CR, DAC_CR_DMAEN) == (DAC_CR_DMAEN)) ? 1U : 0U);
}

/**
  * @brief  DAC Enable channel DMA Underrun Interrupt enable This bit is set and cleared by software.
  * @note   0: DAC channel DMA Underrun Interrupt disabled
  *         1: DAC channel DMA Underrun Interrupt enabled
  * @rmtoll CR          DMAUDRIE            LL_DAC_EnableDMAudrie
  * @param  DACx DAC Instance
  * @retval None
  */
__STATIC_INLINE void LL_DAC_EnableDMAudrie(DAC_TypeDef *DACx)
{
  SET_BIT(DACx->CR, DAC_CR_DMAUDRIE);
}

/**
  * @brief  DAC Disable channel DMA Underrun Interrupt enable This bit is set and cleared by software.
  * @note   0: DAC channel DMA Underrun Interrupt disabled
  *         1: DAC channel DMA Underrun Interrupt enabled
  * @rmtoll CR          DMAUDRIE            LL_DAC_DisableDMAudrie
  * @param  DACx DAC Instance
  * @retval None
  */
__STATIC_INLINE void LL_DAC_DisableDMAudrie(DAC_TypeDef *DACx)
{ 
  CLEAR_BIT(DACx->CR, DAC_CR_DMAUDRIE);
}

/**
  * @brief  Indicate if DMAUDRIE is enabled. DAC channel DMA Underrun Interrupt enable This bit is set and cleared by software.
  * @note   0: DAC channel DMA Underrun Interrupt disabled
  *         1: DAC channel DMA Underrun Interrupt enabled
  * @rmtoll CR          DMAUDRIE            LL_DAC_IsEnabledDMAudrie 
  * @param  DACx DAC Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DAC_IsEnabledDMAudrie(DAC_TypeDef *DACx)
{
  return ((READ_BIT(DACx->CR, DAC_CR_DMAUDRIE) == (DAC_CR_DMAUDRIE)) ? 1U : 0U);
}

/**
  * @brief  DAC Enable channel output to COMP INMINUS enable. This bit is set and cleared by software.
  * @note   0: DAC channel output to COMP INMINUS disabled
  *         1: DAC channel output to COMP INMINUS enabled
  * @rmtoll CR          CMPEN            LL_DAC_EnableComp
  * @param  DACx DAC Instance
  * @retval None
  */
__STATIC_INLINE void LL_DAC_EnableComp(DAC_TypeDef *DACx)
{
  SET_BIT(DACx->CR, DAC_CR_CMPEN);
}

/**
  * @brief  DAC Disable channel output to COMP INMINUS enable. This bit is set and cleared by software.
  * @note   0: DAC channel output to COMP INMINUS disabled
  *         1: DAC channel output to COMP INMINUS enabled
  * @note   True
  * @rmtoll CR          CMPEN            LL_DAC_DisableComp
  * @param  DACx DAC Instance
  * @retval None
  */
__STATIC_INLINE void LL_DAC_DisableComp(DAC_TypeDef *DACx)
{ 
  CLEAR_BIT(DACx->CR, DAC_CR_CMPEN);
}

/**
  * @brief  Indicate if CMPEN is enabled. DAC channel output to COMP INMINUS enable. This bit is set and cleared by software.
  * @note   0: DAC channel output to COMP INMINUS disabled
  *         1: DAC channel output to COMP INMINUS enabled
  * @rmtoll CR          CMPEN            LL_DAC_IsEnabledComp 
  * @param  DACx DAC Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DAC_IsEnabledComp(DAC_TypeDef *DACx)
{
  return ((READ_BIT(DACx->CR, DAC_CR_CMPEN) == (DAC_CR_CMPEN)) ? 1U : 0U);
}

/**
  * @brief  DAC Enable channel output to VCM BUFFER enable. This bit is set and cleared by software.
  * @note   0: DAC channel output to VCM BUFFER disabled
  *         1: DAC channel output to VCM BUFFER enabled
  * @rmtoll CR          VCMEN            LL_DAC_EnableVcmBuffer
  * @param  DACx DAC Instance
  * @retval None
  */
__STATIC_INLINE void LL_DAC_EnableVcmBuffer(DAC_TypeDef *DACx)
{
  SET_BIT(DACx->CR, DAC_CR_VCMEN);
}

/**
  * @brief  DAC Disable channel output to VCM BUFFER enable. This bit is set and cleared by software.
  * @note   0: DAC channel output to VCM BUFFER disabled
  *         1: DAC channel output to VCM BUFFER enabled
  * @rmtoll CR          VCMEN            LL_DAC_DisableVcmBuffer
  * @param  DACx DAC Instance
  * @retval None
  */
__STATIC_INLINE void LL_DAC_DisableVcmBuffer(DAC_TypeDef *DACx)
{ 
  CLEAR_BIT(DACx->CR, DAC_CR_VCMEN);
}

/**
  * @brief  Indicate if VCMEN is enabled. DAC channel output to VCM BUFFER enable. This bit is set and cleared by software.
  * @note   0: DAC channel output to VCM BUFFER disabled
  *         1: DAC channel output to VCM BUFFER enabled
  * @rmtoll CR          VCMEN            LL_DAC_IsEnabledVcmBuffer 
  * @param  DACx DAC Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DAC_IsEnabledVcmBuffer(DAC_TypeDef *DACx)
{
  return ((READ_BIT(DACx->CR, DAC_CR_VCMEN) == (DAC_CR_VCMEN)) ? 1U : 0U);
}

/**
  * @brief  Set the VCMBUFF power-up. This bit is set and cleared by software.
  * @note   0: VCM BUFFER OFF
  *         1: VCM BUFFER ON
  * @rmtoll CR          VCMON       LL_DAC_SetVcmon
  * @param  DACx DAC Instance
  * @param  value can be one of the following values:
  * @arg LL_DAC_CR_VCMON_OFF
  * @arg LL_DAC_CR_VCMON_ON
  * @retval None
  */
__STATIC_INLINE void LL_DAC_SetVcmon(DAC_TypeDef *DACx, uint32_t value)
{
  MODIFY_REG(DACx->CR, DAC_CR_VCMON, value);
}

/**
  * @brief  Get the VCMBUFF power-up. This bit is set and cleared by software.
  * @note   0: VCM BUFFER OFF
  *         1: VCM BUFFER ON
  * @rmtoll CR          VCMON       LL_DAC_GetVcmon
  * @retval can be one of the following values
  * @arg LL_DAC_CR_VCMON_OFF
  * @arg LL_DAC_CR_VCMON_ON
  */
__STATIC_INLINE uint32_t LL_DAC_GetVcmon(DAC_TypeDef *DACx)
{
  return (uint32_t)(READ_BIT(DACx->CR, DAC_CR_VCMON));
}

/**
  * @brief  DAC Enable channel software trigger This bit is set by software to enable/disable the software trigger.
  * @note   0: Software trigger disabled
  *         1: Software trigger enabled
  *         This bit is cleared by hardware (one APB0 clock cycle later) once the
  *         DAC_DHR register value has been loaded into the DAC_DOR register.
  * @rmtoll SWTRIGR          SWTRIG            LL_DAC_EnableSoftwareTrigger
  * @param  DACx DAC Instance
  * @retval None
  */
__STATIC_INLINE void LL_DAC_EnableSoftwareTrigger(DAC_TypeDef *DACx)
{
  SET_BIT(DACx->SWTRIGR, DAC_SWTRIGR_SWTRIG);
}

/**
  * @brief  DAC Disable channel software trigger This bit is set by software to enable/disable the software trigger.
  * @note   0: Software trigger disabled
  *         1: Software trigger enabled
  *         This bit is cleared by hardware (one APB0 clock cycle later) once the
  *         DAC_DHR register value has been loaded into the DAC_DOR register.
  * @rmtoll SWTRIGR          SWTRIG            LL_DAC_DisableSoftwareTrigger
  * @param  DACx DAC Instance
  * @retval None
  */
__STATIC_INLINE void LL_DAC_DisableSoftwareTrigger(DAC_TypeDef *DACx)
{ 
  CLEAR_BIT(DACx->SWTRIGR, DAC_SWTRIGR_SWTRIG);
}

/**
  * @brief  Set the DAC channel 6-bit data 
  * @note   These bits are written by software which specifies 6-bit data for DAC channel.
  * @rmtoll DHR          DACDHR       LL_DAC_SetDacDHR
  * @param  DACx DAC Instance
  * @param  value parameter must be a number between 0 and 63 (form 0x0 to 0x0x3f)
  * @retval None
  */
__STATIC_INLINE void LL_DAC_SetDacDHR(DAC_TypeDef *DACx, uint32_t value)
{
  MODIFY_REG(DACx->DHR, DAC_DHR_DACDHR, value);
}

/**
  * @brief  Get the DAC channel 6-bit data 
  * @note   These bits are written by software which specifies 6-bit data for DAC channel.
  * @rmtoll DHR          DACDHR       LL_DAC_GetDacDHR
  * @retval the DAC channel 6-bit data 
  */
__STATIC_INLINE uint32_t LL_DAC_GetDacDHR(DAC_TypeDef *DACx)
{
  return (uint32_t)(READ_BIT(DACx->DHR, DAC_DHR_DACDHR));
}

/**
  * @brief  Get the DAC channel data output 
  * @note   These bits are read-only, they contain data output for DAC channel.
  * @rmtoll DOR          DACDOR       LL_DAC_GetDacdor
  * @retval data output for DAC channel
  */
__STATIC_INLINE uint32_t LL_DAC_GetDacdor(DAC_TypeDef *DACx)
{
  return (uint32_t)(READ_BIT(DACx->DOR, DAC_DOR_DACDOR));
}

/**
  * @brief  Set the DAC channel DMA underrun flag This bit is set by hardware and cleared by software (by writing it to 1).
  * @note   0: No DMA underrun error condition occurred for DAC channel
  *         1: DMA underrun error condition occurred for DAC channel (the currently selected trigger is driving DAC channel conversion at a frequency higher than the DMA service capability rate)
  * @rmtoll SR          DMAUDR       LL_DAC_SetDmaudr
  * @param  DACx DAC Instance
  * @param  value can be the following value:
  * @arg LL_DAC_SR_DMAUDR_CLEAR_BIT
  * @retval None
  */
__STATIC_INLINE void LL_DAC_SetDmaudr(DAC_TypeDef *DACx, uint32_t value)
{
  MODIFY_REG(DACx->SR, DAC_SR_DMAUDR, value);
}

/**
  * @brief  Get the DAC channel DMA underrun flag This bit is set by hardware and cleared by software (by writing it to 1).
  * @note   0: No DMA underrun error condition occurred for DAC channel
  *         1: DMA underrun error condition occurred for DAC channel (the currently selected trigger is driving DAC channel conversion at a frequency higher than the DMA service capability rate)
  * @rmtoll SR          DMAUDR       LL_DAC_GetDmaudr
  * @retval can be one of the following values
  * @arg LL_DAC_SR_DMAUDR_NO
  * @arg LL_DAC_SR_DMAUDR_YES
  */
__STATIC_INLINE uint32_t LL_DAC_GetDmaudr(DAC_TypeDef *DACx)
{
  return (uint32_t)(READ_BIT(DACx->SR, DAC_SR_DMAUDR));
}

/**
  * @}
  */


/* Exported functions --------------------------------------------------------*/
/** @defgroup DAC_LL_Exported_Functions DAC Exported Functions
  * @{
  */


#if defined(USE_FULL_LL_DRIVER)
/** @defgroup DAC_LL_EF_Init Initialization and de-initialization functions
  * @{
  */

ErrorStatus LL_DAC_Init(DAC_TypeDef *DACx);
ErrorStatus LL_DAC_DeInit(DAC_TypeDef *DACx);

/**
  * @}
  */
#endif /* USE_FULL_LL_DRIVER */

/**
  * @}
  */

/**
  * @}
  */

#endif /* defined(DAC) */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* RF_DRIVER_LL_DAC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
