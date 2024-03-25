/**
******************************************************************************
* @file    rf_driver_ll_timer_subghz.c
* @author  RF Application Team
* @brief   This file provides a set of functions to manage the radio and sleep timers.
* @details The basic support is provided by a low speed oscillator with a nominal speed of 32.768 kHz.
* The low speed oscillator provides a counter that increments at a nominal rate of 30.52 us.
* The counter is 32 bits wide and the low speed oscillator only increments bits 31:4 (most significant 28 bits)
* The remaining 4 bits are interpolated using the high speed 16 MHz clock. The counter unit are called conventionally
* machine time unit (MTU).
* At the end, the counter resolution will be about 1.91 us and it will wrap about every 2.30 hours.
* It is important to notice that the interpolated bits will only be functional when the system is out of sleep.
* While in sleep only the 28 most significant bits will tick.
* The user can setup a timer that will wakeup the system (if asleep) and it will trigger an interrupt when the timer expires.
* The user timer will not take into account the high speed XTAL wakeup time.
* The possible options for the low speed oscillator are:
* - external 32.768 kHz XTAL
* - internal ring oscillator (RO)
*
* The external 32 kHz XTAL meets the assumption made above regarding the nominal speed. The internal RO 
* is sensitive to temperature. Due to this reason, the concept of calibration or compensation is
* introduced. The calibration process involves measuring the nominal 32 kHz period using the high speed 16 MHz clock, once
* this operation is performed, the software will compensate all the timing using the most recent calibration value.
* The calibration is started by hardware at power-on and every 32 low spieed clock cycles.
* The MTU, on the other hand, have a nominal value of 1.91 us, and this value can be assumed constant when using
* external 32 kHz clock, but it will vary (mainly with temperature variation) when using internal RO.
* This software layer provides also the possibility to program a radio timer linked to an already
* configured radio event. The radio timer is clocked by the low speed oscillator too.
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
#include "rf_driver_ll_timer_subghz.h"
#include "system_spirit3.h"

/** @addtogroup RF_DRIVER_LL_Driver
* @{
*/

/** @defgroup TIMER TIMER
  * @brief TIMER module driver
  * @{
  */

/** @defgroup TIMER_Private_Types TIMER Private Types 
* @{
*/
typedef struct timer_context_s {
  uint64_t cumulative_time; /** Absolute system time since power up */
  uint64_t last_calibration_time; /** Absolute system time when last calibration was performed */
  uint32_t hs_startup_time; /** XTAL startup time in us */
  uint16_t wakeup_time_offset; /** Wakeup Time offset expressed */
  uint32_t last_scm_counter; /** Number of 16Mhz fast clock cycles seen in a 32 slow clock period window read in the last calibration*/
  uint32_t fast_clock_freq; /** The fast clock frequency is XO frequency / 3 so typically 16MHz. But it can also be 15.666MHz with a XO at 47MHz or 16.666MHz with a XO at 50MHz. */
  uint32_t slow_clock_freq; /** Slow clock freq */
  uint32_t interpolated_freq; /** Interpolated frequency */
  uint32_t last_interpolated_freq; /** Last interpolated frequency */
  uint64_t timer_max_value; /** TIMER_MAX_VALUE in System Time Unit (1us) */
  uint32_t last_calibration_machine_time; /** Last machine time when calibration was performed */
  uint32_t calibration_machine_interval; /** Calibration Interval MTU */
  uint64_t last_system_time; /** Last System Time */
  uint8_t  periodic_calibration; /** Tells whether periodic hardware calibration is needed or not, i.e. LSO speed varies with temperature, etc. */
} TIMER_ContextType; 

/**
* @}
*/

/** @defgroup TIMER_Private_Macros TIMER Private Macros
* @{
*/

#define TIME_DIFF(a, b)       (((int32_t)((a - b) << (32-TIMER_BITS))) >> (32-TIMER_BITS))
/* This define assumes that a is always greater than b */
#define TIME_ABSDIFF(a, b)       ((a - b) & TIMER_MAX_VALUE)
/* #define IRQ_SAFE */
#define MIN(a,b) ((a) < (b) )? (a) : (b)
#define MAX(a,b) ((a) < (b) )? (b) : (a)

#define ATOMIC_SECTION_BEGIN() uint32_t uwPRIMASK_Bit = __get_PRIMASK(); \
                                __disable_irq(); \
/* Must be called in the same scope of ATOMIC_SECTION_BEGIN */
#define ATOMIC_SECTION_END() __set_PRIMASK(uwPRIMASK_Bit)

/**
* @}
*/
                                  
/**
* @}
*/

/** @defgroup TIMER_Private_Variables TIMER Private Variables
* @{
*/

static TIMER_ContextType TIMER_Context;

/**
* @}
*/

static void _get_calibration_data(TIMER_ContextType *context)
{
  uint16_t scm_counter_currval;
  uint32_t last_interpolated_freq;
  
  scm_counter_currval = TIMER_GET_SLOW_COUNTER_VAL;
  if ((scm_counter_currval != 0) && (scm_counter_currval != context->last_scm_counter)) {
    context->slow_clock_freq = ((32ull * (uint64_t)context->fast_clock_freq * 1000)/ scm_counter_currval);
    context->last_scm_counter = scm_counter_currval;
    last_interpolated_freq = context->interpolated_freq;
    context->interpolated_freq = (16u * context->slow_clock_freq)/1000;
    if (last_interpolated_freq == 0)
      context->last_interpolated_freq = context->interpolated_freq;
    else
      context->last_interpolated_freq = last_interpolated_freq;
    context->slow_clock_freq /= 1000;
    context->timer_max_value = (((uint64_t)TIMER_MAX_VALUE*1000000ull)/context->interpolated_freq);
  }
}

static void _timer_calibrate(TIMER_ContextType *context)
{
  while(TIMER_GET_SLOW_COUNTER_VAL == 0);
  _get_calibration_data(context);  
}

/**
 * @brief  Translate us to MTU
 * @param  time: Time expressed in us to be translated in MTU
 * @retval Machine Time Units
 */
static uint64_t us_to_machinetime(uint64_t time, TIMER_ContextType context)
{
  return ((time * context.interpolated_freq) / 1000000ull);
}

static uint64_t get_system_time_and_machine(TIMER_ContextType *context, uint32_t *current_machine_time)
{
  uint64_t new_time;
  
  ATOMIC_SECTION_BEGIN();
  *current_machine_time = MR_SUBG_GLOB_MISC->ABSOLUTE_TIME & TIMER_MAX_VALUE;
  new_time = ((uint64_t)TIME_ABSDIFF(*current_machine_time, context->last_calibration_machine_time)*1000000ull) / context->interpolated_freq;
  new_time += context->cumulative_time;
  if (new_time < context->last_system_time) {
    new_time += context->timer_max_value; 
  }
  context->last_system_time = new_time;
  
  ATOMIC_SECTION_END();
  
  return new_time;
}

/* 
 * This function is used to get current system time 
 */
static uint64_t get_system_time(TIMER_ContextType *context)
{
  uint32_t current_machine_time;
  return get_system_time_and_machine(context, &current_machine_time);
}

/* 
 * This function update the system time after a calibration.
 * If the user calls too often this function, you could have rounding issues in the integer maths.
 */
static void update_system_time(TIMER_ContextType *context)
{
  uint32_t current_machine_time;
  uint64_t new_time;

  current_machine_time = MR_SUBG_GLOB_MISC->ABSOLUTE_TIME & TIMER_MAX_VALUE;
  new_time = ((uint64_t)TIME_ABSDIFF(current_machine_time, context->last_calibration_machine_time)*1000000ull)/context->last_interpolated_freq;
  context->cumulative_time = context->last_calibration_time + new_time; 
  
  if ((TIMER_Context.periodic_calibration == 0) && 
      (TIME_ABSDIFF(current_machine_time, context->last_calibration_machine_time) < context->calibration_machine_interval)) {
        context->cumulative_time += context->timer_max_value;
  }
  context->last_calibration_machine_time = current_machine_time;
  context->last_calibration_time = context->cumulative_time;
  context->last_interpolated_freq = context->interpolated_freq;
}

/* 
 * This function update the crystal startup time according the latest calibraion values measured 
 */
static void _update_xtal_startup_time(uint16_t hs_startup_time, int32_t freq)
{
  uint16_t time;
  
  time= ((hs_startup_time*freq)/1000000ull);
  if(time >= 255)
    time = 255;
  if(time < 16)
    time = 20;
  
  TIMER_Context.wakeup_time_offset =  time;
  MR_SUBG_GLOB_RETAINED->WAKEUP_CTRL &= ~(MR_SUBG_GLOB_RETAINED_WAKEUP_CTRL_SOC_WAKEUP_OFFSET); 
  MR_SUBG_GLOB_RETAINED->WAKEUP_CTRL |= (TIMER_Context.wakeup_time_offset & MR_SUBG_GLOB_RETAINED_WAKEUP_CTRL_SOC_WAKEUP_OFFSET);
}

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
void TIMER_Init(TIMER_InitType* TIMER_InitStruct)
{
  TIMER_Context.hs_startup_time = TIMER_InitStruct->XTAL_StartupTime;
  
  if (TIMER_RC64PLL_LOCKED) {
    TIMER_Context.fast_clock_freq = 16000000; //??? [FC] understand if the PLL output is 16 MHz if the HSE is 47 MHzor 50 MHz
  } else {
    TIMER_Context.fast_clock_freq = HSE_XTAL_FREQ/3;
  }
    
  if (TIMER_InitStruct->TIMER_PeriodicCalibration || TIMER_InitStruct->TIMER_InitialCalibration) {
    _timer_calibrate(&TIMER_Context);
  } else {
    /* Assume fix frequency at 32.768 kHz */
    TIMER_Context.slow_clock_freq = 32768;
    TIMER_Context.interpolated_freq = 16u * TIMER_Context.slow_clock_freq;
    TIMER_Context.last_interpolated_freq = TIMER_Context.interpolated_freq;
    TIMER_Context.timer_max_value = (((uint64_t)TIMER_MAX_VALUE*1000000ull)/TIMER_Context.interpolated_freq);
  }
  TIMER_Context.periodic_calibration = TIMER_InitStruct->TIMER_PeriodicCalibration;
  _update_xtal_startup_time(TIMER_Context.hs_startup_time, TIMER_Context.slow_clock_freq);
  TIMER_Context.cumulative_time = 0;

  TIMER_Context.last_calibration_machine_time = MR_SUBG_GLOB_MISC->ABSOLUTE_TIME & TIMER_MAX_VALUE;
  TIMER_Context.last_system_time = 0;
  TIMER_Context.last_calibration_time = 0;
}

/**
 * @brief  Perform a low speed clock calibration and store results in the global context.
 *         It updates the XTAL startup time and the interpolator configuration.
 *         It updates also the cumulative STU variable, so it should be called peiodically to manage timer wrapping,
 *         regardless of actual need of periodic calibration.
 *         This function is not re-entrant since it updates the context variable storing the system time.
 * @retval None
 */
void TIMER_Calibrate(void)
{
  if (TIMER_Context.periodic_calibration) {
    TIMER_ContextType ContextToUpdate = TIMER_Context;
    _timer_calibrate(&ContextToUpdate);
    _update_xtal_startup_time(ContextToUpdate.hs_startup_time, ContextToUpdate.slow_clock_freq);
    ATOMIC_SECTION_BEGIN();
    /* Copying only the updated fields.
    Faster than memcpy: TIMER_Context = ContextToUpdate;*/
    TIMER_Context.slow_clock_freq = ContextToUpdate.slow_clock_freq;
    TIMER_Context.last_scm_counter = ContextToUpdate.last_scm_counter;
    TIMER_Context.interpolated_freq = ContextToUpdate.interpolated_freq;
    TIMER_Context.last_interpolated_freq = ContextToUpdate.interpolated_freq;
    TIMER_Context.timer_max_value = ContextToUpdate.timer_max_value;
    update_system_time(&TIMER_Context);
    ATOMIC_SECTION_END();
  }
  else
  {
    ATOMIC_SECTION_BEGIN();
    update_system_time(&TIMER_Context);
    ATOMIC_SECTION_END();
  }
}

/**
 * @brief  Return TRUE if a calibration is on going. It relies on the value of the interrupt status.
 *         Assumption is that no other modules are managing the interrupt status of the calibrator.
 * @retval TRUE if calibration is running, FALSE otherwise.
 */
BOOL TIMER_IsCalibrationRunning(void)
{
  /* Normally the calibration is triggered automatically by hardware at power up (or hardware reset),
     every 32 slow clock cycles */
  return ((TIMER_GET_SLOW_COUNTER_VAL) == 0);
}

/**
 * @brief   Records the result of the last calibration in the internal context.
 *          It updates the XTAL startup time.
 *          It updates also the cumulative STU variable, so it should be called peiodically to manage timer wrapping,
 *          regardless of actual need of periodic calibration.
 * @warning This function is not re-entrant since it updates the context variable storing the system time.
 *          It should be called only in user context and not in interrupt context.
 * @retval  None
 */
void TIMER_UpdateCalibrationData(void)
{
  if (TIMER_Context.periodic_calibration) {
    TIMER_ContextType ContextToUpdate = TIMER_Context;
    _get_calibration_data(&ContextToUpdate);
    _update_xtal_startup_time(ContextToUpdate.hs_startup_time, ContextToUpdate.slow_clock_freq);
    ATOMIC_SECTION_BEGIN();
    /* Copying only the updated fields.
    Faster than memcpy: TIMER_Context = ContextToUpdate;*/
    TIMER_Context.slow_clock_freq = ContextToUpdate.slow_clock_freq;
    TIMER_Context.last_scm_counter = ContextToUpdate.last_scm_counter;
    TIMER_Context.interpolated_freq = ContextToUpdate.interpolated_freq;
    TIMER_Context.last_interpolated_freq = ContextToUpdate.last_interpolated_freq;
    TIMER_Context.timer_max_value = ContextToUpdate.timer_max_value;
    update_system_time(&TIMER_Context);
    ATOMIC_SECTION_END();
  }
  else
  {
    ATOMIC_SECTION_BEGIN();
    update_system_time(&TIMER_Context);
    ATOMIC_SECTION_END();
  }
}

/**
 * @brief  Returns the current system time (STU). The STU is expressed in us.
 *         This is a counter that grows since the power up of the system and it never wraps.
 * @return Current system time
 */
uint64_t TIMER_GetCurrentSysTime(void)
{
  return get_system_time(&TIMER_Context);
}

/**
 * @brief   Returns the system time referred to the absolute machine time passed as parameter.
 * @param   time: Absolute machine time in the future
 * @warning User should guarantee that call to this function are performed in a non-interruptible context.
 * @return  STU value 
 */
uint64_t TIMER_GetFutureSysTime(uint32_t time)
{
  uint64_t delta_systime;
  
  delta_systime = ((uint64_t)TIME_DIFF(time, TIMER_Context.last_calibration_machine_time)*1000000ull)/TIMER_Context.interpolated_freq;
  delta_systime += TIMER_Context.cumulative_time;  
  if (delta_systime < TIMER_Context.last_system_time) {
    delta_systime += TIMER_Context.timer_max_value;
  }
  
  return delta_systime;
}

/**
 * @brief   Returns the system time referred to the absolute machine time passed as parameter.
 * @param   time: Absolute machine time in the past
 * @warning User should guarantee that call to this function are performed in a non-interruptible context.
 * @return  STU value 
 */
uint64_t TIMER_GetPastSysTime(uint32_t time)
{
  uint32_t current_machine_time;
  uint64_t delta_sysTime;
  uint64_t current_system_time = get_system_time_and_machine(&TIMER_Context, &current_machine_time);
  
  delta_sysTime = (((uint64_t)TIME_DIFF(current_machine_time, time)*1000000ull)/TIMER_Context.interpolated_freq);
  
  return (current_system_time-delta_sysTime);
}

/**
 * @brief  Return the current calibration data.
 * @retval None
 */
void TIMER_GetCurrentCalibrationData(TIMER_CalibrationType *data)
{
  data->periodic_calibration = TIMER_Context.periodic_calibration;
  data->slow_clock_freq = TIMER_Context.slow_clock_freq;
  data->last_calibration_time = TIMER_Context.last_calibration_time;
  data->last_calibration_machine_time = TIMER_Context.last_calibration_machine_time; /* Last machine time when calibration was performed */
}

/**
  * @brief  Records the calibration interval. This information 
  *         is used to understand if a absolute time wrap register happens since the device startup configration.
  * @param  time: Calibration interval (STU)
  * @warning This function is not re-entrant since it updates the context variable storing the system time.
  *          It should be called only in user context and not in interrupt context.
* @retval   None
*/
void TIMER_SaveCalibrationInterval(uint32_t time)
{
  
  TIMER_Context.calibration_machine_interval = us_to_machinetime(time, TIMER_Context);
}

/**
 * @brief  Return the status of the Radio timers and the last value programmed in the register.
 * @param  time: pointer to value which is going to have time value (MTU).
 * @retval 0 if no timer has been programmed.
 * @retval 1 if CPU Wakeup Timer has been programmed.
 */
uint8_t TIMER_GetRadioTimerValue(uint32_t *time)
{
  uint8_t ret_val = 0;
  
  if(TIMER_GET_CPU_WAKEUP_EN) 
  {
    *time = MR_SUBG_GLOB_RETAINED->CPU_WAKEUPTIME;
    ret_val =  CPU_WAKEUP_TIMER_BUSY;
  }
  return ret_val;
}

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
uint32_t TIMER_SetWakeupTime(uint32_t delay)
{
  uint32_t current_time;
  uint64_t new_time;

  new_time = TIMER_UsToMachinetime(delay);
  /* If the delay is too small round to minimum 50 tick */
  new_time = MAX(50,new_time);
  current_time = MR_SUBG_GLOB_MISC->ABSOLUTE_TIME;
  /* 4 least significant bits are not taken into account. Then let's round the value */
  MR_SUBG_GLOB_RETAINED->CPU_WAKEUPTIME = ((current_time + (new_time + 8)) & TIMER_MAX_VALUE);
    
  TIMER_ENABLE_CPU_WAKEUP_TIMER;
  
  return current_time;
}

/**
 * @brief Enable CPU wakeup interrupt towards the CPU.
 */
void TIMER_Enable_CPU_WKUP(void)
{
  NVIC_EnableIRQ(CPU_WKUP_IRQn);
}

/**
 * @brief Disable CPU wakeup interrupt towards the CPU.
 */
void TIMER_Disable_CPU_WKUP(void)
{
  NVIC_DisableIRQ(CPU_WKUP_IRQn);
}

/**
 * @brief  Translate time in microseconds into machine time units.
 * @return MTU value 
 */
uint64_t TIMER_UsToMachinetime(uint64_t time)
{
  return us_to_machinetime(time, TIMER_Context);
}

/**
 * @brief   Return the microseconds corresponding to the MTU passed as parameter.
 * @param   time: MTU amount to be converted in us
 * @warning This function is not re-entrant since it updates the context variable storing the system time. It should be called only in
 *          user context and not in interrupt context.
 * @return  time expressed in microseconds 
 */
uint64_t TIMER_MachineTimeToUs(uint64_t time)
{
   return ((time*1000000ull)/TIMER_Context.interpolated_freq);
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
