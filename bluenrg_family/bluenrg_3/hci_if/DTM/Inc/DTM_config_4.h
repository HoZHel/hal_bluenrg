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



/* Number of GATT attributes needed for the DTM */
#define NUM_GATT_ATTRIBUTES                         (60)

#define ISR0_FIFO_SIZE   (256)
#define ISR1_FIFO_SIZE   (768)
#define USER_FIFO_SIZE   (1024)

/* BlueNRG-LP  */
#if defined(CONFIG_DEVICE_BLUENRG_LP)

/******************************************************************************
 * BLE Stack initialization parameters
 ******************************************************************************/

/**
 * Maximum number of simultaneous radio tasks. Radio controller supports up to
 * 128 simultaneous radio tasks, but actual usable max value depends on the
 * available RAM.
 */
/* #define CFG_BLE_NUM_RADIO_TASKS                         (CFG_NUM_RADIO_TASKS) */

/**
 * Maximum number of Attributes that can be stored in the GATT database.
 */
#define CFG_BLE_NUM_GATT_ATTRIBUTES                     (60)

/**
 * Maximum number of concurrent Client's Procedures. This value must be less
 * than or equal to the maximum number of supported links (CFG_BLE_NUM_RADIO_TASKS).
 */
#define CFG_BLE_NUM_OF_CONCURRENT_GATT_CLIENT_PROC      (8)

/**
 * Maximum supported ATT MTU size [23-1020].
 */
#define CFG_BLE_ATT_MTU_MAX                             (247)

/**
 * Maximum duration of the connection event in system time units (625/256 us =~
 * 2.44 us) when the device is in Peripheral role [0-0xFFFFFFFF].
 */
#define CFG_BLE_CONN_EVENT_LENGTH_MAX                   (0xFFFFFFFF)

/**
 * Sleep clock accuracy (ppm).
 */
#if CFG_LSCLK_LSE
/* Change this value according to accuracy of low speed crystal (ppm). */
#define CFG_BLE_SLEEP_CLOCK_ACCURACY                    (100)
#else
/* This value should be kept to 500 ppm when using LSI. */
#define CFG_BLE_SLEEP_CLOCK_ACCURACY                    (500)
#endif

/**
 * Number of extra memory blocks, in addition to the minimum required for the
 * supported links.
 */
#define CFG_BLE_MBLOCK_COUNT_MARGIN                     (20)

/**
 * Maximum number of simultaneous EATT active channels. It must be less than or
 * equal to CFG_BLE_COC_NBR_MAX.
 */
#define CFG_BLE_NUM_EATT_CHANNELS                       (6)

/**
 * Maximum number of channels in LE Credit Based Flow Control mode [0-255].
 * This number must be greater than or equal to CFG_BLE_NUM_EATT_CHANNELS.
 */
#define CFG_BLE_COC_NBR_MAX                             (8)

/**
 * The maximum size of payload data in octets that the L2CAP layer entity is
 * capable of accepting [0-1024].
 */
#define CFG_BLE_COC_MPS_MAX                             (247)

/**
 * Maximum number of Advertising Data Sets, if Advertising Extension Feature is
 * enabled.
 */
/* #define CFG_BLE_NUM_ADV_SETS                            (2) */

/**
 * Maximum number of Periodic Advertising with Responses subevents.
 */
#define CFG_BLE_NUM_PAWR_SUBEVENTS                      (16)

/**
 * Maximum number of subevent data that can be queued in the controller.
 */
#define CFG_BLE_PAWR_SUBEVENT_DATA_COUNT_MAX            (8U)

/**
 * Maximum number of slots for scanning on the secondary advertising channel,
 * if Advertising Extension Feature is enabled.
 */
#define CFG_BLE_NUM_AUX_SCAN_SLOTS                      (4)

/**
 * Maximum number of slots for synchronizing to a periodic advertising train,
 * if Periodic Advertising and Synchronizing Feature is enabled.
 */
#define CFG_BLE_NUM_SYNC_SLOTS                          (2)

/**
 * Two's logarithm of Filter Accept, Resolving and Advertiser list size.
 */
#define CFG_BLE_FILTER_ACCEPT_LIST_SIZE_LOG2            (4)

/**
 * Maximum number of Antenna IDs in the antenna pattern used in CTE connection
 * oriented mode.
 */
#define CFG_BLE_NUM_CTE_ANTENNA_IDS_MAX                 (0)

/**
 * Maximum number of IQ samples in the buffer used in CTE connection oriented mode.
 */
#define CFG_BLE_NUM_CTE_IQ_SAMPLES_MAX                  (0)

/**
 * Maximum number of slots for synchronizing to a Broadcast Isochronous Group.
 */
#define CFG_BLE_NUM_SYNC_BIG_MAX                        (1U)

/**
 * Maximum number of slots for synchronizing to a Broadcast Isochronous Stream.
 */
#define CFG_BLE_NUM_SYNC_BIS_MAX                        (2U)

/**
 * Maximum number of slots for broadcasting a Broadcast Isochronous Group.
 */
#define CFG_BLE_NUM_BRC_BIG_MAX                         (1U)

/**
 * Maximum number of slots for broadcasting a Broadcast Isochronous Stream.
 */
#define CFG_BLE_NUM_BRC_BIS_MAX                         (2U)

/**
 * Maximum number of Connected Isochronous Groups.
 */
#define CFG_BLE_NUM_CIG_MAX                             (1U)

/**
 * Maximum number of Connected Isochronous Streams.
 */
#define CFG_BLE_NUM_CIS_MAX                             (1U)

/**
 * Size of the internal FIFO used for critical controller events produced by the
 * ISR (e.g. rx data packets).
 */
#define CFG_BLE_ISR0_FIFO_SIZE                          (256)

/**
 * Size of the internal FIFO used for non-critical controller events produced by
 * the ISR (e.g. advertising or IQ sampling reports).
 */
#define CFG_BLE_ISR1_FIFO_SIZE                          (768)

/**
 * Size of the internal FIFO used for controller and host events produced
 * outside the ISR.
 */
#define CFG_BLE_USER_FIFO_SIZE                          (1024)

/**
 * Number of allocated memory blocks used for packet allocation.
 * The use of BLE_STACK_MBLOCKS_CALC macro is suggested to calculate the minimum
 * number of memory blocks for a given number of supported links and ATT MTU.
 */
#define CFG_BLE_MBLOCKS_COUNT           (BLE_STACK_MBLOCKS_CALC(CFG_BLE_ATT_MTU_MAX, CFG_BLE_NUM_RADIO_TASKS, CFG_BLE_NUM_EATT_CHANNELS) + CFG_BLE_MBLOCK_COUNT_MARGIN)

/**
 * Macro to calculate the RAM needed by the stack according the number of links,
 * memory blocks, advertising data sets and all the other initialization
 * parameters.
 */
#define BLE_DYN_ALLOC_SIZE (BLE_STACK_TOTAL_BUFFER_SIZE(CFG_BLE_NUM_RADIO_TASKS,\
                                                        CFG_BLE_NUM_EATT_CHANNELS,\
                                                        CFG_BLE_NUM_GATT_ATTRIBUTES,\
                                                        CFG_BLE_NUM_OF_CONCURRENT_GATT_CLIENT_PROC,\
                                                        CFG_BLE_MBLOCKS_COUNT,\
                                                        CFG_BLE_NUM_ADV_SETS,\
                                                        CFG_BLE_NUM_PAWR_SUBEVENTS,\
                                                        CFG_BLE_PAWR_SUBEVENT_DATA_COUNT_MAX,\
                                                        CFG_BLE_NUM_AUX_SCAN_SLOTS,\
                                                        CFG_BLE_FILTER_ACCEPT_LIST_SIZE_LOG2,\
                                                        CFG_BLE_COC_NBR_MAX,\
                                                        CFG_BLE_NUM_SYNC_SLOTS,\
                                                        CFG_BLE_NUM_CTE_ANTENNA_IDS_MAX,\
                                                        CFG_BLE_NUM_CTE_IQ_SAMPLES_MAX,\
                                                        CFG_BLE_NUM_SYNC_BIG_MAX,\
                                                        CFG_BLE_NUM_BRC_BIG_MAX,\
                                                        CFG_BLE_NUM_SYNC_BIS_MAX,\
                                                        CFG_BLE_NUM_BRC_BIS_MAX,\
                                                        CFG_BLE_NUM_CIG_MAX,\
                                                        CFG_BLE_NUM_CIS_MAX,\
                                                        CFG_BLE_ISR0_FIFO_SIZE,\
                                                        CFG_BLE_ISR1_FIFO_SIZE,\
                                                        CFG_BLE_USER_FIFO_SIZE))

#if (BLESTACK_CONTROLLER_ONLY == 0)
#define ACI_ATT_QUEUED_WRITE_SIZE_CONF              (512)
/* Size of buffer shared between GATT_NWK library (used for GATT database and client 
  write procedures) and ADV_NWK library (used for advertising buffers). */
#define ACI_GATT_ADV_NWK_BUFFER_SIZE_CONF           (3072 + 1660 + ACI_ATT_QUEUED_WRITE_SIZE_CONF)
#else
#define ACI_ATT_QUEUED_WRITE_SIZE_CONF              (0)
#define ACI_GATT_ADV_NWK_BUFFER_SIZE_CONF           (1660)
#endif
#define MAX_NUM_CTE_ANTENNA_IDS                     (0U)
#define MAX_NUM_CTE_IQ_SAMPLES                      (0U)

#elif defined(CONFIG_DEVICE_BLUENRG_LPS) /* BlueNRG-LPS  */

/******************************************************************************
 * BLE Stack initialization parameters
 ******************************************************************************/

/**
 * Maximum number of simultaneous radio tasks. Radio controller supports up to
 * 128 simultaneous radio tasks, but actual usable max value depends on the
 * available RAM.
 */
#define CFG_BLE_NUM_RADIO_TASKS                         (CFG_NUM_RADIO_TASKS)

/**
 * Maximum number of Attributes that can be stored in the GATT database.
 */
#define CFG_BLE_NUM_GATT_ATTRIBUTES                     (60)

/**
 * Maximum number of concurrent Client's Procedures. This value must be less
 * than or equal to the maximum number of supported links (CFG_BLE_NUM_RADIO_TASKS).
 */
#define CFG_BLE_NUM_OF_CONCURRENT_GATT_CLIENT_PROC      (3)

/**
 * Maximum supported ATT MTU size [23-1020].
 */
#define CFG_BLE_ATT_MTU_MAX                             (160)

/**
 * Maximum duration of the connection event in system time units (625/256 us =~
 * 2.44 us) when the device is in Peripheral role [0-0xFFFFFFFF].
 */
#define CFG_BLE_CONN_EVENT_LENGTH_MAX                   (0xFFFFFFFF)

/**
 * Sleep clock accuracy (ppm).
 */
#if CFG_LSCLK_LSE
/* Change this value according to accuracy of low speed crystal (ppm). */
#define CFG_BLE_SLEEP_CLOCK_ACCURACY                    (100)
#else
/* This value should be kept to 500 ppm when using LSI. */
#define CFG_BLE_SLEEP_CLOCK_ACCURACY                    (500)
#endif

/**
 * Number of extra memory blocks, in addition to the minimum required for the
 * supported links.
 */
#define CFG_BLE_MBLOCK_COUNT_MARGIN                     (10)

/**
 * Maximum number of simultaneous EATT active channels. It must be less than or
 * equal to CFG_BLE_COC_NBR_MAX.
 */
#define CFG_BLE_NUM_EATT_CHANNELS                       (0)

/**
 * Maximum number of channels in LE Credit Based Flow Control mode [0-255].
 * This number must be greater than or equal to CFG_BLE_NUM_EATT_CHANNELS.
 */
#define CFG_BLE_COC_NBR_MAX                             (1)

/**
 * The maximum size of payload data in octets that the L2CAP layer entity is
 * capable of accepting [0-1024].
 */
#define CFG_BLE_COC_MPS_MAX                             (160)

/**
 * Maximum number of Advertising Data Sets, if Advertising Extension Feature is
 * enabled.
 */
#define CFG_BLE_NUM_ADV_SETS                            (2)

/**
 * Maximum number of Periodic Advertising with Responses subevents.
 */
#define CFG_BLE_NUM_PAWR_SUBEVENTS                      (8)

/**
 * Maximum number of subevent data that can be queued in the controller.
 */
#define CFG_BLE_PAWR_SUBEVENT_DATA_COUNT_MAX            (2U)

/**
 * Maximum number of slots for scanning on the secondary advertising channel,
 * if Advertising Extension Feature is enabled.
 */
#define CFG_BLE_NUM_AUX_SCAN_SLOTS                      (1)

/**
 * Maximum number of slots for synchronizing to a periodic advertising train,
 * if Periodic Advertising and Synchronizing Feature is enabled.
 */
#define CFG_BLE_NUM_SYNC_SLOTS                          (2)

/**
 * Two's logarithm of Filter Accept, Resolving and Advertiser list size.
 */
#define CFG_BLE_FILTER_ACCEPT_LIST_SIZE_LOG2            (3)

/**
 * Maximum number of Antenna IDs in the antenna pattern used in CTE connection
 * oriented mode.
 */
#define CFG_BLE_NUM_CTE_ANTENNA_IDS_MAX                 (8)

/**
 * Maximum number of IQ samples in the buffer used in CTE connection oriented mode.
 */
#define CFG_BLE_NUM_CTE_IQ_SAMPLES_MAX                  (82)

/**
 * Maximum number of slots for synchronizing to a Broadcast Isochronous Group.
 */
#define CFG_BLE_NUM_SYNC_BIG_MAX                        (1U)

/**
 * Maximum number of slots for synchronizing to a Broadcast Isochronous Stream.
 */
#define CFG_BLE_NUM_SYNC_BIS_MAX                        (2U)

/**
 * Maximum number of slots for broadcasting a Broadcast Isochronous Group.
 */
#define CFG_BLE_NUM_BRC_BIG_MAX                         (1U)

/**
 * Maximum number of slots for broadcasting a Broadcast Isochronous Stream.
 */
#define CFG_BLE_NUM_BRC_BIS_MAX                         (2U)

/**
 * Maximum number of Connected Isochronous Groups.
 */
#define CFG_BLE_NUM_CIG_MAX                             (1U)

/**
 * Maximum number of Connected Isochronous Streams.
 */
#define CFG_BLE_NUM_CIS_MAX                             (1U)

/**
 * Size of the internal FIFO used for critical controller events produced by the
 * ISR (e.g. rx data packets).
 */
#define CFG_BLE_ISR0_FIFO_SIZE                          (256)

/**
 * Size of the internal FIFO used for non-critical controller events produced by
 * the ISR (e.g. advertising or IQ sampling reports).
 */
#define CFG_BLE_ISR1_FIFO_SIZE                          (768)

/**
 * Size of the internal FIFO used for controller and host events produced
 * outside the ISR.
 */
#define CFG_BLE_USER_FIFO_SIZE                          (1024)

/**
 * Number of allocated memory blocks used for packet allocation.
 * The use of BLE_STACK_MBLOCKS_CALC macro is suggested to calculate the minimum
 * number of memory blocks for a given number of supported links and ATT MTU.
 */
#define CFG_BLE_MBLOCKS_COUNT           (BLE_STACK_MBLOCKS_CALC(CFG_BLE_ATT_MTU_MAX, CFG_BLE_NUM_RADIO_TASKS, CFG_BLE_NUM_EATT_CHANNELS) + CFG_BLE_MBLOCK_COUNT_MARGIN)

/**
 * Macro to calculate the RAM needed by the stack according the number of links,
 * memory blocks, advertising data sets and all the other initialization
 * parameters.
 */
#define BLE_DYN_ALLOC_SIZE (BLE_STACK_TOTAL_BUFFER_SIZE(CFG_BLE_NUM_RADIO_TASKS,\
                                                        CFG_BLE_NUM_EATT_CHANNELS,\
                                                        CFG_BLE_NUM_GATT_ATTRIBUTES,\
                                                        CFG_BLE_NUM_OF_CONCURRENT_GATT_CLIENT_PROC,\
                                                        CFG_BLE_MBLOCKS_COUNT,\
                                                        CFG_BLE_NUM_ADV_SETS,\
                                                        CFG_BLE_NUM_PAWR_SUBEVENTS,\
                                                        CFG_BLE_PAWR_SUBEVENT_DATA_COUNT_MAX,\
                                                        CFG_BLE_NUM_AUX_SCAN_SLOTS,\
                                                        CFG_BLE_FILTER_ACCEPT_LIST_SIZE_LOG2,\
                                                        CFG_BLE_COC_NBR_MAX,\
                                                        CFG_BLE_NUM_SYNC_SLOTS,\
                                                        CFG_BLE_NUM_CTE_ANTENNA_IDS_MAX,\
                                                        CFG_BLE_NUM_CTE_IQ_SAMPLES_MAX,\
                                                        CFG_BLE_NUM_SYNC_BIG_MAX,\
                                                        CFG_BLE_NUM_BRC_BIG_MAX,\
                                                        CFG_BLE_NUM_SYNC_BIS_MAX,\
                                                        CFG_BLE_NUM_BRC_BIS_MAX,\
                                                        CFG_BLE_NUM_CIG_MAX,\
                                                        CFG_BLE_NUM_CIS_MAX,\
                                                        CFG_BLE_ISR0_FIFO_SIZE,\
                                                        CFG_BLE_ISR1_FIFO_SIZE,\
                                                        CFG_BLE_USER_FIFO_SIZE))

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
/******************************************************************************
 * BLE Stack initialization parameters
 ******************************************************************************/

/**
 * Maximum number of simultaneous radio tasks. Radio controller supports up to
 * 128 simultaneous radio tasks, but actual usable max value depends on the
 * available RAM.
 */
#define CFG_BLE_NUM_RADIO_TASKS                         (CFG_NUM_RADIO_TASKS)

/**
 * Maximum number of Attributes that can be stored in the GATT database.
 */
#define CFG_BLE_NUM_GATT_ATTRIBUTES                     (60)

/**
 * Maximum number of concurrent Client's Procedures. This value must be less
 * than or equal to the maximum number of supported links (CFG_BLE_NUM_RADIO_TASKS).
 */
#define CFG_BLE_NUM_OF_CONCURRENT_GATT_CLIENT_PROC      (8)

/**
 * Maximum supported ATT MTU size [23-1020].
 */
#define CFG_BLE_ATT_MTU_MAX                             (247)

/**
 * Maximum duration of the connection event in system time units (625/256 us =~
 * 2.44 us) when the device is in Peripheral role [0-0xFFFFFFFF].
 */
#define CFG_BLE_CONN_EVENT_LENGTH_MAX                   (0xFFFFFFFF)

/**
 * Sleep clock accuracy (ppm).
 */
#if CFG_LSCLK_LSE
/* Change this value according to accuracy of low speed crystal (ppm). */
#define CFG_BLE_SLEEP_CLOCK_ACCURACY                    (100)
#else
/* This value should be kept to 500 ppm when using LSI. */
#define CFG_BLE_SLEEP_CLOCK_ACCURACY                    (500)
#endif

/**
 * Number of extra memory blocks, in addition to the minimum required for the
 * supported links.
 */
#define CFG_BLE_MBLOCK_COUNT_MARGIN                     (20)

/**
 * Maximum number of simultaneous EATT active channels. It must be less than or
 * equal to CFG_BLE_COC_NBR_MAX.
 */
#define CFG_BLE_NUM_EATT_CHANNELS                       (6)

/**
 * Maximum number of channels in LE Credit Based Flow Control mode [0-255].
 * This number must be greater than or equal to CFG_BLE_NUM_EATT_CHANNELS.
 */
#define CFG_BLE_COC_NBR_MAX                             (8)

/**
 * The maximum size of payload data in octets that the L2CAP layer entity is
 * capable of accepting [0-1024].
 */
#define CFG_BLE_COC_MPS_MAX                             (247)

/**
 * Maximum number of Advertising Data Sets, if Advertising Extension Feature is
 * enabled.
 */
#define CFG_BLE_NUM_ADV_SETS                            (2)

/**
 * Maximum number of Periodic Advertising with Responses subevents.
 */
#define CFG_BLE_NUM_PAWR_SUBEVENTS                      (16)

/**
 * Maximum number of subevent data that can be queued in the controller.
 */
#define CFG_BLE_PAWR_SUBEVENT_DATA_COUNT_MAX            (8U)

/**
 * Maximum number of slots for scanning on the secondary advertising channel,
 * if Advertising Extension Feature is enabled.
 */
#define CFG_BLE_NUM_AUX_SCAN_SLOTS                      (4)

/**
 * Maximum number of slots for synchronizing to a periodic advertising train,
 * if Periodic Advertising and Synchronizing Feature is enabled.
 */
#define CFG_BLE_NUM_SYNC_SLOTS                          (2)

/**
 * Two's logarithm of Filter Accept, Resolving and Advertiser list size.
 */
#define CFG_BLE_FILTER_ACCEPT_LIST_SIZE_LOG2            (4)

/**
 * Maximum number of Antenna IDs in the antenna pattern used in CTE connection
 * oriented mode.
 */
#define CFG_BLE_NUM_CTE_ANTENNA_IDS_MAX                 (8)

/**
 * Maximum number of IQ samples in the buffer used in CTE connection oriented mode.
 */
#define CFG_BLE_NUM_CTE_IQ_SAMPLES_MAX                  (82)

/**
 * Maximum number of slots for synchronizing to a Broadcast Isochronous Group.
 */
#define CFG_BLE_NUM_SYNC_BIG_MAX                        (1U)

/**
 * Maximum number of slots for synchronizing to a Broadcast Isochronous Stream.
 */
#define CFG_BLE_NUM_SYNC_BIS_MAX                        (2U)

/**
 * Maximum number of slots for broadcasting a Broadcast Isochronous Group.
 */
#define CFG_BLE_NUM_BRC_BIG_MAX                         (1U)

/**
 * Maximum number of slots for broadcasting a Broadcast Isochronous Stream.
 */
#define CFG_BLE_NUM_BRC_BIS_MAX                         (2U)

/**
 * Maximum number of Connected Isochronous Groups.
 */
#define CFG_BLE_NUM_CIG_MAX                             (2U)

/**
 * Maximum number of Connected Isochronous Streams.
 */
#define CFG_BLE_NUM_CIS_MAX                             (2U)

/**
 * Size of the internal FIFO used for critical controller events produced by the
 * ISR (e.g. rx data packets).
 */
#define CFG_BLE_ISR0_FIFO_SIZE                          (256)

/**
 * Size of the internal FIFO used for non-critical controller events produced by
 * the ISR (e.g. advertising or IQ sampling reports).
 */
#define CFG_BLE_ISR1_FIFO_SIZE                          (768)

/**
 * Size of the internal FIFO used for controller and host events produced
 * outside the ISR.
 */
#define CFG_BLE_USER_FIFO_SIZE                          (1024)

/**
 * Number of allocated memory blocks used for packet allocation.
 * The use of BLE_STACK_MBLOCKS_CALC macro is suggested to calculate the minimum
 * number of memory blocks for a given number of supported links and ATT MTU.
 */
#define CFG_BLE_MBLOCKS_COUNT           (BLE_STACK_MBLOCKS_CALC(CFG_BLE_ATT_MTU_MAX, CFG_BLE_NUM_RADIO_TASKS, CFG_BLE_NUM_EATT_CHANNELS) + CFG_BLE_MBLOCK_COUNT_MARGIN)

/**
 * Macro to calculate the RAM needed by the stack according the number of links,
 * memory blocks, advertising data sets and all the other initialization
 * parameters.
 */
#define BLE_DYN_ALLOC_SIZE (BLE_STACK_TOTAL_BUFFER_SIZE(CFG_BLE_NUM_RADIO_TASKS,\
                                                        CFG_BLE_NUM_EATT_CHANNELS,\
                                                        CFG_BLE_NUM_GATT_ATTRIBUTES,\
                                                        CFG_BLE_NUM_OF_CONCURRENT_GATT_CLIENT_PROC,\
                                                        CFG_BLE_MBLOCKS_COUNT,\
                                                        CFG_BLE_NUM_ADV_SETS,\
                                                        CFG_BLE_NUM_PAWR_SUBEVENTS,\
                                                        CFG_BLE_PAWR_SUBEVENT_DATA_COUNT_MAX,\
                                                        CFG_BLE_NUM_AUX_SCAN_SLOTS,\
                                                        CFG_BLE_FILTER_ACCEPT_LIST_SIZE_LOG2,\
                                                        CFG_BLE_COC_NBR_MAX,\
                                                        CFG_BLE_NUM_SYNC_SLOTS,\
                                                        CFG_BLE_NUM_CTE_ANTENNA_IDS_MAX,\
                                                        CFG_BLE_NUM_CTE_IQ_SAMPLES_MAX,\
                                                        CFG_BLE_NUM_SYNC_BIG_MAX,\
                                                        CFG_BLE_NUM_BRC_BIG_MAX,\
                                                        CFG_BLE_NUM_SYNC_BIS_MAX,\
                                                        CFG_BLE_NUM_BRC_BIS_MAX,\
                                                        CFG_BLE_NUM_CIG_MAX,\
                                                        CFG_BLE_NUM_CIS_MAX,\
                                                        CFG_BLE_ISR0_FIFO_SIZE,\
                                                        CFG_BLE_ISR1_FIFO_SIZE,\
                                                        CFG_BLE_USER_FIFO_SIZE))

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


/* RAM reserved to manage all the data stack according the number of links,
 * number of services, number of attributes and attribute value length
 */
#define DYNAMIC_MEMORY_SIZE BLE_DYN_ALLOC_SIZE
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
    .MaxNumOfClientProcs = CFG_BLE_NUM_OF_CONCURRENT_GATT_CLIENT_PROC,          \
    .NumOfRadioTasks = CFG_BLE_NUM_RADIO_TASKS,                                 \
    .NumOfEATTChannels = CFG_BLE_NUM_EATT_CHANNELS,                             \
    .NumBlockCount = CFG_BLE_MBLOCKS_COUNT,                                     \
    .ATT_MTU = CFG_BLE_ATT_MTU_MAX,                                             \
    .MaxConnEventLength = MAX_CONN_EVENT_LENGTH_CONF,                           \
    .SleepClockAccuracy = SLEEP_CLOCK_ACCURACY,                                 \
    .NumOfAdvDataSet = CFG_BLE_NUM_ADV_SETS,                                    \
    .NumOfSubeventsPAwR = CFG_BLE_NUM_PAWR_SUBEVENTS,                           \
    .MaxPAwRSubeventDataCount = CFG_BLE_PAWR_SUBEVENT_DATA_COUNT_MAX,           \
    .NumOfAuxScanSlots = CFG_BLE_NUM_AUX_SCAN_SLOTS,				\
    .FilterAcceptListSizeLog2 = CFG_BLE_FILTER_ACCEPT_LIST_SIZE_LOG2,          	\
    .L2CAP_MPS = CFG_BLE_COC_MPS_MAX,                                           \
    .L2CAP_NumChannels = CFG_BLE_COC_NBR_MAX,                                   \
    .NumOfSyncSlots = CFG_BLE_NUM_SYNC_SLOTS,                                   \
    .CTE_MaxNumAntennaIDs = CFG_BLE_NUM_CTE_ANTENNA_IDS_MAX,                    \
    .CTE_MaxNumIQSamples = CFG_BLE_NUM_CTE_IQ_SAMPLES_MAX,                      \
    .NumOfBrcBIG = CFG_BLE_NUM_BRC_BIG_MAX,                                     \
    .NumOfSyncBIS = CFG_BLE_NUM_SYNC_BIS_MAX,                                   \
    .NumOfBrcBIS = CFG_BLE_NUM_BRC_BIS_MAX,                                     \
    .NumOfCIG = CFG_BLE_NUM_CIG_MAX,                                            \
    .NumOfCIS = CFG_BLE_NUM_CIS_MAX,                                            \
    .isr0_fifo_size = CFG_BLE_ISR0_FIFO_SIZE,                                   \
    .isr1_fifo_size = CFG_BLE_ISR1_FIFO_SIZE,                                   \
    .user_fifo_size = CFG_BLE_USER_FIFO_SIZE                                    \
}

#endif // _DTM_CONFIG_H_
