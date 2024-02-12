 /******************** (C) COPYRIGHT 2020 STMicroelectronics ********************
* File Name          : aci_adv_nwk.h
* Author             : AMS - RF Application team
* Description        : daptation layer from stack native advertsing interface
*                      to network interface.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifndef ACI_ADV_NWK_H
#define ACI_ADV_NWK_H

void aci_adv_nwk_init(void);

tBleStatus aci_gap_set_advertising_data_nwk(uint8_t Advertising_Handle, uint8_t Operation, uint8_t Advertising_Data_Length, uint8_t *Advertising_Data);
tBleStatus aci_gap_set_scan_response_data_nwk(uint8_t Advertising_Handle, uint8_t Operation, uint8_t Scan_Response_Data_Length, uint8_t *Scan_Response_Data);
tBleStatus aci_gap_set_periodic_advertising_data_nwk(uint8_t Advertising_Handle, uint8_t Operation, uint8_t Advertising_Data_Length, uint8_t *Advertising_Data);
tBleStatus hci_le_set_advertising_data(uint8_t Advertising_Data_Length, uint8_t Advertising_Data[31]);
tBleStatus hci_le_set_scan_response_data(uint8_t Scan_Response_Data_Length, uint8_t Scan_Response_Data[31]);
tBleStatus hci_le_set_extended_advertising_data(uint8_t Advertising_Handle, uint8_t Operation, uint8_t Fragment_Preference, uint8_t Advertising_Data_Length, uint8_t *Advertising_Data);
tBleStatus hci_le_set_extended_scan_response_data(uint8_t Advertising_Handle, uint8_t Operation, uint8_t Fragment_Preference, uint8_t Scan_Response_Data_Length, uint8_t *Scan_Response_Data);
tBleStatus hci_le_set_periodic_advertising_data(uint8_t Advertising_Handle, uint8_t Operation, uint8_t Advertising_Data_Length, uint8_t Advertising_Data[]);
tBleStatus aci_gap_set_periodic_advertising_data_nwk(uint8_t Advertising_Handle, uint8_t Operation, uint8_t Advertising_Data_Length, uint8_t *Advertising_Data);
#endif /* ACI_ADV_NWK_H */
