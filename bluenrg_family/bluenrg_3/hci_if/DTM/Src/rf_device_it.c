/**
******************************************************************************
* @file    rf_device_it.c
* @author  AMS RF Application Team
* @version V1.0.0
* @date    27-March-2019
* @brief   Main Interrupt Service Routines.
*          This file provides template for all exceptions handler and
*          peripherals interrupt service routine.
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

/* Includes ------------------------------------------------------------------*/
#include "rf_device_it.h"
#include "ble_const.h"
#include "bluenrg_lp_stack.h"
#include "hci_parser.h"
#include "hw_config.h"
#include "transport_layer.h"
#include "hal_miscutil.h" 
#include "rf_driver_hal_vtimer.h"

/** @addtogroup BlueNRG_LP_StdPeriph_Examples
* @{
*/

/** @addtogroup GPIO_Examples
* @{
*/ 

/** @addtogroup GPIO_IOToggle
* @{
*/ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  */
NOSTACK_FUNCTION(NORETURN_FUNCTION(void NMI_IRQHandler(void)))
{
  HAL_CrashHandler(__get_MSP(), NMI_SIGNATURE);  
  /* Go to infinite loop when NMI exception occurs */
  while (1)
  {}
}

/**
* @brief  This function handles Hard Fault exception.
*/
NOSTACK_FUNCTION(NORETURN_FUNCTION(void HardFault_IRQHandler(void)))
{
  HAL_CrashHandler(__get_MSP(), HARD_FAULT_SIGNATURE);  
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles SVCall exception.
  */
void SVC_IRQHandler(void)
{
}


/**
* @brief  This function handles SysTick Handler.
*/
extern volatile uint32_t systick_counter;
void SysTick_IRQHandler(void)
{
  systick_counter++;
}


/******************************************************************************/
/*                 BlueNRG-LP Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (system_bluenrg_lp.c).                                               */
/******************************************************************************/
/**
* @brief  This function handles UART interrupt request.
* @param  None
* @retval None
*/
uint8_t command_in_progress = 0;
static volatile uint32_t dbg_overrun_error_flag_usart = 0;
#ifdef UART_INTERFACE
void USART1_IRQHandler(void)
{
#ifdef NO_DMA  
  uint8_t data;
  
  while (LL_USART_IsActiveFlag_RXNE_RXFNE(BSP_UART))
  {
    /* The Interrupt flag is cleared from the FIFO manager */
    data = LL_USART_ReceiveData8(BSP_UART);
    hci_input(&data, 1);
    
    if(LL_USART_IsActiveFlag_ORE(BSP_UART)) {
      dbg_overrun_error_flag_usart++;
      LL_USART_RequestRxDataFlush(BSP_UART);
      LL_USART_ClearFlag_ORE(BSP_UART);
    }
  }
#endif
}

#ifndef NO_DMA
/**
* @brief  This function handles DMA Handler.
*/
void DMA_IRQHandler(void)
{
  /* Check DMA_CH_UART_TX Transfer Complete interrupt */
  if (LL_DMA_IsActiveFlag_TC1(DMA1)) {
    LL_DMA_ClearFlag_TC1(DMA1);
    
    /* DMA1 finished the transfer of SrcBuffer */
    dma_state = DMA_IDLE;
    
    /* DMA_CH disable */
    LL_DMA_DisableChannel(DMA1, DMA_CH_UART_TX);
    
    DEBUG_NOTES(DMA_TC);
    
    advance_dma();
  }
}
#endif

#endif

/**
* @brief  This function handles GPIO interrupt request.
* @param  None
* @retval None
*/
#ifdef SPI_INTERFACE
extern uint16_t command_fifo_dma_len;
extern uint32_t systick_counter_prev;
void GPIOA_IRQHandler(void)
{
  if(LL_EXTI_IsInterruptPending(BSP_SPI_EXTI_CS_PIN) == SET) 
  {
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
    if(LL_EXTI_GetType(BSP_SPI_EXTI_CS_PIN) == LL_EXTI_TYPE_LEVEL) {     /* if level sensitive */
      LL_EXTI_SetType(LL_EXTI_TYPE_EDGE, BSP_SPI_EXTI_CS_PIN);           /* EDGE sensitive */
      DEBUG_NOTES(EDGE_SENSITIVE);
    }
    LL_EXTI_ClearInterrupt(BSP_SPI_EXTI_CS_PIN);
    
    /* CS pin rising edge - close SPI communication */
    if(LL_EXTI_GetTrigger(BSP_SPI_EXTI_CS_PIN) == LL_EXTI_TRIGGER_RISING_EDGE && LL_GPIO_IsInputPinSet(BSP_SPI_CS_GPIO_PORT, BSP_SPI_CS_PIN) == 1) {
      LL_GPIO_ResetOutputPin(BSP_SPI_IRQ_GPIO_PORT, BSP_SPI_IRQ_PIN);
      DEBUG_NOTES(IRQ_FALL);

      DEBUG_NOTES(GPIO_CS_RISING);
      LL_EXTI_SetTrigger(LL_EXTI_TRIGGER_FALLING_EDGE, BSP_SPI_EXTI_CS_PIN);
      
      if(SPI_STATE_FROM(SPI_PROT_CONFIGURED_EVENT_PEND_STATE)) {
        systick_counter_prev = 0;
        LL_SYSTICK_Disable();
        SPI_STATE_TRANSACTION(SPI_PROT_TRANS_COMPLETE_STATE);
        
        /* Pass the number of data received in fifo_command */
        uint16_t tmp_spi_dma_len;
#ifdef DMA_16
        tmp_spi_dma_len = command_fifo_dma_len - ((LL_DMA_GetDataLength(DMA1, DMA_CH_SPI_RX)) * 2 );
#else
        tmp_spi_dma_len = (command_fifo_dma_len - LL_DMA_GetDataLength(DMA1, DMA_CH_SPI_RX));
#endif
        advance_spi_dma(tmp_spi_dma_len);
        
        if(LL_SPI_GetTxFIFOLevel(BSP_SPI) != LL_SPI_TX_FIFO_EMPTY)
        {
          SPI_FIFO_Flush();
          DEBUG_NOTES(TXFIFO_NE);
        }
      }
    }
    /* CS pin falling edge - start SPI communication */
    else if( LL_GPIO_IsInputPinSet(BSP_SPI_CS_GPIO_PORT, BSP_SPI_CS_PIN) == 0) {
      DEBUG_NOTES(GPIO_CS_FALLING);
      LL_EXTI_SetTrigger(LL_EXTI_TRIGGER_RISING_EDGE, BSP_SPI_EXTI_CS_PIN);
      
      if(SPI_STATE_CHECK(SPI_PROT_CONFIGURED_EVENT_PEND_STATE)) {
        SPI_STATE_TRANSACTION(SPI_PROT_WAITING_HEADER_STATE);
      }
      else if(SPI_STATE_CHECK(SPI_PROT_SLEEP_STATE) || SPI_STATE_CHECK(SPI_PROT_CONFIGURED_STATE)) {
//        SPI_ClearTXFIFO();
//        LL_SPI_TransmitData8(SPIx_INSTANCE, 0xFF);
        SPI_STATE_TRANSACTION(SPI_PROT_CONFIGURED_HOST_REQ_STATE);
      }
//#ifndef DMA_16
//      if(SPI_STATE_CHECK(SPI_PROT_WAITING_HEADER_STATE)) {  // ISSUE_DMA_16
//        LL_DMA_EnableChannel(DMA1, DMA_CH_SPI_RX);
//        LL_DMA_EnableChannel(DMA1, DMA_CH_SPI_TX);
//      }
//#endif
    }
    
  }
  
}
#endif

/* irq_count used for the aci_hal_transmitter_test_packets_process() command implementation */
uint32_t irq_count = 1;
uint16_t num_packets = 0;

void BLE_WKUP_IRQHandler(void)
{
  HAL_VTIMER_WakeUpCallback();
}

void CPU_WKUP_IRQHandler(void) 
{
  HAL_VTIMER_TimeoutCallback();
}

void BLE_ERROR_IRQHandler(void)
{
  volatile uint32_t debug_cmd;
  
  BLUE->DEBUGCMDREG |= 1;

  /* If the device is configured with 
     System clock = 64 MHz and BLE clock = 16 MHz
     a register read is necessary to end fine  
     the clear interrupt register operation,
     due the AHB down converter latency */ 
  debug_cmd = BLUE->DEBUGCMDREG;
}

void BLE_TX_RX_IRQHandler(void)
{
  uint32_t blue_status = BLUE->STATUSREG;
  uint32_t blue_interrupt = BLUE->INTERRUPT1REG;
  
  /** clear all pending interrupts */
  BLUE->INTERRUPT1REG = blue_interrupt;

  HAL_VTIMER_EndOfRadioActivityIsr();
  BLE_STACK_RadioHandler(blue_status|blue_interrupt);
  HAL_VTIMER_RadioTimerIsr();
    
  if(irq_count != num_packets)
  {
    irq_count++;
  }

  /* If the device is configured with 
     System clock = 64 MHz and BLE clock = 16 MHz
     a register read is necessary to end fine  
     the clear interrupt register operation,
     due the AHB down converter latency */ 
  blue_interrupt = BLUE->INTERRUPT1REG;
}

void BLE_RXTX_SEQ_IRQHandler(void)
{
  HAL_RXTX_SEQ_IRQHandler();
}

/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/ 

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
