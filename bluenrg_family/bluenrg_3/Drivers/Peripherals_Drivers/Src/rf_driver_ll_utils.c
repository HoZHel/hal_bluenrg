/**
  ******************************************************************************
  * @file    rf_driver_ll_utils.c
  * @author  RF Application Team
  * @brief   UTILS LL module driver.
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
/* Includes ------------------------------------------------------------------*/
#include "rf_driver_ll_utils.h"
#include "rf_driver_ll_rcc.h"
#include "rf_driver_ll_system.h"
#include "rf_driver_ll_pwr.h"
#if defined(CONFIG_DEVICE_BLUENRG_LP) || defined(CONFIG_DEVICE_BLUENRG_LPS) || defined(CONFIG_DEVICE_BLUENRG_LPF)
#include "system_BlueNRG_LP.h"
#endif
#if defined(CONFIG_DEVICE_SPIRIT3)
#include "system_spirit3.h"
#endif


#ifdef  USE_FULL_ASSERT
#include "rf_driver_assert.h"
#else
#define assert_param(expr) ((void)0U)
#endif

/** @addtogroup RF_DRIVER_LL_Driver
  * @{
  */

/** @addtogroup UTILS_LL
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @addtogroup UTILS_LL_Private_Constants
  * @{
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @addtogroup UTILS_LL_Exported_Functions
  * @{
  */

/** @addtogroup UTILS_LL_EF_DELAY
  * @{
  */

/**
  * @brief  This function configures the Cortex-M SysTick source to have 1ms time base.
  * @note   When a RTOS is used, it is recommended to avoid changing the Systick
  *         configuration by calling this function, for a delay use rather osDelay RTOS service.
  * @param  HCLKFrequency HCLK frequency in Hz
  * @note   HCLK frequency can be calculated thanks to RCC helper macro or function @ref LL_RCC_GetSystemClocksFreq (HCLK2_Frequency field)
  * @retval None
  */
void LL_Init1msTick(uint32_t HCLKFrequency)
{
  /* Use frequency provided in argument */
  LL_InitTick(HCLKFrequency, 1000);
}


/**
  * @brief  This function provides accurate delay (in milliseconds) based
  *         on SysTick counter flag
  * @note   When a RTOS is used, it is recommended to avoid using blocking delay
  *         and use rather osDelay service.
  * @note   To respect 1ms timebase, user should call @ref LL_Init1msTick function which
  *         will configure Systick to 1ms
  * @param  Delay specifies the delay time length, in milliseconds.
  * @retval None
  */
void LL_mDelay(uint32_t Delay)
{
  uint32_t mDelay = Delay;
  __IO uint32_t  tmp = SysTick->CTRL;  /* Clear the COUNTFLAG first */
  /* Add this code to indicate that local variable is not used */
  ((void)tmp);

  /* Add a period to guaranty minimum wait */
  if (mDelay < LL_MAX_DELAY)
  {
    mDelay++;
  }

  while (mDelay != 0U)
  {
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
      mDelay--;
    }
  }
}

/**
  * @}
  */

/**
  * @brief  This function sets the System Core Clock.
  * @param  SysClk system clock divided factor from HSI64MPLL
  *         This parameter can be one of the following values:
  *         SYSCLK_64M
  *         SYSCLK_32M
  *         SYSCLK_16M
  *         SYSCLK_8M
  *         SYSCLK_4M
  *         SYSCLK_2M
  *         SYSCLK_1M
  *         SYSCLK_DIRECT_HSE
  *         SYSCLK_DIRECT_HSE_DIV2
  * @retval SUCCESS or error code
  * @note The SYSCLK_DIRECT_HSE_DIV2 is valid only for SPIRIT3
  */
uint32_t LL_SetSystemCoreClock(uint8_t SysClk)
{
  return SystemClockConfig(SysClk);
}

/**
  * @brief  This function return the System Core Clock expressed in Hz.
  * @retval System Core Clock frequency.
  */
uint32_t LL_GetSystemCoreClock(void)
{ 
  return SystemCoreClock;
}


#if defined(CONFIG_DEVICE_SPIRIT3)
/**
  * @brief This function set the XTAL frequency expressed in Hz.
  * @param freq XTAL frequency expressed in Hz.
  */
void LL_SetXTALFreq(uint32_t freq)
{
  HSE_xtalFrequency = freq;

  switch(LL_RCC_GetRC64MPLLPrescaler())
  {
  case LL_RCC_RC64MPLL_DIV_1:
    if (LL_RCC_DIRECT_HSE_IsEnabled())
      SystemCoreClock = HSE_xtalFrequency;
    else
      SystemCoreClock = (HSE_xtalFrequency/3)*4;
    break;
  case LL_RCC_RC64MPLL_DIV_2:
    if (LL_RCC_DIRECT_HSE_IsEnabled())
      SystemCoreClock = HSE_xtalFrequency/2;
    else
      SystemCoreClock = (HSE_xtalFrequency/3)*2;
    break;
  case LL_RCC_RC64MPLL_DIV_4:
    SystemCoreClock = HSE_xtalFrequency/3;
    break;
  case LL_RCC_RC64MPLL_DIV_8:
    SystemCoreClock = HSE_xtalFrequency/6;
    break;
  case LL_RCC_RC64MPLL_DIV_16:
    SystemCoreClock = HSE_xtalFrequency/12;
    break;
  case LL_RCC_RC64MPLL_DIV_32:
    SystemCoreClock = HSE_xtalFrequency/24;
    break;
   case LL_RCC_RC64MPLL_DIV_64:
     SystemCoreClock = HSE_xtalFrequency/48;
     break;
  }
}

/**
  * @brief  This function return the System Core Clock expressed in Hz.
  * @retval System Core Clock frequency.
  */
uint32_t LL_GetXTALFreq(void)
{
  return HSE_xtalFrequency;
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
