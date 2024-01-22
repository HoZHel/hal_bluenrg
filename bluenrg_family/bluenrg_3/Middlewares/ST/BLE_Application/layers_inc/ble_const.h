/**
  ******************************************************************************
  * @file    ble_const.h
  * @author  AMS - RF Application team
  * @version V1.0.0
  * @date    23-May-2019
  * @brief   Header file that includes Bluetooth LE stack defines for APIs
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
  * <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
  ******************************************************************************
  */
#ifndef __BLE_CONST_H__
#define __BLE_CONST_H__

#include "ble_status.h"
#include "compiler.h"

/**
 *@addtogroup HCI HCI
 *@{
 */

/**
 * @defgroup Link_Layer Link Layer constants and types
 * @brief Constants and types related to Link Layer functions
 * @{
 */

/**
 * Bluetooth 48 bit address (in little-endian order).
 */
typedef uint8_t tBDAddr[6];


/**
 *@name Bluetooth address types
 * Bluetooth address types
 *@{
 */
#define PUBLIC_ADDR                 (0)
#define RANDOM_ADDR                 (1)
#define STATIC_RANDOM_ADDR          (1)
#define RESOLVABLE_PRIVATE_ADDR     (2)
#define NON_RESOLVABLE_PRIVATE_ADDR (3)
/**
 * @}
 */

/**
 * @name Advertising events properties
 * @{
 */
#define ADV_PROP_NONE                     0x00
#define ADV_PROP_CONNECTABLE              0x01
#define ADV_PROP_SCANNABLE                0x02
#define ADV_PROP_DIRECTED                 0x04
#define ADV_PROP_HDC_DIRECTED             0x08
#define ADV_PROP_LEGACY                   0x10
#define ADV_PROP_ANONYMOUS                0x20
#define ADV_PROP_INCLUDE_TX_POWER         0x40
/**
 * @}
 */

/**
 * @name Advertising channels
 * @{
 */
#define ADV_CH_37  0x01
#define ADV_CH_38  0x02
#define ADV_CH_39  0x04
#define ADV_CH_ALL 0x07
/**
 * @}
 */

/**
 * @name Scan types
 * @{
 */
#define PASSIVE_SCAN    0
#define ACTIVE_SCAN     1
/**
 * @}
 */

/**
 * @name Duplicate filtering
 * @{
 */
#define DUPLICATE_FILTER_DISABLED                   0
#define DUPLICATE_FILTER_ENABLED                    1
#define DUPLICATE_FILTER_ENABLED_FOR_SCAN_PERIOD    2
/**
 * @}
 */

/**
 * @name Bitmasks for Link Layer PHYs
 * @{
 */
#define LE_1M_PHY_BIT       0x01
#define LE_2M_PHY_BIT       0x02
#define LE_CODED_PHY_BIT    0x04

/**
 * @}
 */
   
/**
 * @name Values for Link Layer PHYs
 * @{
 */
#define LE_1M_PHY           0x01
#define LE_2M_PHY           0x02
#define LE_CODED_PHY        0x03
#define LE_CODED_PHY_S8     0x03
#define LE_CODED_PHY_S2     0x04

/**
 * @}
 */

/**
 *@name Advertising filter policy
 *Advertising policy for filtering (white list related) 
 *@{
 */
#define ADV_NO_WHITE_LIST_USE           (0x00)  /**< Process scan and connection requests from all devices (i.e., the White List is not in use) */
#define ADV_WHITE_LIST_FOR_ONLY_SCAN    (0x01)  /**< Process connection requests from all devices and only scan requests from devices that are in the White List */
#define ADV_WHITE_LIST_FOR_ONLY_CONN    (0x02)  /**< Process scan requests from all devices and only connection requests from devices that are in the White List */
#define ADV_WHITE_LIST_FOR_ALL          (0x03)  /**< Process scan and connection requests only from devices in the White List. */
/**
 * @}
 */

/**
 *@name Scanning filter policy
 *Advertising policy for filtering (white list related) 
 *@{
 */
/** Accept all advertising and scan response PDUs except directed advertising PDUs not addressed to this device */
#define SCAN_ACCEPT_ALL                                  (0x00)
/** Accept only advertising and scan response PDUs from devices where
  * the advertiser’s address is in the White List. Directed advertising PDUs
  * which are not addressed to this device shall be ignored */
#define SCAN_ACCEPT_WHITE_LIST_ONLY                      (0x01)
/** Accept all advertising and scan response PDUs except directed advertising
  * PDUs where the initiator’s identity address does not address this
  * device.
  *
  * Note: Directed advertising PDUs where the initiator’s address is a
  * resolvable private address that cannot be resolved are also accepted. */
#define SCAN_ACCEPT_ALL_N_USE_RESOLVING_LIST               (0x02)
/** Accept all advertising and scan response PDUs except:
 * @li advertising and scan response PDUs where the advertiser’s identity
 * address is not in the White List; and
 * @li directed advertising PDUs where the initiator’s identity address does
 * not address this device
 *
 * Note: Directed advertising PDUs where the initiator’s address is a
 * resolvable private address that cannot be resolved are also accepted. */
#define SCAN_ACCEPT_WHITE_LIST_ONLY_N_USE_RESOLVING_LIST   (0x03)
/**
 * @}
 */

/**
 * @name Link Layer State
 * @{
 */   
#define LL_IDLE         0x00
#define LL_ADV          0x01
#define LL_CONN_SLAVE   0x02
#define LL_SCAN         0x03
#define LL_CONN_REQ     0x04
#define LL_CONN_MASTER  0x05
#define LL_TX_TEST      0x06
#define LL_RX_TEST      0x07
/**
 * @}
 */
   
/**
 * @name CTE Type
 * @{
 */   
#define CTE_AOA         0x00
#define CTE_AOD_1us     0x01
#define CTE_AOD_2us     0x02
/**
 * @}
 */

/**
 * @name CTE Type bitmask
 * @{
 */   
#define CTE_AOA_BIT         0x01
#define CTE_AOD_1us_BIT     0x02
#define CTE_AOD_2us_BIT     0x04
/**
 * @}
 */
   
/**
 * @name CTE Slot Duration
 * @{
 */   
#define CTE_SLOT_1us        0x01
#define CTE_SLOT_2us        0x02
/**
 * @}
 */

/**
 * @name Reason for hci_le_transmit_power_reporting_event()
 * @{
 */
#define LOCAL_TX_POWER_CHANGE               0x00
#define REMOTE_TX_POWER_CHANGE              0x01
#define READ_REMOTE_TX_POWER_COMPLETE       0x02
/**
 * @}
 */

/**
 * @name Transmit_Power_Level value
 * @{
 */
#define POWER_NOT_MANAGED                   0x7E
#define POWER_NA                            0x7F
/**
 * @}
 */

/**
 * @name Transmit_Power_Level_Flag values
 * @{
 */
#define MIN_TX_POWER_BIT                    0x01
#define MAX_TX_POWER_BIT                    0x02
/**
 * @}
 */
   
/**
 * @name LE Event Mask
 * @see hci_le_set_event_mask()
 * @{
 */
#define HCI_LE_EVT_MASK_BYTE0_CONNECTION_COMPLETE                                         (0x01U)
#define HCI_LE_EVT_MASK_BYTE0_ADVERTISING_REPORT                                          (0x02U)
#define HCI_LE_EVT_MASK_BYTE0_CONNECTION_UPDATE_COMPLETE                                  (0x04U)
#define HCI_LE_EVT_MASK_BYTE0_READ_REMOTE_FEATURES_COMPLETE                               (0x08U)
#define HCI_LE_EVT_MASK_BYTE0_LTK_REQUEST                                                 (0x10U)
#define HCI_LE_EVT_MASK_BYTE0_REMOTE_CONNECTION_PARAMETER_REQUEST                         (0x20U)
#define HCI_LE_EVT_MASK_BYTE0_DATA_LEN_CHANGE                                             (0x40U)
#define HCI_LE_EVT_MASK_BYTE0_READ_LOCAL_P256_COMPLETE                                    (0x80U)
#define HCI_LE_EVT_MASK_BYTE1_GENERATE_DHKEY_COMPLETE                                     (0x01U)
#define HCI_LE_EVT_MASK_BYTE1_ENHANCED_CONNECTION_COMPLETE                                (0x02U)
#define HCI_LE_EVT_MASK_BYTE1_DIRECTED_ADVERTISING_REPORT                                 (0x04U)
#define HCI_LE_EVT_MASK_BYTE1_PHY_UPDATE_COMPLETE                                         (0x08U)
#define HCI_LE_EVT_MASK_BYTE1_EXTENDED_ADVERTISING_REPORT                                 (0x10U)
#define HCI_LE_EVT_MASK_BYTE1_PERIODIC_ADVERTISING_SYNC_ESTABLISHED                       (0x20U)
#define HCI_LE_EVT_MASK_BYTE1_PERIODIC_ADVERTISING_REPORT                                 (0x40U)
#define HCI_LE_EVT_MASK_BYTE1_PERIODIC_ADVERTISING_SYNC_LOST                              (0x80U)
#define HCI_LE_EVT_MASK_BYTE2_SCAN_TIMEOUT                                                (0x01U)
#define HCI_LE_EVT_MASK_BYTE2_ADVERTISING_SET_TERMINATED                                  (0x02U)
#define HCI_LE_EVT_MASK_BYTE2_SCAN_REQUEST_RECEIVED                                       (0x04U)
#define HCI_LE_EVT_MASK_BYTE2_CHANNEL_SELECTION_ALGORITHM                                 (0x08U)
#define HCI_LE_EVT_MASK_BYTE2_CONNECTIONLESS_IQ_REPORT                                    (0x10U)
#define HCI_LE_EVT_MASK_BYTE2_CONNECTION_IQ_REPORT                                        (0x20U)
#define HCI_LE_EVT_MASK_BYTE2_CTE_REQUEST_FAILED                                          (0x40U)
#define HCI_LE_EVT_MASK_BYTE2_PERIODIC_ADVERTISING_SYNC_TRANSFER                          (0x80U)
#define HCI_LE_EVT_MASK_BYTE3_PATH_LOSS_THRESHOLD                                         (0x80U)
#define HCI_LE_EVT_MASK_BYTE4_TRANSMIT_POWER_REPORTING                                    (0x01U)
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 *@addtogroup HAL_LL HAL/LL
 *@{
 */
/**
 *@addtogroup HAL_constants HAL/LL Constants
 *@brief Constants for Hardware abstraction Layer and Link Layer.
 *@{
 */

/**
 * @name Configuration values.
 * See @ref aci_hal_write_config_data() and @ref aci_hal_read_config_data().
 * @{
 */
#define CONFIG_DATA_PUBADDR_OFFSET                  (0x00) /**< Bluetooth public address */
#define CONFIG_DATA_ER_OFFSET                       (0x08) /**< Encryption root key used to derive LTK and CSRK */
#define CONFIG_DATA_IR_OFFSET                       (0x18) /**< Identity root key used to derive LTK and CSRK */
#define CONFIG_DATA_LL_WITHOUT_HOST                 (0x2C) /**< Switch on/off Link Layer only mode. Set to 1 to disable Host.
                           It can be written only if aci_hal_write_config_data() is the first command
                           after reset. */
#define CONFIG_DATA_STATIC_RANDOM_ADDRESS           (0x2E) /**< To set the static random address used by the stack, instead of the one stored in NVM. */
#define CONFIG_DATA_SCAN_CH_MAP                     (0x2F) /**< To set the channel map for scanning. */
#define CONFIG_DATA_STORED_STATIC_RANDOM_ADDRESS    (0x80) /**< The static random address stored in NVM. */
#define CONFIG_DATA_DEBUG_KEY                       (0xD0) /**< Use debug key for secure connections. */
#define CONFIG_DATA_DLE                             (0xD1) /**< To set the maximum allowed parameter values for Data Length Extension */
/**
 * @}
 */

/**
 * @name Length for configuration values.
 * See @ref aci_hal_write_config_data() and @ref aci_hal_read_config_data().
 * @{
 */
#define CONFIG_DATA_PUBADDR_LEN                 (6)
#define CONFIG_DATA_ER_LEN                      (16)
#define CONFIG_DATA_IR_LEN                      (16)
#define CONFIG_DATA_LL_WITHOUT_HOST_LEN         (1)
#define CONFIG_DATA_STATIC_RANDOM_ADDRESS_LEN   (6)
#define CONFIG_DATA_SCAN_CH_MAP_LEN             (1)
#define CONFIG_DATA_DEBUG_KEY_LEN               (1)
#define CONFIG_DATA_DLE_LEN                     (1)
/**
 * @}
 */


/**
 * @name Types for fw error event
 * @brief See aci_hal_fw_error_event().
 * @{
 */

#define HAL_FW_L2CAP_RECOMBINATION_ERROR            0x01 /**< L2CAP layer failed recombining a PDU */
#define HAL_FW_GATT_UNEXPECTED_RESPONSE_ERROR       0x02 /**< GATT layer received an unexpteced response. Peer probably violated the ATT protocol. */
#define HAL_FW_GATT_SEQUENTIAL_PROTOCOL_ERROR       0x03 /**< GATT layer received an unexpteced request. Peer probably violated the sequentiality of ATT protocol. */
#define HAL_FW_BONDING_DB_FULL_GATTSERVICE_ERROR    0x04 /**< No space to store info GATT database info in NVM. Database needs to be cleaned up (remove invalidated records) or some bonding entries need to be removed. */
#define HAL_FW_BONDING_DB_FULL_PAIRING_ERROR        0x05 /**< No space to store bonding info in NVM. Database needs to be cleaned up (remove invalidated records) or some bonding entries need to be removed. */
#define HAL_FW_SCHEDULER_OVERRUN_ERROR              0x06 /**< Link Layer scheduler failed to reschedule slots for too many times. This may indicate an unrecoverable situation. A system reset is recommended. */ 
#define HAL_FW_MEMBUF_NOT_AVAILABLE                 0x07 /**< Out of memory resources for isochronous channels. */
#define HAL_FW_CTE_TIMER_PROGRAM_ERROR              0x08 /**< Error in programming timer for CTE reception functionality. */

/**
 * @}
 */

/**
 * @}
 */
/**
 * @}
 */


/**
 *@addtogroup GAP GAP
 *@brief Generic Access Profile
 *@{
 */

/**
 *@addtogroup GAP_constants GAP Constants
 *@brief Constants for GAP layer
 *@{
 */

/**
 * @name GAP UUIDs
 * @{
 */
#define GAP_SERVICE_UUID                              (0x1800)
#define DEVICE_NAME_CHAR_UUID                         (0x2A00)
#define APPEARANCE_CHAR_UUID                          (0x2A01)
#define PERIPHERAL_PREFERRED_CONN_PARAMS_CHAR_UUID    (0x2A04)
#define CENTRAL_ADDRESS_RESOLUTION_CHAR_UUID          (0x2AA6)
/**
 * @}
 */

/**
 * @name Characteristic value lengths
 * @{
 */
#define APPEARANCE_CHAR_LEN                     (2)
#define PERIPHERAL_PREFERRED_CONN_PARAMS_CHAR_LEN (8)
#define CENTRAL_ADDRESS_RESOLUTION_CHAR_LEN         (1)
/**
 * @}
 */

/**
 * @name AD Types
 * @brief AD types for adv data and scan response data
 * @{
 */

#define AD_TYPE_FLAGS                   (0x01)

#define AD_TYPE_16_BIT_SERV_UUID          (0x02)
#define AD_TYPE_16_BIT_SERV_UUID_CMPLT_LIST     (0x03)
#define AD_TYPE_32_BIT_SERV_UUID                (0x04)
#define AD_TYPE_32_BIT_SERV_UUID_CMPLT_LIST     (0x05)
#define AD_TYPE_128_BIT_SERV_UUID               (0x06)
#define AD_TYPE_128_BIT_SERV_UUID_CMPLT_LIST    (0x07)

#define AD_TYPE_SHORTENED_LOCAL_NAME            (0x08)
#define AD_TYPE_COMPLETE_LOCAL_NAME             (0x09)

/** TX power level AD type*/
#define AD_TYPE_TX_POWER_LEVEL                  (0x0A)

/** Class of device */
#define AD_TYPE_CLASS_OF_DEVICE                 (0x0D)

/** Security manager TK value AD type */
#define AD_TYPE_SEC_MGR_TK_VALUE                (0x10)

/** Security manager OOB flags */
#define AD_TYPE_SEC_MGR_OOB_FLAGS               (0x11)

/** Slave connection interval AD type */
#define AD_TYPE_SLAVE_CONN_INTERVAL             (0x12)

#define AD_TYPE_SERV_SOLICIT_16_BIT_UUID_LIST   (0x14)
#define AD_TYPE_SERV_SOLICIT_32_BIT_UUID_LIST   (0x1F)
#define AD_TYPE_SERV_SOLICIT_128_BIT_UUID_LIST  (0x15)

#define AD_TYPE_16_BIT_UUID_SERVICE_DATA        (0x16)
#define AD_TYPE_32_BIT_UUID_SERVICE_DATA        (0x20)
#define AD_TYPE_128_BIT_UUID_SERVICE_DATA       (0x21)

/** Manufaturer specific data AD type */
#define AD_TYPE_MANUFACTURER_SPECIFIC_DATA      (0xFF)

/**
 * @}
 */

/**
 * @name Flags AD Type bits
 * @brief Bits for Flags AD Type
 * @{
 */
#define FLAG_BIT_LE_LIMITED_DISCOVERABLE_MODE   (0x01)
#define FLAG_BIT_LE_GENERAL_DISCOVERABLE_MODE   (0x02)
#define FLAG_BIT_BR_EDR_NOT_SUPPORTED           (0x04)
#define FLAG_BIT_LE_BR_EDR_CONTROLLER           (0x08)
#define FLAG_BIT_LE_BR_EDR_HOST                 (0x10)
/**
 * @}
 */


/**
 * @name GAP modes
 * @{
 */
#define GAP_MODE_NON_DISCOVERABLE             0
#define GAP_MODE_LIMITED_DISCOVERABLE         1
#define GAP_MODE_GENERAL_DISCOVERABLE         2
#define GAP_MODE_BROADCAST                    3
/**
 * @}
 */

/**
 * @name Privacy type
 * @{
 */
#define PRIVACY_DISABLED                        (0x00)
#define PRIVACY_HOST_ENABLED                    (0x01)
#define PRIVACY_CONTROLLER_ENABLED              (0x02)
/**
 * @}
 */
   
/**
 * @name Operation
 * Values for Operation parameter (see aci_gap_set_advertising_data()).
 * @{
 */   
#define ADV_COMPLETE_DATA           (0x03)
#define ADV_UNCHANGED_DATA            (0x04)   
/**
 * @}
 */

/**
 * @name Recommended advertising intervals
 * Intervals in terms of 625 micro seconds
 * @{
 */
#define GAP_ADV_INT_MIN_USER_INITIATED_CONN          (48)   /* 30 ms  */
#define GAP_ADV_INT_MAX_USER_INITIATED_CONN          (96)   /* 60 ms  */
#define GAP_ADV_INT_MIN_USER_INITIATED_NONCONN       (160)  /* 100 ms */
#define GAP_ADV_INT_MAX_USER_INITIATED_NONCONN       (240)  /* 150 ms */
#define GAP_ADV_INT_MIN_BACKGROUND                   (1600) /* 1 s    */
#define GAP_ADV_INT_MAX_BACKGROUND                   (1920) /* 1.2 s  */   
/**
 * @}
 */

/**
 * @anchor gap_roles
 * @name GAP Roles
 * @{
*/
#define GAP_PERIPHERAL_ROLE                     (0x01)
#define GAP_BROADCASTER_ROLE                    (0x02)
#define GAP_CENTRAL_ROLE                        (0x04)
#define GAP_OBSERVER_ROLE                       (0x08)
/**
 * @}
 */

/**
 * @anchor gap_procedure_codes
 * @name GAP procedure codes
 * Procedure codes for aci_gap_start_procedure()
 * and aci_gap_terminate_proc() commands.
 * @{
 */    
#define GAP_LIMITED_DISCOVERY_PROC                  (0x00)
#define GAP_GENERAL_DISCOVERY_PROC                  (0x01)
#define GAP_AUTO_CONNECTION_ESTABLISHMENT_PROC      (0x02)
#define GAP_GENERAL_CONNECTION_ESTABLISHMENT_PROC   (0x03)
#define GAP_SELECTIVE_CONNECTION_ESTABLISHMENT_PROC (0x04)
#define GAP_OBSERVATION_PROC                        (0x05)
#define GAP_DIRECT_CONNECTION_ESTABLISHMENT_PROC    (0x06)
#define GAP_NAME_DISCOVERY_PROC                     (0x07)

/**
 * @}
 */

/**
 * @anchor IO_capabilities
 * @name IO capabilities
 * @{
 */
#define IO_CAP_DISPLAY_ONLY     (0x00)
#define IO_CAP_DISPLAY_YES_NO     (0x01)
#define IO_CAP_KEYBOARD_ONLY      (0x02)
#define IO_CAP_NO_INPUT_NO_OUTPUT   (0x03)
#define IO_CAP_KEYBOARD_DISPLAY     (0x04)
/**
 * @}
 */

/**
 * @anchor Auth_req
 * @name Authentication requirements
 * @{
 */
#define BONDING                (0x01)
#define NO_BONDING         (0x00)
/**
 * @}
 */

/**
 * @anchor MITM_req
 * @name MITM protection requirements
 * @{
 */
#define MITM_PROTECTION_NOT_REQUIRED  (0x00)
#define MITM_PROTECTION_REQUIRED        (0x01)
/**
 * @}
 */

/**
 * @anchor OOB_Data
 * @name Out-Of-Band data
 * @{
 */
#define OOB_AUTH_DATA_ABSENT     (0x00)
#define OOB_AUTH_DATA_PRESENT        (0x01)
/**
 * @}
 */

/**
 * @anchor Author_req
 * @name Authorization requirements
 * @{
 */
#define AUTHORIZATION_NOT_REQUIRED    (0x00)
#define AUTHORIZATION_REQUIRED      (0x01)
/**
 * @}
 */

/**
 * @anchor Conn_authorization
 * @name Connection authorization
 * @{
 */
#define CONNECTION_AUTHORIZED     (0x01)
#define CONNECTION_REJECTED     (0x02)
/**
 * @}
 */

/**
 * @anchor Use_fixed_pin
 * @name Use fixed pin
 * @{
 */
#define USE_FIXED_PIN_FOR_PAIRING   (0x0)
#define DONOT_USE_FIXED_PIN_FOR_PAIRING         (0x01)
/**
 * @}
 */

/**
 * @anchor link_security_status
 * @name Link security status
 * @{
 */
#define SM_LINK_AUTHENTICATED               (0x01)
#define SM_LINK_AUTHORIZED                  (0x02)
#define SM_LINK_ENCRYPTED                   (0x04)
/**
 * @}
 */

/**
 * @anchor SMP_pairing_failed_codes
 * @name SMP pairing failed reason codes
 * @{
 */
#define PASSKEY_ENTRY_FAILED      (0x01)
#define OOB_NOT_AVAILABLE     (0x02)
#define AUTH_REQ_CANNOT_BE_MET      (0x03)
#define CONFIRM_VALUE_FAILED      (0x04)
#define PAIRING_NOT_SUPPORTED     (0x05)
#define INSUFF_ENCRYPTION_KEY_SIZE    (0x06)
#define CMD_NOT_SUPPORTED     (0x07)
#define UNSPECIFIED_REASON      (0x08)
#define VERY_EARLY_NEXT_ATTEMPT     (0x09)
#define SM_INVALID_PARAMS     (0x0A)
/**
 * @}
 */

/**
 * @anchor pairing_failed_codes
 * @name Pairing failed error codes
 * Error codes in @ref aci_gap_pairing_complete_event event
 * @{
 */
#define SM_PAIRING_SUCCESS              (0x00)
#define SM_PAIRING_TIMEOUT              (0x01)
#define SM_PAIRING_FAILED               (0x02)
/**
 * @}
 */

/**
 * @anchor secure_connections_support
 * @name Secure connection support option code
 * Error codes in @ref aci_gap_set_authentication_requirement API
 * @{
 */
#define SC_IS_NOT_SUPPORTED (0x00) /* Not supported */
#define SC_IS_SUPPORTED     (0x01) /* Supported but optional (i.e. a Legacy Pairing may be accepted) */
#define SC_IS_MANDATORY     (0x02) /* Supported but mandatory (i.e. do not accept Legacy Pairing but only Secure Connections v.4.2 Pairing) */

/**
 * @}
 */

/**
 * @anchor keypress_support
 * @name Secure connection key press notification option code
 * Error codes in @ref aci_gap_set_authentication_requirement API
 * @{
 */
#define KEYPRESS_IS_NOT_SUPPORTED (0x00)  
#define KEYPRESS_IS_SUPPORTED     (0x01) 

/**
 * @}
 */


/**
 * @}
 */

/**
 * @}
 */

/**
 *@addtogroup GATT GATT
 *@{
 */
/**
 *@defgroup GATT_constants GATT Constants
 *@{
 */

/**
 * @anchor Well-Known_UUIDs
 * @name Well-Known UUIDs
 * @{
 */
#define PRIMARY_SERVICE_UUID                       (0x2800)
#define SECONDARY_SERVICE_UUID                     (0x2801)
#define INCLUDE_SERVICE_UUID                       (0x2802)
#define CHARACTERISTIC_UUID                        (0x2803)
#define CHAR_EXTENDED_PROP_DESC_UUID               (0x2900)
#define CHAR_USER_DESC_UUID                        (0x2901)
#define CHAR_CLIENT_CONFIG_DESC_UUID               (0x2902)
#define CHAR_SERVER_CONFIG_DESC_UUID               (0x2903)
#define CHAR_FORMAT_DESC_UUID                      (0x2904)
#define CHAR_AGGR_FMT_DESC_UUID                    (0x2905)
#define GATT_SERVICE_UUID                          (0x1801)
#define GAP_SERVICE_UUID                           (0x1800)
#define SERVICE_CHANGED_UUID                       (0x2A05)
/**
 * @}
 */

/**
 * @anchor Access_permissions
 * @name Access permissions
 * Access permissions for an attribute
 * @{
 */
#define ATTR_NO_ACCESS                             (0x00)
#define ATTR_ACCESS_READ_ONLY                      (0x01) 
#define ATTR_ACCESS_WRITE_REQ_ONLY                 (0x02)
#define ATTR_ACCESS_READ_WRITE                     (0x03)
#define ATTR_ACCESS_WRITE_WITHOUT_RESPONSE         (0x04)
#define ATTR_ACCESS_SIGNED_WRITE_ALLOWED           (0x08)
/**
 * Allows all write procedures
 */
#define ATTR_ACCESS_WRITE_ANY                      (0x0E)
/**
 * @}
 */

/**
 * @anchor Char_properties
 * @name Characteristic properties.
 * @{
 */
#define CHAR_PROP_BROADCAST               (0x01)
#define CHAR_PROP_READ          (0x02)
#define CHAR_PROP_WRITE_WITHOUT_RESP      (0x04)
#define CHAR_PROP_WRITE                     (0x08)
#define CHAR_PROP_NOTIFY              (0x10)
#define CHAR_PROP_INDICATE              (0x20)
#define CHAR_PROP_SIGNED_WRITE                          (0x40)
#define CHAR_PROP_EXT                             (0x80)
/**
 * @}
 */


/**
 * @anchor Security_permissions
 * @name Security permissions for an attribute.
 * @{
 */
#define ATTR_PERMISSION_NONE                       (0x00) /**< No security. */
#define ATTR_PERMISSION_AUTHEN_READ                (0x01) /**< Need authentication to read */
#define ATTR_PERMISSION_AUTHOR_READ                (0x02) /**< Need authorization to read */
#define ATTR_PERMISSION_ENCRY_READ                 (0x04) /**< Link must be encrypted to read */
#define ATTR_PERMISSION_AUTHEN_WRITE               (0x08) /**< Need authentication to write */
#define ATTR_PERMISSION_AUTHOR_WRITE               (0x10) /**< Need authorization to write */
#define ATTR_PERMISSION_ENCRY_WRITE                (0x20) /**< Link must be encrypted for write */
/**
 * @}
 */

/**
 * @anchor UUID_Types
 * @name Type of UUID (16 bit or 128 bit).
 * @{
 */
#define UUID_TYPE_16                               (0x01)
#define UUID_TYPE_128                              (0x02)
/**
 * @}
 */

/**
 * @anchor Service_type
 * @name Type of service (primary or secondary)
 * @{
 */
#define PRIMARY_SERVICE                            (0x01)
#define SECONDARY_SERVICE                          (0x02)
/**
 * @}
 */

/**
 * @anchor Notification_type
 * @name Type of notification
 * @{
 */
#define GATT_NOTIFICATION                          (0x00)
#define GATT_INDICATION                            (0x02)
/**
 * @}
 */

/**
 * @anchor Gatt_Event_Mask
 * @name Gatt Event Mask
 * Type of event generated by GATT server
 * @{
 */
#define GATT_DONT_NOTIFY_EVENTS                       (0x00) /**< Do not notify events. */
#define GATT_NOTIFY_ATTRIBUTE_WRITE                   (0x01) /**< The application will be notified when a client writes to this attribute.
                                                                  An @ref aci_gatt_srv_attribute_modified_event will be issued. */
#define GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP  (0x02) /**< The application will be notified when a write request, a write cmd
                                                                    or a signed write cmd are received by the server for this attribute.*/
#define GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP   (0x04) /**< The application will be notified when a read request of any type is
                                                                  received for this attribute. */
/**
 * @}
 */

/** 
 * @name Type of characteristic length
 * See aci_gatt_add_char()
 * @{
 */
#define CHAR_VALUE_LEN_CONSTANT           (0x00)
#define CHAR_VALUE_LEN_VARIABLE           (0x01)
/**
 * @}
 */


/**
 * @name Encryption key size
 * @{
 */
/**
 * Minimum encryption key size
 */
#define MIN_ENCRY_KEY_SIZE                (7)

/**
 * Maximum encryption key size
 */
#define MAX_ENCRY_KEY_SIZE                (0x10)
/**
 * @}
 */

/**
 * @name Characteristic Presentation Format
 * @{
 */
typedef PACKED(struct) _charactFormat {
    uint8_t format;
    int8_t exp;
    uint16_t unit;
    uint8_t name_space;
    uint16_t desc;
} charactFormat;

/**
 * @}
 */

/**
 * @name Format
 * @{
 */
#define FORMAT_UINT8         0x04
#define FORMAT_UINT16        0x06
#define FORMAT_SINT16        0x0E
#define FORMAT_SINT24        0x0F
/**
 * @}
 */

/**
 * @name Unit
 * @{
 */
#define UNIT_UNITLESS        0x2700
#define UNIT_TEMP_CELSIUS    0x272F
#define UNIT_PRESSURE_BAR    0x2780
/**
 * @}
 */


/**
 * @}
 */
/**
 * @}
 */


/**
 *@addtogroup L2CAP L2CAP
 *@{
 */
/**
 *@addtogroup L2CAP_constants L2CAP Constants
 *@brief Constants for L2CAP layer.
 *@{
 */

/**
 *@name L2CAP Connection Event Type
 *@see aci_l2cap_cfc_connection_event
 *@{
 */
#define L2CAP_CFC_CONN_REQ              0
#define L2CAP_CFC_CONN_RESP             1
#define L2CAP_ECFC_CONN_REQ             2    
#define L2CAP_ECFC_CONN_RESP            3
/**
 *@}
 */

/**
 *@}
 */
/**
 *@}
 */


#endif /* __BLE_CONST_H__ */
