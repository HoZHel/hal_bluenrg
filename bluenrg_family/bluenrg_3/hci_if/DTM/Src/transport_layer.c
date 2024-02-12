/**
******************************************************************************
* @file    tramsport_layer.c 
* @author  AMS RF Application Team
* @version V1.1.0
* @date    27-March-2019
* @brief   Transport layer file
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
#include "system_BlueNRG_LP.h"
#include "rf_device_it.h"
#include "bluenrg_lp_api.h"
#include "transport_layer.h"
#include "rf_driver_hal_power_manager.h"
#include "hw_config.h"
#include "hci_parser.h"
#include "DTM_cmd_db.h"
#include "osal.h"
#include "fifo.h"
#include "cmd.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MAX_EVENT_SIZE  (536)

#if defined(CONFIG_DEVICE_BLUENRG_LP)
#define COMMAND_BUFFER_SIZE  (536 + 4)
#define EVENT_BUFFER_SIZE    2300
#elif defined(CONFIG_DEVICE_BLUENRG_LPS)
#define COMMAND_BUFFER_SIZE  265        /* Decrease buffer size for reducing RAM footprint */
#define EVENT_BUFFER_SIZE    1024
#endif 

#define FIFO_ALIGNMENT       4
#define FIFO_VAR_LEN_ITEM_MAX_SIZE (MAX_EVENT_SIZE)

#define LEGACY_ADV_OPCODE_LOW  0x2006 // Lowest opcode for legacy advertising commands
#define LEGACY_ADV_OPCODE_HIGH 0x200D // Highest opcode for legacy advertising commands

#define EXTENDED_ADV_OPCODE_LOW  0x2036 // Lowest opcode for extended advertising commands
#define EXTENDED_ADV_OPCODE_HIGH 0x204A // Highest opcode for extended advertising commands

// Opcodes of commands that returns command status instead of command complete events
#define HCI_LE_CREATE_CONNECTION_OPCODE                 0x200D
#define HCI_LE_EXTENDED_CREATE_CONNECTION_OPCODE        0x2043
#define HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_OPCODE  0x2044

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ALIGN(2) uint8_t event_buffer[EVENT_BUFFER_SIZE + FIFO_VAR_LEN_ITEM_MAX_SIZE];
uint8_t command_buffer[COMMAND_BUFFER_SIZE];
ALIGN(2) uint8_t command_fifo_buffer_tmp[COMMAND_BUFFER_SIZE];
circular_fifo_t event_fifo, command_fifo;
uint8_t reset_pending = 0; 


typedef PACKED(struct) event_lost_register_s {
  uint8_t event_lost;
  uint8_t event_register[5];
  uint64_t event_lost_code;
} event_lost_register_t;
static event_lost_register_t event_lost_register;

uint8_t dma_state = DMA_IDLE;

#ifdef DEBUG_DTM
DebugLabel debug_buf[DEBUG_ARRAY_LEN] = {EMPTY,};
uint32_t debug_cnt = 0;
#endif

/* SPI protocol variables & definitions */
#ifdef SPI_INTERFACE
#define SPI_HEADER_LEN  (uint8_t)(4)    /* Indeed the header len is 5 due to load of dummy from FIFO */
#define SPI_CTRL_WRITE  (uint8_t)(0x0A)
#define SPI_CTRL_READ   (uint8_t)(0x0B)

SpiProtoType spi_proto_state = SPI_PROT_INIT_STATE;

/* Store first 4 bytes replaced with spi header during send event procedure */
uint8_t event_fifo_header_restore[4] = {0,0,0,0};
uint8_t restore_flag = 0;

/* Flag to signal if a timoeout HEADER_NOT_RECEIVED happens */
static uint8_t header_timeout = 0;

ALIGN(2) uint8_t buff_dma[MAX_EVENT_SIZE + SPI_HEADER_LEN + 1];

#endif

/* Private function prototypes -----------------------------------------------*/
static void enqueue_event(circular_fifo_t *fifo, uint16_t buff_len1, uint8_t *buff_evt1, uint16_t buff_len2, uint8_t *buff_evt2, int8_t overflow_index);
/* Private functions ---------------------------------------------------------*/
extern uint8_t tone_started;

PowerSaveLevels App_PowerSaveLevel_Check(PowerSaveLevels level)
{
  if(tone_started)
  {
    return POWER_SAVE_LEVEL_RUNNING;
  }
#ifndef SPI_INTERFACE
  if (( (dma_state == DMA_IDLE) && (fifo_size(&event_fifo) > 0) )|| (fifo_size(&command_fifo) > 0) || reset_pending) {
    return POWER_SAVE_LEVEL_RUNNING;
  } else {
#ifndef NO_DMA
    return POWER_SAVE_LEVEL_RUNNING;
#else
    return POWER_SAVE_LEVEL_CPU_HALT;
#endif
  }
#else  
  if(((SPI_STATE_CHECK(SPI_PROT_SLEEP_STATE) || SPI_STATE_CHECK(SPI_PROT_CONFIGURED_STATE) ) && (fifo_size(&event_fifo) > 0)) || reset_pending) {
    return POWER_SAVE_LEVEL_RUNNING;
  }
  
  else if(SPI_STATE_CHECK(SPI_PROT_CONFIGURED_EVENT_PEND_STATE) || SPI_STATE_CHECK(SPI_PROT_WAITING_DATA_STATE)) {    
    return POWER_SAVE_LEVEL_CPU_HALT;
  }
  
  else if ((SPI_STATE_CHECK(SPI_PROT_SLEEP_STATE) || SPI_STATE_CHECK(SPI_PROT_CONFIGURED_STATE) ) && (fifo_size(&event_fifo) == 0)) {
    if(!SPI_STATE_CHECK(SPI_PROT_SLEEP_STATE)) // This check is to avoid unnecessary debug annotation
      SPI_STATE_TRANSACTION(SPI_PROT_SLEEP_STATE);
    
    /* Edge mode is the normal mode.
    * Level mode is for SLEEP mode because:
    * when in sleep mode, all the registers setting are lost
    * the wake up occurs when the SPI_CS_PIN is low and this trigger also the irq.
    * This irq will be lost if in edge sensitive mode, because
    * once the BlueNRG-1 is woken up, the restore of the registers setting is done,
    * and so the irq setting on edge detection, but the event is lost due to this delay.
    * So, before go in SLEEP state, the sensitive is changed to the level.
    * In this way, once the restore of the registers setting is done, the event is not lost.
    */   
    LL_EXTI_SetType(LL_EXTI_TYPE_LEVEL, BSP_SPI_EXTI_CS_PIN);
    return POWER_SAVE_LEVEL_STOP_NOTIMER;
  }
  
  else {
    return POWER_SAVE_LEVEL_RUNNING;
  }
#endif
}

/* "If, since the last power-on or reset, the Host has ever issued a legacy
  advertising command and then issues an extended advertising command, or
  has ever issued an extended advertising command and then issues a legacy
  advertising command, the Controller shall return the error code Command
  Disallowed (0x0C)." 
  This function returns 1 if an error has to be given. */
static uint8_t check_legacy_extended_call(uint16_t opcode, uint8_t *buffer_out)
{
  static uint8_t legacy_cmd_issued = FALSE, extended_cmd_issued =  FALSE;
  uint8_t allowed = TRUE;
  
  if(opcode >= LEGACY_ADV_OPCODE_LOW && opcode <= LEGACY_ADV_OPCODE_HIGH){
    if(extended_cmd_issued)
      allowed = FALSE; // Error
    else {
      legacy_cmd_issued = TRUE;
      allowed = TRUE; // OK
    }
  }
  else if(opcode >= EXTENDED_ADV_OPCODE_LOW && opcode <= EXTENDED_ADV_OPCODE_HIGH){
    if(legacy_cmd_issued)
      allowed = FALSE; // Error
    else {
      extended_cmd_issued = TRUE;
      allowed = TRUE; // OK
    }
  }
  
  if(!allowed){
    if(opcode == HCI_LE_CREATE_CONNECTION_OPCODE ||
       opcode == HCI_LE_EXTENDED_CREATE_CONNECTION_OPCODE||
       opcode == HCI_LE_PERIODIC_ADVERTISING_CREATE_SYNC_OPCODE){
      buffer_out[0] = 0x04;
      buffer_out[1] = 0x0F;
      buffer_out[2] = 0x04;
      buffer_out[3] = 0x0C;
      buffer_out[4] = 0x01;
      HOST_TO_LE_16(buffer_out+5,opcode);
      return 7;
    }
    else {
      buffer_out[0] = 0x04;
      buffer_out[1] = 0x0E;
      buffer_out[2] = 0x04;
      buffer_out[3] = 0x01;
      HOST_TO_LE_16(buffer_out+4,opcode);
      buffer_out[6] = 0x0C;
      return 7;      
    }      
  }
  
  return 0;  
}

/* Process Commands */
uint16_t process_command(uint16_t op_code, uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  uint32_t i;
  uint16_t ret_val;
  
  if (op_code == 0x0c03) {
    // For HCI_RESET, reset the system, so that Controller, Timer module and HEAP are reinitialized.
    RAM_VR.Reserved[0] = 0x01; // Remember to send a command complete after reset is completed.
    reset_pending = 1;
    return 0;
  }

  ret_val = check_legacy_extended_call(op_code, buffer_out);
  if(ret_val != 0){
    return ret_val;
  }
  
  for (i = 0; hci_command_table[i].opcode != 0; i++) {
    if (op_code == hci_command_table[i].opcode) {
      ret_val = hci_command_table[i].execute(buffer_in, buffer_in_length, buffer_out, buffer_out_max_length);      
      /* add get crash handler */
      return ret_val;
    }
  }
  

  // Unknown command length
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0F;
  buffer_out[2] = 0x04;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x01;
  HOST_TO_LE_16(buffer_out+5, op_code);
  return 7;
  
}


/**
* @brief  Transport Layer Init.
*	  Init the transport layer.
* @param  None
* @retval None
*/
void transport_layer_init(void)
{  
  LL_RCC_ClocksTypeDef system_clock;
  
  /* Configure SysTick to generate interrupt */
  LL_RCC_GetSystemClocksFreq(&system_clock);
  LL_SYSTICK_Config(system_clock.SYSCLK_Frequency/2);
  LL_SYSTICK_EnableIT();
  LL_SYSTICK_Disable();
  
#ifdef WATCHDOG  
  /* WDG configuration */
  WDG_Configuration();
#endif 
  
  /** GPIO configuration */
  GPIO_Configuration();
  
  /** NVIC configuration */
  NVIC_Configuration();  
  
#ifdef UART_INTERFACE
  /** UART configuration */
  UART_Configuration();
    
  /** Enable UART */
  UART_Cmd(ENABLE);
  
#ifndef NO_DMA
  DMA_Configuration();
#endif
#endif
  
#ifdef SPI_INTERFACE
  
  /** SPI Configuration */
  SPI_Slave_Configuration();
  
  /** Enable SPI interrupts */
  LL_EXTI_EnableIT(BSP_SPI_EXTI_CS_PIN);
  
  DMA_Configuration();
  SPI_STATE_TRANSACTION(SPI_PROT_CONFIGURED_STATE);
#endif

  /* Queue index init */
  fifo_init(&event_fifo, EVENT_BUFFER_SIZE, event_buffer, FIFO_ALIGNMENT);
  fifo_init(&command_fifo, COMMAND_BUFFER_SIZE, command_buffer, FIFO_ALIGNMENT);
  
  /* event_lost_register init */
  event_lost_register.event_lost = 0;
  event_lost_register.event_register[0] = 0x04;
  event_lost_register.event_register[1] = 0xFF;
  event_lost_register.event_register[2] = 0x0A;
  event_lost_register.event_register[3] = 0x02;
  event_lost_register.event_register[4] = 0x00;
  event_lost_register.event_lost_code = 0;
}



/**
* @brief  Send data via transport layer
* @param  None
* @retval Desired sleep level
*/
uint16_t command_fifo_dma_len;


#ifdef UART_INTERFACE
static void transport_layer_send_data(uint8_t *data, uint16_t data_length)
{
#ifdef NO_DMA
  for (uint16_t i = 0; i < data_length; i++) {
    LL_USART_TransmitData8(BSP_UART, data[i]);
    while(LL_USART_IsActiveFlag_TXE_TXFNF(BSP_UART) == RESET);
    while(LL_USART_IsActiveFlag_TC(BSP_UART) == RESET);
  }
  fifo_discard_var_len_item(&event_fifo);
#else
  if (dma_state == DMA_IDLE) {
    dma_state = DMA_IN_PROGRESS;
    DEBUG_NOTES(DMA_REARM);
    DMA_Rearm(DMA_CH_UART_TX, (uint32_t)data, data_length);
  }
#endif
  
}
#else
#ifdef SPI_INTERFACE

static void transport_layer_receive_data(void)
{  
  static uint8_t data[4];
  
  restore_flag = 0;  
  command_fifo_dma_len = (command_fifo.max_size - fifo_size(&command_fifo));
  
  data[0] = (uint8_t)command_fifo_dma_len;
  data[1] = (uint8_t)(command_fifo_dma_len>>8);
  data[2] = 0;
  data[3] = 0;
  
  buff_dma[0] = 0xFF;
  Osal_MemCpy(&buff_dma[1], data, SPI_HEADER_LEN);
    
  DEBUG_NOTES(RECEIVE_DATA);
  DMA_Rearm(DMA_CH_SPI_RX, (uint32_t)command_fifo_buffer_tmp, command_fifo_dma_len);
  DMA_Rearm(DMA_CH_SPI_TX, (uint32_t)buff_dma, SPI_HEADER_LEN+1);
  
}

static void transport_layer_send_data(uint8_t *data, uint16_t data_length)
{  
  restore_flag = 1;
  
  
  event_fifo_header_restore[0] = data[0];
  event_fifo_header_restore[1] = data[1];
  event_fifo_header_restore[2] = data[2];
  event_fifo_header_restore[3] = data[3];
  
  command_fifo_dma_len = (command_fifo.max_size - fifo_size(&command_fifo));
  
  data[0] = (uint8_t)command_fifo_dma_len;
  data[1] = (uint8_t)(command_fifo_dma_len>>8);
  data[2] = (uint8_t)data_length;
  data[3] = (uint8_t)(data_length>>8);
  
  buff_dma[0] = 0xFF;
  Osal_MemCpy(&buff_dma[1], data, data_length+SPI_HEADER_LEN);
  
  /* TODO: replace the 3 get data with a "fifo flush api" */
//#ifdef DMA_16           
  LL_SPI_ReceiveData16(BSP_SPI); // ISSUE_DMA_16
  LL_SPI_ReceiveData16(BSP_SPI);
//#endif
  DEBUG_NOTES(SEND_DATA);
  
  DMA_Rearm(DMA_CH_SPI_RX, (uint32_t)command_fifo_buffer_tmp, command_fifo_dma_len);
  DMA_Rearm(DMA_CH_SPI_TX, (uint32_t)buff_dma, data_length+SPI_HEADER_LEN+1);  
}

#endif
#endif

#ifdef UART_INTERFACE
#ifndef NO_DMA

static void transport_layer_DMA_RX_Data(void)
{
  static uint16_t nmb_bytes_free=DMA_RX_BUFFER_SIZE;
  static uint16_t dma_rx_buffer_index=0;
  volatile int16_t bytes_received;
  volatile uint16_t dma_rx_data_length_reg;
  
  dma_rx_data_length_reg = LL_DMA_GetDataLength(DMA1, DMA_CH_UART_RX);
  bytes_received = nmb_bytes_free - dma_rx_data_length_reg;
  if (bytes_received < 0) {
    hci_input(&DMA_RX_Buffer[dma_rx_buffer_index], nmb_bytes_free);
    dma_rx_buffer_index = (dma_rx_buffer_index+nmb_bytes_free)%DMA_RX_BUFFER_SIZE;
    bytes_received = DMA_RX_BUFFER_SIZE - dma_rx_data_length_reg;   
  } 
  if (bytes_received != 0) {
    hci_input(&DMA_RX_Buffer[dma_rx_buffer_index], bytes_received);
    dma_rx_buffer_index = (dma_rx_buffer_index+bytes_received)%DMA_RX_BUFFER_SIZE;      
  }
  nmb_bytes_free = dma_rx_data_length_reg;  
}

#endif
#endif

volatile uint32_t systick_counter = 0;
uint32_t systick_counter_prev = 0;

/**
* @brief  Advance transport layer state machine
* @param  None
* @retval Desired sleep level
*/
void transport_layer_tick(void)
{
  uint8_t buffer[COMMAND_BUFFER_SIZE], buffer_out[FIFO_VAR_LEN_ITEM_MAX_SIZE];
  uint16_t len;
  uint16_t size = 0;
  
#ifdef WATCHDOG
  /* Reloads IWDG counter with value defined in the reload register */
  LL_IWDG_ReloadCounter(IWDG);
#endif
  
  /* Check reset pending */
  if ((fifo_size(&event_fifo) == 0) && reset_pending) {
#ifdef UART_INTERFACE
    while(LL_USART_IsActiveFlag_TXE_TXFNF(BSP_UART) == RESET);
    while(LL_USART_IsActiveFlag_TC(BSP_UART) == RESET);
#endif
    NVIC_SystemReset();
  }
  
#ifdef SPI_INTERFACE
  if(SPI_STATE_CHECK(SPI_PROT_CONFIGURED_HOST_REQ_STATE)) {
    DEBUG_NOTES(PARSE_HOST_REQ);    
    transport_layer_receive_data();
//#ifndef DMA_16          
    LL_DMA_EnableChannel(DMA1, DMA_CH_SPI_RX); // ISSUE_DMA_16
    LL_DMA_EnableChannel(DMA1, DMA_CH_SPI_TX);
//#endif
    SPI_STATE_TRANSACTION(SPI_PROT_WAITING_HEADER_STATE);
    LL_GPIO_SetOutputPin(BSP_SPI_IRQ_GPIO_PORT, BSP_SPI_IRQ_PIN);       /* Issue the SPI communication request */
    DEBUG_NOTES(IRQ_RISE);
  }
  else
#endif
    
#ifdef UART_INTERFACE

#ifndef NO_DMA
    /* UART DMA Receive queue */
    transport_layer_DMA_RX_Data();
#endif
    
    /* Event queue */
    if ((fifo_size(&event_fifo) > 0) && (dma_state == DMA_IDLE)) {
      uint8_t *ptr;
      DEBUG_NOTES(SEND_DATA);
      if (fifo_get_ptr_var_len_item(&event_fifo, &size, &ptr) == 0) {
        transport_layer_send_data(ptr+FIFO_ALIGNMENT, size);
      }
    }
#endif
  
#ifdef SPI_INTERFACE
  /* Event queue */
  
    if ((fifo_size(&event_fifo) > 0) && (SPI_STATE_CHECK(SPI_PROT_CONFIGURED_STATE) || SPI_STATE_CHECK(SPI_PROT_SLEEP_STATE))) { //&& LL_GPIO_IsInputPinSet(SPIx_GPIO_PORT, SPIx_CS_PIN)) {
    uint8_t *ptr;
    if (fifo_get_ptr_var_len_item(&event_fifo, &size, &ptr) == 0) {
      SPI_STATE_TRANSACTION(SPI_PROT_CONFIGURED_EVENT_PEND_STATE);
      
      SPI_STATE_TRANSACTION(SPI_PROT_WAITING_HEADER_STATE);
      
      if (!header_timeout)
        transport_layer_send_data(ptr, size);
      header_timeout = 0;
      LL_GPIO_SetOutputPin(BSP_SPI_IRQ_GPIO_PORT, BSP_SPI_IRQ_PIN); /* Issue the SPI communication request */
      DEBUG_NOTES(IRQ_RISE);
    }
  }
  
  
  while(SPI_STATE_CHECK(SPI_PROT_WAITING_HEADER_STATE)) {
        volatile uint16_t tmp_spi_dma_len = LL_DMA_GetDataLength(DMA1, DMA_CH_SPI_RX);
#ifdef DMA_16
      if(tmp_spi_dma_len != 0) {
        tmp_spi_dma_len = command_fifo_dma_len - ( (tmp_spi_dma_len-1) << 1);
      }
      else {
        tmp_spi_dma_len = command_fifo_dma_len;
      }
#else
        tmp_spi_dma_len = (command_fifo_dma_len - tmp_spi_dma_len);
#endif
    if(tmp_spi_dma_len > 4) {
      SPI_STATE_TRANSACTION(SPI_PROT_WAITING_DATA_STATE);
      LL_GPIO_ResetOutputPin(BSP_SPI_IRQ_GPIO_PORT, BSP_SPI_IRQ_PIN); /* Issue the SPI communication request */
      DEBUG_NOTES(IRQ_FALL);
      break;
    }
    if(systick_counter_prev == 0) {
      LL_SYSTICK_Enable();
      systick_counter_prev = systick_counter;
    }
    /* wait 1 second */
    if((systick_counter-systick_counter_prev)>2) {
      LL_SYSTICK_Disable();
      systick_counter_prev = 0;
      header_timeout = 1;
      DEBUG_NOTES(HEADER_NOT_RECEIVED);
      
      SPI_STATE_TRANSACTION(SPI_PROT_TRANS_COMPLETE_STATE);
      LL_GPIO_ResetOutputPin(BSP_SPI_IRQ_GPIO_PORT, BSP_SPI_IRQ_PIN);    /* Issue the SPI communication request */
      DEBUG_NOTES(IRQ_FALL);
      
      if(restore_flag) {
        DEBUG_NOTES(ADVANCE_DMA_RESTORE);
        event_fifo.buffer[event_fifo.head] = event_fifo_header_restore[0];
        event_fifo.buffer[event_fifo.head+1] = event_fifo_header_restore[1];
        event_fifo.buffer[event_fifo.head+2] = event_fifo_header_restore[2];
        event_fifo.buffer[event_fifo.head+3] = event_fifo_header_restore[3];
      }
      SPI_STATE_TRANSACTION(SPI_PROT_CONFIGURED_STATE);  
      
      break;
    }
  }
#endif
  
  /* Command FIFO */
  if ((fifo_size(&command_fifo) > 0) && (!reset_pending)) {
    uint16_t opcode;
    uint8_t offset;
    
    fifo_get_var_len_item(&command_fifo, &size, buffer);
    /*  */
    if(buffer[0] == HCI_COMMAND_PKT){
      hci_cmd_hdr *hdr = (hci_cmd_hdr *)buffer;
      opcode = hdr->opcode;
      offset = sizeof(hci_cmd_hdr);
    }
    else if(buffer[0] == HCI_COMMAND_EXT_PKT){
      hci_cmd_ext_hdr *hdr = (hci_cmd_ext_hdr *)buffer;
      opcode = hdr->opcode;
      offset = sizeof(hci_cmd_ext_hdr);
    }
    else {
      /* Unknown packet type */
      fifo_flush(&command_fifo);
      return;
    }
    len=process_command(opcode, buffer+offset, size-offset, buffer_out, sizeof(buffer_out));
#if (BUFFER_CMDS_ON_BUSY == 1)
    uint8_t status_offset = (buffer_out[1] == 0x0E) ? 6 : 3; /* 0x0E: command complete, 0x0F: command status */
    /* Apply command buffering in case of CONTROLLER BUSY error with the exception of the 
     * aci_l2cap_connection_parameter_update_resp command (see  req_pbs #990070)
     */
    if ((*(buffer_out+status_offset) != BLE_ERROR_CONTROLLER_BUSY) || (opcode == 0xfd82))
    {
      DEBUG_NOTES(COMMAND_PROCESSED);
      /* Set user events back to normal queue */
      send_event(buffer_out, len, 1);
      fifo_flush(&command_fifo);
    }
    else
    {
        fifo_roll_back(&command_fifo, size); 
    }
#else
    DEBUG_NOTES(COMMAND_PROCESSED);
    /* Set user events back to normal queue */
    send_event(buffer_out, len, 1);
    fifo_flush(&command_fifo);
#endif 
  }
  
  if(event_lost_register.event_lost==1) {
    if (fifo_put_var_len_item(&event_fifo, 13, event_lost_register.event_register,0,NULL) == 0) {
      event_lost_register.event_lost = 0;
      event_lost_register.event_lost_code = 0;
    }
  }
}

void send_command(uint8_t *cmd, uint16_t len)
{
  fifo_put_var_len_item(&command_fifo, len, cmd, 0, NULL);
}

void enqueue_event(circular_fifo_t *fifo, uint16_t buff_len1, uint8_t *buff_evt1, uint16_t buff_len2, uint8_t *buff_evt2, int8_t overflow_index)
{
  if (fifo_put_var_len_item(fifo, buff_len1, buff_evt1, buff_len2, buff_evt2) != 0) {
    // Event queue overflow!!! TBD
    if ((overflow_index >=0) && (overflow_index < 64)) {
      event_lost_register.event_lost = 1;
      event_lost_register.event_lost_code |= (1 << overflow_index);
    } else {
      // assert 
    }
  }
}

void send_event(uint8_t *buffer_out, uint16_t buffer_out_length, int8_t overflow_index)
{
  if(buffer_out_length != 0) {
    DEBUG_NOTES(ENQUEUE_EVENT);
    enqueue_event(&event_fifo, buffer_out_length, buffer_out, 0, NULL, overflow_index);
  }
}

void send_event_2buffers(uint8_t *buffer_out1, uint16_t buffer_out_length1, uint8_t *buffer_out2, uint16_t buffer_out_length2, int8_t overflow_index)
{
  if(buffer_out_length1 != 0) {
    DEBUG_NOTES(ENQUEUE_EVENT);
    enqueue_event(&event_fifo, buffer_out_length1, buffer_out1, buffer_out_length2, buffer_out2, overflow_index);
  }
}

#ifdef SPI_INTERFACE

#ifdef DMA_16
uint16_t real_packet_len(uint8_t *buff, uint16_t len)
{
  uint16_t nmb_bytes = len;

  switch (buff[0]) {
  case HCI_COMMAND_PKT:
    nmb_bytes = 4 + buff[3];
    break;
  case HCI_COMMAND_EXT_PKT:
    nmb_bytes = 5 + LE_TO_HOST_16(buff+3);
    break;
  case HCI_ACLDATA_PKT:
    nmb_bytes = 5 + ( ((uint16_t)buff[3]) | (((uint16_t)buff[4]) <<8));
    break;
  case HCI_VENDOR_PKT:
    nmb_bytes = 4 + (((uint16_t)buff[2]) | (((uint16_t)buff[3])<<8));
    break;
  default:
    /* Incorrect type. Reset state machine. */
    nmb_bytes = len;
  }
  
  return nmb_bytes;
}
#endif

void advance_spi_dma(uint16_t rx_buffer_len)
{
  uint8_t spi_command;
  
  if(restore_flag) {
    DEBUG_NOTES(ADVANCE_DMA_RESTORE);
    event_fifo.buffer[event_fifo.head] = event_fifo_header_restore[0];
    event_fifo.buffer[event_fifo.head+1] = event_fifo_header_restore[1];
    event_fifo.buffer[event_fifo.head+2] = event_fifo_header_restore[2];
    event_fifo.buffer[event_fifo.head+3] = event_fifo_header_restore[3];
  }
  if(rx_buffer_len>5) {
    /* get ctrl field from command buffer */  
    spi_command = command_fifo_buffer_tmp[0];
    
    if(spi_command == SPI_CTRL_WRITE) {
      DEBUG_NOTES(ADVANCE_DMA_WRITE);
#ifdef DMA_16
      /* If the packet has an ODD number of bytes we need to read another byte */
      uint16_t real_len;
      real_len = real_packet_len(&command_fifo_buffer_tmp[5], rx_buffer_len);
//      if (real_len != (rx_buffer_len-5)) {
//        command_fifo_buffer_tmp[rx_buffer_len] = LL_SPI_ReceiveData8(BSP_SPI);
//        rx_buffer_len++;
//      }
#else
      uint16_t real_len = rx_buffer_len - 5;
#endif
      hci_input(&command_fifo_buffer_tmp[5], real_len);
      command_fifo_buffer_tmp[5] = 0;
      command_fifo_buffer_tmp[0] = 0;
    }
    else if(spi_command == SPI_CTRL_READ) {
      DEBUG_NOTES(ADVANCE_DMA_READ);
      fifo_discard_var_len_item(&event_fifo);
      DEBUG_NOTES(ADVANCE_DMA_DISCARD);
    }
  }
  SPI_STATE_TRANSACTION(SPI_PROT_CONFIGURED_STATE);  
}

#endif

#ifdef UART_INTERFACE
void advance_dma(void)
{
  uint8_t *ptr;
  uint16_t size;
  fifo_discard_var_len_item(&event_fifo);
  
  if (fifo_size(&event_fifo) > 0) {
    if (fifo_get_ptr_var_len_item(&event_fifo, &size, &ptr) == 0) {
      transport_layer_send_data(ptr+FIFO_ALIGNMENT, size);
    }
  }
}
#endif
