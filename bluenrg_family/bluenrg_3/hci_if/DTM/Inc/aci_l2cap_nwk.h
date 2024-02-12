
#ifndef ACI_L2CAP_NWK_H
#define ACI_L2CAP_NWK_H

void aci_l2cap_nwk_init(void);

tBleStatus aci_l2cap_cfc_connection_req_nwk(uint16_t Connection_Handle,
                                            uint16_t SPSM,
                                            uint16_t CID,
                                            uint16_t MTU,
                                            uint16_t MPS,
                                            uint8_t CFC_Policy);

tBleStatus aci_l2cap_cfc_connection_resp_nwk(uint16_t Connection_Handle,
                                             uint8_t Identifier,
                                             uint16_t CID,
                                             uint16_t MTU,
                                             uint16_t MPS,
                                             uint16_t Result,
                                             uint8_t CFC_Policy);

tBleStatus aci_l2cap_transmit_sdu_data_nwk(uint16_t Connection_Handle,
                                           uint16_t CID,
                                           uint16_t SDU_Length,
                                           uint8_t SDU_Data[]);

void aci_l2cap_sdu_data_rx_nwk_event(uint16_t Connection_Handle,
                                     uint16_t CID,
                                     uint16_t RX_Credit_Balance,
                                     uint16_t SDU_Length,
                                     uint8_t SDU_Data[]);

void aci_l2cap_sdu_data_tx_nwk_event(uint16_t Connection_Handle,
                                     uint16_t CID,
                                     uint16_t SDU_Length,
                                     uint16_t TX_Credit_Balance);

void aci_l2cap_nwk_disconnection_complete(uint16_t Connection_Handle,
                                          uint16_t CID);

#endif
