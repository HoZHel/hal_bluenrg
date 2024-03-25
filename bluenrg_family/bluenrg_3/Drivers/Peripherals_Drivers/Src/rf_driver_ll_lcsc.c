/**
  ******************************************************************************
  * @file    rf_driver_ll_lcsc.c
  * @author  RF Application Team
  * @brief   LCSC LL module driver.
*          This file provides firmware functions to manage the following
  *          functionalities of the LCSC peripheral:
  *           + I/O operation methods
  *           + Peripheral State methods
  *
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
      
      (#) The LCSC configuration can be updated using these function:
              (++) Set or get the Measurement Time LL_SetMeasurementTime() and LL_LCSC_GetMeasurementTime()
              (++) Set or get the Capture Time LL_SetCaptureTime() and LL_LCSC_GetCaptureTime()
              (++) Set or get the Inter Capture Time LL_SetInterCaptureTime() and LL_LCSC_GetInterCaptureTime()
              (++) Set or get the Damping Threshold For Lca And Lcb LL_SetDampingThresholdForLcaAndLcb() and LL_LCSC_GetDampingThresholdForLcaAndLcb()
              (++) Set or get the Vcmbuff Recovery Time LL_SetVcmbuffRecoveryTime() and LL_LCSC_GetVcmbuffRecoveryTime()
              (++) Set or get the Vcmbuff Starting Time LL_SetVcmbuffStartingTime() and LL_LCSC_GetVcmbuffStartingTime()
              (++) Set or get the Tamper Measurement Interval LL_SetTamperMeasurementInterval() and LL_LCSC_GetTamperMeasurementInterval()
              (++) Set or get the Damping Threshold For Lct LL_SetDampingThresholdForLct() and LL_LCSC_GetDampingThresholdForLct()
              (++) Set or get the Low Pulse Width For Lca And Lcb LL_SetLowPulseWidthForLcaAndLcb() and LL_LCSC_GetLowPulseWidthForLcaAndLcb()
              (++) Set or get the Low Pulse Width For Lct LL_SetLowPulseWidthForLct() and LL_LCSC_GetLowPulseWidthForLct()
              (++) Enable, disable or check if enable the Clock Wise Interrupt And Wakeup Enable LL_LCSC_EnableClockWiseInterruptAndWakeup(), LL_LCSC_DisableClockWiseInterruptAndWakeup() and LL_LCSC_IsEnabledClockWiseInterruptAndWakeup()
              (++) Enable, disable or check if enable the Anti Clock Wise Interrupt And Wakeup Enable LL_LCSC_EnableAntiClockWiseInterruptAndWakeup(), LL_LCSC_DisableAntiClockWiseInterruptAndWakeup() and LL_LCSC_IsEnabledAntiClockWiseInterruptAndWakeup()
              (++) Enable, disable or check if enable the Tamper Interrupt And Wakeup Enable LL_LCSC_EnableTamperInterruptAndWakeup(), LL_LCSC_DisableTamperInterruptAndWakeup() and LL_LCSC_IsEnabledTamperInterruptAndWakeup()
              (++) Enable, disable or check if enable the Lcab Counter Out Of Bound Wakeup Enable LL_LCSC_EnableLcabCounterOutOfBoundWakeup(), LL_LCSC_DisableLcabCounterOutOfBoundWakeup() and LL_LCSC_IsEnabledLcabCounterOutOfBoundWakeup()
              (++) Enable, disable or check if enable the Lcsc LL_LCSC_Enable(), LL_LCSC_Disable() and LL_LCSC_IsEnabled()
              (++) Get the Clock Wise LL_LCSC_GetClockWise()
              (++) Get the Anti Clock Wise LL_LCSC_GetAntiClockWise()
              (++) Set or get the Clock Wise Target LL_SetClockWiseTarget() and LL_LCSC_GetClockWiseTarget()
              (++) Set or get the Anti Clock Wise Target LL_SetAntiClockWiseTarget() and LL_LCSC_GetAntiClockWiseTarget()
              (++) Get the Lca Comparator Last Damping Count LL_LCSC_GetLcaComparatorLastDampingCount()
              (++) Get the Lcb Comparator Last Damping Count LL_LCSC_GetLcbComparatorLastDampingCount()
              (++) Get the Lct Comparator Last Damping Count LL_LCSC_GetLctComparatorLastDampingCount()
              (++) Get the Clkwise State LL_LCSC_GetClkwiseState()
              (++) Get the Aclkwise State LL_LCSC_GetAclkwiseState()
              (++) Get the Last Dir LL_LCSC_GetLastDir()
              (++) Get the Min Lcab Cnt LL_LCSC_GetMinLcabCnt()
              (++) Get the Max Lcab Cnt LL_LCSC_GetMaxLcabCnt()
              (++) Set or get the Min Lcab Cnt Bound LL_SetMinLcabCntBound() and LL_LCSC_GetMinLcabCntBound()
              (++) Set or get the Max Lcab Cnt Bound LL_SetMaxLcabCntBound() and LL_LCSC_GetMaxLcabCntBound()
              (++) Get the Revision LL_LCSC_GetRevision()
              (++) Get the Version LL_LCSC_GetVersion()
              (++) Get the Product LL_LCSC_GetProduct()
              (++) Check if enable and clear the Clkwise F flag LL_LCSC_IsActiveFlag_CLKWISE_F() and LL_LCSC_ClearFlag_CLKWISE_F
              (++) Check if enable and clear the Aclkwise F flag LL_LCSC_IsActiveFlag_ACLKWISE_F() and LL_LCSC_ClearFlag_ACLKWISE_F
              (++) Check if enable and clear the Tamp F flag LL_LCSC_IsActiveFlag_TAMP_F() and LL_LCSC_ClearFlag_TAMP_F
              (++) Check if enable and clear the Cnt Ofb F flag LL_LCSC_IsActiveFlag_CNT_OFB_F() and LL_LCSC_ClearFlag_CNT_OFB_F
 

  @endverbatim
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
  *******************************************************************************/

#if defined(USE_FULL_LL_DRIVER)

/* Includes ------------------------------------------------------------------*/
#include "rf_driver_ll_lcsc.h"
#include "rf_driver_ll_bus.h"

#ifdef  USE_FULL_ASSERT
#include "rf_driver_assert.h"
#else
#define assert_param(expr) ((void)0U)
#endif

/** @addtogroup RF_DRIVER_LL_Driver
  * @{
  */

#if defined (LCSC)

/** @addtogroup LCSC_LL
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/


/**
  * @}
  */

#endif /* defined (LCSC) */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

