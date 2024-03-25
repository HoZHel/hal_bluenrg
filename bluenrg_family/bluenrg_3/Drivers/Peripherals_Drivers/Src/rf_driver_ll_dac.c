/**
  ******************************************************************************
  * @file    rf_driver_ll_dac.c
  * @author  RF Application Team
  * @brief   DAC LL module driver.
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
#if defined(USE_FULL_LL_DRIVER)

/* Includes ------------------------------------------------------------------*/
#include "rf_driver_ll_dac.h"
#include "rf_driver_ll_bus.h"

#ifdef  USE_FULL_ASSERT
#include "rf_driver_assert.h"
#else
#define assert_param(expr) ((void)0U)
#endif

/** @addtogroup RF_DRIVER_LL_Driver
  * @{
  */

#if defined (DAC)

/** @addtogroup DAC_LL
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup DAC_LL_Exported_Functions
  * @{
  */

/** @addtogroup DAC_LL_EF_Init
  * @{
  */

/**
  * @brief  Initialize DAC registers (Registers restored to their default values).
  * @param  DACx DAC Instance
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: DAC registers are initialized
  *          - ERROR: DAC registers are not initialized
  */
ErrorStatus LL_DAC_Init(DAC_TypeDef *DACx)
{
  ErrorStatus status = SUCCESS;


  return (status);
}
/**
  * @brief  De-initialize DAC registers (Registers restored to their default values).
  * @param  DACx DAC Instance
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: DAC registers are de-initialized
  *          - ERROR: DAC registers are not de-initialized
  */
ErrorStatus LL_DAC_DeInit(DAC_TypeDef *DACx)
{
  ErrorStatus status = SUCCESS;


  return (status);
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

#endif /* defined (DAC) */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

