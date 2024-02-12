
#include "stdint.h"
#include "ble_status.h"
#include "bluenrg_lp_api.h"
#include "aci_l2cap_nwk.h"
#include "ble_const.h"
#include "system_util.h"
#include "string.h"
#include "stack_user_cfg.h"
#include "DTM_config.h"

#if (L2CAP_COS_ENABLED == 1) && (BLESTACK_CONTROLLER_ONLY == 0)

/* Number of buffers used to receive SDUs. Only one per L2CAP channel.
   Only used for CFC L2CAP channels (credit based flow control mode).  */
#define NUM_L2CAP_CFC_RX_BUFFERS                MIN(2, NUM_L2CAP_COCS_CONF)
/* Number of buffers used to transmit SDUs. There can be more than one for each
   L2CAP channel. They are used for both CFC and ECFC L2CAP channels (credit
   based flow control mode and enhanced credit based flow control mode).  */
#define NUM_L2CAP_COS_TX_BUFFERS                7

#define RX_SDU_BUFFER_SIZE                      600
#define TX_SDU_BUFFER_SIZE                      255

typedef struct {
  uint16_t Connection_Handle;
  uint16_t CID;
  uint8_t  data_buffer[RX_SDU_BUFFER_SIZE];
} l2cap_cfc_rx_buffers_t;

typedef struct {
  uint16_t Connection_Handle;
  uint16_t CID;
  uint8_t  data_buffer[TX_SDU_BUFFER_SIZE];
} l2cap_cos_tx_buffers_t;

static l2cap_cfc_rx_buffers_t rx_buffer[NUM_L2CAP_CFC_RX_BUFFERS];
static l2cap_cos_tx_buffers_t tx_buffer[NUM_L2CAP_COS_TX_BUFFERS];


/**
* @brief  Initialize the module for buffer allocation. Mandatory before any use of the module.
* @retval None
*/
void aci_l2cap_nwk_init(void)
{
  uint8_t i;
  
  for(i = 0; i < NUM_L2CAP_CFC_RX_BUFFERS; i++)
  {
    rx_buffer[i].Connection_Handle = 0xFFFF;
  }
  
  for(i = 0; i < NUM_L2CAP_COS_TX_BUFFERS; i++)
  {
    tx_buffer[i].Connection_Handle = 0xFFFF;
  }
}

static int8_t get_free_rx_buffer_index(void)
{
  for(uint8_t i = 0; i < NUM_L2CAP_CFC_RX_BUFFERS; i++)
  {
    if(rx_buffer[i].Connection_Handle == 0xFFFF)
      return i;
  }
  
  return -1;
}

static int8_t get_free_tx_buffer_index(void)
{
  for(uint8_t i = 0; i < NUM_L2CAP_COS_TX_BUFFERS; i++)
  {
    if(tx_buffer[i].Connection_Handle == 0xFFFF)
      return i;
  }
  
  return -1;
}

static int8_t get_rx_buffer_index(uint16_t Connection_Handle, uint16_t CID)
{
  for(uint8_t i = 0; i < NUM_L2CAP_CFC_RX_BUFFERS; i++)
  {
    if(rx_buffer[i].Connection_Handle == Connection_Handle && rx_buffer[i].CID == CID)
      return i;
  }
  
  return -1;
}

static int8_t get_tx_buffer_index_by_buff(void * SDU_Data_Buffer)
{
  for(uint8_t i = 0; i < NUM_L2CAP_COS_TX_BUFFERS; i++)
  {
    if(SDU_Data_Buffer == tx_buffer[i].data_buffer)
      return i;
  }
  
  return -1;
}

static void allocate_rx_buffer(uint8_t i, uint16_t Connection_Handle, uint16_t CID)
{
  rx_buffer[i].Connection_Handle = Connection_Handle;
  rx_buffer[i].CID = CID;
}

static void allocate_tx_buffer(uint8_t i, uint16_t Connection_Handle, uint16_t CID)
{
  tx_buffer[i].Connection_Handle = Connection_Handle;
  tx_buffer[i].CID = CID;
}

static void free_tx_buffer(uint8_t i)
{
  tx_buffer[i].Connection_Handle = 0xFFFF;
}

static void free_all_buffers_by_params(uint16_t Connection_Handle, uint16_t CID)
{
  int8_t i;
  
  i = get_rx_buffer_index(Connection_Handle, CID);
  if(i >= 0)
  {
    rx_buffer[i].Connection_Handle = 0xFFFF;
  }
  
  for(uint8_t index = 0; index < NUM_L2CAP_COS_TX_BUFFERS; index++)
  {
    if(tx_buffer[index].Connection_Handle == Connection_Handle && tx_buffer[index].CID == CID)
      tx_buffer[i].Connection_Handle = 0xFFFF;
  }
}


/** ---------------------------------------------------------
*           L2CAP   COS   CFC   Connection
*  ---------------------------------------------------------
*/
tBleStatus aci_l2cap_cfc_connection_req_nwk(uint16_t Connection_Handle,
                                            uint16_t SPSM,
                                            uint16_t CID,
                                            uint16_t MTU,
                                            uint16_t MPS,
                                            uint8_t CFC_Policy)
{
  tBleStatus ret;
  int8_t     buff_index;
  
  buff_index = get_free_rx_buffer_index();
  if(buff_index < 0)
  {
    return BLE_ERROR_MEMORY_CAPACITY_EXCEEDED;
  }
  
  ret = aci_l2cap_cfc_connection_req(Connection_Handle, SPSM, CID, MTU, MPS, CFC_Policy,
                                     RX_SDU_BUFFER_SIZE, rx_buffer[buff_index].data_buffer);
  
  if(ret == BLE_STATUS_SUCCESS)
  {
    allocate_rx_buffer(buff_index, Connection_Handle, CID);
  }
  
  return ret;
}

tBleStatus aci_l2cap_cfc_connection_resp_nwk(uint16_t Connection_Handle,
                                             uint8_t Identifier,
                                             uint16_t CID,
                                             uint16_t MTU,
                                             uint16_t MPS,
                                             uint16_t Result,
                                             uint8_t CFC_Policy)

{
  tBleStatus ret;
  int8_t     buff_index;
  
  buff_index = get_free_rx_buffer_index();
  if(buff_index < 0)
  {
    return BLE_ERROR_MEMORY_CAPACITY_EXCEEDED;
  }
  
  ret = aci_l2cap_cfc_connection_resp(Connection_Handle, Identifier, CID, MTU, MPS, Result, CFC_Policy,
                                      RX_SDU_BUFFER_SIZE, rx_buffer[buff_index].data_buffer);
  
  if(ret == BLE_STATUS_SUCCESS)
  {
    allocate_rx_buffer(buff_index, Connection_Handle, CID);
  }
  
  return ret;
}

int aci_l2cap_cos_connection_event_preprocess(uint16_t Connection_Handle,
                                              uint8_t Event_Type,
                                              uint8_t Identifier,
                                              uint16_t SPSM,
                                              uint16_t Peer_MTU,
                                              uint16_t Peer_MPS,
                                              uint16_t Initial_Credits,
                                              uint16_t Result,
                                              uint8_t CID_Count,
                                              conn_cid_t conn_cid[])
{
  if(Event_Type == L2CAP_CFC_CONN_RESP && Result != 0)
  {
    for(uint8_t i = 0; i < CID_Count; i++)
    {
      free_all_buffers_by_params(Connection_Handle, conn_cid[i].Local_CID);
    }
  }
  
  return 0;
}

void aci_l2cap_nwk_disconnection_complete(uint16_t Connection_Handle,
                                         uint16_t CID)
{
  free_all_buffers_by_params(Connection_Handle, CID);
}

void aci_l2cap_sdu_data_rx_event(uint16_t Connection_Handle,
                                 uint16_t CID,
                                 uint16_t RX_Credit_Balance,
                                 uint16_t SDU_Length)
{
  tBleStatus ret;
  
  uint8_t SDU_Data_Buffer[RX_SDU_BUFFER_SIZE]; //TODO: Probably need to move outside of CSTACK.
  uint16_t SDU_Length_extracted = 0x0000;
  
  ret = aci_l2cap_extract_sdu_data(Connection_Handle, CID, sizeof(SDU_Data_Buffer), SDU_Data_Buffer, &SDU_Length_extracted);
  
  if(ret == BLE_STATUS_SUCCESS)
  {
    aci_l2cap_sdu_data_rx_nwk_event(Connection_Handle, CID, RX_Credit_Balance, SDU_Length_extracted, SDU_Data_Buffer);
  }
}

tBleStatus aci_l2cap_transmit_sdu_data_nwk(uint16_t Connection_Handle,
                                           uint16_t CID,
                                           uint16_t SDU_Length,
                                           uint8_t SDU_Data[])
{
  tBleStatus ret;
  int8_t buff_index;
  
  if(SDU_Length > TX_SDU_BUFFER_SIZE)
  {
    return BLE_ERROR_MEMORY_CAPACITY_EXCEEDED;
  }
  
  buff_index = get_free_tx_buffer_index();
  if(buff_index < 0)
  {
    return BLE_STATUS_INSUFFICIENT_RESOURCES;
  }
  
  memcpy(tx_buffer[buff_index].data_buffer, SDU_Data, SDU_Length);
  
  ret = aci_l2cap_transmit_sdu_data(Connection_Handle, CID, SDU_Length, tx_buffer[buff_index].data_buffer);
  if(ret == BLE_STATUS_SUCCESS)
  {
    allocate_tx_buffer(buff_index, Connection_Handle, CID);
  }
  return ret;
}

void aci_l2cap_sdu_data_tx_event(uint16_t Connection_Handle,
                                 uint16_t CID,
                                 uint16_t SDU_Length,
                                 void * SDU_Data_Buffer,
                                 uint16_t TX_Credit_Balance)
{
  int8_t buff_index;
  
  buff_index = get_tx_buffer_index_by_buff(SDU_Data_Buffer);
  if(buff_index < 0)
  {
    return; // This should not happen.
  }
  
  free_tx_buffer(buff_index);
  
  aci_l2cap_sdu_data_tx_nwk_event(Connection_Handle, CID, SDU_Length, TX_Credit_Balance);
}

#else /* L2CAP_COS_ENABLED == 0 */

void aci_l2cap_nwk_init(void)
{
}

int aci_l2cap_cos_connection_event_preprocess(uint16_t Connection_Handle,
                                              uint8_t Event_Type,
                                              uint8_t Identifier,
                                              uint16_t SPSM,
                                              uint16_t Peer_MTU,
                                              uint16_t Peer_MPS,
                                              uint16_t Initial_Credits,
                                              uint16_t Result,
                                              uint8_t CID_Count,
                                              conn_cid_t conn_cid[])
{
  return 0;
}

void aci_l2cap_nwk_disconnection_complete(uint16_t Connection_Handle,
                                          uint16_t CID)
{
}

tBleStatus aci_l2cap_transmit_sdu_data_nwk(uint16_t Connection_Handle,
                                           uint16_t CID,
                                           uint16_t SDU_Length,
                                           uint8_t SDU_Data[])
{
  return BLE_ERROR_UNKNOWN_HCI_COMMAND;
}

tBleStatus aci_l2cap_cfc_connection_req_nwk(uint16_t Connection_Handle,
                                            uint16_t SPSM,
                                            uint16_t CID,
                                            uint16_t MTU,
                                            uint16_t MPS,
                                            uint8_t CFC_Policy)
{
  return BLE_ERROR_UNKNOWN_HCI_COMMAND;
}

tBleStatus aci_l2cap_cfc_connection_resp_nwk(uint16_t Connection_Handle,
                                             uint8_t Identifier,
                                             uint16_t CID,
                                             uint16_t MTU,
                                             uint16_t MPS,
                                             uint16_t Result,
                                             uint8_t CFC_Policy)
{
  return BLE_ERROR_UNKNOWN_HCI_COMMAND;
}

#endif
