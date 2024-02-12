/******************** (C) COPYRIGHT 2019 STMicroelectronics ********************
* File Name          : DTM_main.c
* Author             : AMA RF Application Team
* Version            : V1.0.0
* Date               : 27-June-2019
* Description        : BlueNRG-LP DTM main
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/** @addtogroup BlueNRGLP_demonstrations_applications
 *  BlueNRG-LP DTM application \see DTM_main.c for documentation.
 *
 *@{
 */
/** @} */
/** \cond DOXYGEN_SHOULD_SKIP_THIS
 */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "bluenrg_lp_stack.h"
#include "DTM_boot.h"
#include "rf_driver_hal_power_manager.h"
#include "transport_layer.h"
#include "hw_config.h"
#include "hal_miscutil.h" 
#include "DTM_cmd_db.h"
#include "bleplat.h"
#include "rf_driver_hal_vtimer.h"
#include "nvm_db.h"
#include "DTM_burst.h"
#include "aci_l2cap_nwk.h"
#include "aci_gatt_nwk.h"

#define RESET_REASON_NRST       ((uint8_t)0x01)
#define RESET_REASON_WDG        ((uint8_t)0x05)
#define RESET_REASON_LOCKUP     ((uint8_t)0x06)
#define RESET_REASON_POR_BOR    ((uint8_t)0x07)
#define RESET_REASON_CRASH      ((uint8_t)0x08)

/* Add aci_blue_initialized_event() prototype */
void aci_blue_initialized_event(uint8_t Reason_Code);
/* aci_blue_initialized_event with with legacy format (not extended) */
void aci_blue_initialized_legacy_event(uint8_t Reason_Code);

/* Add aci_blue_crash_info_event() prototype */
void aci_blue_crash_info_event(uint8_t Crash_Type,
                               uint32_t SP,
                               uint32_t R0,
                               uint32_t R1,
                               uint32_t R2,
                               uint32_t R3,
                               uint32_t R12,
                               uint32_t LR,
                               uint32_t PC,
                               uint32_t xPSR,
                               uint8_t Debug_Data_Length,
                               uint8_t Debug_Data[]);

extern uint32_t irq_count;
extern uint16_t num_packets;
/*
 ******************************************************************************
 ******************************************************************************
 * 
 * The DTM project includes the following configurations:
 *
 * Configuration       | Description
 *  UART/SPI               |  The DTM with interface UART/SPI
 *  UART/SPI_WITH_UPDATER  |  The DTM including the DTM_Updater in the first page
 *                     |   of the Flash memory. This image is necessary to use
 *                     |   the IFR Tool (GUI) and the DTM Firmware Updater (GUI).
 *  UART_FOR_UPDATER   |  The DTM image with the first page empty (offset 0x200).
 *                     |   This image needs the DTM_Updater to run and it is 
 *                     |   used for the DTM Firmware Updater (GUI).
 *------------------------------------------------------------------------------
 * 
 * The configuration DTM - UART_WITH_UPDATER has the following memory layout:
 * 
 * ------------- 0x1007FFFF
 *             
 *      DTM
 * 
 * ------------- 0x10042000
 *  DTM Updater
 * ------------- 0x10040000
 *
 * The DTM Updater allows to access the memory flash through ACI_HAL commands.
 * The DTM Updater can be activated in the following way:
 *  1) Activation by using ACI_HAL_UPDATER_START
 *  2) Activation by using PA15 pin (high level at start up).
 *     Note: if the PA15 pin is used and is high at start up, this will cause the 
 *     DTM Updater starts. So, to avoid this, the support of the DTM Updater
 *     can be removed.
 * 
 * Any change of relevant pins must be reported
 *  also in DTM Updater firmware.
 *
 ****************************************************************************
 ****************************************************************************
*/
int main(void)
{
  crash_info_t crash_info;
  WakeupSourceConfig_TypeDef wakeupIO;
  PowerSaveLevels stopLevel;

  /* System Init */
  DTM_SystemInit();
  
  /* Stack Initialization */
  DTM_StackInit();
    
  /* Transport Layer Init */
  transport_layer_init();
  
  /* Get crash info */
  HAL_GetCrashInfo(&crash_info); 
  
  if(RAM_VR.Reserved[0] == 0x01){
    // Send a comman complete event for HCI_Reset
    uint8_t buffer_out[] = {0x04,0x0E,0x04,0x01,0x03,0x0C,0x00};
    RAM_VR.Reserved[0] = 0x00;
    send_event(buffer_out,7,-1);
  }
  
  uint8_t reset_reason = RESET_REASON_NRST;
  
  /* EVT_BLUE_INITIALIZED */  
  /* Check the reset reason */
  if(RAM_VR.ResetReason & RCC_CSR_WDGRSTF){
    reset_reason = RESET_REASON_WDG;
  }
  else if(RAM_VR.ResetReason & RCC_CSR_LOCKUPRSTF) {
    reset_reason = RESET_REASON_LOCKUP;
  }
  else if(RAM_VR.ResetReason & RCC_CSR_PORRSTF) {
    reset_reason = RESET_REASON_POR_BOR;
  }
  else if(RAM_VR.ResetReason & RCC_CSR_SFTRSTF && (crash_info.signature&0xFFFF0000) == CRASH_SIGNATURE_BASE) {
    /* Crash event can be reported only after a software reset. */
    reset_reason = RESET_REASON_CRASH;
  }
#if (BLESTACK_CONTROLLER_ONLY == 1)
  /* In controller-only mode, let's send the aci_blue_initialized_event only
     for HW reset or if there is a relevant reset reason. */    
  else if(RAM_VR.ResetReason & RCC_CSR_SFTRSTF){
    reset_reason = 0;
  }
  
  if(reset_reason) {  
    aci_blue_initialized_legacy_event(reset_reason);
  }
#else
  
  aci_blue_initialized_event(reset_reason);
  
#endif /* (BLESTACK_CONTROLLER_ONLY == 1) */

  if((crash_info.signature&0xFFFF0000) == CRASH_SIGNATURE_BASE) { 
    aci_blue_crash_info_event(crash_info.signature&0xFF,
                              crash_info.SP,
                              crash_info.R0,
                              crash_info.R1,
                              crash_info.R2,
                              crash_info.R3,
                              crash_info.R12,
                              crash_info.LR,
                              crash_info.PC,
                              crash_info.xPSR,
                              0,
                              NULL);
  }


  /* IWDG disabled in DEEPSTOP */
  HAL_PWR_MNGR_DeepstopWdgState(DISABLE);
  
  wakeupIO.IO_Mask_High_polarity = 0;
  wakeupIO.IO_Mask_Low_polarity = IO_WAKEUP_PIN;
  wakeupIO.RTC_enable = 0;
  wakeupIO.LPU_enable = 0;

  while(1) {
    
    HAL_VTIMER_Tick();
    BLE_STACK_Tick();
    
#if (BLESTACK_CONTROLLER_ONLY == 0) && (CONNECTION_ENABLED == 1)
    /* NVM and burst commands not needed without Host or connection support. */
    NVMDB_Tick();
    BURST_Tick();
#endif
    
    transport_layer_tick();
    
    if(num_packets != 0 &&  irq_count == num_packets)
    {
      uint32_t Number_Of_TX_Packets = 0;
      uint16_t Number_Of_RX_Packets;
      
      /* Reached number of tx test packets */
      hci_le_test_end(&Number_Of_RX_Packets);
      aci_hal_le_tx_test_packet_number(&Number_Of_TX_Packets);
      aci_hal_le_test_end_event(Number_Of_TX_Packets);
      irq_count = 1;
      num_packets = 0;
    }
    
    HAL_PWR_MNGR_Request(POWER_SAVE_LEVEL_STOP_NOTIMER, wakeupIO, &stopLevel); 
  }
}

/* Implementation of L2CAP disconnection event hook. */
int aci_l2cap_disconnection_complete_event_preprocess(uint16_t Connection_Handle,
                                                      uint16_t CID)
{
  aci_l2cap_nwk_disconnection_complete(Connection_Handle,CID);
  ACI_gatt_nwk_disconnection(Connection_Handle, CID);
  
  return 0;  
}


