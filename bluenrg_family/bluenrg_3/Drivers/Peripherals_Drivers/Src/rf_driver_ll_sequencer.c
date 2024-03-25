/**
  ******************************************************************************
  * @file    rf_driver_ll_sequencer.c
  * @author  RF Application Team
  * @brief   SPIRIT3 MRSubG Sequencer module APIs
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

#include "spirit3.h"
#include <string.h>
#include "rf_driver_ll_sequencer.h"

/*
 * @brief  Set global configuration table for sequencer hardware.
 * @param  Pointer to the global configuration table struct.
 * @retval None
 */
void MRSubG_Sequencer_SetGlobalConfiguration(MRSubG_Sequencer_GlobalConfiguration *cfg)
{
  /* GLOBALTABLE_PTR is relative to device SRAM base address */
  MR_SUBG_GLOB_RETAINED->SEQ_GLOBALTABLE_PTR = ((uint32_t)cfg) - SRAM_BASE;
}

/*
 * @brief  Determine whether sequencer hardware is currently active.
 * @retval 1 in case sequencer is active, otherwise 0
 */
uint8_t MRSubG_Sequencer_IsActive(void)
{
  return (MR_SUBG_GLOB_MISC->SEQUENCER_CTRL & MR_SUBG_GLOB_MISC_SEQUENCER_CTRL_DISABLE_SEQ) ? 0 : 1;
}

/*
 * @brief  Trigger sequencer, i.e., launch sequence of actions.
 * @retval None
 */
void MRSubG_Sequencer_Trigger(void)
{
  MR_SUBG_GLOB_MISC->SEQUENCER_CTRL &= ~MR_SUBG_GLOB_MISC_SEQUENCER_CTRL_DISABLE_SEQ;
  MR_SUBG_GLOB_MISC->SEQUENCER_CTRL |= MR_SUBG_GLOB_MISC_SEQUENCER_CTRL_GEN_SEQ_TRIGGER;
}

/*
 * @brief  Disable sequencer hardware.
 * @retval None
 */
void MRSubG_Sequencer_Disable(void)
{
  MR_SUBG_GLOB_MISC->SEQUENCER_CTRL |= MR_SUBG_GLOB_MISC_SEQUENCER_CTRL_DISABLE_SEQ | MR_SUBG_GLOB_MISC_SEQUENCER_CTRL_GEN_SEQ_TRIGGER;
}

/*
 * @brief  Convert microseconds into interpolated absolute time value. SCM_COUNTER must be ready for meaningful results.
 * @param  microseconds Time in microseconds (integer)
 * @retval Time in interpolated absolute time units, can be used for sequencer intervals and timeouts.
 */
uint32_t MRSubG_Sequencer_Microseconds(uint32_t microseconds)
{
  /* Determine true frequency (relative to 16MHz clock) of "interpolated absolute time" value */
  uint16_t slow_clock_freq = 32000;
  uint16_t scm_counter_currval = READ_REG_FIELD(MR_SUBG_GLOB_MISC->SCM_COUNTER_VAL, MR_SUBG_GLOB_MISC_SCM_COUNTER_VAL_SCM_COUNTER_CURRVAL);
  if (scm_counter_currval != 0)
    slow_clock_freq = 32ull * 16000000ull / scm_counter_currval;
  uint64_t interpolated_absolute_time = 16u * slow_clock_freq;

  return ((uint64_t)microseconds) * interpolated_absolute_time / 1000000ull;
}

/*
 * @brief  Convert milliseconds into interpolated absolute time value. SCM_COUNTER must be ready for meaningful results.
 * @param  milliseconds Time in milliseconds (integer)
 * @retval Time in interpolated absolute time units, can be used for sequencer intervals and timeouts.
 */
uint32_t MRSubG_Sequencer_Milliseconds(uint32_t milliseconds)
{
  return MRSubG_Sequencer_Microseconds(milliseconds * 1000ul);
}

/*
 * @brief  Convert seconds into interpolated absolute time value. SCM_COUNTER must be ready for meaningful results.
 * @param  seconds Time in seconds (integer)
 * @retval Time in interpolated absolute time units, can be used for sequencer intervals and timeouts.
 */
uint32_t MRSubG_Sequencer_Seconds(uint32_t seconds)
{
  return MRSubG_Sequencer_Microseconds(seconds * 1000000ul);
}

/*
 * @brief  Store the current contents of all static SubGHz static configuration registers to
 *         given global configuration table for sequencer.
 * @param  cfg Pointer to global configuration table struct.
 * @retval SUCCESS if operation was successful, ERROR if RAM table pointer is not word-aligned.
 */
ErrorStatus MRSubG_Sequencer_ApplyStaticConfig(MRSubG_Sequencer_GlobalConfiguration *cfg)
{
  /* Ensure GlobalConfiguration RAM table is word-aligned */
  if (((uint32_t)cfg) % 4 != 0)
    return ERROR;

  memcpy((void*)&cfg->StaticConfigReg, (void*)MR_SUBG_GLOB_STATIC, sizeof(MR_SUBG_GLOB_STATIC_TypeDef));

  return SUCCESS;
}

/*
 * @brief  Store the current contents of all static SubGHz static configuration registers to
 *         given global configuration table for sequencer.
 * @param  cfg Pointer to global configuration table struct.
 * @param  cmd The command (e.g., TX / RX / SABORT) to issue upon execution of this sequencer action.
 * @retval SUCCESS if operation was successful, ERROR if RAM table pointer is not word-aligned
 *         or NextAction1Interval / NextAction2Interval value is invalid.
 */
ErrorStatus MRSubG_Sequencer_ApplyDynamicConfig(MRSubG_Sequencer_ActionConfiguration *cfg, MRSubGCmd cmd)
{
  /* Ensure ActionConfiguration RAM table is word-aligned */
  if (((uint32_t)cfg) % 4 != 0)
    return ERROR;

  /* NextAction1Interval and NextAction2Interval must not be smaller than (SOC_WAKEUP_OFFSET + 2) slow clock cycles */
  uint32_t soc_wakeup_offset = READ_REG_FIELD(MR_SUBG_GLOB_RETAINED->WAKEUP_CTRL, MR_SUBG_GLOB_RETAINED_WAKEUP_CTRL_SOC_WAKEUP_OFFSET);
  uint32_t min_next_action_interval = (soc_wakeup_offset + 2) * 16;
  if (cfg->NextAction1Interval != 0 && cfg->NextAction1Interval < min_next_action_interval)
    return ERROR;

  if (cfg->NextAction2Interval != 0 &&  cfg->NextAction2Interval < min_next_action_interval)
    return ERROR;

  /* Copy current dynamic register configuration to ActionConfiguration block */
  memcpy((void*)&cfg->DynamicConfigReg, (void*)MR_SUBG_GLOB_DYNAMIC, sizeof(MR_SUBG_GLOB_DYNAMIC_TypeDef));
  MODIFY_REG_FIELD(cfg->DynamicConfigReg.COMMAND, MR_SUBG_GLOB_DYNAMIC_COMMAND_COMMAND_ID, cmd);

  return SUCCESS;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
