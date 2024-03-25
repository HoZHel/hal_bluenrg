/**
  ******************************************************************************
  * @file    rf_driver_ll_sequencer.h
  * @author  RF Application Team
  * @brief   SPIRIT3 MRSubG Sequencer module APIs
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]
    The LL_SEQUENCER driver contains a set of APIs that can be used to configure
    the SPIRIT3 MRSubG sequencer hardware.
    Typically it is used as follows:

  *** Step 1: Set up ActionConfiguration tables ***
  =================================================
    [..]
    In this step, configuration RAM tables are created for all actions in the
    flow graph.

   (+) Allocate a MRSubG_Sequencer_ActionConfiguration struct for each action
       that is part of your flow graph. The sequencer hardware accesses this
       struct in RAM as the ActionConfiguration RAM table.
       It must persist in RAM while the Action is being executed.
       Make sure that the RAM table is word-aligned.

   (+) For each ActionConfiguration item:

       (++) Set up the dynamic registers (MR_SUBG_GLOB_DYNAMIC) of the SPIRIT3
            as desired upon execution of the action. This can be performed
            using the standard MRSubG_* and packet APIs.
            Do not issue any command in the meantime (i.e., keep COMMAND_ID in
            the COMMAND to 0) to prevent side effects.
       (++) Copy all dynamic register values to the ActionConfiguration RAM
            table using MRSubG_Sequencer_ApplyDynamicConfig and configure
            which command is issued when the action gets executed.
       (++) Change the appropriate values in the
            MRSubG_Sequencer_ActionConfiguration struct to configure under
            which conditions either NextAction1 or NextAction2 are executed,
            to provide a pointer to ActionConfiguration RAM tables for both
            actions and to configure inter-action intervals.
            Use SEQ_MASK_NEVER_MATCH as NextAction1Mask / NextAction2Mask to
            ensure that this particular next action is never executed.
       (++) You may choose to configure an ActionTimeout, i.e. limit the
            maximum time to wait for a mask match for either NextAction1 or
            NextAction2.

  *** Step 2: Set up GlobalConfiguration table ***
  ================================================
    [..]
    In this step, a single configuration RAM table containing the global
    sequencer properties and the contents of the static configuration
    registers is created.

   (+) Allocate a MRSubG_Sequencer_GlobalConfiguration struct.
       The sequencer hardware accesses this struct in RAM as the
       GlobalConfiguration RAM table. The sequencer hardware will update
       properties of this RAM table during operation.

   (+) Modify the GlobalConfiguration struct:

       (++) Set up the static registers (MR_SUBG_GLOB_STATIC) of the SPIRIT3
            as desired upon execution of the action. This can be performed
            using the standard MRSubG_* and packet APIs.
       (++) Copy all static register values to the GlobalConfiguration RAM
            table using MRSubG_Sequencer_ApplyStaticConfig.
       (++) Set the current (i.e., first) action to execute by making
            ActionConfigPtr point to the appropriate ActionConfiguration.

   (+) Tell the sequencer hardware where to find the GlobalConfiguration
       by calling MRSubG_Sequencer_SetGlobalConfiguration.

  *** Step 3: Trigger the sequencer ***
  =====================================
    [..]
   (+) When desired, start the sequencer using MRSubG_Sequencer_Trigger.

   (+) You may choose to intervene into the sequencer control flow
       programmatically using, for example, an interrupt.

   (+) If, at any point, you need to restart the sequencer:

       (++) Set the action to execute next by modifying ActionConfigPtr in
            the GlobalConfiguration struct.
       (++) Re-trigger the sequencer using MRSubG_Sequencer_Trigger.
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
  ******************************************************************************
  */

#ifndef RF_DRIVER_LL_SUBGHZ_SEQUENCER_H
#define RF_DRIVER_LL_SUBGHZ_SEQUENCER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "spirit3.h"
#include "rf_driver_ll_radio_subghz.h"


#define SEQ_MASK_NEVER_MATCH MR_SUBG_GLOB_STATUS_RFSEQ_IRQ_STATUS_SEQ_F /* If we use SEQ_F as the only flag bit to match on, then a match can never occur because SEQ_F is only set when the sequencer terminates. */
#define SEQ_MASK_ALWAYS_MATCH 0x00000000 /* The Sequencer automatically matches a null mask */

/**
 * @brief Sequencer flag bits
 */
typedef enum {
  /* Indicating if the low power mode was allowed during the wait interval since last SeqAction execution */
  SEQ_FLAG_SLEEPENABLE = 0x01,
    
  /* Enable/disable a force reload of the Static registers even if no reset occurs since last reload */
  SEQ_FLAG_FORCERELOAD = 0x02,
  
  /* Clear the internal seq_event_status bus before to start the SeqAction */
  SEQ_FLAG_CLEAREVENTS = 0x04      
  
} MRSubG_Sequencer_Flags;

typedef struct MRSubG_Sequencer_ActionConfiguration MRSubG_Sequencer_ActionConfiguration;

/**
 * @brief Sequencer ActionConfiguration RAM table structure
 */
struct MRSubG_Sequencer_ActionConfiguration {
  /*
   * NextAction1
   */
  /** Pointer to NextAction1, executed upon NextAction1Mask match. */
  MRSubG_Sequencer_ActionConfiguration* NextAction1Ptr;

  /** Time to wait before NextAction1 execution, in interpolated absolute time units. */
  uint32_t NextAction1Interval;

  /** Bitmask for NextAction1 - if SubGHz status register against matches against this mask, NextAction1 is performed next. */
  uint32_t NextAction1Mask;

  /** Configuration flags (ForceClearEvents, ForceReload and SleepEn) for sequencer hardware, set if NextAction1 is performed next. */
  uint32_t NextAction1Ctrl;

  /* NextAction2 */
  /** Pointer to NextAction2, executed upon NextAction1Mask match. */
  MRSubG_Sequencer_ActionConfiguration* NextAction2Ptr;

  /** Time to wait before NextAction2 execution, in interpolated absolute time units. */
  uint32_t NextAction2Interval;

  /** Bitmask for NextAction2 - if SubGHz status register against matches against this mask, NextAction2 is performed next. */
  uint32_t NextAction2Mask;

  /** Configuration flags (ForceClearEvents, ForceReload and SleepEn) for sequencer hardware, set if NextAction2 is performed next. */
  uint32_t NextAction2Ctrl;

  /** Time to wait for either NextAction1Mask or NextAction2Mask match before aborting, in interpolated absolute time units.
   *  Set to 0 for no timeout. */
  uint32_t ActionTimeout;

  /** Dynamic Register Configuration, applied for the current action. */
  MR_SUBG_GLOB_DYNAMIC_TypeDef DynamicConfigReg;
};

/**
 * @brief Sequencer GlobalConfiguration RAM table structure
 */
typedef struct {
  /** Pointer to first ActionConfiguration */
  volatile MRSubG_Sequencer_ActionConfiguration* ActionConfigPtr;

  /** Configuration flags (ForceClearEvents, ForceReload and SleepEn) for sequencer hardware. */
  volatile uint32_t Flags;

  /** SubGHz static configuration registers. */
  MR_SUBG_GLOB_STATIC_TypeDef StaticConfigReg;
} MRSubG_Sequencer_GlobalConfiguration;


void MRSubG_Sequencer_SetGlobalConfiguration(MRSubG_Sequencer_GlobalConfiguration *cfg);
uint8_t MRSubG_Sequencer_IsActive(void);
void MRSubG_Sequencer_Trigger(void);
void MRSubG_Sequencer_Disable(void);

uint32_t MRSubG_Sequencer_Microseconds(uint32_t microseconds);
uint32_t MRSubG_Sequencer_Milliseconds(uint32_t milliseconds);
uint32_t MRSubG_Sequencer_Seconds(uint32_t seconds);

ErrorStatus MRSubG_Sequencer_ApplyStaticConfig(MRSubG_Sequencer_GlobalConfiguration *cfg);
ErrorStatus MRSubG_Sequencer_ApplyDynamicConfig(MRSubG_Sequencer_ActionConfiguration *cfg, MRSubGCmd cmd);

#ifdef __cplusplus
}
#endif

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
