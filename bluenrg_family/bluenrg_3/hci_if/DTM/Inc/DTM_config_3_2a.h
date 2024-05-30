/**
  ******************************************************************************
  * @file    DTM_config.h 
  * @author  VMA RF Application Team
  * @version V1.1.0
  * @date    April-2019
  * @brief   
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
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

#ifndef _DTM_CONFIG_H_
#define _DTM_CONFIG_H_
#include "bluenrg_lpx.h"
#include <bluenrg_lp_stack.h>
#include "stack_user_cfg.h"

/** This file contains all the information needed to init the BlueNRG-LP stack. 
 * NOTE: the current selected configuration is tuned depending on available RAM
 * and Flash size.
 * User can modify, tune the configuration options according to his specific application requirements. 
 */

/* MAX number of links for DTM */
#define DTM_NUM_LINK_CONF                           (CFG_BLE_NUM_RADIO_TASKS)
#define NUM_LINKS                                   (DTM_NUM_LINK_CONF)


/* Number of GATT attributes needed for the DTM */
#define NUM_GATT_ATTRIBUTES                         (DTM_NUM_GATT_ATTRIBUTES_CONF)

#define ISR0_FIFO_SIZE   (256)
#define ISR1_FIFO_SIZE   (768)
#define USER_FIFO_SIZE   (1024)

/* BlueNRG-LP  */
#if defined(CONFIG_DEVICE_BLUENRG_LP) 
/* MAX number of GAP and GATT attributes for DTM */
#define DTM_NUM_GATT_ATTRIBUTES_CONF                (100)
#define NUM_EATT_CHANNELS                           (5U)
#define NUM_AUX_SCAN_SLOTS_CONF                     (4U) 
//#define NUM_SYNC_SLOTS_CONF                         (2U)
#define WHITE_LIST_SIZE_LOG2_CONF                   (4U)
#define L2CAP_MPS_CONF                              (247U)
#define NUM_L2CAP_COCS_CONF                         (2U + NUM_EATT_CHANNELS)
#define MAX_ATT_MTU_CONF                            (247)
#define OPT_MBLOCKS_CONF                            (30)
#if (BLESTACK_CONTROLLER_ONLY == 0)
#define ACI_ATT_QUEUED_WRITE_SIZE_CONF              (512)
/* Size of buffer shared between GATT_NWK library (used for GATT database and client 
  write procedures) and ADV_NWK library (used for advertising buffers). */
#define ACI_GATT_ADV_NWK_BUFFER_SIZE_CONF           (3072 + 1660 + ACI_ATT_QUEUED_WRITE_SIZE_CONF)
#else
#define ACI_ATT_QUEUED_WRITE_SIZE_CONF              (0)
#define ACI_GATT_ADV_NWK_BUFFER_SIZE_CONF           (1660)
#endif
#define NUM_OF_CONCURRENT_GATT_CLIENT_PROC_CONF     (DTM_NUM_LINK_CONF)
#define MAX_NUM_CTE_ANTENNA_IDS                     (0U)
#define MAX_NUM_CTE_IQ_SAMPLES                      (0U)

#elif defined(CONFIG_DEVICE_BLUENRG_LPS) /* BlueNRG-LPS  */

#define DTM_NUM_GATT_ATTRIBUTES_CONF                (60)
#define NUM_EATT_CHANNELS                           (0U)
#if (BLESTACK_CONTROLLER_ONLY == 0)
#define NUM_AUX_SCAN_SLOTS_CONF                     (1U)
#else
#define NUM_AUX_SCAN_SLOTS_CONF                     (3U)
#endif
//#define NUM_SYNC_SLOTS_CONF                         (2U)
#define WHITE_LIST_SIZE_LOG2_CONF                   (3U) 
#define L2CAP_MPS_CONF                              (160U) 
#define NUM_L2CAP_COCS_CONF                         (0U)
#define MAX_ATT_MTU_CONF                            (160U)
#if (BLESTACK_CONTROLLER_ONLY == 0)
#define OPT_MBLOCKS_CONF                            (10)
#define ACI_ATT_QUEUED_WRITE_SIZE_CONF              (300)
#define ACI_GATT_ADV_NWK_BUFFER_SIZE_CONF           (1000 + 256 + ACI_ATT_QUEUED_WRITE_SIZE_CONF)
#else
#define OPT_MBLOCKS_CONF                            (30)
#define ACI_ATT_QUEUED_WRITE_SIZE_CONF              (0)
#define ACI_GATT_ADV_NWK_BUFFER_SIZE_CONF           (512)
#endif
#define NUM_OF_CONCURRENT_GATT_CLIENT_PROC_CONF     (DTM_NUM_LINK_CONF) 
#define MAX_NUM_CTE_ANTENNA_IDS                     (8U)
#define MAX_NUM_CTE_IQ_SAMPLES                      (82U)

#elif CONFIG_DEVICE_BLUENRG_LPF /* BlueNRG-LPF  */

#define DTM_NUM_GATT_ATTRIBUTES_CONF                (100)
#define NUM_EATT_CHANNELS                           (5U)
#define NUM_AUX_SCAN_SLOTS_CONF                     (4U)
//#define NUM_SYNC_SLOTS_CONF                         (2U)
#define WHITE_LIST_SIZE_LOG2_CONF                   (4U)
#define L2CAP_MPS_CONF                              (247U)
#define NUM_L2CAP_COCS_CONF                         (2U + NUM_EATT_CHANNELS)
#define MAX_ATT_MTU_CONF                            (247)
#define OPT_MBLOCKS_CONF                            (30)
#if (BLESTACK_CONTROLLER_ONLY == 0)
#define ACI_ATT_QUEUED_WRITE_SIZE_CONF              (512)
#define ACI_GATT_ADV_NWK_BUFFER_SIZE_CONF           (3072 + 1660 + ACI_ATT_QUEUED_WRITE_SIZE_CONF)
#else
#define ACI_ATT_QUEUED_WRITE_SIZE_CONF              (0)
#define ACI_GATT_ADV_NWK_BUFFER_SIZE_CONF           (1660)
#endif
#define NUM_OF_CONCURRENT_GATT_CLIENT_PROC_CONF     (DTM_NUM_LINK_CONF)
#define MAX_NUM_CTE_ANTENNA_IDS                     (8U)
#define MAX_NUM_CTE_IQ_SAMPLES                      (82U)

#endif

/* Set the number of memory block for packet allocation */
#define MBLOCKS_COUNT           (BLE_STACK_MBLOCKS_CALC(MAX_ATT_MTU_CONF, NUM_LINKS, NUM_EATT_CHANNELS) + OPT_MBLOCKS_CONF)

/* RAM reserved to manage all the data stack according the number of links,
 * number of services, number of attributes and attribute value length
 */
#define DYNAMIC_MEMORY_SIZE (BLE_STACK_TOTAL_BUFFER_SIZE(NUM_LINKS,NUM_EATT_CHANNELS,NUM_GATT_ATTRIBUTES,NUM_OF_CONCURRENT_GATT_CLIENT_PROC_CONF,MBLOCKS_COUNT,\
                                                         NUM_ADV_SETS_CONF,NUM_AUX_SCAN_SLOTS_CONF,WHITE_LIST_SIZE_LOG2_CONF,NUM_L2CAP_COCS_CONF,\
                                                         CFG_BLE_NUM_SYNC_SLOTS, MAX_NUM_CTE_ANTENNA_IDS, MAX_NUM_CTE_IQ_SAMPLES,\
                                                         ISR0_FIFO_SIZE, ISR1_FIFO_SIZE, USER_FIFO_SIZE))

/* Maximum duration of the connection event */
#define MAX_CONN_EVENT_LENGTH_CONF 0xFFFFFFFF

/* Sleep clock accuracy. */
#define SLEEP_CLOCK_ACCURACY        500

#define CALIBRATION_INTERVAL_CONF   10000


/* Radio Config Hot Table */
extern uint8_t hot_table_radio_config[];

/* This structure contains memory and low level hardware configuration data for the device */
#define BLE_STACK_INIT_PARAMETERS {                                             \
    .BLEStartRamAddress = (uint8_t*)dyn_alloc_a,                                \
    .TotalBufferSize = DYNAMIC_MEMORY_SIZE,                                     \
    .NumAttrRecords = NUM_GATT_ATTRIBUTES,                                      \
    .MaxNumOfClientProcs = NUM_OF_CONCURRENT_GATT_CLIENT_PROC_CONF,             \
    .NumOfLinks = NUM_LINKS,                                                    \
    .NumOfEATTChannels = NUM_EATT_CHANNELS,                                     \
    .NumBlockCount = MBLOCKS_COUNT,                                             \
    .ATT_MTU = MAX_ATT_MTU_CONF,                                                \
    .MaxConnEventLength = MAX_CONN_EVENT_LENGTH_CONF,                           \
    .SleepClockAccuracy = SLEEP_CLOCK_ACCURACY,                                 \
    .NumOfAdvDataSet = NUM_ADV_SETS_CONF,                                       \
    .NumOfAuxScanSlots = NUM_AUX_SCAN_SLOTS_CONF,                               \
    .WhiteListSizeLog2 = WHITE_LIST_SIZE_LOG2_CONF,                             \
    .L2CAP_MPS = L2CAP_MPS_CONF,                                                \
    .L2CAP_NumChannels = NUM_L2CAP_COCS_CONF,                                   \
    .NumOfSyncSlots = CFG_BLE_NUM_SYNC_SLOTS,                                      \
    .CTE_MaxNumAntennaIDs = MAX_NUM_CTE_ANTENNA_IDS,                            \
    .CTE_MaxNumIQSamples = MAX_NUM_CTE_IQ_SAMPLES,                              \
    .isr0_fifo_size = ISR0_FIFO_SIZE,                                           \
    .isr1_fifo_size = ISR1_FIFO_SIZE,                                           \
    .user_fifo_size = USER_FIFO_SIZE                                            \
}

#endif // _DTM_CONFIG_H_
