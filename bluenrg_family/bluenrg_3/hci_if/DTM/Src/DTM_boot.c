/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
* File Name          : DTM_boot.c
* Author             : AMS - VMA RF Application Team
* Version            : V1.0.0
* Date               : 25-December-2018
* Description        : BlueNRG-LP DTM boot routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "bluenrg_lpx.h"
#include "DTM_config.h"
#include "hw_config.h"
#include "rf_driver_hal_vtimer.h"
#include "bleplat.h"
#include "aci_gatt_nwk.h"
#include "pka_manager.h"
#include "rng_manager.h"
#include "aes_manager.h"
#include "ble_controller.h"
#include "dm_alloc.h"
#include "aci_adv_nwk.h"   
#include "aci_l2cap_nwk.h"

/* Private typedef -----------------------------------------------------------*/
typedef  PACKED(struct) devConfigS  {
  uint8_t  SWXO_tune;
  uint8_t  LS_source;
  uint8_t  SMPS_management;
  uint8_t  Reserved;
  uint16_t HS_startup_time;
  uint16_t SCA;
  uint8_t  Reserved_2;
  uint32_t max_conn_event_length;
  uint8_t  Test_mode;
} devConfig_t;

/* Private define ------------------------------------------------------------*/

/**
 * @name Device Configuration Registers
 *@{
 */

/**
 *@brief Analog Test Bus 0 register settings
 */
#define ATB0_ANA_ENG_REG    0x3F
/**
 *@brief Analog Test Bus 1 register settings
 */
#define ATB1_ANA_ENG_REG    0x3E
//@} \\Device Configuration Registers

/**
 * @name Device Configuration values
 *@{
 */

/**
 * @brief Analog test bus 0 settings for 
 * normal application mode
 */
#define USER_MODE_ATB0              0x00
/**
 * @brief Analog test bus 1 settings for 
 * normal application mode
 */
#define USER_MODE_ATB1              0x30
/**
 * @brief Analog test bus 0 settings for 
 * Tx/Rx start stop signal measurement
 */
#define TX_RX_START_STOP_MEAS_ATB0  0x38
/**
 * @brief Analog test bus 1 settings for 
 * Tx/Rx start stop signal measurement
 */
#define TX_RX_START_STOP_MEAS_ATB1  0x34

/* Device Configuration value for struct deviceConfigS */
/* NOTE: the device configuration is loaded on DTM_SystemInit()
   from the device Flash memory (first 256 bytes, stacklib_stored_device_id_data).  
   If these bytes are not programmed (0xFF), the associated
   default device configuration (related to 0xFF values) is as follow:
   LS_SOURCE_RO     0xFF  (Internal RO)
   SMSP_10uH        0xFF  (SMPS 10uH)
   USER_MODE        0xFF  (user mode).
   
   The device configuration settings (LS_SOURCE, SMPS, user or test mode)
   can be modified  by using the BlueNRG GUI, Tools, BlueNRG IFR ....
 */
#define LS_SOURCE_RO    0xFF
#define LS_SOURCE_XTAL  0x00

#define SMPS_10uH     0xFF
#define SMPS_2_2uH    0x00
#define SMPS_NONE     0x01
#define SMPS_1_5uH    0x02
   
/* Warning: this is the default value, valid for STEVAL-IDB011V1.
  If this value is changed, BlueNRG GUI will not show the actual value
  if parameter value is set to 0xFF inside device configuration Flash sector. */
#define HSE_TUNE_DEFAULT 32

#define USER_MODE               0xFF
#define LS_CRYSTAL_MEASURE      0x01
#define HS_STARTUP_TIME_MEASURE 0x02 // TBR Verify if supported

/* Device Configuration value for struct deviceConfigS */
#define TX_RX_START_STOP_MEASURE 0x03 // TBR Verify if supported

/**
 * @brief Max HS startup time expressed in system time (1953 us / 2.4414 us)
 */
#define MAX_HS_STARTUP_TIME 0x320

/* Flash Address with the DTM configuration info */
#define FLASH_ADDRESS_STACK_DEV_ID_DATA   (LL_FLASH_END_ADDR - LL_FLASH_PAGE_SIZE + 1)

/* TRIMMING Defines */
#define VALIDITY_TAG 0xFCBCECCC

#define VALIDITY_LOCATION    0x10001EF8
#define TRIMMING_LOCATION    0x10001EE4
#define MR_TRIMMING_LOCATION 0x10001EE8

#define MAIN_REGULATOR_TRIM_Pos (0)
#define MAIN_REGULATOR_TRIM_Msk (0x0F << MAIN_REGULATOR_TRIM_Pos)
#define SMPS_TRIM_Pos           (4)
#define SMPS_TRIM_Msk           (0x07 << SMPS_TRIM_Pos)
#define LSI_LPMU_TRIM_Pos       (8)
#define LSI_LPMU_TRIM_Msk       (0x0F << LSI_LPMU_TRIM_Pos)
#define LSI_BW_TRIM_Pos         (12)
#define LSI_BW_TRIM_Msk         (0x0F << LSI_BW_TRIM_Pos)
#define HSI_TRIM_Pos            (16)
#define HSI_TRIM_Msk            (0x3F << HSI_TRIM_Pos)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static devConfig_t deviceConfig;

NO_INIT(uint32_t dyn_alloc_a[DYNAMIC_MEMORY_SIZE>>2]);
NO_INIT(uint32_t aci_gatt_adv_nwk_buffer[ACI_GATT_ADV_NWK_BUFFER_SIZE_CONF>>2]);

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  SMPS and Trimming value Configuration 
  */
static uint8_t DTM_SmpsTrimConfig(uint8_t smps_config)
{
  uint8_t ret_val = SUCCESS;
  uint8_t mem_config_smps_none = 0;
  uint32_t main_regulator, smps_out_voltage, lsi_bw, hsi_calib;
#ifdef CONFIG_DEVICE_BLUENRG_LP
  uint32_t lsi_lpmu;
#endif
  uint8_t eng_lsi_bw_flag;
  
  if(smps_config != SMPS_NONE) {
    /* After reset wait until SMPS is ready */
    SystemTimer_TimeoutConfig(16000000, 200, TRUE);
    while(LL_PWR_IsSMPSReady() == 0)
    {
      if (SystemTimer_TimeoutExpired()) 
      {
        ret_val = SYSTEM_CONFIG_SMPS_READY_ERROR;
        break;
      }
    }
    /* Disable the System Timer */
    SystemTimer_TimeoutConfig(0, 0, FALSE);
    if (ret_val != SUCCESS) 
    {
      return ret_val;
    }
  }
  
  if(smps_config == SMPS_10uH) {
    LL_PWR_SetSMPSBOM(LL_PWR_SMPS_BOM3);
    /* SMPS clock 4 MHz configuration */
    LL_RCC_SetSMPSPrescaler(LL_RCC_SMPS_DIV_4);
  }
  else if(smps_config == SMPS_2_2uH) {
    LL_PWR_SetSMPSBOM(LL_PWR_SMPS_BOM2);
    /* SMPS clock 8 MHz configuration  */
    LL_RCC_SetSMPSPrescaler(LL_RCC_SMPS_DIV_2);
  }
  else if(smps_config == SMPS_1_5uH) {
    LL_PWR_SetSMPSBOM(LL_PWR_SMPS_BOM1);
    /* SMPS clock 8 MHz configuration  */
    LL_RCC_SetSMPSPrescaler(LL_RCC_SMPS_DIV_2);
  }
  else if(smps_config == SMPS_NONE) {
    mem_config_smps_none = 1;
  }
  else {
    while(1);
  }

  /* Retrieve Trimming values from engineering flash locations */
  if (*(volatile uint32_t*)VALIDITY_LOCATION == VALIDITY_TAG) {
    main_regulator    = ((*(volatile uint32_t*)TRIMMING_LOCATION) & MAIN_REGULATOR_TRIM_Msk) >> MAIN_REGULATOR_TRIM_Pos;
    smps_out_voltage  = ((*(volatile uint32_t*)TRIMMING_LOCATION) & SMPS_TRIM_Msk) >> SMPS_TRIM_Pos;
#ifdef CONFIG_DEVICE_BLUENRG_LP
    lsi_lpmu          = ((*(volatile uint32_t*)TRIMMING_LOCATION) & LSI_LPMU_TRIM_Msk) >> LSI_LPMU_TRIM_Pos;
#endif
    lsi_bw            = ((*(volatile uint32_t*)TRIMMING_LOCATION) & LSI_BW_TRIM_Msk) >> LSI_BW_TRIM_Pos;
    hsi_calib         = ((*(volatile uint32_t*)TRIMMING_LOCATION) & HSI_TRIM_Msk) >> HSI_TRIM_Pos;
    eng_lsi_bw_flag   = TRUE;
  } else {
#ifdef CONFIG_DEVICE_BLUENRG_LP
    main_regulator    = 0x08;
    lsi_lpmu          = 0x08;
    hsi_calib         = 0x1E;
    eng_lsi_bw_flag   = FALSE;
#endif
#if defined(CONFIG_DEVICE_BLUENRG_LPS)
    main_regulator    = 0x0A;
    hsi_calib         = 0x1F;
    lsi_bw            = 8;
    eng_lsi_bw_flag   = TRUE;
#endif
    smps_out_voltage  = 0x03;
  }
  
  /* Set HSI Calibration Trimming value */
  LL_RCC_HSI_SetCalibTrimming(hsi_calib);

  /* Low speed internal RC trimming value set by software */
  if (eng_lsi_bw_flag)
    LL_RCC_LSI_SetTrimming(lsi_bw);
  
#ifdef CONFIG_DEVICE_BLUENRG_LP
  /* Set LSI LPMU Trimming value */
  LL_PWR_SetLSILPMUTrim(lsi_lpmu);
#endif

  /* Set Main Regulator voltage Trimming value */ 
  LL_PWR_SetMRTrim(main_regulator);

  /* Set SMPS output voltage Trimming value */
  LL_PWR_SetSMPSTrim(smps_out_voltage);
  
  /* Set SMPS in LP Open */
  LL_PWR_SetSMPSOpenMode(LL_PWR_SMPS_LPOPEN);
  
  if(mem_config_smps_none == 1) {
    /* No SMPS configuration */
    LL_PWR_SetSMPSMode(LL_PWR_NO_SMPS);
  }

  return SUCCESS;
}


/**
  * @brief  Low Speed Configuration
  */
static uint8_t DTM_LSConfig(uint8_t ls_config)
{
  uint8_t ret_val=SUCCESS;
    
  if(ls_config == LS_SOURCE_XTAL) {
    LL_PWR_SetNoPullB(LL_PWR_PUPD_IO12|LL_PWR_PUPD_IO13);
    LL_RCC_LSCO_SetSource(LL_RCC_LSCO_CLKSOURCE_LSE);
    
    /* Set LSE oscillator drive capability */
    LL_RCC_LSE_SetDriveCapability(LL_RCC_LSEDRIVE_HIGH);
    
    LL_RCC_LSI_Disable();
    
    /* Need to explicitly disable LSE to make LSERDY flag go to 0 even without
    POR, in case it was enabled. */
    LL_RCC_LSE_Disable();  
    if(SystemReadyWait(300, LL_RCC_LSE_IsReady, 0) == FALSE)
    {
      ret_val = SYSTEM_CONFIG_LSE_READY_ERROR;
    }
    
    LL_RCC_LSE_Enable();
    if(SystemReadyWait(300, LL_RCC_LSE_IsReady, 1) == FALSE)
    {
      ret_val = SYSTEM_CONFIG_LSE_READY_ERROR;
    }    
  }
  else if(ls_config == LS_SOURCE_RO) {
    LL_RCC_LSI_Disable();
    if(SystemReadyWait(300, LL_RCC_LSI_IsReady, 0) == FALSE)
    {
      ret_val = SYSTEM_CONFIG_LSI_READY_ERROR;
    }  
    LL_RCC_LSE_Disable();
    LL_RCC_LSCO_SetSource(LL_RCC_LSCO_CLKSOURCE_LSI);
    
    LL_RCC_LSI_Enable();
    if(SystemReadyWait(300, LL_RCC_LSI_IsReady, 1) == FALSE)
    {
      ret_val = SYSTEM_CONFIG_LSI_READY_ERROR;
    }     
  }
  else {
    while(1);
  }  
  
  /* Disable the System Timer */
  SystemTimer_TimeoutConfig(0, 0, FALSE);
  
  switch(deviceConfig.Test_mode)
  {
    LL_GPIO_InitTypeDef GPIO_InitStruct;
    
  case LS_CRYSTAL_MEASURE:
    
    if(deviceConfig.LS_source == LS_SOURCE_XTAL)
      LL_RCC_ConfigLSCO(LL_RCC_LSCOSOURCE_LSE);
    else 
      LL_RCC_ConfigLSCO(LL_RCC_LSCOSOURCE_LSI);
    
    LL_GPIO_StructInit(&GPIO_InitStruct);
    
    /* Configure PA10 for LCO */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
    
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    break;
    
  case HS_STARTUP_TIME_MEASURE:
    // TODO: to be implemented
    break;
  }
  
  
  return ret_val;
}


void DTM_SystemInit(void)
{
  uint8_t *dev_config_addr;

  /* Remap the vector table */
  LL_FLASH_SetVectTable(FLASH, LL_FLASH_CONFIG_IRQ_VECT_TABLE_FLASH);
  
  /* Load device configuration from FLASH memory */
  /* The first 256 bytes are for stacklib_stored_device_id_data */
  dev_config_addr = (uint8_t *) (FLASH_ADDRESS_STACK_DEV_ID_DATA + 0x100);
  memcpy(&deviceConfig, dev_config_addr, sizeof(deviceConfig));
    
#ifdef CONFIG_HW_LS_XTAL
  deviceConfig.LS_source = LS_SOURCE_XTAL;
  deviceConfig.SCA = 100;
#elif defined CONFIG_HW_LS_RO
  deviceConfig.LS_source = LS_SOURCE_RO;
  deviceConfig.SCA = 500;
#endif 
  
#ifdef CONFIG_HW_SMPS_10uH
  deviceConfig.SMPS_management = SMPS_10uH;
#elif defined CONFIG_HW_SMPS_2_2uH
  deviceConfig.SMPS_management = SMPS_2_2uH;
#elif defined CONFIG_HW_SMPS_1_5uH
  deviceConfig.SMPS_management = SMPS_1_5uH;
#elif defined CONFIG_HW_SMPS_NONE
  deviceConfig.SMPS_management = SMPS_NONE;
#endif
  
  /* Vector Table Offset Register */
  SCB->VTOR = (uint32_t) (__vector_table);

  /* Vector Table location */
  //FLASH->CONFIG |=  CONFIG_IRQ_VECT_FLASH_SECTOR; 
  
  /* Store in RAM the AppBase information */
  RAM_VR.AppBase = (uint32_t) (__vector_table);

  /* Enable all the RAM banks in retention during DEEPSTOP */
  LL_PWR_EnableRAMBankRet(LL_PWR_RAMRET_1);
#if defined(LL_PWR_RAMRET_2)
  LL_PWR_EnableRAMBankRet(LL_PWR_RAMRET_2);
#endif
#if defined(LL_PWR_RAMRET_3)
  LL_PWR_EnableRAMBankRet(LL_PWR_RAMRET_3);
#endif

#if defined(PWR_CR2_GPIORET)
      /* Disable the GPIO retention in DEEPSTOP configuration */
      LL_PWR_DisableGPIORET();
#endif
      
  /* HW SMPS and HW Trimming value Configuration */
  DTM_SmpsTrimConfig(deviceConfig.SMPS_management);

  /* Low Speed Crystal Configuration */
  DTM_LSConfig(deviceConfig.LS_source);
  
  /* MR_BLE BIAS current Trimming Configuration */
  if (BLE_SYSCLK_32M != BLE_SYSCLK_NONE) {
    MrBleBiasTrimConfig(TRUE);
  }
  
  /* Set current and capacitors for High Speed Crystal Oscillator */
#ifdef CONFIG_HW_HSE_TUNE
  LL_RCC_HSE_SetCapacitorTuning(CONFIG_HW_HSE_TUNE);
#else
  if(deviceConfig.SWXO_tune < 64){
    LL_RCC_HSE_SetCapacitorTuning(deviceConfig.SWXO_tune);
  }
  else 
  {
    LL_RCC_HSE_SetCapacitorTuning(HSE_TUNE_DEFAULT);
  }
#endif
  LL_RCC_HSE_SetCurrentControl(LL_RCC_HSE_CURRENTMAX_3);
  
#if (USE_SYSCLK_32M == 1) && (CONTROLLER_BIS_ENABLED == 0)

  /* System Clock Configuration */
  SystemClockConfig(SYSCLK_32M);

  /* Radio Clock Configuration */
  RadioClockConfig(BLE_SYSCLK_16M, SYSCLK_32M);

#else

  /* System Clock Configuration */
  SystemClockConfig(SYSCLK_64M);

  /* Radio Clock Configuration */
  RadioClockConfig(BLE_SYSCLK_32M, SYSCLK_64M);

#endif
  
  /* Set all the IRQ priority with a default value */
  setInterruptPriority();

  /* Enable PKA and RNG peripheral clock */
  LL_AHB_EnableClock(LL_AHB_PERIPH_PKA | LL_AHB_PERIPH_RNG);
  
  /* Enable the IRQs */
  __enable_irq();
}

void DTM_StackInit(void)
{
  uint8_t ret;
  uint8_t initialcalibration;
  uint32_t calibration_interval;
  BLE_STACK_InitTypeDef BLE_STACK_InitParams = BLE_STACK_INIT_PARAMETERS;

  if (deviceConfig.max_conn_event_length > 1639344) {
    BLE_STACK_InitParams.MaxConnEventLength = 0xFFFFFFFF;
  } else {
    BLE_STACK_InitParams.MaxConnEventLength = deviceConfig.max_conn_event_length;
  }
  if (deviceConfig.SCA == 0xFFFF) {
    BLE_STACK_InitParams.SleepClockAccuracy = 500;
  } else {
    BLE_STACK_InitParams.SleepClockAccuracy = deviceConfig.SCA;
  }
  
  if(deviceConfig.LS_source){
    initialcalibration = TRUE;
    calibration_interval = CALIBRATION_INTERVAL_CONF;
  }
  else{
    initialcalibration = FALSE;
    calibration_interval = 0;
  }
  
  BLECNTR_InitGlobal();
  
  HAL_VTIMER_InitType VTIMER_InitStruct = {MIN(deviceConfig.HS_startup_time, MAX_HS_STARTUP_TIME), initialcalibration, calibration_interval};
  HAL_VTIMER_Init(&VTIMER_InitStruct);
  
  BLEPLAT_Init();
  if (PKAMGR_Init() == PKAMGR_ERROR)
  {
      while(1);
  }
  if (RNGMGR_Init() != RNGMGR_SUCCESS)
  {
      while(1);
  }
  
  /* Init the AES block */
  AESMGR_Init();
  
  ret = BLE_STACK_Init(&BLE_STACK_InitParams);
  if (ret != 0) {
    while(1);
  }
  
  /* Used by aci_gatt_nwk and adv_buff_alloc libraries.  */
  dm_init(ACI_GATT_ADV_NWK_BUFFER_SIZE_CONF, aci_gatt_adv_nwk_buffer);
  
  aci_adv_nwk_init();

#if (BLESTACK_CONTROLLER_ONLY == 0)  
  ACI_gatt_nwk_init(ACI_ATT_QUEUED_WRITE_SIZE_CONF);
  
  aci_l2cap_nwk_init();
#endif
}

