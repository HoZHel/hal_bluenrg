 /******************** (C) COPYRIGHT 2020 STMicroelectronics ********************
* File Name          : aci_adv_nwk.c
* Author             : AMS - RF Application team
* Description        : Adaptation layer from stack native advertising interface
*                      to network interface.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#include <string.h>
#include "system_util.h"
#include "ble_status.h"
#include "adv_buff_alloc.h"
#include "adv_buff_alloc_tiny.h"
#include "bluenrg_lp_api.h"
#include "ble_const.h"
#include "stack_user_cfg.h"
#include "DTM_config.h"

#define LEGACY_ADV_HANDLE   0xFE
#define MEM_ALLOC_OVERHEAD   8
#define MAX_ADV_DATA_LENGTH MIN(ACI_GATT_ADV_NWK_BUFFER_SIZE_CONF - ACI_ATT_QUEUED_WRITE_SIZE_CONF - MEM_ALLOC_OVERHEAD, 1650)

/** @name Operation codes for setting advertising data
  * @{
  */
#define INTERMEDIATE_FRAGMENT   0
#define FIRST_FRAGMENT          1
#define LAST_FRAGMENT           2
#define COMPLETE_DATA           3
#define UNCHANGED_DATA          4
/**
  * @}
  */

/** @name Options for layer parameter of set_legacy_adv_scan_data
  * @{
  */
#define LL                      0
#define GAP                     1
/**
  * @}
  */

#if CONTROLLER_EXT_ADV_SCAN_ENABLED == 0

static tBleStatus allocate_and_set_data_legacy(uint8_t Advertising_Handle, uint8_t Operation,
                                               uint8_t Data_Length, uint8_t *Data, uint8_t adv_scan_resp, uint8_t layer);

static tBleStatus set_legacy_data_ptr(uint16_t Data_Length, uint8_t *Data, uint8_t adv_scan_resp, uint8_t layer);

void aci_adv_nwk_init(void)
{
  adv_tiny_buff_init();
}

static tBleStatus allocate_and_set_data(uint8_t Advertising_Handle, uint8_t Operation, uint8_t Data_Length, uint8_t *Data, uint8_t adv_scan_resp, uint8_t layer)
{
  return allocate_and_set_data_legacy(Advertising_Handle, Operation, Data_Length, Data, adv_scan_resp, layer);
}

/* Event raised by the stack when a new data pointer becomes active.
   The function will free the the memory pointed by the old pointer.
*/
void aci_hal_adv_scan_resp_data_update_event(void *old_pointer, void *new_pointer)
{
  if (new_pointer != old_pointer)
  {
    adv_tiny_buff_free(old_pointer);
  }
}

// Advertising_Handle is actually not used
static tBleStatus allocate_and_set_data_legacy(uint8_t Advertising_Handle, uint8_t Operation, uint8_t Data_Length, uint8_t *Data, uint8_t adv_scan_resp, uint8_t layer)
{
  tBleStatus ret;
  uint8_t *adv_buffer;
  
  if(Operation != COMPLETE_DATA || Data_Length > 31)
    return BLE_ERROR_INVALID_HCI_CMD_PARAMS;
  
  adv_buffer = adv_tiny_buff_alloc();
  
  if(adv_buffer == NULL)
    return BLE_ERROR_CONTROLLER_BUSY;
  
  memcpy(adv_buffer, Data, Data_Length);
  
  ret = set_legacy_data_ptr(Data_Length, adv_buffer, adv_scan_resp, layer);
  
  if(ret)
  {
    adv_tiny_buff_free(adv_buffer);
  }
  
  return ret;
}

static tBleStatus set_legacy_data_ptr(uint16_t Data_Length, uint8_t *Data, uint8_t adv_scan_resp, uint8_t layer)
{
  if(adv_scan_resp == ADV_DATA){
    // Advertising Data
#if (BLESTACK_CONTROLLER_ONLY == 0)
    if(layer == GAP)
      return aci_gap_set_advertising_data(0, COMPLETE_DATA, Data_Length, Data);
    else
#endif
      return ll_set_legacy_advertising_data_ptr(Data_Length, Data);
  }
  else{
    // Scan Response
#if (BLESTACK_CONTROLLER_ONLY == 0)
    if(layer == GAP)
      return aci_gap_set_scan_response_data(0, Data_Length, Data);
    else
#endif
      return ll_set_legacy_scan_reponse_data_ptr(Data_Length, Data);
  }
}

tBleStatus aci_gap_set_advertising_enable_preprocess(uint8_t Enable,
                                                     uint8_t Number_of_Sets,
                                                     Advertising_Set_Parameters_t Advertising_Set_Parameters[])
{
  return BLE_STATUS_SUCCESS;
}

#else /* CONTROLLER_EXT_ADV_SCAN_ENABLED == 1 */

tBleStatus hci_le_read_maximum_advertising_data_length(uint16_t *Maximum_Advertising_Data_Length)
{
  *Maximum_Advertising_Data_Length = MAX_ADV_DATA_LENGTH;
  
  return BLE_STATUS_SUCCESS;
}

static tBleStatus allocate_and_set_data_ext(uint8_t Advertising_Handle,
                                            uint8_t Operation, uint8_t Data_Length, uint8_t *Data, uint8_t data_type, uint8_t layer);

void aci_adv_nwk_init(void)
{
  adv_buff_init();
}

static tBleStatus allocate_and_set_data(uint8_t Advertising_Handle, uint8_t Operation, uint8_t Data_Length, uint8_t *Data, uint8_t data_type, uint8_t layer)
{
  return allocate_and_set_data_ext(Advertising_Handle, Operation, Data_Length, Data, data_type, layer);
}

tBleStatus hci_le_set_extended_advertising_data(uint8_t Advertising_Handle, uint8_t Operation,
                                                uint8_t Fragment_Preference,
                                                uint8_t Advertising_Data_Length, uint8_t *Advertising_Data)
{
  return allocate_and_set_data(Advertising_Handle,
                                Operation, Advertising_Data_Length, Advertising_Data, ADV_DATA, LL);

}

tBleStatus hci_le_set_extended_scan_response_data(uint8_t Advertising_Handle, uint8_t Operation,
                                                uint8_t Fragment_Preference,
                                                uint8_t Scan_Response_Data_Length, uint8_t *Scan_Response_Data)
{
  return allocate_and_set_data(Advertising_Handle,
                                Operation, Scan_Response_Data_Length, Scan_Response_Data, SCAN_RESP_DATA, LL);
}

#if CONTROLLER_PERIODIC_ADV_ENABLED
tBleStatus hci_le_set_periodic_advertising_data(uint8_t Advertising_Handle, uint8_t Operation,
                                                uint8_t Advertising_Data_Length, uint8_t *Advertising_Data)
{
  return allocate_and_set_data(Advertising_Handle,
                               Operation, Advertising_Data_Length, Advertising_Data, PERIODIC_ADV_DATA, LL);
}

tBleStatus aci_gap_set_periodic_advertising_data_nwk(uint8_t Advertising_Handle,
                                                     uint8_t Operation, uint8_t Advertising_Data_Length, uint8_t *Advertising_Data)
{
  return allocate_and_set_data(Advertising_Handle,
                               Operation, Advertising_Data_Length, Advertising_Data, PERIODIC_ADV_DATA, GAP);
}
#endif


void aci_hal_adv_scan_resp_data_update_event(void *old_pointer, void *new_pointer)
{
  if (new_pointer != old_pointer)
  {
    adv_buff_free_old(old_pointer);
  }
}

static uint8_t adv_data_check_param(uint8_t Advertising_Handle, uint8_t Operation, uint8_t Advertising_Data_Length, uint8_t *adv_enabled)
{
  uint16_t adv_event_prop;
  uint8_t ret;
  BOOL periodic_adv_configured;
  BOOL periodic_adv_enabled;
  
  if(Advertising_Handle == LEGACY_ADV_HANDLE){
    
    *adv_enabled = TRUE;
    
    if(Operation != COMPLETE_DATA || Advertising_Data_Length > 31)
      return BLE_ERROR_INVALID_HCI_CMD_PARAMS;
      
    return BLE_STATUS_SUCCESS;
  }
  
  if(Advertising_Handle > 0xEF || Operation > UNCHANGED_DATA)
    return BLE_ERROR_INVALID_HCI_CMD_PARAMS;
  
  ret = ll_get_advertising_info(Advertising_Handle, adv_enabled, &periodic_adv_configured, &periodic_adv_enabled, &adv_event_prop);
  
  /* "If the advertising set corresponding to the Advertising_Handle parameter does
  not exist, then the Controller shall return the error code Unknown Advertising
  Identifier (0x42)." */
  if(ret!=0) // Advertising set does not exist.
    return BLE_ERROR_UNKNOWN_ADVERTISING_IDENTIFIER;
  
  /* "If the advertising set specifies a type that does not support advertising data, the
  Controller shall return the error code Invalid HCI Command Parameters (0x12)." */   
  if((adv_event_prop & ADV_PROP_LEGACY && adv_event_prop & ADV_PROP_DIRECTED) || // Legacy, directed advertising
     (adv_event_prop & ADV_PROP_SCANNABLE && !(adv_event_prop & ADV_PROP_LEGACY))) // Scannable, non-legacy 
    return BLE_ERROR_INVALID_HCI_CMD_PARAMS;
  
  /* "If the advertising set uses legacy advertising PDUs that support advertising
  data and either Operation is not 0x03 or the Advertising_Data_Length
  parameter exceeds 31 octets, the Controller shall return the error code Invalid
  HCI Command Parameters (0x12)."  */
  if(adv_event_prop & ADV_PROP_LEGACY && (Operation != COMPLETE_DATA || Advertising_Data_Length > 31)) // Legacy advertising
    return BLE_ERROR_INVALID_HCI_CMD_PARAMS;
  
  /* "If Operation is not 0x03 or 0x04 and Advertising_Data_Length is zero, the
  Controller shall return the error code Invalid HCI Command Parameters (0x12)." */   
  if(Operation != COMPLETE_DATA && Operation != UNCHANGED_DATA && Advertising_Data_Length == 0)
    return BLE_ERROR_INVALID_HCI_CMD_PARAMS;
  
  /* "If advertising is currently enabled for the specified advertising set and
  Operation does not have the value 0x03 or 0x04, the Controller shall return the
  error code Command Disallowed (0x0C)." */
  if(*adv_enabled && Operation != COMPLETE_DATA && Operation != UNCHANGED_DATA)
    return BLE_ERROR_COMMAND_DISALLOWED;
  
  return BLE_STATUS_SUCCESS;
}

static uint8_t scan_resp_data_check_param(uint8_t Advertising_Handle, uint8_t Operation, uint8_t Scan_Resp_Data_Length, uint8_t *adv_enabled)
{
  uint16_t adv_event_prop;
  uint8_t ret;
  BOOL periodic_adv_configured;
  BOOL periodic_adv_enabled;
  
  if(Advertising_Handle == LEGACY_ADV_HANDLE){
    
    /* Asssume advertising is always enabled, even if this is not true.
       In this way the old buffer cannot be freed even if the advertising is not enabled. */
    *adv_enabled = TRUE;
    
    if(Operation != COMPLETE_DATA || Scan_Resp_Data_Length > 31)
      return BLE_ERROR_INVALID_HCI_CMD_PARAMS;
      
    return BLE_STATUS_SUCCESS;
  }
  
  if(Operation > COMPLETE_DATA)
    return BLE_ERROR_INVALID_HCI_CMD_PARAMS;
  
  ret = ll_get_advertising_info(Advertising_Handle, adv_enabled, &periodic_adv_configured, &periodic_adv_enabled, &adv_event_prop);
  
  /* "If the advertising set corresponding to the Advertising_Handle parameter does
  not exist, then the Controller shall return the error code Unknown Advertising
  Identifier (0x42)." */
  if(ret!=0) // Advertising set does not exist.
    return BLE_ERROR_UNKNOWN_ADVERTISING_IDENTIFIER;
  
  /* "If the advertising set is non-scannable and the Host uses this command other
      than to discard existing data, the Controller shall return the error code Invalid
      HCI Command Parameters (0x12)." */
  if((!(adv_event_prop & ADV_PROP_SCANNABLE) && Scan_Resp_Data_Length != 0))
    return BLE_ERROR_INVALID_HCI_CMD_PARAMS;
  
  /* If the advertising set uses scannable legacy advertising PDUs and either
     Operation is not 0x03 or the Scan_Response_Data_Length parameter exceeds
     31 octets, the Controller shall return the error code Invalid HCI Command 
     Parameters (0x12). */
  if(adv_event_prop & ADV_PROP_LEGACY && adv_event_prop & ADV_PROP_SCANNABLE
     && (Operation != COMPLETE_DATA || Scan_Resp_Data_Length > 31))
    return BLE_ERROR_INVALID_HCI_CMD_PARAMS;
     
    
  /* "If Operation is not 0x03 and Scan_Response_Data_Length is zero, the
      Controller shall return the error code Invalid HCl Command Parameters (0x12)." */   
  if(Operation != COMPLETE_DATA && Scan_Resp_Data_Length == 0)
    return BLE_ERROR_INVALID_HCI_CMD_PARAMS;
  
  /* "If advertising is currently enabled for the specified advertising set and
      Operation does not have the value 0x03, the Controller shall return the error
      code Command Disallowed (0x0C)." */
  if(*adv_enabled && Operation != COMPLETE_DATA)
    return BLE_ERROR_COMMAND_DISALLOWED;
  
  return BLE_STATUS_SUCCESS;
}

#if CONTROLLER_PERIODIC_ADV_ENABLED
static uint8_t periodic_adv_data_check_param(uint8_t Advertising_Handle, uint8_t Operation, uint8_t Advertising_Data_Length, uint8_t *adv_enabled)
{
  uint16_t adv_event_prop;
  BOOL periodic_adv_configured;
  BOOL periodic_adv_enabled;
  uint8_t ret;
  
  if(Operation > UNCHANGED_DATA)
    return BLE_ERROR_INVALID_HCI_CMD_PARAMS;
  
  ret = ll_get_advertising_info(Advertising_Handle, adv_enabled, &periodic_adv_configured, &periodic_adv_enabled, &adv_event_prop);
  
    /* "If the advertising set corresponding to the Advertising_Handle parameter does
       not exist, then the Controller shall return the error code Unknown Advertising
       Identifier (0x42)." */
  if(ret!=0) // Advertising set does not exist.
    return BLE_ERROR_UNKNOWN_ADVERTISING_IDENTIFIER;
  
  /* "If the advertising set has not been configured for periodic advertising,
     then the Controller shall return the error code Command Disallowed (0x0C)." */
  if(!periodic_adv_configured)
    return BLE_ERROR_COMMAND_DISALLOWED;
  
  /* "If periodic advertising is currently enabled for the specified advertising set and
      Operation does not have the value 0x03 or 0x04, then the Controller shall
      return the error code Command Disallowed (0x0C)." */
  if(periodic_adv_enabled && (Operation != COMPLETE_DATA) && (Operation != UNCHANGED_DATA))
    return BLE_ERROR_COMMAND_DISALLOWED;
  
  return  BLE_STATUS_SUCCESS;
}
#endif

static tBleStatus set_data_ptr(uint8_t Advertising_Handle,
                                    uint8_t Operation, uint16_t Data_Length, uint8_t *Data, uint8_t data_type, uint8_t gap)
{
  if(data_type == ADV_DATA)
  {
    // Advertising Data
#if (BLESTACK_CONTROLLER_ONLY == 0)
    if(gap)
    {
      return aci_gap_set_advertising_data(Advertising_Handle, Operation,
                                          Data_Length, Data);
	}
    else
#endif
    {
      if(Advertising_Handle == LEGACY_ADV_HANDLE)
      {
        return ll_set_legacy_advertising_data_ptr(Data_Length, Data);
      }
      else 
      {
        return ll_set_advertising_data_ptr(Advertising_Handle, Operation, Data_Length, Data);
      }
    }
  }
  else if (data_type == SCAN_RESP_DATA)
  {
    // Scan Response
#if (BLESTACK_CONTROLLER_ONLY == 0)
    if(gap)
    {
      return aci_gap_set_scan_response_data(Advertising_Handle,
                                            Data_Length, Data);
	}
    else 
#endif
    {
      if(Advertising_Handle == LEGACY_ADV_HANDLE)
      {
        return ll_set_legacy_scan_reponse_data_ptr(Data_Length, Data);
      }
      else 
      {
        return ll_set_scan_reponse_data_ptr(Advertising_Handle, Data_Length, Data);
      }
    }
  }
#if CONTROLLER_PERIODIC_ADV_ENABLED
  else { /* data_type == PERIODIC_ADV_DATA */
#if (BLESTACK_CONTROLLER_ONLY == 0)
    if(gap)
    {
      return aci_gap_set_periodic_advertising_data(Advertising_Handle, Data_Length, Data);
    }
    else
#endif
    {
      return ll_set_periodic_advertising_data_ptr(Advertising_Handle, Data_Length, Data);
    }
  }
#else  
  return BLE_ERROR_UNKNOWN_HCI_COMMAND;
#endif
}

static tBleStatus allocate_and_set_data_ext(uint8_t Advertising_Handle,
                                  uint8_t Operation, uint8_t Data_Length, uint8_t *Data, uint8_t data_type, uint8_t layer)
{
  uint8_t *buffer;
  uint16_t buff_len;
  BOOL adv_enabled;
  uint8_t status = BLE_ERROR_UNKNOWN_HCI_COMMAND;
  uint16_t old_buff_len;
  uint8_t extend;
  
  if(Advertising_Handle == LEGACY_ADV_HANDLE && layer != LL){
    return BLE_ERROR_INVALID_HCI_CMD_PARAMS; // This should not happen
  }

  if(data_type == ADV_DATA){
    status = adv_data_check_param(Advertising_Handle, Operation, Data_Length, &adv_enabled);
  }
  else if(data_type == SCAN_RESP_DATA){
    status = scan_resp_data_check_param(Advertising_Handle, Operation, Data_Length, &adv_enabled);
  }
#if CONTROLLER_PERIODIC_ADV_ENABLED
  else { /* PERIODIC_ADV_DATA */
    status = periodic_adv_data_check_param(Advertising_Handle, Operation, Data_Length, &adv_enabled);
  }
#endif
  
  if(status)
    return status;
  
  /* Allocate the buffer */
  switch(Operation){
    
  case UNCHANGED_DATA:
    return set_data_ptr(Advertising_Handle, Operation, Data_Length, Data, data_type, layer);
    
  case FIRST_FRAGMENT:
  case COMPLETE_DATA:
    extend = FALSE;
    adv_buff_free_next(Advertising_Handle, data_type);
    break;
    
  case INTERMEDIATE_FRAGMENT:
  case LAST_FRAGMENT:
    extend = TRUE;
    break;
    
  }
  
  buffer = adv_buff_alloc(Advertising_Handle, Data_Length, extend, &old_buff_len, data_type);
  if((buffer == NULL && Data_Length != 0)|| (old_buff_len + Data_Length > MAX_ADV_DATA_LENGTH)){
    // Tell the stack that current advertising data has to be cancelled.
    set_data_ptr(Advertising_Handle, COMPLETE_DATA, 0, NULL, data_type, layer);
    adv_buff_deactivate_current(Advertising_Handle, data_type);
    adv_buff_free_next(Advertising_Handle, data_type);
    
    return BLE_ERROR_MEMORY_CAPACITY_EXCEEDED;
  }
  
  memcpy(buffer + old_buff_len, Data, Data_Length);
  
  buff_len = old_buff_len + Data_Length;
  
  if(Operation == LAST_FRAGMENT || Operation == COMPLETE_DATA){
    status = set_data_ptr(Advertising_Handle, COMPLETE_DATA, buff_len, buffer, data_type, layer);
    if(status == BLE_STATUS_SUCCESS){
      adv_buff_deactivate_current(Advertising_Handle, data_type);
      adv_buff_activate_next(Advertising_Handle, data_type);
    }
    else{
      // Free what has been allocated so far for this advertising handle
      adv_buff_free_next(Advertising_Handle, data_type);
    }
  }
  else if(Operation == FIRST_FRAGMENT){
    // Discard any existing partial or complete advertising data.
    set_data_ptr(Advertising_Handle, COMPLETE_DATA, 0, NULL, data_type, layer);
    adv_buff_deactivate_current(Advertising_Handle, data_type);
  }
  
  return status;
}

#if CONTROLLER_PERIODIC_ADV_ENABLED
tBleStatus hci_le_set_periodic_advertising_enable_preprocess(uint8_t Enable,
                                                             uint8_t Advertising_Handle)
{
  /* If Enable is set to 0x01 (periodic advertising is enabled) and the periodic
  advertising data in the advertising set is not complete, the Controller shall
  return the error code Command Disallowed (0x0C). */
  if(Enable == 0x01 && new_buff_pending(Advertising_Handle, PERIODIC_ADV_DATA) == TRUE)
    return BLE_ERROR_COMMAND_DISALLOWED;
  
  return BLE_STATUS_SUCCESS;
}
#endif

tBleStatus hci_le_set_extended_advertising_enable_preprocess(uint8_t Enable,
                                                             uint8_t Number_of_Sets,
                                                             Advertising_Set_Parameters_t Advertising_Set_Parameters[])
{
  /*  The remainder of this section only applies if Enable is set to 0x01.
  If the advertising data or scan response data in the advertising set is not
  complete, the Controller shall return the error code Command Disallowed
  (0x0C). */
  if(Enable == 0x01){
    
    for(int i = 0; i < Number_of_Sets; i++){
      if(new_buff_pending(Advertising_Set_Parameters[i].Advertising_Handle, ADV_DATA) == TRUE ||
         new_buff_pending(Advertising_Set_Parameters[i].Advertising_Handle, SCAN_RESP_DATA) == TRUE)
        return BLE_ERROR_COMMAND_DISALLOWED;
    }
    
  }
  
  return BLE_STATUS_SUCCESS;
}

tBleStatus aci_gap_set_advertising_enable_preprocess(uint8_t Enable,
                                                     uint8_t Number_of_Sets,
                                                     Advertising_Set_Parameters_t Advertising_Set_Parameters[])
{
  return hci_le_set_extended_advertising_enable_preprocess(Enable, Number_of_Sets, Advertising_Set_Parameters);
}

#endif


tBleStatus hci_le_set_advertising_data(uint8_t Advertising_Data_Length,
                                       uint8_t Advertising_Data[31])
{
  return allocate_and_set_data(LEGACY_ADV_HANDLE, COMPLETE_DATA, Advertising_Data_Length, Advertising_Data, ADV_DATA, LL);
}

tBleStatus hci_le_set_scan_response_data(uint8_t Scan_Response_Data_Length,
                                         uint8_t Scan_Response_Data[31])
{
  return allocate_and_set_data(LEGACY_ADV_HANDLE, COMPLETE_DATA, Scan_Response_Data_Length, Scan_Response_Data, SCAN_RESP_DATA, LL);
}

tBleStatus aci_gap_set_advertising_data_nwk(uint8_t Advertising_Handle,
                                            uint8_t Operation, uint8_t Advertising_Data_Length, uint8_t *Advertising_Data)
{
  return allocate_and_set_data(Advertising_Handle,
                                Operation, Advertising_Data_Length, Advertising_Data, ADV_DATA, GAP);
}

tBleStatus aci_gap_set_scan_response_data_nwk(uint8_t Advertising_Handle,
                                              uint8_t Operation, uint8_t Scan_Response_Data_Length, uint8_t *Scan_Response_Data)
{
  return allocate_and_set_data(Advertising_Handle,
                                Operation, Scan_Response_Data_Length, Scan_Response_Data, SCAN_RESP_DATA, GAP);
}

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/
