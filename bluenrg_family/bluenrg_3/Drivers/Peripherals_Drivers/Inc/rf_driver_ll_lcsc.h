/**
  ******************************************************************************
  * @file    rf_driver_ll_lcsc.h
  * @author  RF Application Team
  * @brief   Header file of LCSC LL module.
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
#ifndef RF_DRIVER_LL_LCSC_H
#define RF_DRIVER_LL_LCSC_H

#ifdef __cplusplus
extern "C" {
#endif


#define LCSC_CR0_TMEAS_Pos                                                 (0UL)
#define LCSC_CR0_TMEAS_Msk                                                 (0x3fffUL)
#define LCSC_CR0_TMEAS                                                     LCSC_CR0_TMEAS_Msk

#define LCSC_CR0_TCAP_Pos                                                  (16UL)
#define LCSC_CR0_TCAP_Msk                                                  (0x3f0000UL)
#define LCSC_CR0_TCAP                                                      LCSC_CR0_TCAP_Msk

#define LCSC_CR0_TICAP_Pos                                                 (24UL)
#define LCSC_CR0_TICAP_Msk                                                 (0x7000000UL)
#define LCSC_CR0_TICAP                                                     LCSC_CR0_TICAP_Msk

#define LCSC_CR1_LCAB_DAMP_THRES_Pos                                       (0UL)
#define LCSC_CR1_LCAB_DAMP_THRES_Msk                                       (0xffUL)
#define LCSC_CR1_LCAB_DAMP_THRES                                           LCSC_CR1_LCAB_DAMP_THRES_Msk

#define LCSC_CR1_TREC_VCM_Pos                                              (10UL)
#define LCSC_CR1_TREC_VCM_Msk                                              (0x7fc00UL)
#define LCSC_CR1_TREC_VCM                                                  LCSC_CR1_TREC_VCM_Msk

#define LCSC_CR1_TSTART_VCM_Pos                                            (20UL)
#define LCSC_CR1_TSTART_VCM_Msk                                            (0x7ff00000UL)
#define LCSC_CR1_TSTART_VCM                                                LCSC_CR1_TSTART_VCM_Msk

#define LCSC_CR2_TAMP_PSC_Pos                                              (0UL)
#define LCSC_CR2_TAMP_PSC_Msk                                              (0xffUL)
#define LCSC_CR2_TAMP_PSC                                                  LCSC_CR2_TAMP_PSC_Msk

#define LCSC_CR2_LCT_DAMP_THRES_Pos                                        (8UL)
#define LCSC_CR2_LCT_DAMP_THRES_Msk                                        (0xff00UL)
#define LCSC_CR2_LCT_DAMP_THRES                                            LCSC_CR2_LCT_DAMP_THRES_Msk

#define LCSC_PULSE_CR_LCAB_PULSE_WIDTH_Pos                                 (0UL)
#define LCSC_PULSE_CR_LCAB_PULSE_WIDTH_Msk                                 (0xfUL)
#define LCSC_PULSE_CR_LCAB_PULSE_WIDTH                                     LCSC_PULSE_CR_LCAB_PULSE_WIDTH_Msk

#define LCSC_PULSE_CR_LCT_PULSE_WIDTH_Pos                                  (8UL)
#define LCSC_PULSE_CR_LCT_PULSE_WIDTH_Msk                                  (0xf00UL)
#define LCSC_PULSE_CR_LCT_PULSE_WIDTH                                      LCSC_PULSE_CR_LCT_PULSE_WIDTH_Msk

#define LCSC_ENR_CLKWISE_IE_Pos                                            (0UL)
#define LCSC_ENR_CLKWISE_IE_Msk                                            (0x1UL)
#define LCSC_ENR_CLKWISE_IE                                                LCSC_ENR_CLKWISE_IE_Msk

#define LCSC_ENR_ACLKWISE_IE_Pos                                           (1UL)
#define LCSC_ENR_ACLKWISE_IE_Msk                                           (0x2UL)
#define LCSC_ENR_ACLKWISE_IE                                               LCSC_ENR_ACLKWISE_IE_Msk

#define LCSC_ENR_TAMP_IE_Pos                                               (2UL)
#define LCSC_ENR_TAMP_IE_Msk                                               (0x4UL)
#define LCSC_ENR_TAMP_IE                                                   LCSC_ENR_TAMP_IE_Msk

#define LCSC_ENR_CNT_OFB_WKP_IE_Pos                                        (3UL)
#define LCSC_ENR_CNT_OFB_WKP_IE_Msk                                        (0x8UL)
#define LCSC_ENR_CNT_OFB_WKP_IE                                            LCSC_ENR_CNT_OFB_WKP_IE_Msk

#define LCSC_ENR_EN_Pos                                                    (31UL)
#define LCSC_ENR_EN_Msk                                                    (0x80000000UL)
#define LCSC_ENR_EN                                                        LCSC_ENR_EN_Msk

#define LCSC_WHEEL_SR_CLKWISE_Pos                                          (0UL)
#define LCSC_WHEEL_SR_CLKWISE_Msk                                          (0xffffUL)
#define LCSC_WHEEL_SR_CLKWISE                                              LCSC_WHEEL_SR_CLKWISE_Msk

#define LCSC_WHEEL_SR_ACLKWISE_Pos                                         (16UL)
#define LCSC_WHEEL_SR_ACLKWISE_Msk                                         (0xffff0000UL)
#define LCSC_WHEEL_SR_ACLKWISE                                             LCSC_WHEEL_SR_ACLKWISE_Msk

#define LCSC_CONFR_CLKWISE_THRES_Pos                                       (0UL)
#define LCSC_CONFR_CLKWISE_THRES_Msk                                       (0xffffUL)
#define LCSC_CONFR_CLKWISE_THRES                                           LCSC_CONFR_CLKWISE_THRES_Msk

#define LCSC_CONFR_ACLKWISE_THRES_Pos                                      (16UL)
#define LCSC_CONFR_ACLKWISE_THRES_Msk                                      (0xffff0000UL)
#define LCSC_CONFR_ACLKWISE_THRES                                          LCSC_CONFR_ACLKWISE_THRES_Msk

#define LCSC_COMP_CTN_CMP_LCA_CNT_Pos                                      (0UL)
#define LCSC_COMP_CTN_CMP_LCA_CNT_Msk                                      (0xffUL)
#define LCSC_COMP_CTN_CMP_LCA_CNT                                          LCSC_COMP_CTN_CMP_LCA_CNT_Msk

#define LCSC_COMP_CTN_CMP_LCB_CNT_Pos                                      (10UL)
#define LCSC_COMP_CTN_CMP_LCB_CNT_Msk                                      (0x3fc00UL)
#define LCSC_COMP_CTN_CMP_LCB_CNT                                          LCSC_COMP_CTN_CMP_LCB_CNT_Msk

#define LCSC_COMP_CTN_CMP_LCT_CNT_Pos                                      (20UL)
#define LCSC_COMP_CTN_CMP_LCT_CNT_Msk                                      (0xff00000UL)
#define LCSC_COMP_CTN_CMP_LCT_CNT                                          LCSC_COMP_CTN_CMP_LCT_CNT_Msk

#define LCSC_SR_CLKWISE_STATE_Pos                                          (0UL)
#define LCSC_SR_CLKWISE_STATE_Msk                                          (0x3UL)
#define LCSC_SR_CLKWISE_STATE                                              LCSC_SR_CLKWISE_STATE_Msk

#define LCSC_SR_ACLKWISE_STATE_Pos                                         (2UL)
#define LCSC_SR_ACLKWISE_STATE_Msk                                         (0xcUL)
#define LCSC_SR_ACLKWISE_STATE                                             LCSC_SR_ACLKWISE_STATE_Msk

#define LCSC_SR_LAST_DIR_Pos                                               (4UL)
#define LCSC_SR_LAST_DIR_Msk                                               (0x30UL)
#define LCSC_SR_LAST_DIR                                                   LCSC_SR_LAST_DIR_Msk

#define LCSC_STAT_MIN_LCAB_CNT_Pos                                         (0UL)
#define LCSC_STAT_MIN_LCAB_CNT_Msk                                         (0xffUL)
#define LCSC_STAT_MIN_LCAB_CNT                                             LCSC_STAT_MIN_LCAB_CNT_Msk

#define LCSC_STAT_MAX_LCAB_CNT_Pos                                         (8UL)
#define LCSC_STAT_MAX_LCAB_CNT_Msk                                         (0xff00UL)
#define LCSC_STAT_MAX_LCAB_CNT                                             LCSC_STAT_MAX_LCAB_CNT_Msk

#define LCSC_STAT_MIN_LCAB_CNT_BOUND_Pos                                   (16UL)
#define LCSC_STAT_MIN_LCAB_CNT_BOUND_Msk                                   (0xff0000UL)
#define LCSC_STAT_MIN_LCAB_CNT_BOUND                                       LCSC_STAT_MIN_LCAB_CNT_BOUND_Msk

#define LCSC_STAT_MAX_LCAB_CNT_BOUND_Pos                                   (24UL)
#define LCSC_STAT_MAX_LCAB_CNT_BOUND_Msk                                   (0xff000000UL)
#define LCSC_STAT_MAX_LCAB_CNT_BOUND                                       LCSC_STAT_MAX_LCAB_CNT_BOUND_Msk

#define LCSC_VER_REV_Pos                                                   (4UL)
#define LCSC_VER_REV_Msk                                                   (0xf0UL)
#define LCSC_VER_REV                                                       LCSC_VER_REV_Msk

#define LCSC_VER_VER_Pos                                                   (8UL)
#define LCSC_VER_VER_Msk                                                   (0xf00UL)
#define LCSC_VER_VER                                                       LCSC_VER_VER_Msk

#define LCSC_VER_PROD_Pos                                                  (12UL)
#define LCSC_VER_PROD_Msk                                                  (0xf000UL)
#define LCSC_VER_PROD                                                      LCSC_VER_PROD_Msk

#define LCSC_ISR_CLKWISE_F_Pos                                             (0UL)
#define LCSC_ISR_CLKWISE_F_Msk                                             (0x1UL)
#define LCSC_ISR_CLKWISE_F                                                 LCSC_ISR_CLKWISE_F_Msk

#define LCSC_ISR_ACLKWISE_F_Pos                                            (1UL)
#define LCSC_ISR_ACLKWISE_F_Msk                                            (0x2UL)
#define LCSC_ISR_ACLKWISE_F                                                LCSC_ISR_ACLKWISE_F_Msk

#define LCSC_ISR_TAMP_F_Pos                                                (2UL)
#define LCSC_ISR_TAMP_F_Msk                                                (0x4UL)
#define LCSC_ISR_TAMP_F                                                    LCSC_ISR_TAMP_F_Msk

#define LCSC_ISR_CNT_OFB_F_Pos                                             (3UL)
#define LCSC_ISR_CNT_OFB_F_Msk                                             (0x8UL)
#define LCSC_ISR_CNT_OFB_F                                                 LCSC_ISR_CNT_OFB_F_Msk



/* Includes ------------------------------------------------------------------*/
#if defined(CONFIG_DEVICE_SPIRIT3)
#include "spirit3.h"
#endif


/** @addtogroup RF_DRIVER_LL_Driver
  * @{
  */

#if defined(LCSC)


/** @defgroup LCSC_LL LCSC
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup LCSC_LL_Exported_Constants LCSC Exported Constants
  * @{
  */

 /** @defgroup LCSC_LL_CLKWISE_STATE Values
  * @{
  */
#define LL_LCSC_SR_CLKWISE_FSM_Q1                                                 (0U) 
#define LL_LCSC_SR_CLKWISE_FSM_Q2                                                 (                           LCSC_SR_CLKWISE_STATE_0 )
#define LL_LCSC_SR_CLKWISE_FSM_Q3                                                 ( LCSC_SR_CLKWISE_STATE_1                           )
#define LL_LCSC_SR_CLKWISE_FSM_Q4                                                 ( LCSC_SR_CLKWISE_STATE_1 | LCSC_SR_CLKWISE_STATE_0 )
/**
  * @}
  */

 /** @defgroup LCSC_LL_ACLKWISE_STATE Values
  * @{
  */
#define LL_LCSC_SR_ACLKWISE_FSM_Q1                                                (0U) 
#define LL_LCSC_SR_ACLKWISE_FSM_Q2                                                (                            LCSC_SR_ACLKWISE_STATE_0 )
#define LL_LCSC_SR_ACLKWISE_FSM_Q3                                                ( LCSC_SR_ACLKWISE_STATE_1                            )
#define LL_LCSC_SR_ACLKWISE_FSM_Q4                                                ( LCSC_SR_ACLKWISE_STATE_1 | LCSC_SR_ACLKWISE_STATE_0 )
/**
  * @}
  */

 /** @defgroup LCSC_LL_LAST_DIR Values
  * @{
  */
#define LL_LCSC_SR_LAST_DIR_UNKNOWN                                               (0U) 
#define LL_LCSC_SR_LAST_DIR_CLOCKWISE                                             ( LCSC_SR_LAST_DIR_1                      )
#define LL_LCSC_SR_LAST_DIR_COUNTER_CLOCKWISE                                     ( LCSC_SR_LAST_DIR_1 | LCSC_SR_LAST_DIR_0 )
/**
  * @}
  */




/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup LCSC_LL_Exported_Macros LCSC Exported Macros
  * @{
  */



/** @defgroup LCSC_LL_EM_WRITE_READ Common Write and read registers Macros
  * @{
  */

/**
  * @brief  Set the measurement duration (also called TMeas) is the duration of an LC measurement sequence.
  * @note   This period is given in number of slow clock cycles.
  * @rmtoll CR0          TMEAS       LL_LCSC_SetMeasurementTime
  * @param  LCSCx LCSC Instance
  * @param  value parameter must be a number between 0 and 16383 (form 0x0 to 0x3fff)
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_SetMeasurementTime(LCSC_TypeDef *LCSCx, uint32_t value)
{
  MODIFY_REG(LCSCx->CR0, LCSC_CR0_TMEAS, value);
}

/**
  * @brief  Get the measurement duration (also called TMeas) is the duration of an LC measurement sequence 
  *         (including LCA, LCB and LCT if necessary, and also the time left before the next power-up of analog blocks (DAC, COMP, VCMBUFF)).
  * @note   This period is given in number of slow clock cycles.
  * @rmtoll CR0          TMEAS       LL_LCSC_GetMeasurementTime
  * @retval the measurement duration TMeas
  */
__STATIC_INLINE uint32_t LL_LCSC_GetMeasurementTime(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->CR0, LCSC_CR0_TMEAS));
}

/**
  * @brief  Set the capture duration time.
  * @note   Also called Tcapture, it is the duration of one LC measurement, starting from the excitation pulse 
  *         on the LC line and ending once the TCAP number of slow clock cycles has been reached. This duration 
  *         is given in number of slow clock cycles.
  * @rmtoll CR0          TCAP       LL_LCSC_SetCaptureTime
  * @param  LCSCx LCSC Instance
  * @param  value parameter must be a number between 0 and 63 (form 0x0 to 0x3f)
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_SetCaptureTime(LCSC_TypeDef *LCSCx, uint32_t value)
{
  MODIFY_REG(LCSCx->CR0, LCSC_CR0_TCAP, value << LCSC_CR0_TCAP_Pos);
}

/**
  * @brief  Get the capture duration (also called Tcapture).
  * @note   It is the duration of one LC measurement, starting from the excitation pulse on the LC line and ending 
  *         once the TCAP number of slow clock cycles has been reached. This duration is given in number of slow clock cycles.
  * @rmtoll CR0          TCAP       LL_LCSC_GetCaptureTime
  * @retval the capture duration Tcapture
  */
__STATIC_INLINE uint32_t LL_LCSC_GetCaptureTime(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->CR0, LCSC_CR0_TCAP) >> LCSC_CR0_TCAP_Pos);
}

/**
  * @brief  Set the inter-capture duration (also called TinterCapture).
  * @note   It is the duration between the end of a first TCAP and the following TCAP and is equal to TICAP + 2 slow clock cycles. 
  *         This duration is given in number of slow clock cycles.
  * @rmtoll CR0          TICAP       LL_LCSC_SetInterCaptureTime
  * @param  LCSCx LCSC Instance
  * @param  value parameter must be a number between 0 and 7 (form 0x0 to 0x7)
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_SetInterCaptureTime(LCSC_TypeDef *LCSCx, uint32_t value)
{
  MODIFY_REG(LCSCx->CR0, LCSC_CR0_TICAP, value << LCSC_CR0_TICAP_Pos);
}

/**
  * @brief  Get the inter-capture duration (also called TinterCapture).
  * @note   It is the duration between the end of a firstTCAP and the following TCAP and is equal to TICAP + 2 slow clock cycles.
  *         This duration is given in number of slow clock cycles.
  * @rmtoll CR0          TICAP       LL_LCSC_GetInterCaptureTime
  * @retval the inter-capture duration TinterCapture
  */
__STATIC_INLINE uint32_t LL_LCSC_GetInterCaptureTime(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->CR0, LCSC_CR0_TICAP) >> LCSC_CR0_TICAP_Pos);
}

/**
  * @brief  Set the count threshold used to define if the LCA and LCB is near of not of a metallic surface.
  * @rmtoll CR1          LCAB_DAMP_THRES       LL_LCSC_SetDampingThresholdForLcaAndLcb
  * @param  LCSCx LCSC Instance
  * @param  value parameter must be a number between 0 and 255 (form 0x0 to 0xff)
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_SetDampingThresholdForLcaAndLcb(LCSC_TypeDef *LCSCx, uint32_t value)
{
  MODIFY_REG(LCSCx->CR1, LCSC_CR1_LCAB_DAMP_THRES, value << LCSC_CR1_LCAB_DAMP_THRES_Pos);
}

/**
  * @brief  Get the count threshold used to define if the LCA and LCB is near of not of a metallic surface. 
  * @rmtoll CR1          LCAB_DAMP_THRES       LL_LCSC_GetDampingThresholdForLcaAndLcb
  * @retval the damping threshold for LCA and LCB
  */
__STATIC_INLINE uint32_t LL_LCSC_GetDampingThresholdForLcaAndLcb(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->CR1, LCSC_CR1_LCAB_DAMP_THRES) >> LCSC_CR1_LCAB_DAMP_THRES_Pos);
}

/**
  * @brief  Set the Corresponds to the duration TrecoveryVCM (in number of slow clock cycles) between the enable of the VCMBUFF and the establishment of the Voltage to the value VDD33/2.
  * @note   This time is to be considered between each sequence of LC measurement, called TSCAN in the registers. (sequence of LC measurement = LCA, LCB and sometimes LCT). TREC_VCM must not be set to values lower of 0x02
  * @rmtoll CR1          TREC_VCM       LL_LCSC_SetVcmbuffRecoveryTime
  * @param  LCSCx LCSC Instance
  * @param  value parameter must be a number between 0 and 511 (form 0x0 to 0x1ff)
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_SetVcmbuffRecoveryTime(LCSC_TypeDef *LCSCx, uint32_t value)
{
  MODIFY_REG(LCSCx->CR1, LCSC_CR1_TREC_VCM, value << LCSC_CR1_TREC_VCM_Pos);
}

/**
  * @brief  Get the Corresponds to the duration TrecoveryVCM (in number of slow clock cycles) between the enable of the VCMBUFF 
  *         and the establishment of the Voltage to the value VDD33/2.
  * @note   This time is to be considered between each sequence of LC measurement, called TSCAN in the registers. 
  *         (sequence of LC measurement = LCA, LCB and sometimes LCT). TREC_VCM must not be set to values lower of 0x02
  * @rmtoll CR1          TREC_VCM       LL_LCSC_GetVcmbuffRecoveryTime
  * @retval the VCMBUFF Recovery Time
  */
__STATIC_INLINE uint32_t LL_LCSC_GetVcmbuffRecoveryTime(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->CR1, LCSC_CR1_TREC_VCM) >> LCSC_CR1_TREC_VCM_Pos);
}

/**
  * @brief  Set the Corresponds to the duration TStartVCM (in number of slow clock cycles) between the enable of the VCMBUFF 
  *         and the establishment of the Voltage to the value VDD33/2.
  * @note   This time is only to be considered for the first LC measurement after an enabling of the LCSC feature thanks to the LCSC_EN bit. 
  *         TSART_VCM must not be set to values lower of 0x02
  * @rmtoll CR1          TSTART_VCM       LL_LCSC_SetVcmbuffStartingTime
  * @param  LCSCx LCSC Instance
  * @param  value parameter must be a number between 0 and 2047 (form 0x0 to 0x7ff)
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_SetVcmbuffStartingTime(LCSC_TypeDef *LCSCx, uint32_t value)
{
  MODIFY_REG(LCSCx->CR1, LCSC_CR1_TSTART_VCM, value << LCSC_CR1_TSTART_VCM_Pos);
}

/**
  * @brief  Get the Corresponds to the duration TStartVCM (in number of slow clock cycles) between the enable of the VCMBUFF 
  *         and the establishment of the Voltage to the value VDD33/2.
  * @note   This time is only to be considered for the first LC measurement after an enabling of the LCSC feature thanks to the LCSC_EN bit.
  *         TSART_VCM must not be set to values lower of 0x02
  * @rmtoll CR1          TSTART_VCM       LL_LCSC_GetVcmbuffStartingTime
  * @retval the VCMBUFF Starting Time
  */
__STATIC_INLINE uint32_t LL_LCSC_GetVcmbuffStartingTime(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->CR1, LCSC_CR1_TSTART_VCM) >> LCSC_CR1_TSTART_VCM_Pos);
}

/**
  * @brief  Set the tamper detection prescaler comparing to the measurement period
  * @note   0x0: the tamper detection and the LCT measurement are never done
  *         others: the LCT measurement is done once every TAMP_PSC sequences of measurements.
  * @rmtoll CR2          TAMP_PSC       LL_LCSC_SetTamperMeasurementInterval
  * @param  LCSCx LCSC Instance
  * @param  value parameter must be a number between 0 and 255 (form 0x0 to 0xff)
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_SetTamperMeasurementInterval(LCSC_TypeDef *LCSCx, uint32_t value)
{
  MODIFY_REG(LCSCx->CR2, LCSC_CR2_TAMP_PSC, value << LCSC_CR2_TAMP_PSC_Pos);
}

/**
  * @brief  Get the tamper detection prescaler comparing to the measurement period
  * @note   0x00: the tamper detection and the LCT measurement are never done
  *         others: the LCT measurement is done once every TAMP_PSC sequences of measurements.
  * @rmtoll CR2          TAMP_PSC       LL_LCSC_GetTamperMeasurementInterval
  * @retval the tamper detection prescaler
  */
__STATIC_INLINE uint32_t LL_LCSC_GetTamperMeasurementInterval(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->CR2, LCSC_CR2_TAMP_PSC) >> LCSC_CR2_TAMP_PSC_Pos);
}

/**
  * @brief  Set the count threshold used to define if the LCT is near of not of a metallic surface.
  * @rmtoll CR2          LCT_DAMP_THRES       LL_LCSC_SetDampingThresholdForLct
  * @param  LCSCx LCSC Instance
  * @param  value parameter must be a number between 0 and 255 (form 0x0 to 0xff)
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_SetDampingThresholdForLct(LCSC_TypeDef *LCSCx, uint32_t value)
{
  MODIFY_REG(LCSCx->CR2, LCSC_CR2_LCT_DAMP_THRES, value << LCSC_CR2_LCT_DAMP_THRES_Pos);
}

/**
  * @brief  Get the count threshold used to define if the LCT is near of not of a metallic surface.
  * @rmtoll CR2          LCT_DAMP_THRES       LL_LCSC_GetDampingThresholdForLct
  * @retval the damping threshold for LCT
  */
__STATIC_INLINE uint32_t LL_LCSC_GetDampingThresholdForLct(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->CR2, LCSC_CR2_LCT_DAMP_THRES) >> LCSC_CR2_LCT_DAMP_THRES_Pos);
}

/**
  * @brief  Set the Pulse width programming for generated pulse.
  * @rmtoll PULSE_CR          LCAB_PULSE_WIDTH       LL_LCSC_SetLowPulseWidthForLcaAndLcb
  * @param  LCSCx LCSC Instance
  * @param  value parameter must be a number between 0 and 15 (form 0x0 to 0xf)
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_SetLowPulseWidthForLcaAndLcb(LCSC_TypeDef *LCSCx, uint32_t value)
{
  MODIFY_REG(LCSCx->PULSE_CR, LCSC_PULSE_CR_LCAB_PULSE_WIDTH, value << LCSC_PULSE_CR_LCAB_PULSE_WIDTH_Pos);
}

/**
  * @brief  Get the Pulse width programming for generated pulse.
  * @rmtoll PULSE_CR          LCAB_PULSE_WIDTH       LL_LCSC_GetLowPulseWidthForLcaAndLcb
  * @retval the low pulse width for LCA and LCB
  */
__STATIC_INLINE uint32_t LL_LCSC_GetLowPulseWidthForLcaAndLcb(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->PULSE_CR, LCSC_PULSE_CR_LCAB_PULSE_WIDTH) >> LCSC_PULSE_CR_LCAB_PULSE_WIDTH_Pos);
}

/**
  * @brief  Set the Pulse width programming for generated pulse.
  * @rmtoll PULSE_CR          LCT_PULSE_WIDTH       LL_LCSC_SetLowPulseWidthForLct
  * @param  LCSCx LCSC Instance
  * @param  value parameter must be a number between 0 and 15 (form 0x0 to 0xf)
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_SetLowPulseWidthForLct(LCSC_TypeDef *LCSCx, uint32_t value)
{
  MODIFY_REG(LCSCx->PULSE_CR, LCSC_PULSE_CR_LCT_PULSE_WIDTH, value << LCSC_PULSE_CR_LCT_PULSE_WIDTH_Pos);
}

/**
  * @brief  Get the Pulse width programming for generated pulse.
  * @rmtoll PULSE_CR          LCT_PULSE_WIDTH       LL_LCSC_GetLowPulseWidthForLct
  * @retval the low pulse width for LCT
  */
__STATIC_INLINE uint32_t LL_LCSC_GetLowPulseWidthForLct(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->PULSE_CR, LCSC_PULSE_CR_LCT_PULSE_WIDTH) >> LCSC_PULSE_CR_LCT_PULSE_WIDTH_Pos);
}

/**
  * @brief  LCSC Enable Enable the IRQ and Wakeup event generation once the CLKWISE has reached the CLKWISE_THRES value.
  * @rmtoll ENR          CLKWISE_IE            LL_LCSC_EnableClockWiseInterruptAndWakeup
  * @param  LCSCx LCSC Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_EnableClockWiseInterruptAndWakeup(LCSC_TypeDef *LCSCx)
{
  SET_BIT(LCSCx->ENR, LCSC_ENR_CLKWISE_IE);
}

/**
  * @brief  LCSC Disable Enable the IRQ and Wakeup event generation once the CLKWISE has reached the CLKWISE_THRES value.
  * @rmtoll ENR          CLKWISE_IE            LL_LCSC_DisableClockWiseInterruptAndWakeup
  * @param  LCSCx LCSC Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_DisableClockWiseInterruptAndWakeup(LCSC_TypeDef *LCSCx)
{ 
  CLEAR_BIT(LCSCx->ENR, LCSC_ENR_CLKWISE_IE);
}

/**
  * @brief  Indicate if CLKWISE_IE is enabled. Enable the IRQ and Wakeup event generation once the CLKWISE has reached the CLKWISE_THRES value. 
  * @rmtoll ENR          CLKWISE_IE            LL_LCSC_IsEnabledClockWiseInterruptAndWakeup 
  * @param  LCSCx LCSC Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCSC_IsEnabledClockWiseInterruptAndWakeup(LCSC_TypeDef *LCSCx)
{
  return ((READ_BIT(LCSCx->ENR, LCSC_ENR_CLKWISE_IE) == (LCSC_ENR_CLKWISE_IE)) ? 1U : 0U);
}

/**
  * @brief  LCSC Enable Enable the IRQ and Wakeup event generation once the ACLKWISE value is equal or greater than the ACLKWISE_THRES bits.
  * @rmtoll ENR          ACLKWISE_IE            LL_LCSC_EnableAntiClockWiseInterruptAndWakeup
  * @param  LCSCx LCSC Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_EnableAntiClockWiseInterruptAndWakeup(LCSC_TypeDef *LCSCx)
{
  SET_BIT(LCSCx->ENR, LCSC_ENR_ACLKWISE_IE);
}

/**
  * @brief  LCSC Disable Enable the IRQ and Wakeup event generation once the ACLKWISE value is equal or greater than the ACLKWISE_THRES bits.
  * @rmtoll ENR          ACLKWISE_IE            LL_LCSC_DisableAntiClockWiseInterruptAndWakeup
  * @param  LCSCx LCSC Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_DisableAntiClockWiseInterruptAndWakeup(LCSC_TypeDef *LCSCx)
{ 
  CLEAR_BIT(LCSCx->ENR, LCSC_ENR_ACLKWISE_IE);
}

/**
  * @brief  Indicate if ACLKWISE_IE is enabled.
  * @rmtoll ENR          ACLKWISE_IE            LL_LCSC_IsEnabledAntiClockWiseInterruptAndWakeup 
  * @param  LCSCx LCSC Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCSC_IsEnabledAntiClockWiseInterruptAndWakeup(LCSC_TypeDef *LCSCx)
{
  return ((READ_BIT(LCSCx->ENR, LCSC_ENR_ACLKWISE_IE) == (LCSC_ENR_ACLKWISE_IE)) ? 1U : 0U);
}

/**
  * @brief  LCSC Enable Enable the IRQ and the Wakeup event generation on a tamper detection.
  * @rmtoll ENR          TAMP_IE            LL_LCSC_EnableTamperInterruptAndWakeup
  * @param  LCSCx LCSC Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_EnableTamperInterruptAndWakeup(LCSC_TypeDef *LCSCx)
{
  SET_BIT(LCSCx->ENR, LCSC_ENR_TAMP_IE);
}

/**
  * @brief  LCSC Disable Enable the IRQ and the Wakeup event generation on a tamper detection.
  * @rmtoll ENR          TAMP_IE            LL_LCSC_DisableTamperInterruptAndWakeup
  * @param  LCSCx LCSC Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_DisableTamperInterruptAndWakeup(LCSC_TypeDef *LCSCx)
{ 
  CLEAR_BIT(LCSCx->ENR, LCSC_ENR_TAMP_IE);
}

/**
  * @brief  Indicate if TAMP_IE is enabled. 
  * @rmtoll ENR          TAMP_IE            LL_LCSC_IsEnabledTamperInterruptAndWakeup 
  * @param  LCSCx LCSC Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCSC_IsEnabledTamperInterruptAndWakeup(LCSC_TypeDef *LCSCx)
{
  return ((READ_BIT(LCSCx->ENR, LCSC_ENR_TAMP_IE) == (LCSC_ENR_TAMP_IE)) ? 1U : 0U);
}

/**
  * @brief  LCSC Enable Enable the IRQ and the Wakeup event generation on a LCAB count out of bounds
  * @rmtoll ENR          CNT_OFB_WKP_IE            LL_LCSC_EnableLcabCounterOutOfBoundWakeup
  * @param  LCSCx LCSC Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_EnableLcabCounterOutOfBoundWakeup(LCSC_TypeDef *LCSCx)
{
  SET_BIT(LCSCx->ENR, LCSC_ENR_CNT_OFB_WKP_IE);
}

/**
  * @brief  LCSC Disable Enable the IRQ and the Wakeup event generation on a LCAB count out of bounds
  * @rmtoll ENR          CNT_OFB_WKP_IE            LL_LCSC_DisableLcabCounterOutOfBoundWakeup
  * @param  LCSCx LCSC Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_DisableLcabCounterOutOfBoundWakeup(LCSC_TypeDef *LCSCx)
{ 
  CLEAR_BIT(LCSCx->ENR, LCSC_ENR_CNT_OFB_WKP_IE);
}

/**
  * @brief  Indicate if CNT_OFB_WKP_IE is enabled.
  * @rmtoll ENR          CNT_OFB_WKP_IE            LL_LCSC_IsEnabledLcabCounterOutOfBoundWakeup 
  * @param  LCSCx LCSC Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCSC_IsEnabledLcabCounterOutOfBoundWakeup(LCSC_TypeDef *LCSCx)
{
  return ((READ_BIT(LCSCx->ENR, LCSC_ENR_CNT_OFB_WKP_IE) == (LCSC_ENR_CNT_OFB_WKP_IE)) ? 1U : 0U);
}

/**
  * @brief  LCSC Enable Enable (start) or Disable ( stop) the LCSC Measurement.
  * @rmtoll ENR          LCSC_EN            LL_LCSC_Enable
  * @param  LCSCx LCSC Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_Enable(LCSC_TypeDef *LCSCx)
{
  SET_BIT(LCSCx->ENR, LCSC_ENR_LCSC_EN);
}

/**
  * @brief  LCSC Disable Enable (start) or Disable ( stop) the LCSC Measurement.
  * @rmtoll ENR          LCSC_EN            LL_LCSC_Disable
  * @param  LCSCx LCSC Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_Disable(LCSC_TypeDef *LCSCx)
{ 
  CLEAR_BIT(LCSCx->ENR, LCSC_ENR_LCSC_EN);
}

/**
  * @brief  Indicate if LCSC_EN is enabled. Enable (start) or Disable ( stop) the LCSC Measurement.
  * @rmtoll ENR          LCSC_EN            LL_LCSC_IsEnabled
  * @param  LCSCx LCSC Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCSC_IsEnabled(LCSC_TypeDef *LCSCx)
{
  return ((READ_BIT(LCSCx->ENR, LCSC_ENR_LCSC_EN) == (LCSC_ENR_LCSC_EN)) ? 1U : 0U);
}

/**
  * @brief  Get the Number of Clock Wise revolutions 
  * @rmtoll WHEEL_SR          CLKWISE       LL_LCSC_GetClockWise
  * @retval the Number of Clock Wise revolutions 
  */
__STATIC_INLINE uint32_t LL_LCSC_GetClockWise(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->WHEEL_SR, LCSC_WHEEL_SR_CLKWISE) >> LCSC_WHEEL_SR_CLKWISE_Pos);
}

/**
  * @brief  Get the Number of Anti Clock Wise revolutions 
  * @rmtoll WHEEL_SR          ACLKWISE       LL_LCSC_GetAntiClockWise
  * @retval the Number of Anti Clock Wise revolutions 
  */
__STATIC_INLINE uint32_t LL_LCSC_GetAntiClockWise(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->WHEEL_SR, LCSC_WHEEL_SR_ACLKWISE) >> LCSC_WHEEL_SR_ACLKWISE_Pos);
}

/**
  * @brief  Set the Number of Clock Wise revolutions target
  * @rmtoll CONFR          CLKWISE_THRES       LL_LCSC_SetClockWiseTarget
  * @param  LCSCx LCSC Instance
  * @param  value parameter must be a number between 0 and 65535 (form 0x0 to 0xffff)
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_SetClockWiseTarget(LCSC_TypeDef *LCSCx, uint32_t value)
{
  MODIFY_REG(LCSCx->CONFR, LCSC_CONFR_CLKWISE_THRES, value << LCSC_CONFR_CLKWISE_THRES_Pos);
}

/**
  * @brief  Get the Number of Clock Wise revolutions target
  * @rmtoll CONFR          CLKWISE_THRES       LL_LCSC_GetClockWiseTarget
  * @retval the Number of Clock Wise revolutions target
  */
__STATIC_INLINE uint32_t LL_LCSC_GetClockWiseTarget(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->CONFR, LCSC_CONFR_CLKWISE_THRES) >> LCSC_CONFR_CLKWISE_THRES_Pos);
}

/**
  * @brief  Set the Number of Anti Clock Wise revolutions target
  * @rmtoll CONFR          ACLKWISE_THRES       LL_LCSC_SetAntiClockWiseTarget
  * @param  LCSCx LCSC Instance
  * @param  value parameter must be a number between 0 and 65535 (form 0x0 to 0xffff)
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_SetAntiClockWiseTarget(LCSC_TypeDef *LCSCx, uint32_t value)
{
  MODIFY_REG(LCSCx->CONFR, LCSC_CONFR_ACLKWISE_THRES, value << LCSC_CONFR_ACLKWISE_THRES_Pos);
}

/**
  * @brief  Get the Number of Anti Clock Wise revolutions target
  * @rmtoll CONFR          ACLKWISE_THRES       LL_LCSC_GetAntiClockWiseTarget
  * @retval the Number of Anti Clock Wise revolutions target
  */
__STATIC_INLINE uint32_t LL_LCSC_GetAntiClockWiseTarget(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->CONFR, LCSC_CONFR_ACLKWISE_THRES) >> LCSC_CONFR_ACLKWISE_THRES_Pos);
}

/**
  * @brief  Get the LCA Comparator last damping count
  * @rmtoll COMP_CTN          CMP_LCA_CNT       LL_LCSC_GetLcaComparatorLastDampingCount
  * @retval the LCA Comparator last damping count
  */
__STATIC_INLINE uint32_t LL_LCSC_GetLcaComparatorLastDampingCount(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->COMP_CTN, LCSC_COMP_CTN_CMP_LCA_CNT) >> LCSC_COMP_CTN_CMP_LCA_CNT_Pos);
}

/**
  * @brief  Get the LCB Comparator last damping count 
  * @rmtoll COMP_CTN          CMP_LCB_CNT       LL_LCSC_GetLcbComparatorLastDampingCount
  * @retval the LCB Comparator last damping count 
  */
__STATIC_INLINE uint32_t LL_LCSC_GetLcbComparatorLastDampingCount(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->COMP_CTN, LCSC_COMP_CTN_CMP_LCB_CNT) >> LCSC_COMP_CTN_CMP_LCB_CNT_Pos);
}

/**
  * @brief  Get the LCT Comparator last damping count 
  * @rmtoll COMP_CTN          CMP_LCT_CNT       LL_LCSC_GetLctComparatorLastDampingCount
  * @retval the LCT Comparator last damping count
  */
__STATIC_INLINE uint32_t LL_LCSC_GetLctComparatorLastDampingCount(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->COMP_CTN, LCSC_COMP_CTN_CMP_LCT_CNT) >> LCSC_COMP_CTN_CMP_LCT_CNT_Pos);
}

/**
  * @brief  Get the current state of the LCSC clockwise FSM
  * @note   00: Q1
  *         01: Q2
  *         10: Q3
  *         11: Q4
  * @rmtoll SR          CLKWISE_STATE       LL_LCSC_GetClkwiseState
  * @retval can be one of the following values
  * @arg LL_LCSC_SR_CLKWISE_FSM_Q1
  * @arg LL_LCSC_SR_CLKWISE_FSM_Q2
  * @arg LL_LCSC_SR_CLKWISE_FSM_Q3
  * @arg LL_LCSC_SR_CLKWISE_FSM_Q4
  */
__STATIC_INLINE uint32_t LL_LCSC_GetClkwiseState(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->SR, LCSC_SR_CLKWISE_STATE));
}

/**
  * @brief  Get the current state of the LCSC anti clockwise FSM
  * @note   00: Q1
  *         01: Q2
  *         10: Q3
  *         11: Q4
  * @rmtoll SR          ACLKWISE_STATE       LL_LCSC_GetAclkwiseState
  * @retval can be one of the following values
  * @arg LL_LCSC_SR_ACLKWISE_FSM_Q1
  * @arg LL_LCSC_SR_ACLKWISE_FSM_Q2
  * @arg LL_LCSC_SR_ACLKWISE_FSM_Q3
  * @arg LL_LCSC_SR_ACLKWISE_FSM_Q4
  */
__STATIC_INLINE uint32_t LL_LCSC_GetAclkwiseState(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->SR, LCSC_SR_ACLKWISE_STATE));
}

/**
  * @brief  Get the last direction detected
  * @note   0X: unknown (default value when LCSC is enabled)
  *         10: clockwise
  *         11: counterclockwise
  * @rmtoll SR          LAST_DIR       LL_LCSC_GetLastDir
  * @retval can be one of the following values
  * @arg LL_LCSC_SR_LAST_DIR_UNKNOWN
  * @arg LL_LCSC_SR_LAST_DIR_CLOCKWISE
  * @arg LL_LCSC_SR_LAST_DIR_COUNTER_CLOCKWISE
  */
__STATIC_INLINE uint32_t LL_LCSC_GetLastDir(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->SR, LCSC_SR_LAST_DIR));
}

/**
  * @brief  Get the Minimum of CMP_LCA_CNT, CMP_LCB_CNT reached during the measurement
  * @rmtoll STAT          MIN_LCAB_CNT       LL_LCSC_GetMinLcabCnt
  * @retval the Minimum of CMP_LCA_CNT
  */
__STATIC_INLINE uint32_t LL_LCSC_GetMinLcabCnt(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->STAT, LCSC_STAT_MIN_LCAB_CNT) >> LCSC_STAT_MIN_LCAB_CNT_Pos);
}

/**
  * @brief  Get the Maximum of CMP_LCA_CNT, CMP_LCB_CNT reached during the measurement
  * @rmtoll STAT          MAX_LCAB_CNT       LL_LCSC_GetMaxLcabCnt
  * @retval the Maximum of CMP_LCA_CNT
  */
__STATIC_INLINE uint32_t LL_LCSC_GetMaxLcabCnt(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->STAT, LCSC_STAT_MAX_LCAB_CNT) >> LCSC_STAT_MAX_LCAB_CNT_Pos);
}

/**
  * @brief  Set the Minimum bound of CMP_LCA_COUNT, CMP_LCB_COUNT used when monitoring the MIN_LCAB_CNT
  * @rmtoll STAT          MIN_LCAB_CNT_BOUND       LL_LCSC_SetMinLcabCntBound
  * @param  LCSCx LCSC Instance
  * @param  value parameter must be a number between 0 and 255 (form 0x0 to 0xff)
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_SetMinLcabCntBound(LCSC_TypeDef *LCSCx, uint32_t value)
{
  MODIFY_REG(LCSCx->STAT, LCSC_STAT_MIN_LCAB_CNT_BOUND, value << LCSC_STAT_MIN_LCAB_CNT_BOUND_Pos);
}

/**
  * @brief  Get the Minimum bound of CMP_LCA_COUNT, CMP_LCB_COUNT used when monitoring the MIN_LCAB_CNT
  * @rmtoll STAT          MIN_LCAB_CNT_BOUND       LL_LCSC_GetMinLcabCntBound
  * @retval the Minimum bound of CMP_LCA_COUNT
  */
__STATIC_INLINE uint32_t LL_LCSC_GetMinLcabCntBound(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->STAT, LCSC_STAT_MIN_LCAB_CNT_BOUND) >> LCSC_STAT_MIN_LCAB_CNT_BOUND_Pos);
}

/**
  * @brief  Set the Maximum bound of CMP_LCA_COUNT, CMP_LCB_COUNT used when monitoring the MAX_LCAB_CNT
  * @rmtoll STAT          MAX_LCAB_CNT_BOUND       LL_LCSC_SetMaxLcabCntBound
  * @param  LCSCx LCSC Instance
  * @param  value parameter must be a number between 0 and 255 (form 0x0 to 0xff)
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_SetMaxLcabCntBound(LCSC_TypeDef *LCSCx, uint32_t value)
{
  MODIFY_REG(LCSCx->STAT, LCSC_STAT_MAX_LCAB_CNT_BOUND, value << LCSC_STAT_MAX_LCAB_CNT_BOUND_Pos);
}

/**
  * @brief  Get the Maximum bound of CMP_LCA_COUNT, CMP_LCB_COUNT used when monitoring the MAX_LCAB_CNT 
  * @rmtoll STAT          MAX_LCAB_CNT_BOUND       LL_LCSC_GetMaxLcabCntBound
  * @retval the Maximum bound of CMP_LCA_COUNT 
  */
__STATIC_INLINE uint32_t LL_LCSC_GetMaxLcabCntBound(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->STAT, LCSC_STAT_MAX_LCAB_CNT_BOUND) >> LCSC_STAT_MAX_LCAB_CNT_BOUND_Pos);
}

/**
  * @brief  Get the Revision of the RFIP
  * @note   to be used for metal fixes
  * @rmtoll VER          REV       LL_LCSC_GetRevision
  * @retval the Revision of the RFIP 
  */
__STATIC_INLINE uint32_t LL_LCSC_GetRevision(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->VER, LCSC_VER_REV) >> LCSC_VER_REV_Pos);
}

/**
  * @brief  Get the Version of the RFIP
  * @note   to be used for cut upgrades
  * @rmtoll VER          VER       LL_LCSC_GetVersion
  * @retval the Version of the RFIP
  */
__STATIC_INLINE uint32_t LL_LCSC_GetVersion(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->VER, LCSC_VER_VER) >> LCSC_VER_VER_Pos);
}

/**
  * @brief  Get the Used for major upgrades
  * @rmtoll VER          PROD       LL_LCSC_GetProduct
  * @retval the Used for major upgrades
  */
__STATIC_INLINE uint32_t LL_LCSC_GetProduct(LCSC_TypeDef *LCSCx)
{
  return (uint32_t)(READ_BIT(LCSCx->VER, LCSC_VER_PROD) >> LCSC_VER_PROD_Pos);
}

/**
  * @brief  Indicate if CLKWISE_F is enabled. Clock Wise Flag
  * @note   0: counter CLKWISE has not reached CLKWISE_THRES
  *         1: counter CLKWISE has reached CLKWISE_THRES
  * @rmtoll ISR          CLKWISE_F            LL_LCSC_IsActiveFlag_CLKWISE_F 
  * @param  LCSCx LCSC Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCSC_IsActiveFlag_CLKWISE_F(LCSC_TypeDef *LCSCx)
{
  return ((READ_BIT(LCSCx->ISR, LCSC_ISR_CLKWISE_F) == (LCSC_ISR_CLKWISE_F)) ? 1U : 0U);
}

/**
  * @brief  Clear LCSC. Clock Wise Flag
  * @rmtoll ISR          CLKWISE_F            LL_LCSC_ClearFlag_CLKWISE_F
  * @param  LCSCx LCSC Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_ClearFlag_CLKWISE_F(LCSC_TypeDef *LCSCx)
{
  SET_BIT(LCSCx->ISR, LCSC_ISR_CLKWISE_F);
}

/**
  * @brief  Indicate if ACLKWISE_F is enabled. Anti Clock Wise Flag
  * @note   0: counter ACLKWISE has not reached ACLKWISE_THRES
  *         1: counter CLKWISE has reached CLKWISE_THRES
  * @rmtoll ISR          ACLKWISE_F            LL_LCSC_IsActiveFlag_ACLKWISE_F 
  * @param  LCSCx LCSC Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCSC_IsActiveFlag_ACLKWISE_F(LCSC_TypeDef *LCSCx)
{
  return ((READ_BIT(LCSCx->ISR, LCSC_ISR_ACLKWISE_F) == (LCSC_ISR_ACLKWISE_F)) ? 1U : 0U);
}

/**
  * @brief  Clear LCSC. Anti Clock Wise Flag
  * @rmtoll ISR          ACLKWISE_F            LL_LCSC_ClearFlag_ACLKWISE_F
  * @param  LCSCx LCSC Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_ClearFlag_ACLKWISE_F(LCSC_TypeDef *LCSCx)
{
  SET_BIT(LCSCx->ISR, LCSC_ISR_ACLKWISE_F);
}

/**
  * @brief  Indicate if TAMP_F is enabled. Tamper Flag
  * @note   0: tamper not detected
  *         1: tamper detected
  * @rmtoll ISR          TAMP_F            LL_LCSC_IsActiveFlag_TAMP_F 
  * @param  LCSCx LCSC Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCSC_IsActiveFlag_TAMP_F(LCSC_TypeDef *LCSCx)
{
  return ((READ_BIT(LCSCx->ISR, LCSC_ISR_TAMP_F) == (LCSC_ISR_TAMP_F)) ? 1U : 0U);
}

/**
  * @brief  Clear LCSC. Tamper Flag
  * @rmtoll ISR          TAMP_F            LL_LCSC_ClearFlag_TAMP_F
  * @param  LCSCx LCSC Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_ClearFlag_TAMP_F(LCSC_TypeDef *LCSCx)
{
  SET_BIT(LCSCx->ISR, LCSC_ISR_TAMP_F);
}

/**
  * @brief  Indicate if CNT_OFB_F is enabled. Out of Bound Counter Flag
  * @note   0: Out of Bound not detected
  *         1: Out of Bound detected
  * @rmtoll ISR          CNT_OFB_F            LL_LCSC_IsActiveFlag_CNT_OFB_F 
  * @param  LCSCx LCSC Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_LCSC_IsActiveFlag_CNT_OFB_F(LCSC_TypeDef *LCSCx)
{
  return ((READ_BIT(LCSCx->ISR, LCSC_ISR_CNT_OFB_F) == (LCSC_ISR_CNT_OFB_F)) ? 1U : 0U);
}

/**
  * @brief  Clear LCSC. Out of Bound Counter Flag
  * @rmtoll ISR          CNT_OFB_F            LL_LCSC_ClearFlag_CNT_OFB_F
  * @param  LCSCx LCSC Instance
  * @retval None
  */
__STATIC_INLINE void LL_LCSC_ClearFlag_CNT_OFB_F(LCSC_TypeDef *LCSCx)
{
  SET_BIT(LCSCx->ISR, LCSC_ISR_CNT_OFB_F);
}


/**
  * @}
  */


/* Exported functions --------------------------------------------------------*/

/**
  * @}
  */

#endif /* defined(LCSC) */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* RF_DRIVER_LL_LCSC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
