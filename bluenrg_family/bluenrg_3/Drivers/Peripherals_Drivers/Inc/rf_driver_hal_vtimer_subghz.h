/**
******************************************************************************
* @file    rf_driver_hal_vtimer_subghz.h 
* @author  RF Application Team
* @brief   Virtual timer high level APIs
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
#ifndef RF_DRIVER_HAL_VTIMER_SUBGZ_H
#define RF_DRIVER_HAL_VTIMER_SUBGZ_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "rf_driver_ll_timer_subghz.h"
#include "rf_driver_hal_power_manager.h"

/** @addtogroup RF_DRIVER_HAL_Driver
  * @{
  */ 

/** @addtogroup VTIMER
  * @{
  */

/** @defgroup VTIMER_Exported_Types VTIMER Exported Types
  * @{
  */ 

typedef enum {
  RADIO_TIMER_OFF = 0,
  RADIO_TIMER_PENDING = 1,
} TimerStatus; 

typedef void (*VTIMER_CallbackType)(void *);
typedef struct VTIMER_HandleTypeS {
	uint64_t expiryTime; /*!< Managed internally when the timer is started */
	VTIMER_CallbackType callback; /*!< Pointer to the user callback */
	BOOL active; /*!< Managed internally when the timer is started */
	struct VTIMER_HandleTypeS *next; /*!< Managed internally when the timer is started */
	void *userData; /*!< Pointer to user data */
} VTIMER_HandleType;

typedef struct HAL_VTIMER_InitS {
  uint16_t XTAL_StartupTime;             /*!< XTAL startup in us */
 /**
 * Enable initial estimation of the frequency of the Low Speed Oscillator, otherwise it will be assumed fixed at 32.768 kHz.
 * Ignored if periodic calibration is active (PeriodicCalibrationInterval != 0).
 */
  BOOL EnableInitialCalibration;                
  uint32_t PeriodicCalibrationInterval;  /*!< Periodic calibration interval in ms, to disable set to 0 */
} HAL_VTIMER_InitType;

/**
  * @}
  */ 

/** @defgroup VTIMER_Exported_Constants VTIMER Exported Constants
* @{
*/

/**
* @}
*/ 

/** @defgroup VTIMER_Exported_Functions VTIMER Exported Functions
  * @{
  */

/**
 * @brief  Starts a one-shot virtual timer for the given relative timeout value expressed in ms
 * @param  timerHandle: The virtual timer
 * @param  msRelTimeout: The relative time, from current time, expressed in ms
 * @retval 0 if the timerHandle is valid.
 * @retval 1 if the timerHandle is not valid or it is already started.
 */
int HAL_VTIMER_StartTimerMs(VTIMER_HandleType *timerHandle, uint32_t msRelTimeout);

/**
 * @brief  Stops the one-shot virtual timer specified if found
 * @param  timerHandle: The virtual timer
 * @retval None
 */
void HAL_VTIMER_StopTimer(VTIMER_HandleType *timerHandle);

/**
 * @brief  This function returns the current reference system time expressed in system time units (us).
 *         The returned value can be used as absolute time parameter where needed in the other
 *         HAL_VTIMER* APIs
 * @return absolute current time expressed in system time units (us).
 */
uint64_t HAL_VTIMER_GetCurrentSysTime(void);

/**
 * @brief  Starts a one-shot virtual timer for the given absolute timeout value
 *         expressed in internal time units (us).
 * @param  timerHandle: The virtual timer
 * @param  time: Absolute time expressed in us.
 * @retval 0 if the timerHandle is valid.
 * @retval 1 if the timerHandle is not valid. It is already started.
*/
int HAL_VTIMER_StartTimerSysTime(VTIMER_HandleType *timerHandle, uint64_t time);

/**
 * @brief Returns the absolute expiry time of a running timer expressed in system time units (us).
 * @param timerHandle: The virtual timer
 * @param sysTime: Absolute time expressed in system time units (us).
 */
int HAL_VTIMER_ExpiryTime(VTIMER_HandleType *timerHandle, uint64_t *sysTime);

/**
 * @brief  Initialize the timer module. It must be placed in the initialization
 *         section of the application.
 * @retval None
*/
void HAL_VTIMER_Init(HAL_VTIMER_InitType *HAL_TIMER_InitStruct);

/**
 * @brief  Timer module state machine. Check and schedule the calibration.
 *         Check expired timers and execute user callback.
 *         It must be placed inside the infinite loop.
 * @retval None
*/
void HAL_VTIMER_Tick(void);

/**
 * @brief  Return the consensus of the Virtual timer management to go in sleep.
 * @retval TRUE if all vtimers have been served and the calibration procedure has already finished. 
 * @retval FALSE if the vtimer Tick is still busy.
*/
BOOL HAL_VTIMER_SleepCheck(void);

/**
 * @brief  Virtual timer Timeout Callback. It signals that a host timeout occured.
 * @retval None
 */
void HAL_VTIMER_TimeoutCallback(void);

/**
 * @brief  Returns the number of timers in the queue.
 * @return Number of timers in the queue.
 */
uint32_t HAL_VTIMER_GetPendingTimers(void);


/**
 * @brief  Returns the admitted sleep mode according to the next timer activity.
 * @return Sleep mode 
*/
PowerSaveLevels HAL_VTIMER_PowerSaveLevelCheck(PowerSaveLevels level);

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

#endif /*RF_DRIVER_HAL_VTIMER_SUBGHZ_H */
/******************* (C) COPYRIGHT STMicroelectronics *****END OF FILE****/  
