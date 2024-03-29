

/**
  ******************************************************************************
  * @file    bluenrg_lp_l2cap_aci.h
  * @author  AMS - RF Application team
  * @date    20 November 2023
  * @brief   Header file for external uC - BlueNRG-x in network coprocessor mode (l2cap_aci)
  *          Autogenerated files, do not edit!!
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
  * <h2><center>&copy; COPYRIGHT STMicroelectronics</center></h2>
  ******************************************************************************
  */
#ifndef _BLUENRG_LP_L2CAP_ACI_H_
#define _BLUENRG_LP_L2CAP_ACI_H_

#include "bluenrg_lp_types.h"
/** Documentation for C struct cid_t */
typedef struct cid_t_s {
  /** List of CID values representing the channel endpoints on the local device. Each
   *  entry in the array shall be non-zero and represents a request for a
   *  channel. The value of each CID shall be from the dynamically allocated
   *  range for LE devices (0x0040-0x007F) and shall not be already allocated to
   *  a different channel on the device sending the request.
   *  Values:
   *  - 0x0040 ... 0x007F
   */
  uint16_t Local_CID;
} cid_t;
/**
 * @brief Send an L2CAP connection parameter update request from the slave to
 *        the master. An @ref aci_l2cap_connection_update_resp_event event will
 *        be raised when the master will respond to the  request (accepts or
 *        rejects).
 * @param Connection_Handle Connection handle that identifies the connection.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Conn_Interval_Min Minimum value for the connection event interval.
 *        This shall be less than or equal to Conn_Interval_Max. Time = N * 1.25
 *        msec.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms) 
 * @param Conn_Interval_Max Maximum value for the connection event interval.
 *        This shall be greater than or equal to Conn_Interval_Min. Time = N *
 *        1.25 msec.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms) 
 * @param Slave_latency Slave latency for the connection in number of connection
 *        events.
 *        Values:
 *        - 0x0000 ... 0x01F3
 * @param Timeout_Multiplier Defines connection timeout parameter in the
 *        following manner: Timeout Multiplier * 10ms.
 *        Values:
 *        - 10 (100 ms)  ... 3200 (32000 ms) 
 * @retval Value indicating success or error code.
 */
tBleStatus aci_l2cap_connection_parameter_update_req(uint16_t Connection_Handle,
                                                     uint16_t Conn_Interval_Min,
                                                     uint16_t Conn_Interval_Max,
                                                     uint16_t Slave_latency,
                                                     uint16_t Timeout_Multiplier);
/**
 * @brief Accept or reject a connection update. This command should be sent in
 *        response to a @ref aci_l2cap_connection_update_req_event event from
 *        the controller. The accept parameter has to be set if the connection
 *        parameters given in the event are acceptable.
 * @param Connection_Handle Connection handle that identifies the connection.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Conn_Interval_Min Minimum value for the connection event interval.
 *        This shall be less than or equal to Conn_Interval_Max. Time = N * 1.25
 *        msec.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms) 
 * @param Conn_Interval_Max Maximum value for the connection event interval.
 *        This shall be greater than or equal to Conn_Interval_Min. Time = N *
 *        1.25 msec.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms) 
 * @param Slave_latency Slave latency for the connection in number of connection
 *        events.
 *        Values:
 *        - 0x0000 ... 0x01F3
 * @param Timeout_Multiplier Defines connection timeout parameter in the
 *        following manner: Timeout Multiplier * 10ms.
 *        Values:
 *        - 10 (100 ms)  ... 3200 (32000 ms) 
 * @param Minimum_CE_Length Information parameter about the minimum length of
 *        connection needed for this LE connection. Time = N * 0.625 msec.
 * @param Maximum_CE_Length Information parameter about the maximum length of
 *        connection needed for this LE connection. Time = N * 0.625 msec.
 * @param Identifier Identifier received in ACI_L2CAP_Connection_Update_Req
 *        event.
 * @param Accept Specify if connection update parameters are acceptable or not.
 *        Values:
 *        - 0x00: Reject
 *        - 0x01: Accept
 * @retval Value indicating success or error code.
 */
tBleStatus aci_l2cap_connection_parameter_update_resp(uint16_t Connection_Handle,
                                                      uint16_t Conn_Interval_Min,
                                                      uint16_t Conn_Interval_Max,
                                                      uint16_t Slave_latency,
                                                      uint16_t Timeout_Multiplier,
                                                      uint16_t Minimum_CE_Length,
                                                      uint16_t Maximum_CE_Length,
                                                      uint8_t Identifier,
                                                      uint8_t Accept);
/**
 * @brief Create and configure an L2CAP channel between two devices using LE
 *        Credit Based Flow Control mode.
 * @param Connection_Handle Handle identifying the connection.
 * @param SPSM Simplified Protocol/Service Multiplexer
 *        Values:
 *        - 0x0001 ... 0x00FF
 * @param CID It represents the channel endpoint on the device sending the
 *        request and receiving the response.
 *        Values:
 *        - 0x0001 ... 0x00FF
 * @param MTU The maximum SDU size (in octets) that the L2CAP layer entity
 *        sending the L2CAP_LE_CREDIT_BASED_CONNECTION_REQ can receive on this
 *        channel.
 *        Values:
 *        - 23 ... 65535
 * @param MPS The maximum PDU payload size (in octets) that the L2CAP layer
 *        entity sending the L2CAP_LE_CREDIT_BASED_CONNECTION_REQ is capable of
 *        receiving on this channel.
 *        Values:
 *        - 23 ... 65533
 * @param CFC_Policy Policy to handle flow control. If 0, flow control is
 *        handled by application: credits must be sent using
 *        aci_l2cap_send_flow_control_credits(). If 1, flow control is handled
 *        automatically by the stack.
 *        Values:
 *        - 0x00: L2CAP_CFC_MANUAL
 *        - 0x01: L2CAP_CFC_AUTO
 * @retval Value indicating success or error code.
 */
tBleStatus aci_l2cap_cfc_connection_req_nwk(uint16_t Connection_Handle,
                                            uint16_t SPSM,
                                            uint16_t CID,
                                            uint16_t MTU,
                                            uint16_t MPS,
                                            uint8_t CFC_Policy);
/**
 * @brief Command to be sent to respond to a request to open an L2CAP channel
 *        using LE Credit based Flow Control (the request is notified through
 *        aci_l2cap_cos_connection_event()).
 * @param Connection_Handle Connection handle that identifies the connection.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Identifier Identifier of the request.
 * @param CID The CID represents the local channel endpoint on the device.
 * @param MTU The MTU field specifies the maximum SDU size (in octets) that the
 *        L2CAP layer entity sending the L2CAP_LE_CREDIT_BASED_CONNECTION_RSP
 *        can receive on this channel.
 *        Values:
 *        - 23 ... 65535
 * @param MPS The MPS field specifies the maximum PDU payload size (in octets)
 *        that the L2CAP layer entity sending the
 *        L2CAP_LE_CREDIT_BASED_CONNECTION_RSP is capable of receiving on this
 *        channel.
 *        Values:
 *        - 23 ... 65533
 * @param Result It indicates the outcome of the connection request. A result
 *        value of 0x0000 indicates success while a non-zero value indicates a
 *        fail.
 *        Values:
 *        - 0x0000: L2CAP_CONNECTION_SUCCESSFUL
 *        - 0x0002: L2CAP_CONN_FAIL_SPSM_NOT_SUPPORTED
 *        - 0x0004: L2CAP_CONN_FAIL_INSUFFICIENT_RESOURCES
 *        - 0x0005: L2CAP_CONN_FAIL_INSUFFICIENT_AUTHENTICATION
 *        - 0x0006: L2CAP_CONN_FAIL_INSUFFICIENT_AUTHORIZATION
 *        - 0x0007: L2CAP_CONN_FAIL_KEY_SIZE_TOO_SHORT
 *        - 0x0008: L2CAP_CONN_FAIL_INSUFFICIENT_ENCRYPTION
 *        - 0x0009: L2CAP_CONN_FAIL_INVALID_SOURCE_CID
 *        - 0x000A: L2CAP_CONN_FAIL_SOURCE_CID_ALREADY_ALLOCATED
 *        - 0x000B: L2CAP_CONN_FAIL_UNACCEPTABLE_PARAMETERS
 * @param CFC_Policy Policy to handle flow control. If 0, flow control is
 *        handled by application: credits must be sent using
 *        aci_l2cap_send_flow_control_credits(). If 1, flow control is handled
 *        automatically by the stack.
 *        Values:
 *        - 0x00: L2CAP_CFC_MANUAL
 *        - 0x01: L2CAP_CFC_AUTO
 * @retval Value indicating success or error code.
 */
tBleStatus aci_l2cap_cfc_connection_resp_nwk(uint16_t Connection_Handle,
                                             uint8_t Identifier,
                                             uint16_t CID,
                                             uint16_t MTU,
                                             uint16_t MPS,
                                             uint16_t Result,
                                             uint8_t CFC_Policy);
/**
 * @brief Command to be issued when the device is capable of receiving
 *        additional K-frames in LE Credit Based Flow Control mode.
 * @param Connection_Handle Connection handle that identifies the connection.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param CID The local channel endpoint that identifies the L2CAP channel.
 * @param RX_Credits Additional number of K-frames that local L2CAP layer entity
 *        can currently receive from the peer.
 * @param CFC_Policy Policy to handle flow control. If 0, flow control is
 *        handled by application: credits must be sent using
 *        aci_l2cap_send_flow_control_credits(). If 1, flow control is handled
 *        automatically by the stack.
 *        Values:
 *        - 0x00: L2CAP_CFC_MANUAL
 *        - 0x01: L2CAP_CFC_AUTO
 * @param[out] RX_Credit_Balance Current number of K-frames that peer's L2CAP
 *             layer entity can send.
 * @retval Value indicating success or error code.
 */
tBleStatus aci_l2cap_send_flow_control_credits(uint16_t Connection_Handle,
                                               uint16_t CID,
                                               uint16_t RX_Credits,
                                               uint8_t CFC_Policy,
                                               uint16_t *RX_Credit_Balance);
/**
 * @brief Command to terminate an L2CAP channel.
 * @param Connection_Handle
 * @param CID Local endpoint of the channel to be disconnected.
 * @retval Value indicating success or error code.
 */
tBleStatus aci_l2cap_disconnect(uint16_t Connection_Handle,
                                uint16_t CID);
/**
 * @brief Function to be called to send an SDU using an  L2CAP channel in LE
 *        Credit Based Flow Control mode. An ACI_L2CAP_SDU_DATA_TX_NWK_EVENT is
 *        raised when the SDU has been processed.
 * @param Connection_Handle Connection handle that identifies the connection.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param CID The local channel endpoint that identifies the L2CAP channel.
 * @param SDU_Length Length of the SDU to be transmitted.
 * @param SDU_Data Data contained in the SDU to be transmitted. Data must be
 *        valid until the SDU is transmitted.
 * @retval Value indicating success or error code.
 */
tBleStatus aci_l2cap_transmit_sdu_data_nwk(uint16_t Connection_Handle,
                                           uint16_t CID,
                                           uint16_t SDU_Length,
                                           uint8_t SDU_Data[]);
/**
 * @brief Command to send an L2CAP_CREDIT_BASED_CONNECTION_REQ packet in order
 *        to request the creation and configuration of up to 5 L2CAP channels on
 *        a specified SPSM.
 * @param Connection_Handle Identifier received in the
 *        aci_l2cap_ecfc_connection_event.
 * @param SPSM Simplified Protocol/Service Multiplexer
 *        Values:
 *        - 0x0001 ... 0x00FF
 * @param MTU The maximum SDU size (in octets) that the L2CAP layer entity can
 *        receive on each of the Source CID channels (represented by Local_CID
 *        array parameter). This is equal to the maximum size of an ATT packet
 *        on the Enhanced ATT bearer.
 *        Values:
 *        - 0x0040 ... 0xFFFF
 * @param MPS The maximum PDU payload size (in octets) that the local L2CAP
 *        layer is capable of receiving on each of the Source CID channels
 *        (represented by Local_CID array parameter).
 *        Values:
 *        - 0x0040 ... 0xFFFF
 * @param CID_Count The number of potential Enhanced ATT bearers that are going
 *        to be opened. This is the number of L2CAP channels to be opened in
 *        Enhanced Credit Based Flow Control mode.
 *        Values:
 *        - 0x01 ... 0x05
 * @param cid See @ref cid_t
 * @retval Value indicating success or error code.
 */
tBleStatus aci_l2cap_ecfc_connection_req(uint16_t Connection_Handle,
                                         uint16_t SPSM,
                                         uint16_t MTU,
                                         uint16_t MPS,
                                         uint8_t CID_Count,
                                         cid_t cid[]);
/**
 * @brief Command to send an L2CAP_CREDIT_BASED_CONNECTION_RSP packet in order
 *        to respond to an incoming L2CAP_CREDIT_BASED_CONNECTION_REQ for the
 *        creation of new enhanced credit based flow control channels. It has to
 *        be used upon the reception of an ACI_L2CAP_ECFC_CONNECTION_EVENT.
 * @param Connection_Handle Identifier received in the
 *        aci_l2cap_ecfc_connection_event.
 * @param Identifier Identifier received in the aci_l2cap_ecfc_connection_event.
 * @param MTU The maximum SDU size (in octets) that the local L2CAP layer entity
 *        can receive on each of the Source CID channels (represented by
 *        Local_CID array parameter). This is equal to the maximum size of an
 *        ATT packet on the Enhanced ATT bearer.
 *        Values:
 *        - 0x0040 ... 0xFFFF
 * @param MPS The maximum PDU payload size (in octets) that the local L2CAP
 *        layer entity is capable of receiving on each of the Source CID
 *        channels (represented by Local_CID array parameter).
 *        Values:
 *        - 0x0040 ... 0xFFFF
 * @param Result It indicates the outcome of the connection request. A result
 *        value of 0x0000 indicates success while a non-zero value indicates a
 *        fail.
 *        Values:
 *        - 0x0000: L2CAP_CONN_SUCCESSFUL
 *        - 0x0002: L2CAP_CONN_FAIL_SPSM_NOT_SUPPORTED
 *        - 0x0004: L2CAP_CONN_FAIL_INSUFFICIENT_RESOURCES
 *        - 0x0005: L2CAP_CONN_FAIL_INSUFFICIENT_AUTHENTICATION
 *        - 0x0006: L2CAP_CONN_FAIL_INSUFFICIENT_AUTHORIZATION
 *        - 0x0007: L2CAP_CONN_FAIL_KEY_SIZE_TOO_SHORT
 *        - 0x0008: L2CAP_CONN_FAIL_INSUFFICIENT_ENCRYPTION
 *        - 0x0009: L2CAP_CONN_FAIL_INVALID_SOURCE_CID
 *        - 0x000A: L2CAP_CONN_FAIL_SOURCE_CID_ALREADY_ALLOCATED
 *        - 0x000B: L2CAP_CONN_FAIL_UNACCEPTABLE_PARAMETERS
 *        - 0x000C: L2CAP_CONN_FAIL_INVALID_PARAMETERS
 *        - 0x000D: L2CAP_CONN_FAIL_NO_INFO
 *        - 0x000E: L2CAP_CONN_FAIL_AUTHENTICATION_PENDING
 *        - 0x000F: L2CAP_CONN_FAIL_AUTHORIZATION_PENDING
 * @param CID_Count The number of potential Enhanced ATT bearers that are going
 *        to be opened. This is the number of L2CAP channels to be opened in
 *        Enhanced Credit Based Flow Control mode.
 *        Values:
 *        - 0x01 ... 0x05
 * @param cid See @ref cid_t
 * @retval Value indicating success or error code.
 */
tBleStatus aci_l2cap_ecfc_connection_resp(uint16_t Connection_Handle,
                                          uint8_t Identifier,
                                          uint16_t MTU,
                                          uint16_t MPS,
                                          uint16_t Result,
                                          uint8_t CID_Count,
                                          cid_t cid[]);
/**
 * @brief Command to send an L2CAP_CREDIT_BASED_RECONFIGURE_REQ packet in order
 *        to request to change its receive MTU or MPS values compared to when
 *        the channels were created or last reconfigured.
 * @param Connection_Handle Identifier received in the
 *        aci_eatt_connection_event.
 * @param MTU The maximum SDU size (in octets) that the L2CAP layer entity can
 *        receive on each of the Source CID channels (represented by Local_CID
 *        array parameter). This is equal to the maximum size of an ATT packet
 *        on the Enhanced ATT bearer.
 *        Values:
 *        - 0x0040 ... 0xFFFF
 * @param MPS The maximum PDU payload size (in octets) that the local L2CAP
 *        layer is capable of receiving on each of the Source CID channels
 *        (represented by Local_CID array parameter).
 *        Values:
 *        - 0x0040 ... 0xFFFF
 * @param CID_Count The number of potential Enhanced ATT bearers that are going
 *        to be opened. This is the number of L2CAP channels to be opened in
 *        Enhanced Credit Based Flow Control mode.
 *        Values:
 *        - 0x01 ... 0x05
 * @param cid See @ref cid_t
 * @retval Value indicating success or error code.
 */
tBleStatus aci_l2cap_ecfc_reconfigure_req(uint16_t Connection_Handle,
                                          uint16_t MTU,
                                          uint16_t MPS,
                                          uint8_t CID_Count,
                                          cid_t cid[]);
/**
 * @brief Command to send an L2CAP_CREDIT_BASED_RECONFIGURE_RSP packet in order
 *        to respond to an incoming L2CAP_CREDIT_BASED_RECONFIGURE_REQ. It has
 *        to be used upon the reception of an
 *        ACI_L2CAP_ECFC_RECONFIGURATION_EVENT.
 * @param Connection_Handle Identifier received in the
 *        aci_eatt_connection_event.
 * @param Identifier Identifier received in the aci_eatt_connection_event.
 * @param Result It indicates the outcome of the connection request. A result
 *        value of 0x0000 indicates success while a non-zero value indicates the
 *        connection request was refused.
 *        Values:
 *        - 0x0000: L2CAP_RECONFIG_SUCCESSFUL
 *        - 0x0001: L2CAP_MTU_REDUCTION_NOT_ALLOWED
 *        - 0x0002: L2CAP_MPS_REDUCTION_NOT_ALLOWED
 *        - 0x0003: L2CAP_INVALID_DESTINATION_CID
 *        - 0x0004: L2CAP_UNACCEPTABLE_PARAMETERS
 * @retval Value indicating success or error code.
 */
tBleStatus aci_l2cap_ecfc_reconfigure_resp(uint16_t Connection_Handle,
                                           uint8_t Identifier,
                                           uint16_t Result);
/**
     * @}
     */
    #endif /* _BLUENRG_LP_L2CAP_ACI_H_ */
