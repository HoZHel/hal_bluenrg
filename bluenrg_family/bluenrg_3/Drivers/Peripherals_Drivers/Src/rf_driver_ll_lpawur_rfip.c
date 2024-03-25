/**
  ******************************************************************************
  * @file    rf_driver_ll_LPAWUR_rfip.c
  * @author  RF Application Team - EMEA
  * @brief   SPIRIT3 LPAWUR RFIP module APIs
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
#include "rf_driver_ll_lpawur_rfip.h"
#include "rf_driver_ll_utils.h"

#ifdef  USE_FULL_ASSERT
#include "rf_driver_assert.h"
#else
#define assert_param(expr) ((void)0U)
#endif

// ---------------> Internal (private) LPAWUR_RFIP functions

// End Of Internal (private) LPAWUR_RFIP functions <---------------

/**
* @brief  Get the IP version of the LPAWUR block.
* @param  None.
* @retval IP version.
*/
SLPAWUR_Version LPAWUR_GetVersion(void)
{
  SLPAWUR_Version LPAWUR_Version;

  LPAWUR_Version.product = READ_REG_FIELD(LPAWUR->RFIP_VERSION, LPAWUR_RFIP_VERSION_PRODUCT);
  LPAWUR_Version.version = READ_REG_FIELD(LPAWUR->RFIP_VERSION, LPAWUR_RFIP_VERSION_VERSION);
  LPAWUR_Version.revision = READ_REG_FIELD(LPAWUR->RFIP_VERSION, LPAWUR_RFIP_VERSION_REVISION);

  return LPAWUR_Version;
}

/**
* @brief  Configure the RF IP of LPAWUR.
* @param  pxSLPAWUR_RFConfigStruct SLPAWUR_RFConfig configuration structure.
* @retval None.
*/
void LPAWUR_RFConfigInit(SLPAWUR_RFConfig* pxSLPAWUR_RFConfigStruct)
{
  assert_param(IS_EDICAL(pxSLPAWUR_RFConfigStruct->EnergyDetectorIcal));  
  assert_param(IS_AGC_MODE(pxSLPAWUR_RFConfigStruct->AgcMode));
  assert_param(IS_AGC_HOLD_MODE(pxSLPAWUR_RFConfigStruct->AgcHoldMode));

  //Set Energy Detector Calibration value
  LPAWUR_SetEdIcal(pxSLPAWUR_RFConfigStruct->EnergyDetectorIcal);

  //Configure Clock divider
  LPAWUR_SetClkDiv(pxSLPAWUR_RFConfigStruct->ClockDivider);

  //Configure Energy Detector switch
  LPAWUR_SetEdSwitch(pxSLPAWUR_RFConfigStruct->EnergyDetectorSwitch);

  //Configure AGC Reset Mode
  LPAWUR_SetAgcResetMode(pxSLPAWUR_RFConfigStruct->AgcResetMode);

  //Configure AGC Hold Mode
  LPAWUR_SetAgcHoldMode(pxSLPAWUR_RFConfigStruct->AgcHoldMode);

  //Configure Agc Mode
  LPAWUR_SetAgcMode(pxSLPAWUR_RFConfigStruct->AgcMode);
}

/**
* @brief  Get the LPAWUR RF IP configuration.
* @param  None.
* @retval SLPAWUR_RFConfig configuration structure.
*/
SLPAWUR_RFConfig LPAWUR_GetRFConfig(void)
{
  SLPAWUR_RFConfig SLPAWUR_CurrentRFConfig;

  SLPAWUR_CurrentRFConfig.EnergyDetectorIcal = LPAWUR_GetEdIcal();  
  SLPAWUR_CurrentRFConfig.ClockDivider = LPAWUR_GetClkDiv();
  SLPAWUR_CurrentRFConfig.EnergyDetectorSwitch = LPAWUR_GetEdSwitch();
  SLPAWUR_CurrentRFConfig.AgcResetMode = LPAWUR_GetAgcResetMode();
  SLPAWUR_CurrentRFConfig.AgcHoldMode = LPAWUR_GetAgcHoldMode();
  SLPAWUR_CurrentRFConfig.AgcMode = LPAWUR_GetAgcMode();

  return SLPAWUR_CurrentRFConfig;
}

/**
* @brief  Configure the frame of LPAWUR RF IP.
* @param  pxSLPAWUR_FrameInitStruct SLPAWUR_FrameInit configuration structure.
* @retval None.
*/
void LPAWUR_FrameInit(SLPAWUR_FrameInit* pxSLPAWUR_FrameInitStruct)
{
  assert_param(IS_LPAWUR_PAYLOAD_LENGTH(pxSLPAWUR_FrameInitStruct->PayloadLength));

  //Set the Slow Clock Cycle Per Bit counter
  LPAWUR_SetSlowClkCyclePerBitCnt(pxSLPAWUR_FrameInitStruct->SlowClkCyclePerBitCnt);

  //Set the Paylaod Length
  LPAWUR_SetPayloadLength(pxSLPAWUR_FrameInitStruct->PayloadLength);

  //Set the SYNC threshold
  LPAWUR_SetSyncThrCnt(pxSLPAWUR_FrameInitStruct->SyncThr);

  //Set the SYNC Length
  LPAWUR_SetSyncLength(pxSLPAWUR_FrameInitStruct->SyncLength);

  //Set the Preamble threshold counter
  LPAWUR_SetPreambleThrCnt(pxSLPAWUR_FrameInitStruct->PreambleThrCnt);

  //Set the Preamble Enable State
  LPAWUR_SetPreambleEnable(pxSLPAWUR_FrameInitStruct->PreambleEnable);

  //Set the SYNC counter timeout
  LPAWUR_SetFrameSyncCntTimeout(pxSLPAWUR_FrameInitStruct->FrameSyncCntTimeout);

  //Configure the SYNC pattern
  LPAWUR_SetSync((uint32_t)(pxSLPAWUR_FrameInitStruct->FrameSyncPattenHigh<<16) +\
                 (uint32_t)(pxSLPAWUR_FrameInitStruct->FrameSyncPatternLow));

}


/**
* @brief  Get the frame configuration of LPAWUR RF IP.
* @param  None.
* @retval SLPAWUR_FrameInit configuration structure.
*/
SLPAWUR_FrameInit LPAWUR_GetFrameInfo(void)
{
  SLPAWUR_FrameInit SLPAWUR_CurrentFrameInfo;

  SLPAWUR_CurrentFrameInfo.SlowClkCyclePerBitCnt = LPAWUR_GetSlowClkCyclePerBitCnt();
  SLPAWUR_CurrentFrameInfo.PayloadLength = LPAWUR_GetPayloadLength();
  SLPAWUR_CurrentFrameInfo.SyncThr = LPAWUR_GetSyncThrCnt();
  SLPAWUR_CurrentFrameInfo.SyncLength = LPAWUR_GetSyncLength();
  SLPAWUR_CurrentFrameInfo.PreambleThrCnt = LPAWUR_GetPreambleThrCnt();
  SLPAWUR_CurrentFrameInfo.PreambleEnable = LPAWUR_GetPreambleEnable();
  SLPAWUR_CurrentFrameInfo.FrameSyncCntTimeout = LPAWUR_GetFrameSyncCntTimeout();
  SLPAWUR_CurrentFrameInfo.FrameSyncPattenHigh = LPAWUR_GetSyncPatternHigh();
  SLPAWUR_CurrentFrameInfo.FrameSyncPatternLow = LPAWUR_GetSyncPatternLow();

  return SLPAWUR_CurrentFrameInfo;
}


/**
* @brief  Configure the IRQ of LPAWUR.
* @param  IrqEnable LPAWUR_IrqEnable configuration structure.
* @retval None.
*/
void LPAWUR_IrqConfig(LPAWUR_IrqEnable IrqEnable)
{
  assert_param(IS_IRQ_ENABLE(IrqEnable));
  if(IrqEnable == IRQ_BIT_SYNC_DETECTED)
  {
  	MODIFY_REG_FIELD(LPAWUR->IRQ_ENABLE,LPAWUR_IRQ_ENABLE_BIT_SYNC_DETECTED_E,0x01);
  	MODIFY_REG_FIELD(LPAWUR->IRQ_ENABLE,LPAWUR_IRQ_ENABLE_FRAME_SYNC_COMPLETE_E,0x00);
  	MODIFY_REG_FIELD(LPAWUR->IRQ_ENABLE,LPAWUR_IRQ_ENABLE_FRAME_COMPLETE_E,0x00);
  	MODIFY_REG_FIELD(LPAWUR->IRQ_ENABLE,LPAWUR_IRQ_ENABLE_FRAME_VALID_E,0x00);
  }
  else if(IrqEnable == IRQ_FRAME_SYNC_COMPLETE)
  {
  	MODIFY_REG_FIELD(LPAWUR->IRQ_ENABLE,LPAWUR_IRQ_ENABLE_BIT_SYNC_DETECTED_E,0x00);
  	MODIFY_REG_FIELD(LPAWUR->IRQ_ENABLE,LPAWUR_IRQ_ENABLE_FRAME_SYNC_COMPLETE_E,0x01);
  	MODIFY_REG_FIELD(LPAWUR->IRQ_ENABLE,LPAWUR_IRQ_ENABLE_FRAME_COMPLETE_E,0x00);
  	MODIFY_REG_FIELD(LPAWUR->IRQ_ENABLE,LPAWUR_IRQ_ENABLE_FRAME_VALID_E,0x00);
  }
  else if(IrqEnable == IRQ_FRAME_COMPLETE)
  {
  	MODIFY_REG_FIELD(LPAWUR->IRQ_ENABLE,LPAWUR_IRQ_ENABLE_BIT_SYNC_DETECTED_E,0x00);
  	MODIFY_REG_FIELD(LPAWUR->IRQ_ENABLE,LPAWUR_IRQ_ENABLE_FRAME_SYNC_COMPLETE_E,0x00);
  	MODIFY_REG_FIELD(LPAWUR->IRQ_ENABLE,LPAWUR_IRQ_ENABLE_FRAME_COMPLETE_E,0x01);
  	MODIFY_REG_FIELD(LPAWUR->IRQ_ENABLE,LPAWUR_IRQ_ENABLE_FRAME_VALID_E,0x00);
  }
  else if(IrqEnable == IRQ_FRAME_VALID)
  {
  	MODIFY_REG_FIELD(LPAWUR->IRQ_ENABLE,LPAWUR_IRQ_ENABLE_BIT_SYNC_DETECTED_E,0x00);
  	MODIFY_REG_FIELD(LPAWUR->IRQ_ENABLE,LPAWUR_IRQ_ENABLE_FRAME_SYNC_COMPLETE_E,0x00);
  	MODIFY_REG_FIELD(LPAWUR->IRQ_ENABLE,LPAWUR_IRQ_ENABLE_FRAME_COMPLETE_E,0x00);
  	MODIFY_REG_FIELD(LPAWUR->IRQ_ENABLE,LPAWUR_IRQ_ENABLE_FRAME_VALID_E,0x01);
  }
}


/**
* @brief  Get the RX status of LPAWUR.
* @param  None.
* @retval LPAWUR_Status configuration structure.
*/
LPAWUR_Status LPAWUR_GetStatus(void)
{
  LPAWUR_Status status = NO_STATUS;
  if(READ_REG_FIELD(LPAWUR->STATUS,LPAWUR_STATUS_BIT_SYNC_DETECTED_F))
  	status = BIT_SYNC_DETECTED_F;
  if(READ_REG_FIELD(LPAWUR->STATUS,LPAWUR_STATUS_FRAME_SYNC_COMPLETE_F))
  	status = FRAME_SYNC_COMPLETE_F;
  if(READ_REG_FIELD(LPAWUR->STATUS,LPAWUR_STATUS_FRAME_COMPLETE_F))
  	status = FRAME_COMPLETE_F;
  if(READ_REG_FIELD(LPAWUR->STATUS,LPAWUR_STATUS_FRAME_VALID_F))
  	status = FRAME_VALID_F;

  return status;
}


/**
* @brief  Reset the RX status of LPAWUR.
* @param  None.
* @retval None.
*/
void LPAWUR_ClearStatus(void)
{
  MODIFY_REG_FIELD(LPAWUR->STATUS,LPAWUR_STATUS_BIT_SYNC_DETECTED_F,0x01);
  MODIFY_REG_FIELD(LPAWUR->STATUS,LPAWUR_STATUS_FRAME_SYNC_COMPLETE_F,0x01);
  MODIFY_REG_FIELD(LPAWUR->STATUS,LPAWUR_STATUS_FRAME_COMPLETE_F,0x01);
  MODIFY_REG_FIELD(LPAWUR->STATUS,LPAWUR_STATUS_FRAME_VALID_F,0x01);
}


/**
* @brief  Read the error status of LPAWUR.
* @param  None.
* @retval LPAWUR_Error value.
*/
LPAWUR_Error LPAWUR_ReadError(void)
{
  return (LPAWUR_Error)(READ_REG_FIELD(LPAWUR->STATUS,LPAWUR_STATUS_ERROR_F));
}


/**
* @brief  Clear the error status of LPAWUR.
* @param  None.
* @retval None.
*/
void LPAWUR_ClearErrorStatus(void)
{
  MODIFY_REG_FIELD(LPAWUR->STATUS, LPAWUR_STATUS_ERROR_F, 0x01);
}


/**
* @brief  Set the Slow Clock Cycle Bit Counter.
* @param  ClkCyclePerBit.
* @retval None.
*/
void LPAWUR_SetSlowClkCyclePerBitCnt(uint8_t ClkCyclePerBit)
{
  MODIFY_REG_FIELD(LPAWUR->FRAME_CONFIG0, LPAWUR_FRAME_CONFIG0_SLOW_CLK_CYCLE_PER_BIT_CNT, ClkCyclePerBit);
}


/**
* @brief  Get the Slow Clock Cycle Bit Counter.
* @param  None.
* @retval ClkCyclePerBit.
*/
uint8_t LPAWUR_GetSlowClkCyclePerBitCnt(void)
{
  return (uint8_t)(READ_REG_FIELD(LPAWUR->FRAME_CONFIG0,LPAWUR_FRAME_CONFIG0_SLOW_CLK_CYCLE_PER_BIT_CNT));
}


/**
* @brief  Set the Payload Length.
* @param  length.
* @retval None.
*/
void LPAWUR_SetPayloadLength(uint8_t length)
{
  assert_param(IS_LPAWUR_PAYLOAD_LENGTH(length));
  MODIFY_REG_FIELD(LPAWUR->FRAME_CONFIG0, LPAWUR_FRAME_CONFIG0_PAYLOAD_LENGTH,length);
}


/**
* @brief  Get the payload length.
* @param  None.
* @retval Length.
*/
uint8_t LPAWUR_GetPayloadLength(void)
{
  return (uint8_t)(READ_REG_FIELD(LPAWUR->FRAME_CONFIG0, LPAWUR_FRAME_CONFIG0_PAYLOAD_LENGTH));
}


/**
* @brief  Set SYNC threshold counter.
* @param  SyncThrCnt.
* @retval None.
*/
void LPAWUR_SetSyncThrCnt(uint8_t SyncThrCnt)
{
  MODIFY_REG_FIELD(LPAWUR->FRAME_CONFIG0,LPAWUR_FRAME_CONFIG0_SYNC_THRESHOLD_COUNT,SyncThrCnt);
}


/**
* @brief  Get the SYNC threshold counter.
* @param  None.
* @retval Sync threshold counter.
*/
uint8_t LPAWUR_GetSyncThrCnt(void)
{
  return (uint8_t)(READ_REG_FIELD(LPAWUR->FRAME_CONFIG0, LPAWUR_FRAME_CONFIG0_SYNC_THRESHOLD_COUNT));
}


/**
* @brief  Set the LPAWUR SYNC length.
* @param  SyncLen.
* @retval None.
*/
void LPAWUR_SetSyncLength(uint8_t SyncLen)
{
  MODIFY_REG_FIELD(LPAWUR->FRAME_CONFIG0, LPAWUR_FRAME_CONFIG0_SYNC_LENGTH, SyncLen);
}


/**
* @brief  Get the LPAWUR SYNC length.
* @param  None.
* @retval SyncLen.
*/
uint8_t LPAWUR_GetSyncLength(void)
{
  return (uint8_t)(READ_REG_FIELD(LPAWUR->FRAME_CONFIG0,LPAWUR_FRAME_CONFIG0_SYNC_LENGTH));
}


/**
* @brief  Set the Preamble threshold counter.
* @param  cnt counter value.
* @retval None.
*/
void LPAWUR_SetPreambleThrCnt(uint8_t cnt)
{
  MODIFY_REG_FIELD(LPAWUR->FRAME_CONFIG0,LPAWUR_FRAME_CONFIG0_PREAMBLE_THRESHOLD_COUNT,cnt);
}


/**
* @brief  Get the Preamble threshold counter.
* @param  None.
* @retval Counter value.
*/
uint8_t LPAWUR_GetPreambleThrCnt(void)
{
  return (uint8_t)(READ_REG_FIELD(LPAWUR->FRAME_CONFIG0,LPAWUR_FRAME_CONFIG0_PREAMBLE_THRESHOLD_COUNT));
}


/**
* @brief  Set the Timing recovery loop algorithm selection.
* @param  algoSel the algorithm selection.
* 	This value is type of @ref TrecLoopAlgo.
* @retval None.
*/
void LPAWUR_SetTRecAlgorithm(TrecLoopAlgo algoSel)
{
  if(algoSel == DEFAULT)
  {
    MODIFY_REG_FIELD(LPAWUR->FRAME_CONFIG1,LPAWUR_FRAME_CONFIG1_TREC_LOOP_ALGO_SEL, 0x0);
  }
  else
  {
    MODIFY_REG_FIELD(LPAWUR->FRAME_CONFIG1,LPAWUR_FRAME_CONFIG1_TREC_LOOP_ALGO_SEL, 0x1);
  }
}


/**
* @brief  Get the Timing recovery loop algorithm selection.
* @param  None.
* @retval algorithm selection.
* 	This value is type of @ref TrecLoopAlgo.
*/
TrecLoopAlgo LPAWUR_GetTRecAlgorithm(void)
{
  if(READ_REG_FIELD(LPAWUR->FRAME_CONFIG1,LPAWUR_FRAME_CONFIG1_TREC_LOOP_ALGO_SEL))
    return TWO_STEPS;
  else
    return DEFAULT;
}


/**
* @brief  Set the preamble enable.
* @param  state.
* @retval None.
*/
void LPAWUR_SetPreambleEnable(FunctionalState state)
{
  if(state == ENABLE)
  {
    MODIFY_REG_FIELD(LPAWUR->FRAME_CONFIG1,LPAWUR_FRAME_CONFIG1_PREAMBLE_ENABLE,0x1);
  }
  else
  {
    MODIFY_REG_FIELD(LPAWUR->FRAME_CONFIG1,LPAWUR_FRAME_CONFIG1_PREAMBLE_ENABLE,0x0);
  }
}


/**
* @brief  Get the preamble enable state.
* @param  None.
* @retval Preamble enable state.
*/
FunctionalState LPAWUR_GetPreambleEnable(void)
{
  if(READ_REG_FIELD(LPAWUR->FRAME_CONFIG1,LPAWUR_FRAME_CONFIG1_PREAMBLE_ENABLE))
    return ENABLE;
  else
    return DISABLE;
}


/**
* @brief  Set Frame Sync counter timeout.
* @param  timeout.
* @retval None.
*/
void LPAWUR_SetFrameSyncCntTimeout(uint8_t timeout)
{
  MODIFY_REG_FIELD(LPAWUR->FRAME_CONFIG1,LPAWUR_FRAME_CONFIG1_FRAME_SYNC_COUNTER_TIMEOUT,timeout);
}


/**
* @brief  Get the Frame Sync counter timeout value.
* @param  None.
* @retval Counter timeout.
*/
uint8_t LPAWUR_GetFrameSyncCntTimeout(void)
{
  return (uint8_t)(READ_REG_FIELD(LPAWUR->FRAME_CONFIG1,LPAWUR_FRAME_CONFIG1_FRAME_SYNC_COUNTER_TIMEOUT));
}


/**
* @brief  Set the Kp parameter for clock recovery.
* @param  v_kp
* @retval None
*/
void LPAWUR_SetKp(uint8_t v_kp)
{
  MODIFY_REG_FIELD(LPAWUR->FRAME_CONFIG1,LPAWUR_FRAME_CONFIG1_KP,v_kp);
}


/**
* @brief  Get teh Kp parameter.
* @param  None.
* @retval Kp.
*/
uint8_t LPAWUR_GetKp(void)
{
  return (uint8_t)(READ_REG_FIELD(LPAWUR->FRAME_CONFIG1,LPAWUR_FRAME_CONFIG1_KP));
}


/**
* @brief  Set the Ki parameter for clock recovery.
* @param  v_ki.
* @retval None.
*/
void LPAWUR_SetKi(uint8_t v_ki)
{
  MODIFY_REG_FIELD(LPAWUR->FRAME_CONFIG1,LPAWUR_FRAME_CONFIG1_KI,v_ki);
}


/**
* @brief  Get the Ki parameter.
* @param  None.
* @retval Ki.
*/
uint8_t LPAWUR_GetKi(void)
{
  return (uint8_t)(READ_REG_FIELD(LPAWUR->FRAME_CONFIG1,LPAWUR_FRAME_CONFIG1_KI));
}


/**
* @brief  Set the LPAWUR Sync word.
* @param  sync 32 bits SYNC word.
* @retval None.
*/
void LPAWUR_SetSync(uint32_t sync)
{
  uint16_t SyncHigh = (uint16_t)(sync>>16);
  uint16_t SyncLow = (uint16_t)(sync&0x0000FFFF);
  MODIFY_REG_FIELD(LPAWUR->FRAME_SYNC_CONFIG, LPAWUR_FRAME_SYNC_CONFIG_FRAME_SYNC_PATTERN_H, SyncHigh);
  MODIFY_REG_FIELD(LPAWUR->FRAME_SYNC_CONFIG, LPAWUR_FRAME_SYNC_CONFIG_FRAME_SYNC_PATTERN_L, SyncLow);
}


/**
* @brief  Get the LPAWUR SYNC word.
* @param  None.
* @retval 32 bits SYNC word (manchester coded).
*/
uint32_t LPAWUR_GetSync(void)
{
  uint16_t SyncHigh = READ_REG_FIELD(LPAWUR->FRAME_SYNC_CONFIG,LPAWUR_FRAME_SYNC_CONFIG_FRAME_SYNC_PATTERN_H);
  uint16_t SyncLow = READ_REG_FIELD(LPAWUR->FRAME_SYNC_CONFIG,LPAWUR_FRAME_SYNC_CONFIG_FRAME_SYNC_PATTERN_L);

  return (uint32_t)((SyncHigh<<16) + SyncLow);
}


/**
* @brief  Get the High SYNC pattern.
* @param  None.
* @retval 16bits HIGH SYNC pattern (manchester encoded).
*/
uint16_t LPAWUR_GetSyncPatternHigh(void)
{
  uint16_t SyncHigh = READ_REG_FIELD(LPAWUR->FRAME_SYNC_CONFIG,LPAWUR_FRAME_SYNC_CONFIG_FRAME_SYNC_PATTERN_H);
  return SyncHigh;
}


/**
* @brief  Get the Low SYNC pattern.
* @param  None.
* @retval 16bits LOW SYNC pattern (manchester encoded).
*/
uint16_t LPAWUR_GetSyncPatternLow(void)
{
  uint16_t SyncLow = READ_REG_FIELD(LPAWUR->FRAME_SYNC_CONFIG,LPAWUR_FRAME_SYNC_CONFIG_FRAME_SYNC_PATTERN_L);
  return SyncLow;
}


/**
* @brief  Set the LPAWUR Wake Up level.
* @param  xWakeUpLvl.
* @retval None.
*/
void LPAWUR_SetWakeUpLevel(WakeUpLevel xWakeUpLvl)
{
  assert_param(IS_WAKEUP(xWakeUpLvl));
  MODIFY_REG_FIELD(LPAWUR->RFIP_CONFIG,LPAWUR_RFIP_CONFIG_WAKEUP_LEVEL,xWakeUpLvl);
}


/**
* @brief  Get the LPAWUR Wake Up level.
* @param  None.
* @retval WakeUpLevel.
*/
WakeUpLevel LPAWUR_GetWakeUpLevel(void)
{
  return (WakeUpLevel)READ_REG_FIELD(LPAWUR->RFIP_CONFIG,LPAWUR_RFIP_CONFIG_WAKEUP_LEVEL);
}


/**
* @brief  Enable or Disable the LPAWUR feature.
* @param  state.
* @retval None.
*/
void LPAWUR_SetState(FunctionalState state)
{
  if(state == ENABLE)
  {
    SET_BIT(LPAWUR->RFIP_CONFIG,LPAWUR_RFIP_CONFIG_LPAWUR_ENABLE);
  }
  else
  {
    CLEAR_BIT(LPAWUR->RFIP_CONFIG,LPAWUR_RFIP_CONFIG_LPAWUR_ENABLE);
  }
}


/**
* @brief  Get the LPAWUR feature state.
* @param  None.
* @retval state.
*/
FunctionalState LPAWUR_GetState(void)
{
  if(READ_REG_FIELD(LPAWUR->RFIP_CONFIG,LPAWUR_RFIP_CONFIG_LPAWUR_ENABLE))
    return ENABLE;
  else
    return DISABLE;
}


/**
* @brief  Set the Energy Detector calibration value.
* @param  value calibration value.
* @retval None.
*/
void LPAWUR_SetEdIcal(EdIcal value)
{
  assert_param(IS_EDICAL(value));
  MODIFY_REG_FIELD(LPAWUR->RF_CONFIG,LPAWUR_RF_CONFIG_ED_ICAL,value);
}


/**
* @brief  Get the ED calibration value.
* @param  None.
* @retval calibration value.
*/
EdIcal LPAWUR_GetEdIcal(void)
{
  return (EdIcal)READ_REG_FIELD(LPAWUR->RF_CONFIG,LPAWUR_RF_CONFIG_ED_ICAL);
}


/**
* @brief  Set the AGC High Level value.
* @param  value AGC High Level value.
* @retval None.
*/
void LPAWUR_SetAGCHighLevel(AgcHighLevel value)
{
  assert_param(IS_AGC_HIGH_LEVEL(value));
  MODIFY_REG_FIELD(LPAWUR->RF_CONFIG, LPAWUR_RF_CONFIG_AGC_HIGH_LVL, value);
}


/**
* @brief  Get the AGC High Level value.
* @param  None.
* @retval AGC High Level value.
*/
AgcHighLevel LPAWUR_GetAGCHighLevel(void)
{
  return (AgcHighLevel)READ_REG_FIELD(LPAWUR->RF_CONFIG, LPAWUR_RF_CONFIG_AGC_HIGH_LVL);
}


/**
* @brief  Set the DC level shift enable.
* @param  state enable state.
* @retval None.
*/
void LPAWUR_EnableDCControl(FunctionalState state)
{
  if(state == ENABLE)
  {
    SET_BIT(LPAWUR->RF_CONFIG,LPAWUR_RF_CONFIG_ED_DC_CTRL); /* DC level shift (default value) */
  }
  else
  {
    CLEAR_BIT(LPAWUR->RF_CONFIG,LPAWUR_RF_CONFIG_ED_DC_CTRL);  /* No DC level shift */
  }
}

/**
* @brief  Set the AGC Low Level value.
* @param  value AGC Low Level value.
* @retval None.
*/
void LPAWUR_SetAGCLowLevel(AgcLowLevel value)
{
  assert_param(IS_AGC_LOW_LEVEL(value));
  MODIFY_REG_FIELD(LPAWUR->RF_CONFIG, LPAWUR_RF_CONFIG_AGC_LOW_LVL, value);
}


/**
* @brief  Get the AGC Low Level value.
* @param  None.
* @retval AGC Low Level value.
*/
AgcLowLevel LPAWUR_GetAGCLowLevel(void)
{
  return (AgcLowLevel)READ_REG_FIELD(LPAWUR->RF_CONFIG, LPAWUR_RF_CONFIG_AGC_LOW_LVL);
}


/**
* @brief  Set the Clock divider
* @param  div.
* @retval None.
*/
void LPAWUR_SetClkDiv(uint8_t div)
{
  MODIFY_REG_FIELD(LPAWUR->RF_CONFIG,LPAWUR_RF_CONFIG_CLKDIV,div);
}


/**
* @brief  Get the clock divider value.
* @param  None.
* @retval div.
*/
uint8_t LPAWUR_GetClkDiv(void)
{
  return (uint8_t)READ_REG_FIELD(LPAWUR->RF_CONFIG,LPAWUR_RF_CONFIG_CLKDIV);
}


/**
* @brief  Set the ED switch.
* @param  state.
* @retval None.
*/
void LPAWUR_SetEdSwitch(FunctionalState state)
{
  if(state == ENABLE)
  {
    SET_BIT(LPAWUR->RF_CONFIG,LPAWUR_RF_CONFIG_ED_SWITCH);
  }
  else
  {
    CLEAR_BIT(LPAWUR->RF_CONFIG,LPAWUR_RF_CONFIG_ED_SWITCH);
  }
}


/**
* @brief  Get the ED switch feature state.
* @param  None.
* @retval state.
*/
FunctionalState LPAWUR_GetEdSwitch(void)
{
  if(READ_REG_FIELD(LPAWUR->RF_CONFIG,LPAWUR_RF_CONFIG_ED_SWITCH))
    return ENABLE;
  else
    return DISABLE;
}


/**
* @brief  Set the AGC reset Mode.
* @param  mode.
* @retval None.
*/
void LPAWUR_SetAgcResetMode(AgcResetMode mode)
{
  if(mode == AGC_RESET_MODE_NEVER)
  {
    SET_BIT(LPAWUR->AGC_CONFIG,LPAWUR_AGC_CONFIG_AGC_RESET_MODE);
  }
  else
  {
    CLEAR_BIT(LPAWUR->AGC_CONFIG,LPAWUR_AGC_CONFIG_AGC_RESET_MODE);
  }
}


/**
* @brief  Get the AGC reset mode.
* @param  None.
* @retval state.
*/
AgcResetMode LPAWUR_GetAgcResetMode(void)
{
  if(READ_REG_FIELD(LPAWUR->AGC_CONFIG,LPAWUR_AGC_CONFIG_AGC_RESET_MODE))
    return AGC_RESET_MODE_NEVER;
  else
    return AGC_RESET_MODE_AFTER_FRAME;
}


/**
* @brief  Set the AGC Hold mode.
* @param  mode.
* @retval None.
*/
void LPAWUR_SetAgcHoldMode(AgcHoldType mode)
{
  assert_param(IS_AGC_HOLD_MODE(mode));
  MODIFY_REG_FIELD(LPAWUR->AGC_CONFIG,LPAWUR_AGC_CONFIG_AGC_HOLD_MODE,mode);
}


/**
* @brief  Get the AGC Hold mode.
* @param  None.
* @retval hold mode.
*/
AgcHoldType LPAWUR_GetAgcHoldMode(void)
{
  return (AgcHoldType)READ_REG_FIELD(LPAWUR->AGC_CONFIG,LPAWUR_AGC_CONFIG_AGC_HOLD_MODE);
}


/**
* @brief  Set the AGC mode.
* @param  mode.
* @retval None.
*/
void LPAWUR_SetAgcMode(AgcMode mode)
{
  assert_param(IS_AGC_MODE(mode));
  MODIFY_REG_FIELD(LPAWUR->AGC_CONFIG,LPAWUR_AGC_CONFIG_AGC_MODE,mode);
}


/**
* @brief  Get the AGC mode.
* @param  None.
* @retval mode.
*/
AgcMode LPAWUR_GetAgcMode(void)
{
  return (AgcMode)READ_REG_FIELD(LPAWUR->AGC_CONFIG,LPAWUR_AGC_CONFIG_AGC_MODE);
}


/**
* @brief  Get the payload.
* @param  buff buffer and length.
* @retval None.
*/
void LPAWUR_GetPayload(uint8_t* buff)
{
  uint32_t payload0 = READ_REG_FIELD(LPAWUR->PAYLOAD_0,LPAWUR_PAYLOAD_0_PAYLOAD_0);
  uint32_t payload1 = READ_REG_FIELD(LPAWUR->PAYLOAD_1,LPAWUR_PAYLOAD_1_PAYLOAD_1);
  uint8_t length = LPAWUR_GetPayloadLength();

  for( uint8_t i=0; i<4 ; i++)
  {
    buff[i] = ((payload0>>(i*8)) & 0xFF);
  }

  if(length > 4)
  {
    for(uint8_t j=0; j<length-4; j++)
    {
	buff[j+4] = ((payload1>>(j*8)) & 0xFF);
    }
  }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
