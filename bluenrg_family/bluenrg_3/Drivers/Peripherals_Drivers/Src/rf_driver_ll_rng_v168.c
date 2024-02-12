/**
  ******************************************************************************
  * @file    rf_driver_ll_rng_v168.c
  * @author  RF Application Team
  * @brief   RNG LL module driver.
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
#if defined(USE_FULL_LL_DRIVER)

/* Includes ------------------------------------------------------------------*/
#include "rf_driver_ll_rng_v168.h"
#include "rf_driver_ll_bus.h"

#ifdef  USE_FULL_ASSERT
#include "rf_driver_assert.h"
#else
#define assert_param(expr) ((void)0U)
#endif

/** @addtogroup RF_DRIVER_LL_Driver
  * @{
  */

#if defined (RNG)

/** @addtogroup RNG_LL
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup RNG_LL_Exported_Functions
  * @{
  */

/** @addtogroup RNG_LL_EF_Init
  * @{
  */

/**
  * @brief  De-initialize RNG registers (Registers restored to their default values).
  * @param  RNGx RNG Instance
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: RNG registers are de-initialized
  *          - ERROR: not applicable
  */
ErrorStatus LL_RNG_DeInit(RNG_TypeDef *RNGx)
{
  /* Check the parameters */
  assert_param(IS_RNG_ALL_INSTANCE(RNGx));

  /* Enable RNG reset state */
  LL_AHB_ForceReset(LL_AHB_PERIPH_RNG);

  /* Release RNG from reset state */
  LL_AHB_ReleaseReset(LL_AHB_PERIPH_RNG);
  return (SUCCESS);
}

/**
  * @brief  Initialize RNG registers according to the specified parameters in RNG_InitStruct.
  * @param  RNGx RNG Instance
  * @param  RNG_InitStruct pointer to a LL_RNG_InitTypeDef structure
  *         that contains the configuration information for the specified RNG peripheral.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: RNG registers are initialized according to RNG_InitStruct content
  *          - ERROR: not applicable
  */
ErrorStatus LL_RNG_Init(RNG_TypeDef *RNGx, LL_RNG_InitTypeDef *RNG_InitStruct)
{
 
  /* Check that the divider value is equal to the one to configure. If not, it must redo the write and checking. */
  while( LL_RNG_GetSamplingClockEnableDivider(RNGx) != RNG_InitStruct->SamplingClockDivider)
  {
     /* While the new divider value is being resynchronized with the TRNG core clock domain, it is not possible to write another new value. */
     LL_RNG_SetSamplingClockEnableDivider(RNGx, RNG_InitStruct->SamplingClockDivider);
  }
  
  
  return (SUCCESS);
}

/**
  * @brief Set each @ref LL_RNG_InitTypeDef field to default value.
  * @param RNG_InitStruct pointer to a @ref LL_RNG_InitTypeDef structure
  *                       whose fields will be set to default values.
  * @retval None
  */
void LL_RNG_StructInit(LL_RNG_InitTypeDef *RNG_InitStruct)
{
    RNG_InitStruct->SamplingClockDivider = 0;

}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* RNG */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

