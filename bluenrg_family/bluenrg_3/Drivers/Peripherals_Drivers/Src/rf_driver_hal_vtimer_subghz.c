/**
******************************************************************************
* @file    rf_driver_hal_vtimer_subghz.c 
* @author  RF Application Team
* @brief   Virtual timer and Radio timer high level APIs
* @details This file implements the software layer that provides the virtualization of the
* resources of a single hardware timer in order to allocate many user virtual timers.
* The only constraint to the number of virtual timers is the memory.
* Each instance of a virtual timer is placed in an queue ordered by the expiration time
* and it can be linked to a callback.
* The timer tick is in charge to execute the callback linked to each virtual timer
* and to update the hardware timeout to gurantee the expiration of the next virtual
* timer in the queue.
* A special virtual timer called calibration/anti-wrapping timer is automatically armed
* by the software. This timer can address two tasks:
* - it is in charge to maintain the never wrapping virtual time base.
* - if the slow clock calibration is enabled, it starts the calibration procedure at each
* calibration interval specified during the initialization.
*
* A timer is intended as an event programmed in the future at a certain absolute expiration time
* on a time base. In this implementation the time base grows on 64 bits. Then, it never wraps.
* However, due to hardware timer finite length and in order to maintain the timing coherency, the time base
* must be maintained at least one time before the hardware timer wraps.  
* Then even if the slow clock calibration is disabled, the calibration/anti-wrapping timer
* is always active with the only role to maintain the time base and it will expire
* at a rate that depends on the hardware timer capability.
* The time base unit is a MTU that is the unit exposed to the user and it is 
* equal to interpolated counter (1/16*slow_clock_frequency)us. 
* The calibration/anti-wrapping mechanism is not managed by the user.
*
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
/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include <stdio.h>
#include "rf_driver_hal_vtimer_subghz.h"

/** @addtogroup RF_DRIVER_HAL_Driver
  * @{
  */

/** @defgroup VTIMER VTIMER
  * @brief VTIMER module driver
  * @{
  */

/** @defgroup VTIMER_Private_Macros VTIMER Private Macros
  * @{
  */

#define ATOMIC_SECTION_BEGIN() uint32_t uwPRIMASK_Bit = __get_PRIMASK(); \
                                __disable_irq(); \
/* Must be called in the same scope of ATOMIC_SECTION_BEGIN */
#define ATOMIC_SECTION_END() __set_PRIMASK(uwPRIMASK_Bit)

#define MAX(a,b) ((a) < (b) )? (b) : (a)
#define MIN(a,b) ((a) < (b) )? (a) : (b)
#define DIFF8(a,b) ((a)>=(b) ? ((a)-(b)) : (256+((a)-(b))))
#define INCREMENT_EXPIRE_COUNT_ISR (HAL_VTIMER_Context.expired_count = ((HAL_VTIMER_Context.expired_count + 1) == HAL_VTIMER_Context.served_count) ? HAL_VTIMER_Context.expired_count : (HAL_VTIMER_Context.expired_count + 1))
#define INCREMENT_EXPIRE_COUNT ATOMIC_SECTION_BEGIN(); INCREMENT_EXPIRE_COUNT_ISR ; ATOMIC_SECTION_END();

/**
  * @}
  */

/** @defgroup VTIMER_Private_Constants VTIMER Private Constants
  * @{
  */
/* Extra margin to consider before going in low power mode */
#define LOW_POWER_THR (30) //??? [FC]

#define FIVE_SECONDS 5000000
/**
  * @}
  */

/** @defgroup VTIMER_Private_Types VTIMER Private Types
  * @{
  */

typedef struct HAL_VTIMER_ContextS {
  uint64_t PeriodicCalibrationInterval;  /*!< Periodic calibration interval in us, to disable set to 0 */
  BOOL calibration_in_progress;  /*!< Flag to indicate that a periodic calibration has been started */
  VTIMER_HandleType *rootNode; /*!< First timer of the host timer queue */
  BOOL enableTimeBase;      /*!< Internal flag. User can ignore it*/
  uint32_t hs_startup_time; /*!< HS startup time */
  uint8_t expired_count; /*!< Progressive number to indicate expired timers */
  uint8_t served_count; /*!< Progressive number to indicate served expired timers */
} HAL_VTIMER_ContextType;

/**
  * @}
  */

/** @defgroup VTIMER_Private_Variables VTIMER Private Variables
  * @{
  */

static HAL_VTIMER_ContextType HAL_VTIMER_Context;
static VTIMER_HandleType calibrationTimer;
static TIMER_CalibrationType calibrationData;

/**
  * @}
  */


/** @defgroup VTIMER_Private_Function VTIMER Private Function 
  * @{
  */

static void calibration_callback (void *handle)
{
  HAL_VTIMER_Context.calibration_in_progress = TRUE;
}

static VTIMER_HandleType * _remove_timer_in_queue(VTIMER_HandleType *rootNode, VTIMER_HandleType *handle)
{
  VTIMER_HandleType *current = rootNode;
  VTIMER_HandleType *prev = NULL;
  VTIMER_HandleType *returnValue = rootNode;
  
  while ((current!=NULL) && (current != handle)) {
    prev = current;
    current=current->next;
  }
  
  if (current == NULL) {
    /* Not found */
  }
  else if (current == rootNode) {
    /* New root node */
    returnValue = current->next;
  }
  else {
    prev->next = current->next;
  }
  
  return returnValue;
}

static VTIMER_HandleType * _insert_timer_in_queue(VTIMER_HandleType *rootNode, VTIMER_HandleType *handle)
{
  VTIMER_HandleType *current = rootNode;
  VTIMER_HandleType *prev = NULL;
  VTIMER_HandleType *returnValue = rootNode;
  
  while ((current!=NULL) && (current->expiryTime < handle->expiryTime)) {
    prev = current;
    current=current->next;
  }
  
  handle->next = current;
  
  if (prev == NULL) {
    /* We are the new root */
    returnValue = handle;
  }
  else {
    prev->next = handle;
  }
  
  return returnValue;
}

/* Set timeout and skip non active timers */
static VTIMER_HandleType *_update_user_timeout(VTIMER_HandleType *rootNode, uint8_t *expired)
{
  VTIMER_HandleType *curr = rootNode;
  VTIMER_HandleType *rootOrig = rootNode;
  int64_t delay;

  *expired =0;
  while (curr != NULL) {
    if (curr->active) {
      ATOMIC_SECTION_BEGIN();
      delay = curr->expiryTime-TIMER_GetCurrentSysTime();
      if (delay > 0) {
        /* Protection against interrupt must be used to avoid that the called function will be interrupted
          and so the timer programming will happen after the target time is already passed
          leading to a timer expiring after timer wraps, instead of the expected delay */
        TIMER_SetWakeupTime(delay);
        ATOMIC_SECTION_END();
        break;
      }
      else {
        *expired = 1;
        ATOMIC_SECTION_END();
        break;
      }
    }
    curr=curr->next;
  }
  if (*expired)
    return rootOrig;
  return curr;
}

/* Check the number of expired timer from rootNode (ordered list of timers) and return the list of expired timers */
static VTIMER_HandleType *_check_callbacks(VTIMER_HandleType *rootNode,VTIMER_HandleType **expiredList)
{
  
  VTIMER_HandleType *curr = rootNode;
  VTIMER_HandleType *prev = NULL;
  VTIMER_HandleType *returnValue = rootNode;
  *expiredList = rootNode;
  
  int64_t delay;
  uint32_t expiredCount = 0;
  
  while (curr != NULL) {
    
    if (curr->active) {
      delay = curr->expiryTime-TIMER_GetCurrentSysTime();
      
      if (delay > 5) { /*TBR*/
        /* End of expired timers list*/
        break;
      }
    } 
    
    prev = curr;
    curr=curr->next;
    expiredCount++;
  }
  
  if (expiredCount) {
    /* Some timers expired */
    prev->next=NULL;
    returnValue = curr;
  }
  else {
    /* No timer expired */
    *expiredList = NULL;
  }
  
  return returnValue;
}

static int _start_timer(VTIMER_HandleType *timerHandle, uint64_t time)
{
  uint8_t expired = 0;

  /* The timer is already started*/
  if(timerHandle->active)
  {
    return 1;
  }
  timerHandle->expiryTime = time;
  timerHandle->active = TRUE;
  if (_insert_timer_in_queue(HAL_VTIMER_Context.rootNode, timerHandle) == timerHandle) {
    HAL_VTIMER_Context.rootNode = _update_user_timeout(timerHandle, &expired);
    if (expired) {
      /* A new root timer is already expired, mimic timer expire that is normally signaled
       through the interrupt handler that increase the number of expired timers*/
      INCREMENT_EXPIRE_COUNT;
    }
  }
  return expired;
}

static void _virtualTimeBaseEnable(FunctionalState state)
{
  if(state != DISABLE)
  {
    if (HAL_VTIMER_Context.enableTimeBase == FALSE)
    {
      calibration_callback(&calibrationTimer);
      HAL_VTIMER_Context.enableTimeBase = TRUE;
    }
  }
  else
  {
    HAL_VTIMER_StopTimer(&calibrationTimer);
    HAL_VTIMER_Context.enableTimeBase = FALSE;
  }
}

/**
  * @}
  */

/** @defgroup VTIMER_Exported_Functions VTIMER Exported Functions
  * @{
  */

/**
 * @brief  Virtual timer Timeout Callback. It signals that a host timeout occured.
 * @retval None
 */
void HAL_VTIMER_TimeoutCallback(void)
{ 
  TIMER_DISABLE_CPU_WAKEUP_TIMER;
  INCREMENT_EXPIRE_COUNT_ISR;
  TIMER_CLEAR_CPU_WAEKUP_INT;
}

/**
 * @brief  Starts a one-shot virtual timer for the given absolute timeout value
 *         expressed in system time units (us).
 * @param  timerHandle: The virtual timer
 * @param  time: Absolute time expressed in us.
 * @retval 0 if the timerHandle is valid.
 * @retval 1 if the timerHandle is not valid. It is already started.
*/
int HAL_VTIMER_StartTimerSysTime(VTIMER_HandleType *timerHandle, uint64_t time)
{
  uint8_t retVal;
  retVal = _start_timer(timerHandle, time);
  _virtualTimeBaseEnable(ENABLE);
  return retVal;
}

/**
 * @brief  Starts a one-shot virtual timer for the given relative timeout value expressed in ms
 * @param  timerHandle: The virtual timer
 * @param  msRelTimeout: The relative time, from current time, expressed in ms
 * @retval 0 if the timerHandle is valid.
 * @retval 1 if the timerHandle is not valid. It is already started.
 */
int HAL_VTIMER_StartTimerMs(VTIMER_HandleType *timerHandle, uint32_t msRelTimeout)
{
  uint64_t temp = msRelTimeout*1000;
  uint8_t retVal;
 
  retVal = _start_timer(timerHandle, (TIMER_GetCurrentSysTime() + temp));
  _virtualTimeBaseEnable(ENABLE);
 
  return retVal;
}

/**
 * @brief  Stops the one-shot virtual timer specified if found
 * @param  timerHandle: The virtual timer
 * @retval None
 */
void HAL_VTIMER_StopTimer(VTIMER_HandleType *timerHandle)
{
  VTIMER_HandleType *rootNode = _remove_timer_in_queue(HAL_VTIMER_Context.rootNode, timerHandle);
  uint8_t expired = 0;
  timerHandle->active=FALSE;
  if (HAL_VTIMER_Context.rootNode != rootNode) {
    HAL_VTIMER_Context.rootNode = _update_user_timeout(rootNode, &expired);
    if (expired) {
      /* A new root timer is already expired, mimic timer expire */
      INCREMENT_EXPIRE_COUNT;
    }
  }
  else
     HAL_VTIMER_Context.rootNode = rootNode;
}	

/**
 * @brief  This function returns the current reference time expressed in system time units.
 *         The returned value can be used as absolute time parameter where needed in the other
 *         HAL_VTIMER* APIs
 * @return absolute current time expressed in system time units.
 */
uint64_t HAL_VTIMER_GetCurrentSysTime(void)
{
  return TIMER_GetCurrentSysTime();
}

/**
 * @brief Returns the absolute expiry time of a running timer expressed in internal time units.
 * @param timerHandle: The virtual timer
 * @param sysTime: Absolute time expressed in internal time units.
 */
int HAL_VTIMER_ExpiryTime(VTIMER_HandleType *timerHandle, uint64_t *sysTime)
{
  *sysTime = timerHandle->expiryTime;
  return 0;
}

/**
 * @brief  Returns the number of timers in the queue.
 * @return number of timers in the queue.
 */
uint32_t HAL_VTIMER_GetPendingTimers(void)
{
  VTIMER_HandleType *curr = HAL_VTIMER_Context.rootNode;
  uint32_t counter = 0;
  while (curr != NULL) {
    counter++;
    curr = curr->next;
  }
  return counter;
}

/**
 * @brief  Initialize the timer module. It must be placed in the initialization
 *         section of the application.
 * @retval None
 */
void HAL_VTIMER_Init(HAL_VTIMER_InitType *HAL_TIMER_InitStruct)
{
  TIMER_InitType TIMER_InitStruct;
  TIMER_Enable_CPU_WKUP();

  TIMER_InitStruct.TIMER_InitialCalibration = HAL_TIMER_InitStruct->EnableInitialCalibration;
  TIMER_InitStruct.TIMER_PeriodicCalibration = (HAL_TIMER_InitStruct->PeriodicCalibrationInterval!=0);
  TIMER_InitStruct.XTAL_StartupTime = HAL_TIMER_InitStruct->XTAL_StartupTime;
  TIMER_Init(&TIMER_InitStruct);
  TIMER_GetCurrentCalibrationData(&calibrationData); 
  HAL_VTIMER_Context.rootNode = NULL;
  HAL_VTIMER_Context.enableTimeBase = TRUE;
  HAL_VTIMER_Context.hs_startup_time = HAL_TIMER_InitStruct->XTAL_StartupTime;
  HAL_VTIMER_Context.expired_count=0;
  HAL_VTIMER_Context.served_count=0;
  HAL_VTIMER_Context.PeriodicCalibrationInterval = HAL_TIMER_InitStruct->PeriodicCalibrationInterval*1000; /* Calibration interval expressed in microsecond */
  HAL_VTIMER_Context.calibration_in_progress = FALSE;
  if (HAL_VTIMER_Context.PeriodicCalibrationInterval == 0)
    HAL_VTIMER_Context.PeriodicCalibrationInterval = TIMER_MachineTimeToUs(TIMER_MAX_VALUE-TIMER_WRAPPING_MARGIN);
  else
    HAL_VTIMER_Context.PeriodicCalibrationInterval = MIN(HAL_VTIMER_Context.PeriodicCalibrationInterval,
                                                         TIMER_MachineTimeToUs(TIMER_MAX_VALUE-TIMER_WRAPPING_MARGIN));
  calibrationTimer.callback = calibration_callback;
  calibrationTimer.userData = NULL;
  _start_timer(&calibrationTimer, TIMER_GetCurrentSysTime() + HAL_VTIMER_Context.PeriodicCalibrationInterval);
  TIMER_SaveCalibrationInterval(HAL_VTIMER_Context.PeriodicCalibrationInterval);  
}

/**
 * @brief  Timer module state machine. Check and schedule the calibration.
 *         Check expired timers and execute user callback.
 *         It must be placed inside the infinite loop.
 * @retval None
 */
void HAL_VTIMER_Tick(void)
{
  uint8_t expired = 0;

  /* Check for expired timers */  
  while (DIFF8(HAL_VTIMER_Context.expired_count,HAL_VTIMER_Context.served_count)) {
    VTIMER_HandleType *expiredList, *curr;
    uint8_t to_be_served = DIFF8(HAL_VTIMER_Context.expired_count,HAL_VTIMER_Context.served_count);
    
    HAL_VTIMER_Context.rootNode = _check_callbacks(HAL_VTIMER_Context.rootNode, &expiredList);
    
    /* Call all the user callbacks */
    curr=expiredList;
    while (curr != NULL) {

      /* Save next pointer, in case callback start the timer again */
      VTIMER_HandleType *next = curr->next;
      curr->active = FALSE;
      if (curr->callback)
        curr->callback(curr); /* we are sure a callback is set?*/
      curr = next;
    }
    
    HAL_VTIMER_Context.rootNode = _update_user_timeout(HAL_VTIMER_Context.rootNode, &expired);
    if (expired == 1) {
      /* A new root timer is already expired, mimic timer expire */
      INCREMENT_EXPIRE_COUNT;
    }
    HAL_VTIMER_Context.served_count += to_be_served;
  }
  
  /* Check for periodic calibration */
  if (HAL_VTIMER_Context.calibration_in_progress) {
     if (TIMER_IsCalibrationRunning() == FALSE) {
      /* Calibration is completed */
      HAL_VTIMER_Context.calibration_in_progress = FALSE;
      /* Collect calibration data */
      TIMER_UpdateCalibrationData();
      TIMER_GetCurrentCalibrationData(&calibrationData);
      HAL_VTIMER_StopTimer(&calibrationTimer);
      /* Schedule next calibration event */
      _start_timer(&calibrationTimer, TIMER_GetCurrentSysTime() + HAL_VTIMER_Context.PeriodicCalibrationInterval);
    }		
  }
  /* if there is a periodic calibration, start it in advance during the active phase */
  else{
    if(calibrationData.periodic_calibration){
      if( TIMER_GetCurrentSysTime() > (calibrationData.last_calibration_time + FIVE_SECONDS))
      {
        calibration_callback(&calibrationTimer);
      }
    }
  }
}

/**
 * @brief  Returns the admitted sleep mode according to the next timer activity.
 * @return Sleep mode 
 */
PowerSaveLevels HAL_VTIMER_PowerSaveLevelCheck(PowerSaveLevels level)
{
  if (HAL_VTIMER_SleepCheck() == FALSE)
  {
    return POWER_SAVE_LEVEL_RUNNING;
  }

  if(HAL_VTIMER_Context.rootNode != NULL && HAL_VTIMER_Context.rootNode->active)
  {
    if(HAL_VTIMER_Context.rootNode->expiryTime < (TIMER_GetCurrentSysTime() + LOW_POWER_THR + HAL_VTIMER_Context.hs_startup_time))
    {
      return POWER_SAVE_LEVEL_CPU_HALT;
    }
    
    if(level == POWER_SAVE_LEVEL_STOP_NOTIMER)
    {
      if((HAL_VTIMER_Context.rootNode->next == NULL) && (HAL_VTIMER_Context.rootNode == &calibrationTimer))
      {
        _virtualTimeBaseEnable(DISABLE);
        TIMER_DISABLE_CPU_WAKEUP_TIMER;
        return POWER_SAVE_LEVEL_STOP_NOTIMER;
      }
    }
    level = POWER_SAVE_LEVEL_STOP_WITH_TIMER;
  }
  return level;
}

/**
 * @brief  Return the consensus of the Virtual timer management to go in sleep.
 * @retval TRUE if all vtimers have been served and the calibration procedure has already finished. 
 * @retval FALSE if the vtimer Tick is still busy.
 */
BOOL HAL_VTIMER_SleepCheck(void)
{    
  return ((HAL_VTIMER_Context.expired_count == HAL_VTIMER_Context.served_count) && (HAL_VTIMER_Context.calibration_in_progress == FALSE));
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


  /******************* (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
