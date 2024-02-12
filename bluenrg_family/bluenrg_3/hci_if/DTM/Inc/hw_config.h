/**
  ******************************************************************************
  * @file    hw_config.h 
  * @author  VMA RF Application Team
  * @version V1.0.0
  * @date    July-2015
  * @brief   
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
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HW_CONFIG_H
#define HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "rf_driver_hal_power_manager.h"
#include "rf_driver_ll_cortex.h"
#include "rf_driver_ll_rcc.h"
#include "rf_driver_ll_iwdg.h"
#include "rf_driver_ll_usart.h"
#include "rf_driver_ll_bus.h"
#include "rf_driver_ll_gpio.h"
#include "rf_driver_ll_pwr.h"
#include "rf_driver_ll_dma.h"
#include "rf_driver_ll_flash.h"
#include "rf_driver_ll_spi.h"
#include "rf_driver_ll_exti.h"

#include "bluenrg_lp_evb_config.h"

/* Exported defines ------------------------------------------------------------*/
/* USART DMA Channel */
#define DMA_CH_UART_TX          LL_DMA_CHANNEL_1
#define DMA_CH_UART_RX          LL_DMA_CHANNEL_2

/* SPI DMA channels */
#define DMA_CH_SPI_TX           LL_DMA_CHANNEL_3
#define DMA_CH_SPI_RX           LL_DMA_CHANNEL_1

#define EXTI_CLK_ENABLE()       LL_APB0_EnableClock(LL_APB0_PERIPH_SYSCFG)

#ifdef UART_INTERFACE
#define IO_WAKEUP_PIN   0
#endif
#ifdef SPI_INTERFACE
#define IO_WAKEUP_PIN   BSP_SPI_WAKEUP_PIN
#endif

#ifdef DEBUG_DTM
#define DEBUG_GPIO_CLK_ENABLE()      LL_AHB_EnableClock(LL_AHB_PERIPH_GPIOB)   /* Enable the peripheral clock of GPIOB */
#define DEBUG_GPIO_PORT              GPIOB
#define DEBUG_TEST_1_PIN             LL_GPIO_PIN_4
#define DEBUG_TEST_2_PIN             LL_GPIO_PIN_7
#endif

/* Exported constants --------------------------------------------------------*/
#ifndef NO_DMA

#define DMA_RX_BUFFER_SIZE 1024

extern uint8_t DMA_RX_Buffer[DMA_RX_BUFFER_SIZE];

#endif
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef WATCHDOG
void WDG_Configuration(void);
#endif

void NVIC_Configuration(void);
void GPIO_Configuration(void);

#ifdef UART_INTERFACE
void UART_Configuration(void);
void UART_Cmd(FunctionalState state);
#endif

#ifdef SPI_INTERFACE
void SPI_Slave_Configuration(void);
void SPI_FIFO_Flush(void);
#endif

#ifndef NO_DMA
void DMA_Configuration(void);
void DMA_Rearm(uint32_t dma_channel, uint32_t buffer, uint32_t size);
uint8_t DMAUart_SendData(uint32_t address, uint8_t size);
#endif

void NVIC_DisableRadioIrq(void);
void NVIC_EnableRadioIrq(void);

#endif /* HW_CONFIG_H */

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
