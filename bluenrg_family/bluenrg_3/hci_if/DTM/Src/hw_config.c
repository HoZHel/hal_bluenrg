/**
******************************************************************************
* @file    UART/Interrupt/main.c
* @author  VMA RF Application Team
* @version V1.1.0
* @date    27-March-2018
* @brief   HW configuration for the GUI application
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
#include "rf_device_it.h"
#include "rf_driver_ll_system.h"
#include "hw_config.h"

/** @addtogroup BlueNRG_LP_StdPeriph_Examples
* @{
*/

/** @addtogroup UART Interrupt Example
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
#ifndef NO_DMA
uint8_t DMA_RX_Buffer[DMA_RX_BUFFER_SIZE];
#endif
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
* @brief  WDG configuration routine
* @param  None
* @retval None
*/
void WDG_Configuration(void)
{
#ifdef WATCHDOG 
  uint32_t dev_cut_version;
  
  /* BlueNRG-LP device cut version used */
  dev_cut_version = (LL_SYSCFG_GetDeviceVersion()<<4)|LL_SYSCFG_GetDeviceRevision(); 

  /* Enable watchdog clocks  */
  LL_APB0_EnableClock(LL_APB0_PERIPH_WDG);
  
  /* The reset release is effective only 2 slow clock periods after the APB writing */
  if (dev_cut_version == LL_BLUENRG_LP_CUT_10) {
    /* Delay of 2 32KHz clock cycle needed for the IWDG Kernel reset */
    for (volatile int i=0; i<450; i++)
      __asm("NOP");
  } else {
    LL_APB0_ForceReset(LL_APB0_PERIPH_WDG);
    LL_APB0_ReleaseReset(LL_APB0_PERIPH_WDG);
    while(LL_RCC_IsActiveFlag_WDGRSTREL() == 0);
  }
  
  /* Enable IWDG */
  LL_IWDG_Enable(IWDG);
  
  /* Enable Write Access */
  LL_IWDG_EnableWriteAccess(IWDG);
  
  /* Setup the whatchdog prescaler */
  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_32);
  
  /* Set watchdog reload period (granularity second) */
  LL_IWDG_SetReloadCounter(IWDG, (WATCHDOG_TIME*1000)); 
  
  /* Disable Write Access */
  LL_IWDG_DisableWriteAccess(IWDG);
  
  /* Wait until the IWDG is configured */
  while (!LL_IWDG_IsReady(IWDG));
  
  /* Reload IWDG counter with value defined in the reload register */
  LL_IWDG_ReloadCounter(IWDG);
#endif
}

/**
* @brief  Configures the nested vectored interrupt controller.
* @param  None
* @retval None
*/
void NVIC_Configuration(void)
{
#ifdef UART_INTERFACE 
#ifdef NO_DMA 
  
  /* Enable the UART Interrupt */ 
  NVIC_SetPriority(BSP_UART_IRQn, LL_NVIC_MED_PRIORITY);
  NVIC_EnableIRQ(BSP_UART_IRQn);
  
#endif  
#endif
  
#ifdef SPI_INTERFACE

  /* Configures SPI CS EXTI line */
  LL_EXTI_InitTypeDef EXTI_InitStruct;
    
  EXTI_CLK_ENABLE();
  
  LL_EXTI_StructInit(&EXTI_InitStruct);
  
  EXTI_InitStruct.Line           = BSP_SPI_EXTI_CS_PIN;
  EXTI_InitStruct.LineCommand    = ENABLE;
  EXTI_InitStruct.Type           = LL_EXTI_TYPE_EDGE;
  EXTI_InitStruct.Trigger        = LL_EXTI_TRIGGER_FALLING_EDGE;
  LL_EXTI_Init(&EXTI_InitStruct);
  
  /* Clear pending interrupt */
  LL_EXTI_ClearInterrupt(BSP_SPI_EXTI_CS_PIN);
  
  /* Enable the SPI CS Interrupt */
  NVIC_SetPriority(BSP_SPI_EXTI_CS_IRQn, LL_NVIC_MED_PRIORITY);
  NVIC_EnableIRQ(BSP_SPI_EXTI_CS_IRQn);
  
#endif
  
  /* Enable the Blue Controller Interrupt */
  NVIC_EnableRadioIrq();
}

void NVIC_DisableRadioIrq(void)
{
  /* Disable the Blue Controller Interrupt */  
  NVIC_DisableIRQ(BLE_TX_RX_IRQn);
}

void NVIC_EnableRadioIrq(void)
{
  /* Enable the Blue Controller Interrupt */
  NVIC_SetPriority(BLE_TX_RX_IRQn, LL_NVIC_CRITICAL_PRIORITY);
  NVIC_EnableIRQ(BLE_TX_RX_IRQn);
}

/**
* @brief  GPIO Configuration.
*	  Configure outputs GPIO pins.
* @param  None
* @retval None
*/
void GPIO_Configuration(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct;  
  
#ifdef UART_INTERFACE 
  
  /* Enable the peripheral clock of GPIO Port */
  BSP_UART_GPIO_CLOCK_ENABLE();

  /* Init Structure */
  LL_GPIO_StructInit(&GPIO_InitStruct);
  
  /* Configure Tx/Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
  GPIO_InitStruct.Pin = BSP_UART_TX_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = BSP_UART_TX_GPIO_AF_N;
  
  LL_GPIO_Init(BSP_UART_TX_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = BSP_UART_RX_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = BSP_UART_RX_GPIO_AF_N;
  
  LL_GPIO_Init(BSP_UART_RX_GPIO_PORT, &GPIO_InitStruct);

#ifdef UART_FLOW_CONTROL_HW

  GPIO_InitStruct.Pin = BSP_UART_RTS_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = BSP_UART_RTS_GPIO_AF_N;

  LL_GPIO_Init(BSP_UART_RTS_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BSP_UART_CTS_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  GPIO_InitStruct.Alternate = BSP_UART_CTS_GPIO_AF_N;

  LL_GPIO_Init(BSP_UART_CTS_GPIO_PORT, &GPIO_InitStruct);

#endif /* UART_FLOW_CONTROL_HW */

#endif
  
#ifdef SPI_INTERFACE
  /* Enable the peripheral clock of GPIO Port */
  BSP_SPI_GPIO_CLOCK_ENABLE();
  
  /* Init Structure */
  LL_GPIO_StructInit(&GPIO_InitStruct);
  
  /** Configure pins for SPI. */
  GPIO_InitStruct.Pin = BSP_SPI_SCK_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = BSP_SPI_SCK_GPIO_AF_N;
  LL_GPIO_Init(BSP_SPI_SCK_GPIO_PORT, &GPIO_InitStruct);  
  
  GPIO_InitStruct.Pin = BSP_SPI_MOSI_PIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  GPIO_InitStruct.Alternate = BSP_SPI_MOSI_GPIO_AF_N;
  LL_GPIO_Init(BSP_SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = BSP_SPI_MISO_PIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  GPIO_InitStruct.Alternate = BSP_SPI_MISO_GPIO_AF_N;
  LL_GPIO_Init(BSP_SPI_MISO_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = BSP_SPI_CS_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = BSP_SPI_CS_GPIO_AF_N;
  LL_GPIO_Init(BSP_SPI_CS_GPIO_PORT, &GPIO_InitStruct); 
  
  GPIO_InitStruct.Pin = BSP_SPI_IRQ_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  LL_GPIO_Init(BSP_SPI_IRQ_GPIO_PORT, &GPIO_InitStruct); 

#endif
  
#ifdef DEBUG_DTM
  
  DEBUG_GPIO_CLK_ENABLE();
  
  /* Init Structure */
  LL_GPIO_StructInit(&GPIO_InitStruct);

  GPIO_InitStruct.Pin = DEBUG_TEST_1_PIN | DEBUG_TEST_2_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(DEBUG_GPIO_PORT, &GPIO_InitStruct);

#endif
  
}

#ifdef UART_INTERFACE
/**
* @brief  UART Configuration.
*	  Configure UART peripheral.
* @param  None
* @retval None
*/
void UART_Configuration(void)
{
  LL_USART_InitTypeDef USART_InitStruct;
  
  /** Enable UART clock */
  LL_APB1_EnableClock(LL_APB1_PERIPH_USART);
  
  /** Init Structure */
  LL_USART_StructInit(&USART_InitStruct);
  
  /** Configure UART */
  USART_InitStruct.PrescalerValue      = LL_USART_PRESCALER_DIV1;
  USART_InitStruct.BaudRate            = UART_BAUDRATE;
  USART_InitStruct.DataWidth           = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits            = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity              = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection   = LL_USART_DIRECTION_TX_RX;
#ifdef UART_FLOW_CONTROL_HW
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_RTS_CTS;
#else
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
#endif
  USART_InitStruct.OverSampling        = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(BSP_UART, &USART_InitStruct);
    
  /* Enable the RX FIFO and setup the trigger points for receive FIFO interrupt to every byte */
  LL_USART_ConfigFIFOsThreshold(BSP_UART, LL_USART_FIFOTHRESHOLD_1_8, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_EnableFIFO(BSP_UART);
#ifdef NO_DMA
  LL_USART_EnableIT_RXNE_RXFNE(BSP_UART);
#endif
}

void UART_Cmd(FunctionalState state)
{
  if (state != DISABLE) {
    LL_USART_Enable(BSP_UART);
  }
  else {
    LL_USART_Disable(BSP_UART);
  }
}
#endif

#ifdef SPI_INTERFACE
/**
* @brief  SPI configuration.
* @param  None
* @retval None
*/
void SPI_Slave_Configuration(void)
{
  LL_SPI_InitTypeDef SPI_InitStruct;
  
  /* Enable SPI and GPIO clocks */
  BSP_SPI_CLK_ENABLE();
  
  /* Configure SPI in Slave mode */
  LL_SPI_StructInit(&SPI_InitStruct);
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_SLAVE;
#ifdef DMA_16
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_16BIT;
#else
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
#endif
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_HARD_INPUT;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 7;
  LL_SPI_Init(BSP_SPI, &SPI_InitStruct);
  LL_SPI_SetStandard(BSP_SPI, LL_SPI_PROTOCOL_MOTOROLA);
  LL_SPI_DisableNSSPulseMgt(BSP_SPI);
  
  LL_SPI_SetNSSMode(BSP_SPI, LL_SPI_NSS_HARD_INPUT);
#ifdef DMA_16
  LL_SPI_SetRxFIFOThreshold(BSP_SPI, LL_SPI_RX_FIFO_TH_HALF);
#else
  LL_SPI_SetRxFIFOThreshold(BSP_SPI, LL_SPI_RX_FIFO_TH_QUARTER);
#endif
  
  LL_SPI_Enable(BSP_SPI);
  
}

extern uint16_t command_fifo_dma_len;
void SPI_FIFO_Flush(void)
{
  LL_SPI_DeInit(BSP_SPI);
 
#ifndef NO_DMA  
  LL_AHB_ForceReset(LL_AHB_PERIPH_DMA);
  LL_AHB_ReleaseReset(LL_AHB_PERIPH_DMA);
#endif
  
  SPI_Slave_Configuration();
#ifndef NO_DMA 
  DMA_Configuration();
#endif
  
  command_fifo_dma_len = 0;
}

#endif

#ifndef NO_DMA
/**
* @brief  DMA configuration.
* @param  None
* @retval None
*/
void DMA_Configuration(void)
{  
  LL_DMA_InitTypeDef DMA_InitStruct;
  
  /* Enable DMA peripheral clock */
  LL_AHB_EnableClock(LL_AHB_PERIPH_DMA);
  
  /* Configure DMA TX channel */
  LL_DMA_StructInit(&DMA_InitStruct);
  
#ifdef SPI_INTERFACE
  /* Configure DMA SPI Tx channel */
  DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
  DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
#ifdef DMA_16
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD; 
  DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
#else
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
  DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
#endif
  DMA_InitStruct.PeriphRequest = BSP_SPI_TX_DMA_REQ;
  DMA_InitStruct.Priority = LL_DMA_PRIORITY_MEDIUM;
  LL_DMA_Init(DMA1, DMA_CH_SPI_TX, &DMA_InitStruct);

  /* Configure DMA SPI Rx channel */
  DMA_InitStruct.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
  DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
#ifdef DMA_16
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD; 
  DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD; 
#else
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
  DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
#endif
  DMA_InitStruct.PeriphRequest = BSP_SPI_RX_DMA_REQ;
  DMA_InitStruct.Priority = LL_DMA_PRIORITY_HIGH;
  LL_DMA_Init(DMA1, DMA_CH_SPI_RX, &DMA_InitStruct);

  /* Enable DMAReq_Tx/Rx */
  LL_SPI_EnableDMAReq_TX(BSP_SPI);
  LL_SPI_EnableDMAReq_RX(BSP_SPI);
#endif
  
#ifdef UART_INTERFACE
  
  /* USART TX DMA configuration */
  DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
  DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
  DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
  DMA_InitStruct.PeriphRequest = LL_DMAMUX_REQ_USART1_TX;
  DMA_InitStruct.Priority = LL_DMA_PRIORITY_HIGH;
  LL_DMA_Init(DMA1, DMA_CH_UART_TX, &DMA_InitStruct);

  /* Enable DMA_CH_UART_TX Transfer Complete interrupt */
  LL_DMA_EnableIT_TC(DMA1, DMA_CH_UART_TX);
    
  /* Enable UART_DMAReq_Tx */
  LL_USART_EnableDMAReq_TX(BSP_UART);
  
  /* Enable the DMA Interrupt */
  NVIC_SetPriority(DMA_IRQn, LL_NVIC_HIGH_PRIORITY);
  NVIC_EnableIRQ(DMA_IRQn);
  
  /* USART RX DMA configuration */
  DMA_InitStruct.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  DMA_InitStruct.Mode = LL_DMA_MODE_CIRCULAR;
  DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
  DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
  DMA_InitStruct.PeriphRequest = LL_DMAMUX_REQ_USART1_RX;
  DMA_InitStruct.Priority = LL_DMA_PRIORITY_HIGH;
  LL_DMA_Init(DMA1, DMA_CH_UART_RX, &DMA_InitStruct);
    
  /* Enable UART_DMAReq_Rx */
  LL_USART_EnableDMAReq_RX(BSP_UART);
  
  /* Set DMA Max Data length for circular buffer */
  LL_DMA_SetDataLength(DMA1, DMA_CH_UART_RX, DMA_RX_BUFFER_SIZE);
  
  /* Configure the DMA RX Addresses */
  LL_DMA_ConfigAddresses(DMA1, DMA_CH_UART_RX, LL_USART_DMA_GetRegAddr(BSP_UART, LL_USART_DMA_REG_DATA_RECEIVE), (uint32_t)&DMA_RX_Buffer, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  /* Enable DMA USART RX Channel */
  LL_DMA_EnableChannel(DMA1, DMA_CH_UART_RX);
  
#endif
}

void DMA_Rearm(uint32_t dma_channel, uint32_t buffer, uint32_t size)
{
  /* Disable DMA Channel */
  LL_DMA_DisableChannel(DMA1, dma_channel);
  
#ifdef UART_INTERFACE    
  /* Rearm the DMA transfer */
  LL_DMA_ConfigAddresses(DMA1, dma_channel, buffer, LL_USART_DMA_GetRegAddr(BSP_UART, LL_USART_DMA_REG_DATA_TRANSMIT), LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
#endif

#ifdef SPI_INTERFACE 

#ifdef DMA_16
  LL_SPI_Disable(BSP_SPI);
  if ((size & 0x01) == 0) {
    size = (size >> 1);
    if (dma_channel == DMA_CH_SPI_TX) {
      LL_SPI_SetDMAParity_TX(BSP_SPI, LL_SPI_DMA_PARITY_EVEN);
    } else {
      LL_SPI_SetDMAParity_RX(BSP_SPI, LL_SPI_DMA_PARITY_EVEN);
    }
  } else {
    size = (size >> 1) + 1;
    if (dma_channel == DMA_CH_SPI_TX) {
      LL_SPI_SetDMAParity_TX(BSP_SPI, LL_SPI_DMA_PARITY_ODD);
    } else {
      LL_SPI_SetDMAParity_RX(BSP_SPI, LL_SPI_DMA_PARITY_ODD);
    }
  }
  LL_SPI_Enable(BSP_SPI);
#endif
  
  /* TODO: replace the 3 get data with a "fifo flush api" */
//#ifdef DMA_16
  LL_SPI_ReceiveData16(BSP_SPI);  // ISSUE_DMA_16
  LL_SPI_ReceiveData16(BSP_SPI);
//#endif
  
  /* Rearm the DMA transfer */
  if (dma_channel == DMA_CH_SPI_TX) {
    LL_DMA_ConfigAddresses(DMA1, dma_channel, buffer, LL_SPI_DMA_GetRegAddr(BSP_SPI), LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  }
  else {
    LL_DMA_ConfigAddresses(DMA1, dma_channel, LL_SPI_DMA_GetRegAddr(BSP_SPI), buffer, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
  }
#endif
  
  LL_DMA_SetDataLength(DMA1, dma_channel, size);
  
#ifdef UART_INTERFACE
  /* Enable DMA Channel */
  LL_DMA_EnableChannel(DMA1, dma_channel);
#endif
  
//#ifdef DMA_16
  /* Enable DMA Channel */
  LL_DMA_EnableChannel(DMA1, dma_channel);  // ISSUE_DMA_16
//#endif
}

#endif
