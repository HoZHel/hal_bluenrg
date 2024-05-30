/**
******************************************************************************
* @file	hci_parser.c 
* @author	VMA RF Application Team
	* @version V1.2.0
	* @date	April-2018
* @brief	 Transport layer file
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
* <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/

#include "ble_const.h"
#include "bluenrg_lp_api.h"
#include "bluenrg_lp_events.h"
#include "bluenrg_lp_stack.h"
#include "hci_parser.h"
#include "hw_config.h"
#include "cmd.h"
#include "transport_layer.h"
#include "osal.h"
#include "DTM_cmd_en.h"
#include "adv_buff_alloc.h"
#include "pawr_buff_alloc.h"
#include "adv_buff_alloc_tiny.h"


/* Private macro -------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#ifndef DTM_DEBUG
/* Macro for debug purposes: not relevant x application */
#define DTM_DEBUG 0
#endif 

#define HCI_PACKET_SIZE 536 // Maximum size of HCI packets are 255 bytes + the HCI header (3 bytes) + 1 byte for transport layer.
#define MAX_ISO_DATA_LOAD_LENGTH 512 // This value should be less than FIFO_VAR_LEN_ITEM_MAX_SIZE - 5

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static uint8_t hci_buffer[HCI_PACKET_SIZE];
static volatile uint16_t hci_pckt_len = 0;

uint8_t buffer_out[HCI_PACKET_SIZE];
uint16_t buffer_out_len=0;

/* Private function prototypes -----------------------------------------------*/
void packet_received(void);

hci_state hci_input(uint8_t *buff, uint16_t len)
{
	static hci_state state = WAITING_TYPE;
	
	static uint16_t collected_payload_len = 0;
	static uint16_t payload_len;
	static uint16_t header_len;
	uint8_t byte;
	uint16_t i = 0;
	
	while(hci_pckt_len < HCI_PACKET_SIZE && i++ < len){
		
	byte = *buff++;

	if(state == WAITING_TYPE)
		hci_pckt_len = 0;
	
	hci_buffer[hci_pckt_len++] = byte;		
		
	if(state == WAITING_TYPE){
		
		state = WAITING_HEADER;
		
		if(byte == HCI_COMMAND_PKT){
		header_len = 4;
		}
		else if(byte == HCI_COMMAND_EXT_PKT){
		header_len = 5;
		}
		else if(byte == HCI_ACLDATA_PKT || byte == HCI_ISO_DATA_PKT){
		header_len = 5;
		}
		else if(byte == HCI_VENDOR_PKT){
		header_len = 4;
		}
		else {
		state = WAITING_TYPE;		
		}
	}
	else if(state == WAITING_HEADER){
		
		if(hci_pckt_len == header_len){
					
		// The entire header has been received
		uint8_t pckt_type = hci_buffer[0];
		collected_payload_len = 0;
		payload_len = 0;
		
		if(pckt_type == HCI_COMMAND_PKT){
			hci_cmd_hdr *hdr = (hci_cmd_hdr *)hci_buffer;
			payload_len = hdr->param_len;
		}
		else if(pckt_type == HCI_COMMAND_EXT_PKT){
			hci_cmd_ext_hdr *hdr = (hci_cmd_ext_hdr *)hci_buffer;
			payload_len = hdr->param_len;
		}
		else if(pckt_type == HCI_ACLDATA_PKT){
			hci_acl_hdr *hdr = (hci_acl_hdr *)hci_buffer;
			payload_len = hdr->dlen;
		}
		else if(pckt_type == HCI_ISO_DATA_PKT){
			hci_iso_data_hdr *hdr = (hci_iso_data_hdr *)hci_buffer;
			payload_len = hdr->dlen & 0x3FFF;
		}
		else if(pckt_type == HCI_VENDOR_PKT){
			hci_vendor_hdr *hdr = (hci_vendor_hdr *)hci_buffer;
			payload_len = hdr->param_len;
		}
		if(payload_len == 0){
			state = WAITING_TYPE;
			packet_received();
		}
		else {
			state = WAITING_PAYLOAD;						
		}
		}		
	}
	else if(state == WAITING_PAYLOAD){
		collected_payload_len++;
		if(collected_payload_len >= payload_len){
		state = WAITING_TYPE;
		packet_received();
		}		
	}
	}
	
	return state;
}

void packet_received(void)
{ 
	switch(hci_buffer[HCI_TYPE_OFFSET]) {
	case HCI_VENDOR_PKT: /* In SPI mode never gets HCI_VENDOR_PKT */
	buffer_out_len = parse_cmd(hci_buffer, hci_pckt_len, buffer_out);
	send_event(buffer_out, buffer_out_len, 1);
	break;
	case HCI_ACLDATA_PKT:
	{
		uint16_t connHandle;
		uint16_t dataLen;
		uint8_t* pduData;
		uint8_t	pb_flag;
		uint8_t	bc_flag;
		
		connHandle = ((hci_buffer[2] & 0x0F) << 8) + hci_buffer[1];
		dataLen = (hci_buffer[4] << 8) + hci_buffer[3];
		pduData = hci_buffer+5;
		pb_flag = (hci_buffer[2] >> 4) & 0x3;
		bc_flag = (hci_buffer[2] >> 6) & 0x3;
		hci_tx_acl_data(connHandle, pb_flag, bc_flag, dataLen, pduData);
	}
	break;
#if HCI_TX_ISO_DATA_ENABLED
	case HCI_ISO_DATA_PKT:
	{
		uint16_t connection_hanlde;
		uint16_t iso_data_load_len;
		uint8_t* iso_data_load;
		uint8_t	pb_flag;
		uint8_t	ts_flag;
		
		connection_hanlde = LE_TO_HOST_16(hci_buffer+1) & 0x0FFF;
		iso_data_load_len = LE_TO_HOST_16(hci_buffer+3) & 0x3FFF;
		pb_flag = (hci_buffer[2] >> 4) & 0x3;
		ts_flag = (hci_buffer[2] >> 6) & 0x1;
		iso_data_load = &hci_buffer[5];
		hci_tx_iso_data(connection_hanlde, pb_flag, ts_flag, iso_data_load_len, iso_data_load);
	}
	break;
#endif
	case HCI_COMMAND_PKT:
	case HCI_COMMAND_EXT_PKT:
	send_command(hci_buffer, hci_pckt_len);
	break;
	default:
	// Error case not allowed TBR
	break;
	}
}

tBleStatus hci_rx_acl_data_event(uint16_t connHandle, uint8_t pb_flag, uint8_t bc_flag, uint16_t dataLen, uint8_t* pduData)
{
	uint8_t buffer_out[251+5];
	
	buffer_out[0] = 0x02;
	buffer_out[1] = connHandle & 0xFF;
	buffer_out[2] = (connHandle >> 8 & 0x0F) | (pb_flag << 4) | (bc_flag << 6) ;
	Osal_MemCpy(buffer_out+3, &dataLen, 2);
	Osal_MemCpy(buffer_out+5, pduData, dataLen);
	send_event(buffer_out, dataLen+2+2+1, -1);
	return 0;
}

tBleStatus hci_le_rx_iso_data_event(uint16_t Connection_Handle, uint8_t	PB_Flag, uint8_t TS_Flag, uint16_t ISO_Data_Load_Length,	uint8_t *ISO_Data_Load)
{
	uint8_t buffer_out[MAX_ISO_DATA_LOAD_LENGTH+5];
	
	if(ISO_Data_Load_Length + 5 > sizeof(buffer_out)) // Header is 5 bytes
	return 0;
	
	buffer_out[0] = HCI_ISO_DATA_PKT;
	Connection_Handle &= 0x0FFF;
	HOST_TO_LE_16(buffer_out+1, Connection_Handle);
	
	PB_Flag &= 0x03;
	buffer_out[2] |= (PB_Flag << 4);
	
	TS_Flag &= 0x01;
	buffer_out[2] |= (TS_Flag << 6);
	
	HOST_TO_LE_16(buffer_out+3, ISO_Data_Load_Length);
	
	Osal_MemCpy(buffer_out+5, ISO_Data_Load, ISO_Data_Load_Length);
	
	send_event(buffer_out, ISO_Data_Load_Length + 5, -1);
	
	return 0;
}

typedef int (*hci_event_process)(uint8_t *buffer_in);

typedef struct hci_event_table_type_s {
	uint16_t evt_code;
	hci_event_process process;
} hci_events_table_type, hci_le_meta_events_table_type, hci_vendor_specific_events_table_type;

typedef struct aci_hal_adv_scan_resp_data_update_event_rp0_s {
	void * Old_Pointer;
	void * New_Pointer;
} __packed aci_hal_adv_scan_resp_data_update_event_rp0;

typedef struct aci_hal_pawr_data_free_event_rp0_s {
	void * Buffer;
	uint8_t Type;
} __packed aci_hal_pawr_data_free_event_rp0;

int aci_hal_adv_scan_resp_data_update_event_preprocess(void *old_pointer, void *new_pointer)
{
	if (new_pointer != old_pointer)
	{
#if (CONTROLLER_EXT_ADV_SCAN_ENABLED == 0)
		adv_tiny_buff_free(old_pointer);
#else
		adv_buff_free_old(old_pointer);
#endif
	}

	/* Do not forward the event. */
	return 1;
}

int aci_hal_adv_scan_resp_data_update_event_process(uint8_t *buffer_in)
{
	int ret;
	/* Input params */
	aci_hal_adv_scan_resp_data_update_event_rp0 *evt = (aci_hal_adv_scan_resp_data_update_event_rp0 *)buffer_in;

	ret = aci_hal_adv_scan_resp_data_update_event_preprocess(evt->Old_Pointer, evt->New_Pointer);

	return ret;
}
#if (CONTROLLER_PERIODIC_ADV_WR_ENABLED)
int aci_hal_pawr_data_free_event_preprocess(void *buffer, uint8_t type)
{
	pawr_buff_free(buffer, type);

	/* Do not forward the event. */
	return 1;
}

int aci_hal_pawr_data_free_event_process(uint8_t *buffer_in)
{
	int ret = 0;

	/* Input params */
	aci_hal_pawr_data_free_event_rp0 *evt = (aci_hal_pawr_data_free_event_rp0 *)buffer_in;
	ret = aci_hal_pawr_data_free_event_preprocess(evt->Buffer, evt->Type);

	return ret;
}
#endif
#define ACI_HAL_ADV_SCAN_RESP_DATA_UPDATE_VSEVT_CODE	0x0010
#define ACI_HAL_PAWR_DATA_FREE_VSEVT_CODE		0x0011
static const hci_vendor_specific_events_table_type hci_vendor_specific_events_table[] = {
	{ACI_HAL_ADV_SCAN_RESP_DATA_UPDATE_VSEVT_CODE, aci_hal_adv_scan_resp_data_update_event_process},
#if (CONTROLLER_PERIODIC_ADV_WR_ENABLED)
	{ACI_HAL_PAWR_DATA_FREE_VSEVT_CODE, aci_hal_pawr_data_free_event_process},
#endif
	{0,NULL},
};
void BLE_STACK_Event(uint8_t *hci_pckt, uint16_t length)
{
	uint16_t i;
	int ret = 0;

	if(hci_pckt[0] == HCI_EVENT_PKT || hci_pckt[0] == HCI_EVENT_EXT_PKT)
	{
		uint8_t	 evt_code = hci_pckt[1];
		uint8_t	*evt_payload = hci_pckt+3;
		uint16_t ecode;

		if(hci_pckt[0] == HCI_EVENT_EXT_PKT)
		{
			evt_payload++;
		}
		ecode = evt_payload[0] + (evt_payload[1] << 8);
		if(evt_code == HCI_VENDOR_PKT)
		{
			for (i = 0; hci_vendor_specific_events_table[i].evt_code != 0; i++)
			{
				if (ecode == hci_vendor_specific_events_table[i].evt_code)
				{
					ret = hci_vendor_specific_events_table[i].process((void *)(evt_payload+2));
					break;
				}
			}
		}
	}
	if (!ret) {
		send_event((uint8_t *)hci_pckt, length, -1);
	}

}
