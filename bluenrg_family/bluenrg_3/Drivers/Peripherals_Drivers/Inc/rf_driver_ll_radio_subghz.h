/**
  ******************************************************************************
  * @file    rf_driver_ll_radio_subghz.h
  * @author  RF Application Team
  * @brief   Header file of Radio Subghz LL module.
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

#ifndef RF_DRIVER_LL_RADIO_SUBGHZ_H
#define RF_DRIVER_LL_RADIO_SUBGHZ_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "system_util.h"
#include "rf_driver_hal_power_manager.h"

#define HIGH_BAND_FACTOR	4	/*!< Band select factor for high band. Factor B in the equation of the user manual */
#define LOW_BAND_FACTOR		8	/*!< Band select factor for middle band. Factor B in the equation of the user manual */

#define LOW_BAND_LOWER_LIMIT		413000000   /*!< Lower limit of the low band */
#define LOW_BAND_UPPER_LIMIT		479000000   /*!< Upper limit of the low band */
#define HIGH_BAND_LOWER_LIMIT		826000000   /*!< Lower limit of the high band */
#define HIGH_BAND_UPPER_LIMIT		958000000   /*!< Upper limit of the high band */

#define MINIMUM_DATARATE		100	/*!< Minimum datarate supported by SPIRIT3 100 bps */
#define MAXIMUM_DATARATE		300000  /*!< Maximum datarate supported by SPIRIT3 300 ksps (600 kbps) */


/**
  * @brief  SPIRIT3 Commands codes enumeration
  */
typedef enum
{
  CMD_NOP = ((uint8_t)(0x00)),                 /* No action. This 'no' command can be requested at any time, whatever the on-going command or in IDLE */
  CMD_TX = ((uint8_t)(0x01)),			/* Start a TX sequence */
  CMD_RX = ((uint8_t)(0x02)),		        /* Start a RX sequence */
  CMD_LOCKRX = ((uint8_t)(0x03)),               /* Start a RF sequence up to PLL locked based on RX frequency */
  CMD_LOCKTX = ((uint8_t)(0x04)),               /* Start a RF sequence up to PLL locked based on TX frequency */
  CMD_SABORT = ((uint8_t)(0x05)),               /* Stop any on-going RX/TX/LOCKRX/LOCKTX command */
  CMD_CALIB_SAFEASK = ((uint8_t)(0x0A)),        /* Launch a PA Safe-ASK calibration to get the max safe PA code to be used */
  CMD_RELOAD_RX_TIMER = ((uint8_t)(0x06)),      /* Reload with a new timeout and new stop conditions and restart the RX Timer */
  CMD_CALIB_AGC = ((uint8_t)(0x0B))         /* Start needed HW features to run an AGC_ATTEN trim sequence at SW level */
} MRSubGCmd;

/**
  * @brief  SPIRIT3 Radio FSM state enumaration
  */
typedef enum
{
  STATE_IDLE = 0x00,
  STATE_ENA_RF_REG = 0x01,
  STATE_WAIT_ACTIVE2 = 0x02,
  STATE_ACTIVE2 = 0x03,
  STATE_ENA_CURR = 0x04,
  STATE_SYNTH_SETUP = 0x05,
  STATE_CALIB_VCO = 0x06,
  STATE_LOCKRXTX = 0x07,
  STATE_LOCKONTX = 0x08,
  STATE_EN_PA = 0x09,
  STATE_TX = 0x0A,
  STATE_PA_DWN_ANA = 0x0B,
  STATE_END_TX = 0x0C,
  STATE_LOCKONRX = 0x0D,
  STATE_EN_RX = 0x0E,
  STATE_EN_LNA = 0x0F,
  STATE_RX = 0x10,
  STATE_END_RX = 0x11,
  STATE_SYNTH_PWDN = 0x12
} MRSubGFSMState;

/**
 * @brief  SPIRIT3 Modulation enumeration
 */
typedef enum {
  MOD_2FSK       = 0x00, /*!< 2-FSK modulation selected */
  MOD_4FSK       = 0x01, /*!< 4-FSK modulation selected */
  MOD_2GFSK05    = 0x12, /*!< 2GFSK modulation selected with BT = 0.5 */
  MOD_2GFSK1     = 0x02, /*!< 2GFSK modulation selected with BT = 1 */
  MOD_4GFSK05 	 = 0x13, /*!< 4GFSK modulation selected with BT = 0.5 */
  MOD_4GFSK1 	 = 0x03, /*!< 4GFSK modulation selected with BT = 1 */
  MOD_ASK      	 = 0x05, /*!< ASK modulation selected. */
  MOD_OOK      	 = 0x05, /*!< OOK modulation selected. */
  MOD_POLAR      = 0x06, /*!< Polar modulation selected. */
  MOD_CW         = 0x07, /*!< CW modulation selected */
} MRSubGModSelect;

/**
* @brief  BT Values for GFSK
*/
typedef enum {
  BT_1 = 0,
  BT_05 = 1
} MRSubG_BTSelect;

/**
* @brief  SPIRIT3 PA modes
*/
typedef enum{
  PA_LEGACY     = 0x00, /* SPIRIT 'legacy' mode */
  PA_FIR 	= 0x01, /* FIR active (to be used in ASK/OOK modulation only) */
  PA_DIRECT     = 0x02  /* Direct mode */
} MRSubG_PAMode;

/**
* @brief  SPIRIT3 PA drive modes
*/
typedef enum{
  PA_DRV_TX             = 0x01, /* up to 10dBm */
  PA_DRV_TX_HP          = 0x02, /* up to 14dBm */
  PA_DRV_TX_TX_HP       = 0x03  /* up to 20dBm */
} MRSubG_PA_DRVMode;

/**
 * @brief  SPIRIT3 Transmission modes
 */
typedef enum {
  ISI_EQ_DISABLED = 0x00,
  ISI_EQ_SP = 0x01,
  ISI_EQ_DP = 0x02
} MRSubG_ISIEQMode;


/**
 * @brief  SPIRIT3 Transmission modes
 */
typedef enum{
  TX_NORMAL             = 0x00, /* Only payload is provided through RAM buffers Rest of the frame built from configuration registers (PREAMBLE, SYNC, CRC...). */
  TX_DIRECT_BUFFERS     = 0x01, /* Full bit stream (including PREAMBLE, SYNC, CRC...) to be provided through RAM buffers. */
  TX_DIRECT_GPIO        = 0x02, /* Full bit stream (including PREAMBLE, SYNC, CRC...) to be provided serially through the TX DATA GPIO. */
  TX_PN                 = 0x03  /* Internal PN generator send a polynomial bit stream on the antenna. */
} MRSubGTXMode;

/**
 * @brief  SPIRIT3 Reception modes
 */
typedef enum{
  RX_NORMAL 		= 0x00, /* Only payload is stored into the RAM buffers. CRC and packet length are readable in dedicated status registers */
  RX_DIRECT_BUFFERS 	= 0x01, /* Full bit stream is stored into the RAM buffers. */
  RX_DIRECT_GPIO 	= 0x02, /* Full bit stream is provided serially through the RX DATA GPIO */
  RX_IQ_SAMPLING 	= 0x03, /* Raw I/Q sampling taken at the output of the Channel filter inside the demodulator are stored in RAM. */
  RX_FREQDETEC_SAMPLING = 0x04, /* Raw data taken at the output of the frequency detector inside the demodulator (detection of the instantaneous frequency changes) are stored in RAM.*/
  RX_SOFTBIT_SAMPLING 	= 0x05  /* Raw data taken at the output of the post-filter inside the demodulator (soft bits before the 0/1 detection) are stored in RAM. */
} MRSubGRXMode;

/**
 * @brief  SPIRIT3 selection of the PRBS polynomial
 */
typedef enum{
  PN9   = 0x00, /* x^9 +x^5 + 1 with the initial state equal to 0x1FF. */
  PN15  = 0x01, /* x^15 +x^14 + 1 with the initial state equal to 0x7FFF. */  
} MRSubG_PN_SEL;

/**
 * @brief  SPIRIT3 Time Capture selection trigger
 */
typedef enum{
  TC_SEL_DISABLE        = 0x00, /* The feature is disabled, no time capture will occur. */
  TC_SEL_END_OF_TX      = 0x01, /* The interpolated absolute time is latched on end of transmission information. */
  TC_SEL_END_OF_RX      = 0x02, /* The interpolated absolute time is latched on end of reception information whatever the CRC result (RX OK or CRC error). */  
  TC_SEL_SYNC_DETECT    = 0x03, /* The interpolated absolute time is latched on the SYNC word detection event. */  
} MRSubG_TimeCaptureSel;


/**
 * @brief  SPIRIT3 AFC reinitialization option to improve the AFC behavior on frames with PREAMBLE smaller than 64 bits
 */
typedef enum{
  AFC_REINIT_NONE       = 0x00, /* No reinitialization allowed. */
  AFC_REINIT_RSSI       = 0x01, /* Reinitialization based on RSSI ramping. */
  AFC_REINIT_FDEV       = 0x02, /* Reinitialization based on abnormal Fdev. */  
  AFC_REINIT_RSSI_FDEV  = 0x03, /* Reinitialization based on both RSSI and abnormal Fdev. <- DEFAULT */
} MRSubG_AFCReinit;

/**
 * @brief  SPIRIT3 Sub-GHz Radio Config structure definition
 */
typedef struct {
  uint32_t          lFrequencyBase;     /*!< Specifies the base carrier frequency (in Hz) */
  MRSubGModSelect   xModulationSelect;  /*!< Specifies the modulation @ref MRSubGModSelect */
  uint32_t          lDatarate;          /*!< Specifies the datarate expressed in sps.*/
  uint32_t          lFreqDev;           /*!< Specifies the frequency deviation expressed in Hz. */
  uint32_t          lBandwidth;         /*!< Specifies the channel filter bandwidth expressed in Hz. */
  uint8_t           dsssExp;            /*!< Specifies the DSSS spreading exponent. Use 0 to disable DSSS. */
} SMRSubGConfig;


/**
 * @brief  SPIRIT3 Sub-GHz radio RF version structure definition
 */
typedef struct {
  uint8_t       revision;       /*!< Revision of the RFIP (to be used for metal fixes)*/
  uint8_t       version;        /*!< Version of the RFIP (to be used for cut upgrades)*/
  uint8_t       product;        /*!< Used for major upgrades (new protocols support / new features)*/
} SMRSubGVersion;

/**
 * @brief  SPIRIT3 MR_SUBG public interface
 */
SMRSubGVersion MRSubGGetVersion(void);
uint8_t MRSubG_Init(SMRSubGConfig* pxSRadioInitStruct);
void MRSubG_StrobeCommand(MRSubGCmd xCommandCode);
void MRSubG_SetTXMode(MRSubGTXMode rxMode);
void MRSubG_SetRXMode(MRSubGRXMode rxMode);
void MRSubG_GetInfo(SMRSubGConfig* pxSRadioInitStruct);
void MRSubG_SetChannel(uint8_t cChannel);
uint8_t MRSubG_GetChannel(void);
void MRSubG_SetChannelSpace(uint8_t lChannelSpace);
uint8_t MRSubG_GetChannelSpace(void);
void MRSubG_SetPAMode(MRSubG_PAMode paMode);
void MRSubG_SetPADriveMode(MRSubG_PA_DRVMode paDrvMode);
void MRSubG_SetFrequencyBase(uint32_t lFBase);
uint32_t MRSubG_GetFrequencyBase(void);
void MRSubG_SetDatarate(uint32_t lDatarate);
uint32_t MRSubG_GetDatarate(void);
void MRSubG_SetFrequencyDev(uint32_t lFDev);
uint32_t MRSubG_GetFrequencyDev(void);
void MRSubG_SetChannelBW(uint32_t lBandwidth);
uint32_t MRSubG_GetChannelBW(void);
void MRSubG_SetModulation(MRSubGModSelect xModulation, uint8_t dsssExponent);
void MRSubG_Force2FSKSync(FunctionalState forceSync);
MRSubGModSelect MRSubG_GetModulation(void);
int32_t MRSubG_GetRssidBm(void);
void MRSubG_SetRSSIThreshold(int16_t rssiTh);
int32_t MRSubG_GetRSSIThreshold(void);
void MRSubG_SetConstellationMapping(uint8_t nConMap);
void MRSubG_SwapSymbolMapping(FunctionalState xNewState);
void MRSubG_SetBTSel(MRSubG_BTSelect bSel);
void MRSubG_SetFrequencyInterpolator(FunctionalState xNewState);
void MRSubG_SetISIEqualizer(MRSubG_ISIEQMode isiEq);
void MRSubG_SetPADegen(FunctionalState xNewState);
void MRSubG_SetPALeveldBm(uint8_t cIndex, int8_t lPowerdBm, MRSubG_PA_DRVMode drvMode);
int8_t MRSubG_GetPALeveldBm(void);
uint8_t MRSubG_GetPALevelMaxIndex(void);
void MRSubG_SetPALevelMaxIndex(uint8_t cIndex);
void MRSubG_SetAlmostEmptyThresholdTx(uint8_t cThrTx);
void MRSubG_SetAlmostFullThresholdRx(uint8_t cThrRx);
uint32_t MRSubG_GetBytesOfTransaction(void);
MRSubGFSMState MRSubG_GetRadioFSMState(void);
PowerSaveLevels RADIO_STACK_SleepCheck(void);

#ifdef __cplusplus
}
#endif

#endif
