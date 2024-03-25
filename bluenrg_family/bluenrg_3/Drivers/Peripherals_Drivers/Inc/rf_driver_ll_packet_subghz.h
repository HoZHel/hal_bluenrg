/**
  ******************************************************************************
  * @file    rf_driver_ll_packet_subghz.h
  * @author  RF Application Team
  * @brief   Header file of Packet Subghz LL module.
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
  
#ifndef RF_DRIVER_LL_PACKET_SUBGHZ_H
#define RF_DRIVER_LL_PACKET_SUBGHZ_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "system_util.h"
#include "rf_driver_ll_radio_subghz.h"

#define WMBUS_PREAMBLE_LEN_S1S2LONGHEADER           (uint16_t)279
#define WMBUS_PREAMBLE_LEN_S1MS2T2OTHERTOMETER      (uint16_t)15
#define WMBUS_PREAMBLE_LEN_T1T2METERTOOTHER         (uint16_t)19
#define WMBUS_PREAMBLE_LEN_R2                       (uint16_t)39
#define WMBUS_PREAMBLE_LEN_N1N2                     (uint16_t)8

#define WMBUS_SYNC_LEN_S1S2LONGHEADER               (uint8_t)17
#define WMBUS_SYNC_LEN_S1MS2T2OTHERTOMETER          (uint8_t)17
#define WMBUS_SYNC_LEN_T1T2METERTOOTHER             (uint8_t)9
#define WMBUS_SYNC_LEN_R2                           (uint8_t)17
#define WMBUS_SYNC_LEN_N1N2                         (uint16_t)15

#define WMBUS_SYNCWORD_S1S2LONGHEADER           (uint32_t)0x1DA58000
#define WMBUS_SYNCWORD_S1MS2T2OTHERTOMETER      (uint32_t)0x1DA58000
#define WMBUS_SYNCWORD_T1T2METERTOOTHER         (uint32_t)0x0F400000
#define WMBUS_SYNCWORD_R2                       (uint32_t)0x1DA58000
#define WMBUS_SYNCWORD_N1N2                     (uint32_t)0xf68d0000

                                   
/**
  * @brief  SPIRIT3 preamble pattern enumeration
  */
typedef enum
{
  PRE_SEQ_0101 = 0x00, /* Suitable for 2-(G)FSK and ASK/OOK */
  PRE_SEQ_1010 = 0x01, /* Suitable for 2-(G)FSK and ASK/OOK */
  PRE_SEQ_0011 = 0x02, /* Suitable for 2-(G)FSK and ASK/OOK (not recommended - not the most convenient for the receiver) */
  PRE_SEQ_1100 = 0x03, /* Suitable for 2-(G)FSK and ASK/OOK (not recommended - not the most convenient for the receiver) */
  PRE_SEQ_0111 = 0x00, /* Suitable for 4-(G)FSK */
  PRE_SEQ_0010 = 0x01, /* Suitable for 4-(G)FSK */
  PRE_SEQ_1101 = 0x02, /* Suitable for 4-(G)FSK */
  PRE_SEQ_1000 = 0x03  /* Suitable for 4-(G)FSK */
} MRSubG_PreambleSeq;


/**
  * @brief  SPIRIT3 preamble pattern enumeration
  */
typedef enum
{
  POST_SEQ_0101 = 0x00, /* 010101... */
  POST_SEQ_1010 = 0x01, /* 101010... */
  POST_SEQ_OTHER = 0x02  /* depends on last payload/CRC bit. If 0: 101010... - If 1: 010101... */
} MRSubG_PostambleSeq;

/**
* @brief  SPIRIT3 packet coding options
*/
typedef enum
{
  CODING_NONE = 0x00,		/* None */
  CODING_FEC = 0x01,		/* FEC in TX / Viterbi in RX */
  CODING_3o6 = 0x02,		/* 3of6 coding */
  CODING_MANCHESTER = 0x03	/* Manchester coding */
} MRSubG_PcktCoding;


/**
* @brief  Whitening before FEC feature
*/
typedef enum
{
  FEC_THEN_WHITENING    = 0, // SPIRIT3 Order
  WHITENING_THEN_FEC    = 1
} MRSubG_WhiteningBeforeFECType;


/**
 * @brief  CRC length in bytes enumeration.
 */
typedef enum {
  PKT_NO_CRC               = 0x00, /*!< No CRC                              */
  PKT_CRC_MODE_8BITS       = 0x01, /*!< CRC length 8 bits  - poly: 0x07     */
  PKT_CRC_MODE_16BITS_1    = 0x02, /*!< CRC length 16 bits - poly: 0x8005   */
  PKT_CRC_MODE_16BITS_2    = 0x03, /*!< CRC length 16 bits - poly: 0x1021   */
  FCS_16BIT 	           = 0x03, /*!< CRC length 16 bits - poly: 0x1021   */
  PKT_CRC_MODE_24BITS      = 0x04, /*!< CRC length 24 bits - poly: 0x864CFB */
  PKT_CRC_MODE_32BITS      = 0x05, /*!< CRC length 32 bits - poly: 0x04C11DB7 */
  FCS_32BIT 	           = 0x05, /*!< CRC length 32 bits - poly: 0x04C11DB7 */
} MRSubG_PcktCrcMode;

/**
 * @brief Fixed or Variable length mode.
 * 0: FIXED length mode (no LENGTH field added in the frame in TX and no decode in RX)
 * 1: VARIABLE length mode (LENGTH field put in the frame in TX and decoded in RX)
 */
typedef enum
{
  FIXED 	= 0,
  VARIABLE 	= !FIXED
} MRSubG_LengthMode;

/**
 * @brief Manchester encoding polarity.
 * 	0: "0" is encoded with "01" and "1" is encoded with "10"
 *	1: "0" is encoded with "10" and "1" is encoded with "01"
 */
typedef enum
{
  MANCHESTER_TYPE0 = 0,
  MANCHESTER_TYPE1 = 1
} MRSubG_ManchesterPolarity;


/**
 * @brief 1 or 2 bytes length width.
 * 0: LENGTH bit field is defined on 1 byte
 * 1: LENGTH bit field is defined on 2 bytes
 */
typedef enum
{
  BYTE_LEN_1 	= 0,
  BYTES_LEN_2 	= 1
} MRSubG_LenWidthhMode;

/**
 * @brief  SPIRIT3 Basic Packet Init structure definition.
 */
typedef enum {
  PKT_BASIC 	= 0,
  PKT_802_15_4 	= 1
} MRSubG_PcktType;

/**
 * @brief  WMbus submode enumeration.
 */
typedef enum {
  WMBUS_SUBMODE_NOT_CONFIGURED            = 0,   /*!< WMBUS submode S1, S2 (long header) - Header length = WMBUS_prmbl_ctrl + 279 (in "01" bit pairs) , Sync word = 0x7696 (length 18 bits) */
  WMBUS_SUBMODE_S1_S2_LONG_HEADER,               /*!< WMBUS submode S1, S2 (long header) - Header length = WMBUS_prmbl_ctrl + 279 (in "01" bit pairs) , Sync word = 0x7696 (length 18 bits) */
  WMBUS_SUBMODE_S1_M_S2_T2_OTHER_TO_METER,       /*!< WMBUS submode S1-m, S2, T2 (other to meter) - Header length = WMBUS_prmbl_ctrl + 15 (in "01" bit pairs) , Sync word = 0x7696 (length 18 bits)*/
  WMBUS_SUBMODE_T1_T2_METER_TO_OTHER,            /*!< WMBUS submode T1, T2 (meter to other) - Header length = WMBUS_prmbl_ctrl + 19 (in "01" bit pairs) ,  Sync word = 0x3D (length 10 bits)*/
  WMBUS_SUBMODE_R2_SHORT_HEADER,                 /*!< WMBUS submode R2, short header - Header length = WMBUS_prmbl_ctrl + 39 (in "01" bit pairs) , Sync word = 0x7696 (length 18 bits)*/
} WMbusSubmode;

/**
 * @brief  SPIRIT3 Basic Packet Init structure definition.
 */
typedef struct {
  uint16_t		PreambleLength;         /*!< Set the preamble length of packet. From 1 to 2046 bit. */
  uint16_t		PostambleLength;	/*!< Set the postable length of packet. From 1 to 126 bit. */
  uint8_t		SyncLength;             /*!< Set the sync word length of packet in bits. From 1 to 64 bits. */
  uint32_t		SyncWord;              	/*!< Set the sync words in MSB. */
  MRSubG_LengthMode	FixVarLength;           /*!< Enable the variable length mode. */
  FunctionalState	ExtendedPktLenField;    /*!< Extend the length field from 1 byte to 2 bytes. Variable length mode only. */
  MRSubG_PreambleSeq	PreambleSequence;	/*!< Select the PREAMBLE pattern to be applied. */
  MRSubG_PostambleSeq	PostambleSequence;	/*!< Packet postamble control: postamble bit sequence selection. */
  MRSubG_PcktCrcMode	CrcMode;                /*!< Set the CRC type. @ref StackCrcMode */
  FunctionalState	AddressField;           /*!< Enable the destination address field. */
  MRSubG_PcktCoding     Coding;			/*!< Enable the FEC/Viterbi. */
  FunctionalState	DataWhitening;          /*!< Enable the data whitening. */
  MRSubG_LenWidthhMode	LengthWidth;		/*!< Set the length width. this bit field is considered/relevant only if FIX_VAR_LEN=1 */
  FunctionalState       SyncPresent;		/*!< ndicate if a SYNC word is present on the frame or not (null length) */
} MRSubG_PcktBasicFields;

/**
 * @brief  SPIRIT3 WMBUS Packet Init structure definition.
 */
typedef struct {
  WMbusSubmode  xWMbusSubmode;          /*!< Set the WMBUS submode. @ref WMbusSubmode */
  uint16_t      PreambleLength;         /*!< Set the preamble length of packet. From 1 to 2046 bit. */
  uint16_t      PostambleLength;	/*!< Set the postable length of packet. From 1 to 126 bit. */
} MRSubG_WMBUS_PcktFields;


/**
 * @brief  SPIRIT3 802.15.4 FEC encoding types.
 */
typedef enum {
  FEC_15_4_G_NONE 	        = 0,
  FEC_15_4_G_NRNSC 	        = 1,
  FEC_15_4_G_RSC 	        = 2,
  FEC_15_4_G_RSC_Interleaving 	= 3
} MRSubG_FEC15_4gType;


/**
 * @brief  SPIRIT3 802.15.4 Packet Init structure definition.
 */
typedef struct {
  MRSubGModSelect Modulation;        /*!< Enumerated value as follow:
        - 0: MOD_2FSK
        - 1: MOD_4FSK */
  uint8_t             PreambleLength;         /*!< number of times the standard preamble sequence shall be repeated.
        The standard preamble sequence is "01010101" for 2-FSK and "0111011101110111" for 4-FSK. From 4 to 64. */
  MRSubG_PcktCrcMode FCSType;                /*!< Enumerated value as follow:
        - 0: FCS_32BIT CRC mode5
        - 1: FCS_16BIT CRC mode3 */
  FunctionalState	  Whitening;          /*!< Enable the data whitening. */
  MRSubG_FEC15_4gType	  FecType;		/*!< FEC type for 802.15.4g */
  uint16_t            FrameLength;		/*!< size of payload + FCS, in bytes */
} MRSubG_802_15_4_PcktFields;


void MRSubG_SetPreambleLength(uint16_t cPreambleLength);
uint16_t MRSubG_GetPreambleLength(void);
void MRSubG_SetPreambleSeq(MRSubG_PreambleSeq cPreambleSeq);
MRSubG_PreambleSeq MRSubG_GetPreambleSeq(void);
void MRSubG_SetPostambleLength(uint16_t cPostambleLength);
uint16_t MRSubG_GetPostambleLength(void);
void MRSubG_SetPostamblSeq(MRSubG_PostambleSeq cPostambleSeq);
MRSubG_PostambleSeq MRSubG_GetPostambleSeq(void);
void MRSubG_SetSyncPresent(FunctionalState cSyncPresent);
FunctionalState MRSubG_GetSyncPresent(void);
void MRSubG_SetSecondarySync(FunctionalState cSecondarySync);
FunctionalState MRSubG_GetSecondarySync(void);
void MRSubG_SetSyncLength(uint8_t cSyncLength);
uint8_t MRSubG_GetSyncLength(void);
void MRSubG_SetSyncWord(uint32_t lSyncWord);
uint32_t MRSubG_GetSyncWord(void);
void MRSubG_SetSecondarySyncWord(uint32_t lSecSyncWord);
uint32_t MRSubG_GetSecondarySyncWord(void);
void MRSUBG_SetPacketLength(uint16_t nPacketLen);
uint16_t MRSUBG_GetPacketLength(void);
void MRSubG_PktBasicSetPayloadLength(uint16_t nPayloadLength);
uint16_t MRSubG_PktBasicGetPayloadLength(void);
void MRSubG_SetLenWidth(MRSubG_LenWidthhMode lenWidth);
MRSubG_LenWidthhMode MRSubG_GetLenWidth(void);
void MRSUBG_SetFixedVariableLength(MRSubG_LengthMode lenType);
MRSubG_LengthMode MRSUBG_GetFixedVariableLength(void);
void MRSubG_PacketHandlerSetCrcMode(MRSubG_PcktCrcMode xPktCrcMode);
void MRSubG_PacketHandlerManchesterType(MRSubG_ManchesterPolarity manType);
void MRSubG_PacketHandlerWhitening(FunctionalState xNewState);
void MRSubG_PacketHandlerByteSwap(FunctionalState xNewState);
void MRSubG_PacketHandlerWhiteningFecOrder(MRSubG_WhiteningBeforeFECType xWhitFecOrder);
void MRSubG_PacketHandlerCoding(MRSubG_PcktCoding cCoding);
void MRSubG_PacketHandlerSetWhiteningInit(uint16_t whit_init);
MRSubG_PcktCrcMode MRSubG_PacketHandlerGetCrcMode(void);
void MRSubG_PacketBasicInit(MRSubG_PcktBasicFields* pxPktBasicInit);
void MRSubG_WMBus_PacketInit(MRSubG_WMBUS_PcktFields* pxPktWMbusInit);
void MRSubG_802_15_4_PacketInit(MRSubG_802_15_4_PcktFields* px802_15_4PktInit);

#ifdef __cplusplus
}
#endif

#endif
