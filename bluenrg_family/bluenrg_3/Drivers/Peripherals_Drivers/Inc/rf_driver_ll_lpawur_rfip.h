/**
  ******************************************************************************
  * @file    rf_driver_ll_lpawur_rfip.h
  * @author  RF Application Team - EMEA
  * @brief   SPIRIT-3 LPAWUR RFIP module APIs
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
#ifndef RF_DRIVER_LL_LPAWUR_RFIP_H
#define RF_DRIVER_LL_LPAWUR_RFIP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "system_util.h"

#define MINIMUM_LPAWUR_PAYLOAD_LEN		0x1
#define MAXIMUM_LPAWUR_PAYLOAD_LEN		0x8

#define IS_LPAWUR_PAYLOAD_LENGTH(PAYLOAD_LEN) ((PAYLOAD_LEN)>=MINIMUM_LPAWUR_PAYLOAD_LEN && \
                                             (PAYLOAD_LEN)<=MAXIMUM_LPAWUR_PAYLOAD_LEN)

#define IS_WAKEUP(WAKEUP) (((WAKEUP) == WAKEUP_BIT_SYNC) || \
					((WAKEUP) == WAKEUP_FRAME_SYNC) || \
					((WAKEUP) == WAKEUP_FRAME_COMPLETE) || \
					((WAKEUP) == WAKEUP_FRAME_VALID) )

#define IS_EDICAL(EDICAL) (((EDICAL) == ED_ICAL_VBAT_1_70_TO_2_00) || \
					((EDICAL) == ED_ICAL_VBAT_2_00_TO_2_25) || \
					((EDICAL) == ED_ICAL_VBAT_2_25_TO_2_50) || \
					((EDICAL) == ED_ICAL_VBAT_2_50_TO_2_75) || \
					((EDICAL) == ED_ICAL_VBAT_2_75_TO_3_00) || \
					((EDICAL) == ED_ICAL_VBAT_3_00_TO_3_25) || \
					((EDICAL) == ED_ICAL_VBAT_3_25_TO_3_50) || \
					((EDICAL) == ED_ICAL_VBAT_3_50_TO_3_75) )

#define IS_AGC_HIGH_LEVEL(AGC_LEVEL) (((AGC_LEVEL) == AGC_VBAT_0500) || \
					((AGC_LEVEL) == AGC_VBAT_0575) || \
                                        ((AGC_LEVEL) == AGC_VBAT_0650) || \
                                        ((AGC_LEVEL) == AGC_VBAT_0725) || \
                                        ((AGC_LEVEL) == AGC_VBAT_0800) || \
                                        ((AGC_LEVEL) == AGC_VBAT_0875) || \
                                        ((AGC_LEVEL) == AGC_VBAT_0950) || \
                                        ((AGC_LEVEL) == AGC_VBAT_1025) || \
                                        ((AGC_LEVEL) == AGC_VBAT_1100) || \
                                        ((AGC_LEVEL) == AGC_VBAT_1175) || \
                                        ((AGC_LEVEL) == AGC_VBAT_1250) || \
                                        ((AGC_LEVEL) == AGC_VBAT_1325) || \
                                        ((AGC_LEVEL) == AGC_VBAT_1400) || \
                                        ((AGC_LEVEL) == AGC_VBAT_1475) || \
                                        ((AGC_LEVEL) == AGC_VBAT_1550) || \
                                        ((AGC_LEVEL) == AGC_VBAT_1625) )

#define IS_AGC_LOW_LEVEL(AGC_LEVEL) (((AGC_LEVEL) == AGC_LOW_0) || \
                                      ((AGC_LEVEL) == AGC_LOW_1) || \
                                      ((AGC_LEVEL) == AGC_LOW_2) || \
                                      ((AGC_LEVEL) == AGC_LOW_3) )
                                          
#define IS_AGC_HOLD_MODE(HOLD_MODE) (((HOLD_MODE) == AGC_HOLD_AFTER_PREAMBLE) || \
					((HOLD_MODE) == AGC_HOLD_AFTER_SYNC) )

#define IS_AGC_MODE(AGC_MODE) (((AGC_MODE) == AGC_MODE_MAX) || \
					((AGC_MODE) == AGC_MODE_OFF) || \
					((AGC_MODE) == AGC_MODE_ON) || \
					((AGC_MODE) == AGC_MODE_HOLD) )

#define IS_IRQ_ENABLE(IRQ_EN) (((IRQ_EN) == IRQ_BIT_SYNC_DETECTED) || \
					((IRQ_EN) == IRQ_FRAME_SYNC_COMPLETE) || \
					((IRQ_EN) == IRQ_FRAME_COMPLETE) || \
					((IRQ_EN) == IRQ_FRAME_VALID) )

#define IS_ERROR(ERROR) (((ERROR) == ERROR_NO) || \
					((ERROR) == ERROR_SYNC) || \
					((ERROR) == ERROR_FRAMING) || \
					((ERROR) == ERROR_CRC) )

#define IS_TREC_LOOP_ALGO(STATE) (((STATE) == DEFAULT) || ((STATE) == TWO_STEPS))

/**
 * @brief  SPIRIT3 LPAWUR Wakeup enumeration
 */
typedef enum {
  WAKEUP_BIT_SYNC               = 0x00, /*!< the bit Sync has been detected */
  WAKEUP_FRAME_SYNC    	        = 0x01, /*!< Frame sync detected */
  WAKEUP_FRAME_COMPLETE	        = 0x02, /*!< Frame_complete detected */
  WAKEUP_FRAME_VALID            = 0x03  /*!< Frame_Valid detected (default value) */
} WakeUpLevel;


/**
 * @brief  SPIRIT3 LPAWUR IRQ enumeration
 */
typedef enum {
  IRQ_BIT_SYNC_DETECTED		= 0x00, /*!< Enables interrupt on BIT_SYNC_DETECTED_F flag */
  IRQ_FRAME_SYNC_COMPLETE	= 0x01, /*!< Enables interrupt on FRAME_SYNC_COMPLETE_F flag */
  IRQ_FRAME_COMPLETE		= 0x02, /*!< Enables interrupt on FRAME_COMPLETE_F flag */
  IRQ_FRAME_VALID    		= 0x03  /*!< Enables interrupt on FRAME_VALID_F flag */
} LPAWUR_IrqEnable;

/**
 * @brief  SPIRIT3 LPAWUR STATUS enumeration
 */
typedef enum {
  STATUS_BIT_SYNC_DETECTED	= 0x00, /*!< status BIT_SYNC_DETECTED*/
  STATUS_FRAME_SYNC_COMPLETE 	= 0x01, /*!< status FRAME_SYNC_COMPLETE */
  STATUS_FRAME_COMPLETE		= 0x02, /*!< status FRAME_COMPLETE */
  STATUS_FRAME_VALID    	= 0x03  /*!< status FRAME_VALID */
} LPAWUR_Satus;

/**
 * @brief  SPIRIT3 LPAWUR ERROR enumeration
 */
typedef enum {
  ERROR_NO		= 0x00, /*!< NO ERROR */
  ERROR_SYNC	= 0x01, /*!< error in SYNC word */
  ERROR_FRAMING	= 0x02, /*!< FRAMING error */
  ERROR_CRC    	= 0x03  /*!< CRC error */
} LPAWUR_Error;

/**
 * @brief  SPIRIT3 LPAWUR ED_ICAL enumeration
 */
typedef enum {
  ED_ICAL_VBAT_1_70_TO_2_00	= 0x00, /*!< VBAT=1.7 to 2.0V (default value) */
  ED_ICAL_VBAT_2_00_TO_2_25	= 0x01, /*!< VBAT=2.0 to 2.25V */
  ED_ICAL_VBAT_2_25_TO_2_50	= 0x02, /*!< VBAT=2.25 to 2.5V */
  ED_ICAL_VBAT_2_50_TO_2_75	= 0x03, /*!< VBAT=2.5 to 2.75V */
  ED_ICAL_VBAT_2_75_TO_3_00	= 0x04, /*!< VBAT=2.75 to 3.0V */
  ED_ICAL_VBAT_3_00_TO_3_25	= 0x05, /*!< VBAT=3.0 to 3.25V */
  ED_ICAL_VBAT_3_25_TO_3_50	= 0x06, /*!< VBAT=3.25 to 3.5V */
  ED_ICAL_VBAT_3_50_TO_3_75	= 0x07  /*!< VBAT=3.5 to 3.75V */
} EdIcal;

/**
 * @brief  SPIRIT3 LPAWUR AGC_HIGH_LEVEL enumeration
 */
typedef enum {
  AGC_VBAT_0500	= 0x00, /*!< AGC VBAT=0.5V */
  AGC_VBAT_0575	= 0x01, /*!< AGC VBAT=0.575V */
  AGC_VBAT_0650	= 0x02, /*!< AGC VBAT=0.650V */
  AGC_VBAT_0725	= 0x03, /*!< AGC VBAT=0.725V */
  AGC_VBAT_0800	= 0x04, /*!< AGC VBAT=0.8V (default value) */
  AGC_VBAT_0875	= 0x05, /*!< AGC VBAT=0.875V */
  AGC_VBAT_0950	= 0x06, /*!< AGC VBAT=0.950V */
  AGC_VBAT_1025	= 0x07, /*!< AGC VBAT=1.025V */
  AGC_VBAT_1100	= 0x08, /*!< AGC VBAT=1.1V */  
  AGC_VBAT_1175	= 0x09, /*!< AGC VBAT=1.175V */  
  AGC_VBAT_1250	= 0x0A, /*!< AGC VBAT=1.25V */
  AGC_VBAT_1325	= 0x0B, /*!< AGC VBAT=1.325V */
  AGC_VBAT_1400	= 0x0C, /*!< AGC VBAT=1.4V */
  AGC_VBAT_1475	= 0x0D, /*!< AGC VBAT=1.475V */
  AGC_VBAT_1550	= 0x0E, /*!< AGC VBAT=1.55V */
  AGC_VBAT_1625	= 0x0F, /*!< AGC VBAT=1.625V */
} AgcHighLevel;

/**
 * @brief  SPIRIT3 LPAWUR AGC_LOW_LEVEL enumeration
 */
typedef enum {
  AGC_LOW_0	= 0x00, /*!< AGC reference level = 0 V */
  AGC_LOW_1	= 0x01, /*!< AGC reference level = 100 mV */
  AGC_LOW_2	= 0x02, /*!< AGC reference level = 200 mV (default value) */
  AGC_LOW_3	= 0x03, /*!< AGC reference level = 300 mV */
} AgcLowLevel;

/**
 * @brief  SPIRIT3 LPAWUR AGC Hold Mode enumeration
 */
typedef enum {
  AGC_HOLD_AFTER_PREAMBLE       = 0x00, /*!< AGC Hold after preamble detection (default) */
  AGC_HOLD_AFTER_SYNC		= 0x01  /*!< AGC Hold after Sync detection */
} AgcHoldType;

/**
 * @brief  SPIRIT3 LPAWUR AGC Hold Mode enumeration
 */
typedef enum {
  AGC_MODE_MAX 			= 0x00, /*!< AGC Mode MAX (default) */
  AGC_MODE_OFF    		= 0x01, /*!< AGC Mode OFF */
  AGC_MODE_ON     		= 0x02, /*!< AGC Mode ON */
  AGC_MODE_HOLD    		= 0x03  /*!< AGC HOLD: the AGC is ON and its value is frozen during the LPAWUR frame detection */
} AgcMode;

typedef enum {
  AGC_RESET_MODE_AFTER_FRAME    = 0x00, /*!< reset the AGC after the complete frame detection */
  AGC_RESET_MODE_NEVER          = 0x01  /*!< the AGC is never reset */
} AgcResetMode;

/**
 * @brief  SPIRIT3 LPAWUR STATUS enumeration
 */
typedef enum {
  BIT_SYNC_DETECTED_F   = 0x00, /*!< Preamble (= bit sync) has been detected, the content of the PAYLOAD_X registers is not yet	valid */
  FRAME_SYNC_COMPLETE_F = 0x01, /*!< Frame Sync has been detected, the content of the PAYLOAD_X registers is not yet valid */
  FRAME_COMPLETE_F	= 0x02, /*!< Frame (payload + CRC) received, the content of the PAYLOAD_X registers is valid */
  FRAME_VALID_F		= 0x03, /*!< Frame (payload + CRC) received without error (the CRC has been checked and is matching with the received CRC) */
  NO_STATUS			= 0x04  /*!< Frame (payload + CRC) received without error (the CRC has been checked and is matching with the received CRC) */
} LPAWUR_Status;

/**
 * @brief  SPIRIT3 LPAWUR TREC LOOP Algorithm 
 */
typedef enum
{
  DEFAULT = 0,          /*!< The timing recovery loop algorithm is a second order loop with a defined leakage (0.98) */
  TWO_STEPS = !DEFAULT  /*!< The timing recovery loop algorithm is with two steps. First order loop until preamble detected and second order loop after */
} TrecLoopAlgo;

/**
 * @brief  SPIRIT3 LPAWUR Frame Init structure definition
 */
typedef struct {
  uint8_t		SlowClkCyclePerBitCnt;   /*!< The number of expected slow clock cycle per each manchester coded bit (default 16) */
  uint8_t  		PayloadLength; 		 /*!<  The number of data Bytes in the payload (decoded).
 						Only the values 1 (8bits) to 8 (64bits) included are supported, other values are not supported
						and shall not be used.
 						Default value is 7 bytes / 56 bits */
  uint8_t		SyncThr;          	/*!< Detection threshold when receiving the Frame sync (Manchester encoded). The frame sync
						detection uses a correlator to detect the address.
						- If FRAME_CONFIG0.SYNC_LENGTH=0, recommended threshold is 16 (0x10)
						- If FRAME_CONFIG0.SYNC_LENGTH=1, recommended threshold is 32 (0x20)
 						Default value is 16 (0x10) */
  uint8_t         SyncLength;           	/*!< Frame sync pattern length:
						- If the input signal sync_len=0 (default), a 8-bits frame sync is used, the Low word
						(FRAME_CONFIG1.FRAME_SYNC_PATTERN_L) is describing the frame sync pattern
						value (16 bits after encoding)
						- If the input signal sync_len=1, a 16-bits frame sync is used, both Low
						(FRAME_SYNC_CONFIG.FRAME_SYNC_PATTERN_L) and High
						(FRAME_SYNC_CONFIG.FRAME_SYNC_PATTERN_H) words are storing the pattern
						value (32 bits after encoding) */
  uint8_t         PreambleThrCnt;         /*!< The number of transitions for preamble detection when receiving the Manchester encoded
						preamble.
 						Default value is 18 (0x12)*/
  FunctionalState	PreambleEnable;	        /*!< Preamble detection enable.
						- If PREAMBLE_ENABLE = 1 (default value) the sync detection starts after the preamble
						found,
						- If PREAMBLE_ENABLE = 0, the sync detection starts when the LPAWUR_rx is enabled*/
  uint8_t	      FrameSyncCntTimeout;	/*!< The timeout in Manchester encoded bits for the Frame Sync.
						It represents the number of samples after which a sync_error is raised if the frame sync is not
						detected.
						The default value is 70 (0x46)*/
  uint16_t        FrameSyncPattenHigh;	/*!<  The value of the frame sync pattern, High word, Manchester encoded, used only when the
						frame sync length is 32 bits (default 0x0000)*/
  uint16_t        FrameSyncPatternLow;	/*!< The value of the frame sync pattern, Low word, Manchester encoded, used both when the
						frame sync length is 16 bits and 32 bits (default 0x9696 which represent a frame sync value of
						0x99)*/
} SLPAWUR_FrameInit;

/**
 * @brief  SPIRIT3 LPAWUR RF Config structure definition
 */
typedef struct {
  EdIcal          	EnergyDetectorIcal;     /*!< Current versus VBAT calibration for ED*/  
  FunctionalState		AgcSpeedUpEnable;		/*!< Agc Speed Up enable */
  uint8_t			ClockDivider;		/*!< Calibrate the 4kHz clock (programmable divider)*/
  FunctionalState		EnergyDetectorSwitch;	/*!< Energy Detector as input switch (test mode) deactivated by default*/
  AgcResetMode		AgcResetMode;		/*!< The AGC reset behavior when the AGC is working in ON or HOLD mode
 									0x0: reset the AGC after the complete frame detection (default)
 									0x1: the AGC is never reset*/
  AgcHoldType		AgcHoldMode;		/*!< The behavior when the AGC is ON and is working in HOLD mode
 									0x0: hold after the preamble detection (default)
 									0x1: hold after the sync detection*/
  AgcMode			AgcMode;			/*!< Define the working mode of the AGC:
 									0x0: AGC MAX (default value) : the AGC is ON and is always operating at maximum value
 									0x1: AGC OFF: the AGC is OFF
 									0x2: AGC ON : the AGC is ON
 									0x3: AGC HOLD: the AGC is ON and its value is frozen during the LPAWUR frame detection,
									depending of the AGC_HOLD_MODE configuration after a preamble or after the frame sync.*/
} SLPAWUR_RFConfig;

/**
 * @brief  SPIRIT3 LPAWUR RF version structure definition
 */
typedef struct {
  uint8_t         revision;	/*!< Revision of the RFIP (to be used for metal fixes)*/
  uint8_t  		version;	/*!< Version of the RFIP (to be used for cut upgrades)*/
  uint8_t       	product;	/*!< Used for major upgrades (new protocols support / new features)*/
} SLPAWUR_Version;

/**
 * @brief  SPIRIT3 LPAWUR_RFIP public interface
 */
SLPAWUR_Version LPAWUR_GetVersion(void);


void LPAWUR_RFConfigInit(SLPAWUR_RFConfig* pxSLPAWUR_RFConfigStruct);
SLPAWUR_RFConfig LPAWUR_GetRFConfig(void);

void LPAWUR_FrameInit(SLPAWUR_FrameInit* pxSLPAWUR_FrameInitStruct);
SLPAWUR_FrameInit LPAWUR_GetFrameInfo(void);

void LPAWUR_IrqConfig(LPAWUR_IrqEnable IrqEnable);

LPAWUR_Status LPAWUR_GetStatus(void);
void LPAWUR_ClearStatus(void);

LPAWUR_Error LPAWUR_ReadError(void);
void LPAWUR_ClearErrorStatus(void);

void LPAWUR_SetSlowClkCyclePerBitCnt(uint8_t ClkCyclePerBit);
uint8_t LPAWUR_GetSlowClkCyclePerBitCnt(void);

void LPAWUR_SetPayloadLength(uint8_t length);
uint8_t LPAWUR_GetPayloadLength(void);

void LPAWUR_SetSyncThrCnt(uint8_t SyncThrCnt);
uint8_t LPAWUR_GetSyncThrCnt(void);

void LPAWUR_SetSyncLength(uint8_t SyncLen);
uint8_t LPAWUR_GetSyncLength(void);

void LPAWUR_SetPreambleThrCnt(uint8_t cnt);
uint8_t LPAWUR_GetPreambleThrCnt(void);

void LPAWUR_SetTRecAlgorithm(TrecLoopAlgo algoSel);
TrecLoopAlgo LPAWUR_GetTRecAlgorithm(void);

void LPAWUR_SetPreambleEnable(FunctionalState state);
FunctionalState LPAWUR_GetPreambleEnable(void);

void LPAWUR_SetFrameSyncCntTimeout(uint8_t timeout);
uint8_t LPAWUR_GetFrameSyncCntTimeout(void);

void LPAWUR_SetKp(uint8_t v_kp);
uint8_t LPAWUR_GetKp(void);

void LPAWUR_SetKi(uint8_t v_ki);
uint8_t LPAWUR_GetKi(void);

void LPAWUR_SetSync(uint32_t sync);
uint32_t LPAWUR_GetSync(void);
uint16_t LPAWUR_GetSyncPatternHigh(void);
uint16_t LPAWUR_GetSyncPatternLow(void);

void LPAWUR_SetWakeUpLevel(WakeUpLevel xWakeUpLvl);
WakeUpLevel LPAWUR_GetWakeUpLevel(void);

void LPAWUR_SetState(FunctionalState state);
FunctionalState LPAWUR_GetState(void);

void LPAWUR_SetEdIcal(EdIcal value);
EdIcal LPAWUR_GetEdIcal(void);

void LPAWUR_SetAGCHighLevel(AgcHighLevel value);
AgcHighLevel LPAWUR_GetAGCHighLevel(void);

void LPAWUR_EnableDCControl(FunctionalState state);

void LPAWUR_SetAGCLowLevel(AgcLowLevel value);
AgcLowLevel LPAWUR_GetAGCLowLevel(void);

void LPAWUR_AgcSpupEnable(FunctionalState state);
FunctionalState LPAWUR_AgcSpupGetState(void);

void LPAWUR_SetClkDiv(uint8_t div);
uint8_t LPAWUR_GetClkDiv(void);

void LPAWUR_SetEdSwitch(FunctionalState state);
FunctionalState LPAWUR_GetEdSwitch(void);

void LPAWUR_SetAgcResetMode(AgcResetMode state);
AgcResetMode LPAWUR_GetAgcResetMode(void);

void LPAWUR_SetAgcHoldMode(AgcHoldType mode);
AgcHoldType LPAWUR_GetAgcHoldMode(void);

void LPAWUR_SetAgcMode(AgcMode mode);
AgcMode LPAWUR_GetAgcMode(void);

void LPAWUR_GetPayload(uint8_t* buff);


#ifdef __cplusplus
}
#endif


#endif
