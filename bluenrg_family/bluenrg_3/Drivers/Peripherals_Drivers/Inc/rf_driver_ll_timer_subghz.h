/**
******************************************************************************
* @file    rf_driver_ll_timer_subghz.h
* @author  RF Application Team
* @brief   This file contains all the functions prototypes for the TIMER 
*          firmware library.
******************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2023 STMicroelectronics</center></h2>
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef RF_DRIVER_LL_TIMER_SUBGHZ_H
#define RF_DRVIER_LL_TIMER_SUBGHZ_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "spirit3.h"

/** @addtogroup RF_DRIVER_LL_Driver
  * @{
  */

/** @addtogroup TIMER
  * @{
  */ 

/** @defgroup TIMER_Exported_Types TIMER Exported Types
  * @{
  */ 

/** 
  * @brief Structure definition of TIMER initialization
  */ 
typedef struct
{
  uint16_t XTAL_StartupTime;      /*!< XTAL startup in us */
 /**
 * Enable initial estimation of the frequency of the Low Speed Oscillator, otherwise it will be assumed fixed at 32.768 kHz.
 * Ignored if periodic calibration is active (TIMER_PeriodicCalibration = TRUE).
 */
  BOOL TIMER_InitialCalibration;
  BOOL TIMER_PeriodicCalibration; /*!< Assume that 32 KHz clock requires periodic calibration, i.e. varies with temperature */
} TIMER_InitType;

typedef struct timer_calibration_s {
  uint64_t last_calibration_time; /** Absolute system time when last calibration was performed */
  uint32_t slow_clock_freq; /** Slow clock freq */
  uint8_t  periodic_calibration; /** Tells whether periodic hardware calibration is needed or not */
  uint32_t last_calibration_machine_time; /** Last machine time when calibration was performed */
} TIMER_CalibrationType;

/**
  * @}
  */ 

#define CPU_WAKEUP_TIMER_BUSY   (0x01U)

/** This define represents the marging we take for long sleep to allow the system to
    avoid to have the counter wrapping. It is expressed in machine time, so it is variable
    when using internal RO */
#define TIMER_WRAPPING_MARGIN (4096)

/** This #define represents the number of significant bits in the timer */
#define TIMER_BITS (32)
#define TIMER_MAX_VALUE (0xFFFFFFFFU >> (32-TIMER_BITS))

/**
  * @}
  */ 

/** @defgroup TIMER_Exported_Macros TIMER Exported Macros
  * @{
  */
#define TIMER_GET_SLOW_COUNTER_VAL          (MR_SUBG_GLOB_MISC->SCM_COUNTER_VAL & MR_SUBG_GLOB_MISC_SCM_COUNTER_VAL_SCM_COUNTER_CURRVAL)

#define TIMER_DISABLE_RFIP_WAKEUP_TIMER     MR_SUBG_GLOB_RETAINED->WAKEUP_CTRL &= ~(MR_SUBG_GLOB_RETAINED_WAKEUP_CTRL_RFIP_WAKEUP_EN)
#define TIMER_DISABLE_CPU_WAKEUP_TIMER      MR_SUBG_GLOB_RETAINED->WAKEUP_CTRL &= ~(MR_SUBG_GLOB_RETAINED_WAKEUP_CTRL_CPU_WAKEUP_EN)


#define TIMER_DISABLE_RADIO_WAKEUP_TIMER    TIMER_DISABLE_RFIP_WAKEUP_TIMER

#define TIMER_GET_RFIP_WAKEUP_EN            MR_SUBG_GLOB_RETAINED->WAKEUP_CTRL & MR_SUBG_GLOB_RETAINED_WAKEUP_CTRL_RFIP_WAKEUP_EN
#define TIMER_GET_CPU_WAKEUP_EN             MR_SUBG_GLOB_RETAINED->WAKEUP_CTRL & MR_SUBG_GLOB_RETAINED_WAKEUP_CTRL_CPU_WAKEUP_EN


#define TIMER_ENABLE_RFIP_WAKEUP_TIMER      MR_SUBG_GLOB_RETAINED->WAKEUP_CTRL |= MR_SUBG_GLOB_RETAINED_WAKEUP_CTRL_RFIP_WAKEUP_EN
#define TIMER_ENABLE_CPU_WAKEUP_TIMER       MR_SUBG_GLOB_RETAINED->WAKEUP_CTRL |= MR_SUBG_GLOB_RETAINED_WAKEUP_CTRL_CPU_WAKEUP_EN
    
#define TIMER_CLEAR_RFIP_WAEKUP_INT         MR_SUBG_GLOB_MISC->WAKEUP_IRQ_STATUS = MR_SUBG_GLOB_MISC_WAKEUP_IRQ_STATUS_RFIP_WAKEUP_F
#define TIMER_CLEAR_CPU_WAEKUP_INT          MR_SUBG_GLOB_MISC->WAKEUP_IRQ_STATUS = MR_SUBG_GLOB_MISC_WAKEUP_IRQ_STATUS_CPU_WAKEUP_F

#define TIMER_GET_RFIP_WAKEUP_INT_STATUS    MR_SUBG_GLOB_MISC->WAKEUP_IRQ_STATUS & MR_SUBG_GLOB_MISC_WAKEUP_IRQ_STATUS_RFIP_WAKEUP_F
#define TIMER_GET_CPU_WAKEUP_INT_STATUS     MR_SUBG_GLOB_MISC->WAKEUP_IRQ_STATUS & MR_SUBG_GLOB_MISC_WAKEUP_IRQ_STATUS_CPU_WAKEUP_F

#define TIMER_RC64PLL_LOCKED                RCC->CR & RCC_CR_HSIPLLRDY

/**
  * @}
  */ 


/** @defgroup TIMER_Exported_Functions TIMER Exported Functions
  * @{
  */

/**
  * @brief  Initialize the TIMER functionality
  * @retval None
  */
void TIMER_Init(TIMER_InitType* TIMER_InitStruct);

/**
  * @brief  Perform a low speed clock calibration and store results in the global context.
  *         It updates the XTAL startup time and the interpolator configuration.
  *         It updates also the cumulative time variable, so it should be called peiodically to manage timer wrapping,
  *         regardless of actual need of periodic calibration.
  *         This function is not re-entrant since it updates the context variable storing the system time.
  * @retval None
  */
void TIMER_Calibrate(void);

/**
  * @brief  Return TRUE if a calibration is on going.
  * @retval TRUE if calibration is running, FALSE otherwise.
  */
BOOL TIMER_IsCalibrationRunning(void);

/**
  * @brief   Records the result of the last calibration in the internal context.
  *          It updates the XTAL startup time.
  *          It updates also the cumulative system time variable, so it should be called peiodically to manage timer wrapping,
  *          regardless of actual need of periodic calibration.
  * @warning This function is not re-entrant since it updates the context variable storing the system time.
  *          It should be called only in user context and not in interrupt context.
  * @retval  None
  */
void TIMER_UpdateCalibrationData(void);

/**
  * @brief   Set the wakeup time to the specified delay. Only 28 most significant bits of the delay
  *          are taken into account. The XTAL startup time is not taken into account for the wakeup, i.e. the system does not wait for
  *          the XTAL startup time parameter to trigger the interrupt.
  *          The delay is translated into machine time unit (MTU) and it is assigned to the wakeup register.
  * @param   delay: Delay from the current time expressed in system time unit (us). Range is 0 to maximum value of STU. 
  *          The maximum value STU is dependent on the speed of the low speed oscillator.
  *          A value too small will force the timer to wrap, so it is reccommended to use at least 5-10 STU.
  * @warning This function should be called with interrupts disabled to avoid programming the timer with a value in the past
  * @return  Current time in MTU.
  */
uint32_t TIMER_SetWakeupTime(uint32_t delay);

/**
  * @brief  Return the current calibration data.
  * @retval None
  */
void TIMER_GetCurrentCalibrationData(TIMER_CalibrationType *data);

/**
  * @brief   Records the calibration interval. This information 
  *          is used to understand if an absolute time register 
  *          wrap happens since the device startup configration.
  * @param   time: Calibration interval (STU)
  * @warning This function is not re-entrant since it updates the context variable storing the system time.
  *          It should be called only in user context and not in interrupt context.
  * @retval  None
  */
void TIMER_SaveCalibrationInterval(uint32_t time);

/**
 * @brief  Return the current system time (STU). The STU is expressed in us.
 *         This is a counter that grows since the power up of the system and it never wraps.
 * @return Current system time
 */
uint64_t TIMER_GetCurrentSysTime(void);

/**
  * @brief  Return the status of the Radio timer and the last value programmed in the register.
  * @param  time: pointer to value which is going to have time value (MTU).
  * @retval 0 if no timer has been programmed.
  * @retval 1 if CPU Wakeup Timer has been programmed.
  */
uint8_t TIMER_GetRadioTimerValue(uint32_t *time);

/**
  * @brief   Return the system time referred to the absolute machine time passed as parameter.
  * @param   time: Absolute machine time in the past
  * @warning User should guarantee that call to this function are performed in a non-interruptible context.
  * @return  STU value 
  */
uint64_t TIMER_GetPastSysTime(uint32_t time);

/**
  * @brief  Translate time in microseconds into machine time units.
  * @return MTU value 
  */
uint64_t TIMER_UsToMachinetime(uint64_t time);

/**
 * @brief   Return the microseconds corresponding to the MTU passed as parameter.
 * @param   time: MTU amount to be converted in us
 * @warning This function is not re-entrant since it updates the context variable storing the system time. It should be called only in
 *          user context and not in interrupt context.
 * @return  time expressed in microseconds 
 */
uint64_t TIMER_MachineTimeToUs(uint64_t time);

/**
  * @brief Enable CPU wakeup interrupt towards the CPU.
  */
void TIMER_Enable_CPU_WKUP(void);

/**
  * @brief Disable CPU wakeup interrupt towards the CPU.
  */
void TIMER_Disable_CPU_WKUP(void);

/**
 * @brief Get the current absolute time expressed in machine absolute time unit.
 */
__STATIC_INLINE uint32_t TIMER_GetCurrentMachineTime(void) { return (MR_SUBG_GLOB_MISC->ABSOLUTE_TIME&TIMER_MAX_VALUE); }

/**
 * @brief Set the value of a host timeout in machine absolute time units.
 */
__STATIC_INLINE void TIMER_SetHostTimeoutReg(uint32_t value) { MR_SUBG_GLOB_RETAINED->CPU_WAKEUPTIME = value; }

/**
 * @brief Get the value of a host timeout in machine absolute time units.
 */
__STATIC_INLINE uint32_t TIMER_GetHostTimeoutReg(void) { return MR_SUBG_GLOB_RETAINED->CPU_WAKEUPTIME; }

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif /* RF_DRIVER_LL_TIMER_SUBGHZ_H */

/******************* (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
