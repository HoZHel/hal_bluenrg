/**
 ******************************************************************************
 * @file    ble_controller.h
 * @author  AMS - RF Application Team
 * @brief   This file contains all the functions prototypes for the BLE CONTROLLER.
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2019 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BLECNTR_H
#define BLECNTR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "system_util.h"
#include "bleplat.h"  
  
/** @addtogroup BLECNTR_Peripheral  BLE CONTROLLER
 * @{
 */
  
/** @defgroup BLECNTR_Exported_Macros           Exported Macros
 * @{
 */

#define BLECNTR_ResultStatus                            BLEPLAT_CNTR_ResultStatus
#define BLECNTR_SUCCESS                                 BLEPLAT_CNTR_SUCCESS
#define BLECNTR_ERROR                                   BLEPLAT_CNTR_ERROR
#define BLECNTR_ERROR_TIMEOUT                           BLEPLAT_CNTR_ERROR_TIMEOUT
  
#define BLECNTR_Transaction                             BLEPLAT_CNTR_Transaction
#define BLECNTR_TxRx                                    BLEPLAT_CNTR_TxRx
#define BLECNTR_RxTx                                    BLEPLAT_CNTR_RxTx
#define BLECNTR_TxTx                                    BLEPLAT_CNTR_TxTx
#define BLECNTR_RxRx                                    BLEPLAT_CNTR_RxRx
  
#define BLECNTR_TXRXPACK_TypeDef                        BLEPLAT_CNTR_TXRXPACK_TypeDef

#define BLECNTR_Init                                    BLEPLAT_CNTR_Init
#define BLECNTR_Deinit                                  BLEPLAT_CNTR_Deinit
#define BLECNTR_InitGlobal                              BLEPLAT_CNTR_InitGlobal 
#define BLECNTR_ClearInterrupt                          BLEPLAT_CNTR_ClearInterrupt  
#define BLECNTR_ClearSemareq                            BLEPLAT_CNTR_ClearSemareq 
#define BLECNTR_TxRxSkip                                BLEPLAT_CNTR_TxRxSkip 
#define BLECNTR_GetCipherTextPtr                        BLEPLAT_CNTR_GetCipherTextPtr 
#define BLECNTR_GetClrTextPtr                           BLEPLAT_CNTR_GetClrTextPtr 
#define BLECNTR_GetEncKeyPtr                            BLEPLAT_CNTR_GetEncKeyPtr 
#define BLECNTR_GetEncryptDoneStatus                    BLEPLAT_CNTR_GetEncryptDoneStatus 
#define BLECNTR_GetIqsamplesMissingError                BLEPLAT_CNTR_GetIqsamplesMissingError 
#define BLECNTR_GetIqsamplesNumber                      BLEPLAT_CNTR_GetIqsamplesNumber 
#define BLECNTR_getIqsamplesReady                       BLEPLAT_CNTR_getIqsamplesReady 
#define BLECNTR_GetIsrLatency                           BLEPLAT_CNTR_GetIsrLatency 
#define BLECNTR_GetTimercapture                         BLEPLAT_CNTR_GetTimercapture 
#define BLECNTR_GlobDisableBlue                         BLEPLAT_CNTR_GlobDisableBlue 
#define BLECNTR_GlobEnableBlue                          BLEPLAT_CNTR_GlobEnableBlue 
#define BLECNTR_GlobEnableIntnoactivelerrorInt          BLEPLAT_CNTR_GlobEnableIntnoactivelerrorInt 
#define BLECNTR_GlobEnableOverrunAct2Int                BLEPLAT_CNTR_GlobEnableOverrunAct2Int 
#define BLECNTR_GlobGetDefaultAntennaid                 BLEPLAT_CNTR_GlobGetDefaultAntennaid 
#define BLECNTR_GlobGetWakeupinitdelay                  BLEPLAT_CNTR_GlobGetWakeupinitdelay 
#define BLECNTR_GlobReloadRadioConfigP                  BLEPLAT_CNTR_GlobReloadRadioConfigP 
#define BLECNTR_GlobSetChkflagautoclearena              BLEPLAT_CNTR_GlobSetChkflagautoclearena 
#define BLECNTR_GlobSetDefaultAntennaid                 BLEPLAT_CNTR_GlobSetDefaultAntennaid 
#define BLECNTR_GlobSetInitRadioDelayTxCal              BLEPLAT_CNTR_GlobSetInitRadioDelayTxCal 
#define BLECNTR_GlobSetInitRadioDelayTxNocal            BLEPLAT_CNTR_GlobSetInitRadioDelayTxNocal 
#define BLECNTR_GlobSetRadioConfigP                     BLEPLAT_CNTR_GlobSetRadioConfigP 
#define BLECNTR_GlobSetWakeupinitdelay                  BLEPLAT_CNTR_GlobSetWakeupinitdelay 
#define BLECNTR_GlobWriteConfigEnd                      BLEPLAT_CNTR_GlobWriteConfigEnd 
#define BLECNTR_GlobWritePeriodslow                     BLEPLAT_CNTR_GlobWritePeriodslow 
#define BLECNTR_GlobWriteRcvdelay                       BLEPLAT_CNTR_GlobWriteRcvdelay 
#define BLECNTR_GlobWriteRcvdelay1                      BLEPLAT_CNTR_GlobWriteRcvdelay1 
#define BLECNTR_GlobWriteSlot                           BLEPLAT_CNTR_GlobWriteSlot 
#define BLECNTR_GlobWriteTimer12initdelaycal            BLEPLAT_CNTR_GlobWriteTimer12initdelaycal 
#define BLECNTR_GlobWriteTimer2initdelaynocal           BLEPLAT_CNTR_GlobWriteTimer2initdelaynocal 
#define BLECNTR_GlobWriteTxDataReadyCheck               BLEPLAT_CNTR_GlobWriteTxDataReadyCheck 
#define BLECNTR_GlobWriteTxReadyTimeout                 BLEPLAT_CNTR_GlobWriteTxReadyTimeout 
#define BLECNTR_GlobWriteTxdelay                        BLEPLAT_CNTR_GlobWriteTxdelay
#define BLECNTR_GlobWriteTxdelayEnd                     BLEPLAT_CNTR_GlobWriteTxdelayEnd 
#define BLECNTR_IntGetIntStatusAnyHwError               BLEPLAT_CNTR_IntGetIntStatusAnyHwError  
#define BLECNTR_IntGetIntStatusRxOverflowError          BLEPLAT_CNTR_IntGetIntStatusRxOverflowError  
#define BLECNTR_IntGetIntStatusBitAct2Error             BLEPLAT_CNTR_IntGetIntStatusBitAct2Error  
#define BLECNTR_IntGetIntStatusBitTimerOverrun          BLEPLAT_CNTR_IntGetIntStatusBitTimerOverrun  
#define BLECNTR_IntGetIntStatusCrcErr                   BLEPLAT_CNTR_IntGetIntStatusCrcErr  
#define BLECNTR_IntGetIntStatusDone                     BLEPLAT_CNTR_IntGetIntStatusDone  
#define BLECNTR_IntGetIntStatusEncErr                   BLEPLAT_CNTR_IntGetIntStatusEncErr  
#define BLECNTR_IntGetIntStatusLenErr                   BLEPLAT_CNTR_IntGetIntStatusLenErr  
#define BLECNTR_IntGetIntStatusNoactiveError            BLEPLAT_CNTR_IntGetIntStatusNoactiveError  
#define BLECNTR_IntGetIntStatusRxOk                     BLEPLAT_CNTR_IntGetIntStatusRxOk  
#define BLECNTR_IntGetIntStatusTimeout                  BLEPLAT_CNTR_IntGetIntStatusTimeout  
#define BLECNTR_IntGetIntStatusTrigRcv                  BLEPLAT_CNTR_IntGetIntStatusTrigRcv  
#define BLECNTR_IntGetIntStatusTxDone                   BLEPLAT_CNTR_IntGetIntStatusTxDone  
#define BLECNTR_IntGetIntStatusTxOk                     BLEPLAT_CNTR_IntGetIntStatusTxOk  
#define BLECNTR_IntGetIntStatusTxRxSkip                 BLEPLAT_CNTR_IntGetIntStatusTxRxSkip  
#define BLECNTR_IntGetIntStatusTxError1                 BLEPLAT_CNTR_IntGetIntStatusTxError1  
#define BLECNTR_IntGetIntStatusTxError3                 BLEPLAT_CNTR_IntGetIntStatusTxError3  
#define BLECNTR_PacketClrCrcinitSel                     BLEPLAT_CNTR_PacketClrCrcinitSel 
#define BLECNTR_PacketClrCteSamplingEn                  BLEPLAT_CNTR_PacketClrCteSamplingEn 
#define BLECNTR_PacketClrIncChan                        BLEPLAT_CNTR_PacketClrIncChan 
#define BLECNTR_PacketClrPllTrig                        BLEPLAT_CNTR_PacketClrPllTrig 
#define BLECNTR_PacketDisableWhitening                  BLEPLAT_CNTR_PacketDisableWhitening 
#define BLECNTR_PacketGetCteSamplingEn                  BLEPLAT_CNTR_PacketGetCteSamplingEn 
#define BLECNTR_PacketGetDataPtr                        BLEPLAT_CNTR_PacketGetDataPtr 
#define BLECNTR_PacketInitTo0                           BLEPLAT_CNTR_PacketInitTo0 
#define BLECNTR_PacketSetAdvPduFormat                   BLEPLAT_CNTR_PacketSetAdvPduFormat 
#define BLECNTR_PacketSetCrcinitSel                     BLEPLAT_CNTR_PacketSetCrcinitSel 
#define BLECNTR_PacketSetCteSamplingEn                  BLEPLAT_CNTR_PacketSetCteSamplingEn 
#define BLECNTR_PacketSetDataPduFormat                  BLEPLAT_CNTR_PacketSetDataPduFormat 
#define BLECNTR_PacketSetDataPtr                        BLEPLAT_CNTR_PacketSetDataPtr 
#define BLECNTR_PacketSetIncChan                        BLEPLAT_CNTR_PacketSetIncChan 
#define BLECNTR_PacketSetIntCrcErr                      BLEPLAT_CNTR_PacketSetIntCrcErr 
#define BLECNTR_PacketSetIntDone                        BLEPLAT_CNTR_PacketSetIntDone 
#define BLECNTR_PacketSetIntRcvOk                       BLEPLAT_CNTR_PacketSetIntRcvOk 
#define BLECNTR_PacketSetIntTimeout                     BLEPLAT_CNTR_PacketSetIntTimeout 
#define BLECNTR_PacketSetIntTrigRcv                     BLEPLAT_CNTR_PacketSetIntTrigRcv 
#define BLECNTR_PacketSetIntTxOk                        BLEPLAT_CNTR_PacketSetIntTxOk 
#define BLECNTR_PacketSetKeepsemareq                    BLEPLAT_CNTR_PacketSetKeepsemareq 
#define BLECNTR_PacketSetNextPtr                        BLEPLAT_CNTR_PacketSetNextPtr 
#define BLECNTR_PacketSetNextRxMode                     BLEPLAT_CNTR_PacketSetNextRxMode 
#define BLECNTR_PacketSetNextSlot                       BLEPLAT_CNTR_PacketSetNextSlot 
#define BLECNTR_PacketSetNextTxMode                     BLEPLAT_CNTR_PacketSetNextTxMode 
#define BLECNTR_PacketSetNsEn                           BLEPLAT_CNTR_PacketSetNsEn 
#define BLECNTR_PacketSetPllTrig                        BLEPLAT_CNTR_PacketSetPllTrig 
#define BLECNTR_PacketSetRxReady                        BLEPLAT_CNTR_PacketSetRxReady 
#define BLECNTR_PacketSetTimeout                        BLEPLAT_CNTR_PacketSetTimeout 
#define BLECNTR_PacketSetTimer2Active                   BLEPLAT_CNTR_PacketSetTimer2Active 
#define BLECNTR_PacketSetTimerTrigDone                  BLEPLAT_CNTR_PacketSetTimerTrigDone 
#define BLECNTR_PacketSetTimerTrigRcv                   BLEPLAT_CNTR_PacketSetTimerTrigRcv 
#define BLECNTR_PacketSetTxReady                        BLEPLAT_CNTR_PacketSetTxReady 
#define BLECNTR_SetRadioConfig                          BLEPLAT_CNTR_SetRadioConfig 
#define BLECNTR_SetRcvLen                               BLEPLAT_CNTR_SetRcvLen 
#define BLECNTR_SmCteOff                                BLEPLAT_CNTR_SmCteOff 
#define BLECNTR_SmCteOn                                 BLEPLAT_CNTR_SmCteOn 
#define BLECNTR_SmEnRadioConfig                         BLEPLAT_CNTR_SmEnRadioConfig 
#define BLECNTR_SmEncOff                                BLEPLAT_CNTR_SmEncOff 
#define BLECNTR_SmEncOn                                 BLEPLAT_CNTR_SmEncOn 
#define BLECNTR_SmGetAccessAddr                         BLEPLAT_CNTR_SmGetAccessAddr 
#define BLECNTR_SmGetChannelMap                         BLEPLAT_CNTR_SmGetChannelMap 
#define BLECNTR_SmGetCteAntennaPatternLen               BLEPLAT_CNTR_SmGetCteAntennaPatternLen 
#define BLECNTR_SmGetCteAodNaoa                         BLEPLAT_CNTR_SmGetCteAodNaoa 
#define BLECNTR_SmGetCteSlotWidth                       BLEPLAT_CNTR_SmGetCteSlotWidth 
#define BLECNTR_SmGetCteStatus                          BLEPLAT_CNTR_SmGetCteStatus 
#define BLECNTR_SmGetCteTime                            BLEPLAT_CNTR_SmGetCteTime 
#define BLECNTR_SmGetEncIvPtr                           BLEPLAT_CNTR_SmGetEncIvPtr 
#define BLECNTR_SmGetEncKeyPtr                          BLEPLAT_CNTR_SmGetEncKeyPtr 
#define BLECNTR_SmGetEncStatus                          BLEPLAT_CNTR_SmGetEncStatus 
#define BLECNTR_SmGetHopIncr                            BLEPLAT_CNTR_SmGetHopIncr 
#define BLECNTR_SmGetMode                               BLEPLAT_CNTR_SmGetMode 
#define BLECNTR_SmGetPrevRxPacketDataPtr                BLEPLAT_CNTR_SmGetPrevRxPacketDataPtr 
#define BLECNTR_SmGetPrevRxPacketPtr                    BLEPLAT_CNTR_SmGetPrevRxPacketPtr 
#define BLECNTR_SmGetPrevTxPacketDataPtr                BLEPLAT_CNTR_SmGetPrevTxPacketDataPtr 
#define BLECNTR_SmGetPrevTxPacketPtr                    BLEPLAT_CNTR_SmGetPrevTxPacketPtr 
#define BLECNTR_SmGetRemapChan                          BLEPLAT_CNTR_SmGetRemapChan 
#define BLECNTR_SmGetRxCount                            BLEPLAT_CNTR_SmGetRxCount 
#define BLECNTR_SmGetRxPhy                              BLEPLAT_CNTR_SmGetRxPhy 
#define BLECNTR_SmGetTxPacketPtr                        BLEPLAT_CNTR_SmGetTxPacketPtr 
#define BLECNTR_SmGetTxPhy                              BLEPLAT_CNTR_SmGetTxPhy 
#define BLECNTR_SmGetTxPwr                              BLEPLAT_CNTR_SmGetTxPwr 
#define BLECNTR_SmGetUnmappedChan                       BLEPLAT_CNTR_SmGetUnmappedChan 
#define BLECNTR_SmInitTo0                               BLEPLAT_CNTR_SmInitTo0 
#define BLECNTR_SmSetAccessAddr                         BLEPLAT_CNTR_SmSetAccessAddr 
#define BLECNTR_SmSetChannelMap                         BLEPLAT_CNTR_SmSetChannelMap 
#define BLECNTR_SmSetCrcInit                            BLEPLAT_CNTR_SmSetCrcInit 
#define BLECNTR_SmSetCteAntennaPatternLen               BLEPLAT_CNTR_SmSetCteAntennaPatternLen 
#define BLECNTR_SmSetCteAntennaPatternPtr               BLEPLAT_CNTR_SmSetCteAntennaPatternPtr 
#define BLECNTR_SmGetCteAntennaPatternPtr               BLEPLAT_CNTR_SmGetCteAntennaPatternPtr 
#define BLECNTR_SmSetCteAoa                             BLEPLAT_CNTR_SmSetCteAoa 
#define BLECNTR_SmSetCteAod                             BLEPLAT_CNTR_SmSetCteAod 
#define BLECNTR_SmSetCteIqsamplesPtr                    BLEPLAT_CNTR_SmSetCteIqsamplesPtr 
#define BLECNTR_SmSetCteMaxIqsamplesNumb                BLEPLAT_CNTR_SmSetCteMaxIqsamplesNumb 
#define BLECNTR_SmSetCteSlotWidth                       BLEPLAT_CNTR_SmSetCteSlotWidth 
#define BLECNTR_SmSetCteTime                            BLEPLAT_CNTR_SmSetCteTime 
#define BLECNTR_SmSetDataLength                         BLEPLAT_CNTR_SmSetDataLength 
#define BLECNTR_SmSetDataLengthExtnEn                   BLEPLAT_CNTR_SmSetDataLengthExtnEn 
#define BLECNTR_SmSetHopIncr                            BLEPLAT_CNTR_SmSetHopIncr 
#define BLECNTR_SmSetRemapChan                          BLEPLAT_CNTR_SmSetRemapChan 
#define BLECNTR_SmSetRxCount                            BLEPLAT_CNTR_SmSetRxCount 
#define BLECNTR_SmSetRxCountDirectionBit                BLEPLAT_CNTR_SmSetRxCountDirectionBit 
#define BLECNTR_SmSetRxMode                             BLEPLAT_CNTR_SmSetRxMode 
#define BLECNTR_SmSetRxPacketPtr                        BLEPLAT_CNTR_SmSetRxPacketPtr 
#define BLECNTR_SmSetRxPhy                              BLEPLAT_CNTR_SmSetRxPhy 
#define BLECNTR_SmSetTxCount                            BLEPLAT_CNTR_SmSetTxCount 
#define BLECNTR_SmSetTxCountDirectionBit                BLEPLAT_CNTR_SmSetTxCountDirectionBit 
#define BLECNTR_SmSetTxMode                             BLEPLAT_CNTR_SmSetTxMode 
#define BLECNTR_SmSetTxPacketPtr                        BLEPLAT_CNTR_SmSetTxPacketPtr 
#define BLECNTR_SmSetTxPhy                              BLEPLAT_CNTR_SmSetTxPhy 
#define BLECNTR_SmSetTxPwr                              BLEPLAT_CNTR_SmSetTxPwr 
#define BLECNTR_SmSetUnmappedChan                       BLEPLAT_CNTR_SmSetUnmappedChan 
#define BLECNTR_SmToggleNesn                            BLEPLAT_CNTR_SmToggleNesn 
#define BLECNTR_SmToggleSn                              BLEPLAT_CNTR_SmToggleSn 
#define BLECNTR_StartEncrypt                            BLEPLAT_CNTR_StartEncrypt 
#define BLECNTR_TimeDiff                                BLEPLAT_CNTR_TimeDiff 
#define BLECNTR_DemodDelaySt                            BLEPLAT_CNTR_DemodDelaySt 
#define BLECNTR_GeTimer2TimeoutForIfs                   BLEPLAT_CNTR_GeTimer2TimeoutForIfs 

/**
 * @}
 */

/** @defgroup BLECNTR_Exported_Types Exported Types
 * @{
 */
	

/**
 * @}
 */

/** @defgroup BLECNTR_Exported_Constants  Exported Constants
 * @{
 */


/* Start of declarations relative to the Blue HW registers 
 * Note that the declaration of BLUE_t is redundant w.r.t the declaration of
 * BLUE_TypeDef done in file BlueNRG_LP.h but it is currently needed beacause
 * most APIs use the bit fields declared in BLUE_t
 */

#ifndef LL_PHY_CODED
#define LL_PHY_CODED 0x04U
#endif

#ifndef CONV_ADR
#define CONV_ADR(ADDR) ((uint8_t*)(ADDR))
#endif

#ifndef BLUE_TRANS_STRUCT_PTR_CAST
#define BLUE_TRANS_STRUCT_PTR_CAST(PTR) ((BlueTransStruct*)(void*)(uint8_t*)(PTR))
#endif


#define PHY_1MBPS          (0x0U)
#define PHY_2MBPS          (0x1U)
#define PHY_CODED_S2       (0x6U)
#define PHY_CODED_S8       (0x4U)

#define BIT_TX_SKIP (0x0UL)

/**
 * @}
 */


/** @defgroup BLECNTR_Exported_Functions        Exported Functions
 * @{
 */
BLECNTR_ResultStatus BLECNTR_Init(void);

BLECNTR_ResultStatus BLECNTR_Deinit(void);

void BLECNTR_InitGlobal(void);
void BLECNTR_ClearInterrupt(uint32_t x);
void BLECNTR_ClearSemareq(void);
void BLECNTR_TxRxSkip(void);
uint32_t* BLECNTR_GetCipherTextPtr(void);
uint32_t* BLECNTR_GetClrTextPtr(void);
uint32_t* BLECNTR_GetEncKeyPtr(void);
BOOL BLECNTR_GetEncryptDoneStatus(void);
uint8_t BLECNTR_GetIqsamplesMissingError(void);
uint8_t BLECNTR_GetIqsamplesNumber(void);
uint8_t BLECNTR_getIqsamplesReady(void);
uint8_t BLECNTR_GetIsrLatency(void);
uint32_t BLECNTR_GetTimercapture(void);
void BLECNTR_GlobDisableBlue(void);
void BLECNTR_GlobEnableBlue(void);
void BLECNTR_GlobEnableIntnoactivelerrorInt(void);
void BLECNTR_GlobEnableTxRxSkipInt(void);
void BLECNTR_GlobEnableOverrunAct2Int(void);
uint8_t BLECNTR_GlobGetDefaultAntennaid(void);
uint8_t BLECNTR_GlobGetWakeupinitdelay(void);
void BLECNTR_GlobReloadRadioConfigP(void);
void BLECNTR_GlobSetChkflagautoclearena(void);
void BLECNTR_GlobSetDefaultAntennaid(uint8_t x);
void BLECNTR_GlobSetInitRadioDelayTxCal(uint8_t x);
void BLECNTR_GlobSetInitRadioDelayTxNocal(uint8_t x);
void BLECNTR_GlobSetRadioConfigP(volatile uint32_t* x);
void BLECNTR_GlobSetWakeupinitdelay(uint8_t x);
void BLECNTR_GlobWriteConfigEnd(uint8_t x);
void BLECNTR_GlobWritePeriodslow(uint16_t x);
void BLECNTR_GlobWriteRcvdelay(uint8_t x);
void BLECNTR_GlobWriteRcvdelay1(uint8_t x);
void BLECNTR_GlobWriteSlot(uint8_t slotNo);
void BLECNTR_GlobWriteTimer12initdelaycal(uint8_t x);
void BLECNTR_GlobWriteTimer2initdelaynocal(uint8_t x);
void BLECNTR_GlobWriteTxDataReadyCheck(uint8_t x);
void BLECNTR_GlobWriteTxReadyTimeout(uint8_t x);
void BLECNTR_GlobWriteTxdelay(uint8_t x) ;
void BLECNTR_GlobWriteTxdelayEnd(uint8_t x);
uint32_t BLECNTR_IntGetIntStatusAnyHwError(uint32_t x);
uint32_t BLECNTR_IntGetIntStatusRxOverflowError(uint32_t x);
uint32_t BLECNTR_IntGetIntStatusBitAct2Error(uint32_t x);
uint32_t BLECNTR_IntGetIntStatusBitTimerOverrun(uint32_t x);
uint32_t BLECNTR_IntGetIntStatusCrcErr(uint32_t x);
uint32_t BLECNTR_IntGetIntStatusDone(uint32_t x);
uint32_t BLECNTR_IntGetIntStatusEncErr(uint32_t x);
uint32_t BLECNTR_IntGetIntStatusLenErr(uint32_t x);
uint32_t BLECNTR_IntGetIntStatusNoactiveError(uint32_t x);
uint32_t BLECNTR_IntGetIntStatusRxOk(uint32_t x);
uint32_t BLECNTR_IntGetIntStatusTimeout(uint32_t x);
uint32_t BLECNTR_IntGetIntStatusTrigRcv(uint32_t x);
uint32_t BLECNTR_IntGetIntStatusTxDone(uint32_t x);
uint32_t BLECNTR_IntGetIntStatusTxOk(uint32_t x);
uint32_t BLECNTR_IntGetIntStatusTxRxSkip(uint32_t x);
uint32_t BLECNTR_IntGetIntStatusTxError1(uint32_t x);
uint32_t BLECNTR_IntGetIntStatusTxError3(uint32_t x);
void BLECNTR_PacketClrCrcinitSel(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketClrCteSamplingEn(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketClrIncChan(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketClrPllTrig(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketDisableWhitening(BLECNTR_TXRXPACK_TypeDef* packetP);
uint8_t BLECNTR_PacketGetCteSamplingEn(BLECNTR_TXRXPACK_TypeDef* packetP);
uint8_t* BLECNTR_PacketGetDataPtr(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketInitTo0(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetAdvPduFormat(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetCrcinitSel(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetCteSamplingEn(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetDataPduFormat(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetDataPtr(BLECNTR_TXRXPACK_TypeDef* packetP, void* dataP);
void BLECNTR_PacketSetIncChan(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetIntCrcErr(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetIntDone(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetIntRcvOk(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetIntTimeout(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetIntTrigRcv(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetIntTxOk(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetKeepsemareq(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetNextPtr(BLECNTR_TXRXPACK_TypeDef* packetP, BLECNTR_TXRXPACK_TypeDef* packetNextP);
void BLECNTR_PacketSetNextRxMode(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetNextSlot(BLECNTR_TXRXPACK_TypeDef* packetP, uint8_t slot);
void BLECNTR_PacketSetNextTxMode(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetNsEn(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetPllTrig(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetRxReady(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetTimeout(BLECNTR_TXRXPACK_TypeDef* packetP, uint32_t x);
void BLECNTR_PacketSetTimer2Active(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetTimerTrigDone(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetTimerTrigRcv(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_PacketSetTxReady(BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_SetRadioConfig(uint8_t* value);
void BLECNTR_SetRcvLen(BLECNTR_TXRXPACK_TypeDef* packetP, uint32_t rcvLen);
void BLECNTR_SmCteOff(uint8_t smNo);
void BLECNTR_SmCteOn(uint8_t smNo);
void BLECNTR_SmEnRadioConfig(uint8_t smNo, uint32_t enable);
void BLECNTR_SmEncOff(uint8_t smNo);
void BLECNTR_SmEncOn(uint8_t smNo);
uint32_t BLECNTR_SmGetAccessAddr(uint8_t smNo);
void BLECNTR_SmGetChannelMap(uint8_t smNo, uint8_t* chanMap);
uint8_t BLECNTR_SmGetCteAntennaPatternLen(uint8_t smNo);
uint8_t BLECNTR_SmGetCteAodNaoa(uint8_t smNo);
uint8_t BLECNTR_SmGetCteSlotWidth(uint8_t smNo);
uint8_t BLECNTR_SmGetCteStatus(uint8_t smNo);
uint8_t BLECNTR_SmGetCteTime(uint8_t smNo);
uint32_t* BLECNTR_SmGetEncIvPtr(uint8_t smNo);
uint32_t* BLECNTR_SmGetEncKeyPtr(uint8_t smNo);
uint8_t BLECNTR_SmGetEncStatus(uint8_t smNo);
uint8_t BLECNTR_SmGetHopIncr(uint8_t smNo);
uint8_t BLECNTR_SmGetMode(uint8_t smNo);
uint8_t* BLECNTR_SmGetPrevRxPacketDataPtr(uint8_t smNo);
BLECNTR_TXRXPACK_TypeDef* BLECNTR_SmGetPrevRxPacketPtr(uint8_t smNo) ;
uint8_t* BLECNTR_SmGetPrevTxPacketDataPtr(uint8_t smNo);
BLECNTR_TXRXPACK_TypeDef* BLECNTR_SmGetPrevTxPacketPtr(uint8_t smNo);
uint8_t BLECNTR_SmGetRemapChan(uint8_t smNo);
void BLECNTR_SmGetRxCount(uint8_t smNo, uint32_t* packetCount);
uint8_t BLECNTR_SmGetRxPhy(uint8_t smNo);
BLECNTR_TXRXPACK_TypeDef* BLECNTR_SmGetTxPacketPtr(uint8_t smNo);
uint8_t BLECNTR_SmGetTxPhy(uint8_t smNo);
uint8_t BLECNTR_SmGetTxPwr(uint8_t smNo);
uint8_t BLECNTR_SmGetUnmappedChan(uint8_t smNo);
void BLECNTR_SmInitTo0(uint8_t smNo);
void BLECNTR_SmSetAccessAddr(uint8_t smNo, uint32_t x);
void BLECNTR_SmSetChannelMap(uint8_t smNo, uint8_t* chanMap);
void BLECNTR_SmSetCrcInit(uint8_t smNo, uint32_t x);
void BLECNTR_SmSetCteAntennaPatternLen(uint8_t smNo, uint8_t antPattLen);
void BLECNTR_SmSetCteAntennaPatternPtr(uint8_t smNo, uint8_t* antPattP);
uint32_t BLECNTR_SmGetCteAntennaPatternPtr(uint8_t smNo);
void BLECNTR_SmSetCteAoa(uint8_t smNo);
void BLECNTR_SmSetCteAod(uint8_t smNo);
void BLECNTR_SmSetCteIqsamplesPtr(uint8_t smNo, uint32_t* iqSamplesP);
void BLECNTR_SmSetCteMaxIqsamplesNumb(uint8_t smNo, uint8_t iqsamplesNumb);
void BLECNTR_SmSetCteSlotWidth(uint8_t smNo, uint32_t cteSlot);
void BLECNTR_SmSetCteTime(uint8_t smNo, uint8_t cteTime);
void BLECNTR_SmSetDataLength(uint8_t smNo, uint8_t length);
void BLECNTR_SmSetDataLengthExtnEn(uint8_t smNo);
void BLECNTR_SmSetHopIncr(uint8_t smNo, uint8_t x);
void BLECNTR_SmSetRemapChan(uint8_t smNo, uint8_t chan);
void BLECNTR_SmSetRxCount(uint8_t smNo, uint32_t* packetCount);
void BLECNTR_SmSetRxCountDirectionBit(uint8_t smNo);
void BLECNTR_SmSetRxMode(uint8_t smNo);
void BLECNTR_SmSetRxPacketPtr(uint8_t smNo, BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_SmSetRxPhy(uint8_t smNo, uint8_t rxPhy);
void BLECNTR_SmSetTxCount(uint8_t smNo, uint32_t* packetCount);
void BLECNTR_SmSetTxCountDirectionBit(uint8_t smNo);
void BLECNTR_SmSetTxMode(uint8_t smNo);
void BLECNTR_SmSetTxPacketPtr(uint8_t smNo, BLECNTR_TXRXPACK_TypeDef* packetP);
void BLECNTR_SmSetTxPhy(uint8_t smNo, uint8_t txPhy);
void BLECNTR_SmSetTxPwr(uint8_t smNo, uint8_t paLevel);
void BLECNTR_SmSetUnmappedChan(uint8_t smNo, uint8_t chan);
void BLECNTR_SmToggleNesn(uint8_t smNo);
void BLECNTR_SmToggleSn(uint8_t smNo);
void BLECNTR_StartEncrypt(void);
uint32_t BLECNTR_TimeDiff(uint32_t x, uint32_t y);
uint8_t BLECNTR_DemodDelaySt(uint8_t RxPHY);
uint32_t BLECNTR_GeTimer2TimeoutForIfs(uint32_t T_Ifs, BLECNTR_Transaction Transaction, BOOL Cal_Enabled);

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

#endif /* BLECNTR_H */
