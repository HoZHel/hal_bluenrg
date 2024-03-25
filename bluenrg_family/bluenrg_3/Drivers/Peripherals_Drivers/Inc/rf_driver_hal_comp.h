/**
  ******************************************************************************
  * @file    rf_driver_hal_comp.h
  * @author  RF Application Team
  * @brief   Header file of COMP HAL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef RF_DRIVER_HAL_COMP_H
#define RF_DRIVER_HAL_COMP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "rf_driver_hal_def.h"
#include "rf_driver_ll_system.h"

/** @addtogroup RF_DRIVER_HAL_Driver
  * @{
  */
#if defined (COMP1) 

/** @addtogroup COMP
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup COMP_Exported_Types COMP Exported Types
  * @{
  */

/**
  * @brief  COMP Init structure definition
  */
typedef struct
{

  uint32_t Mode;               /*!< Set comparator operating mode to adjust power and speed.
                                    Note: For the characteristics of comparator power modes
                                          (propagation delay and power consumption), refer to device datasheet.
                                    This parameter can be a value of @ref COMP_PowerMode */

  uint32_t InputPlus;          /*!< Set comparator input plus (non-inverting input).
                                    This parameter can be a value of @ref COMP_InputPlus */

  uint32_t InputMinus;         /*!< Set comparator input minus (inverting input).
                                    This parameter can be a value of @ref COMP_InputMinus */

  uint32_t Hysteresis;         /*!< Set comparator hysteresis mode of the input minus.
                                    This parameter can be a value of @ref COMP_Hysteresis */

  uint32_t OutputPol;          /*!< Set comparator output polarity.
                                    This parameter can be a value of @ref COMP_OutputPolarity */

  uint32_t BlankingSrce;       /*!< Set comparator blanking source.
                                    This parameter can be a value of @ref COMP_BlankingSrce */

  uint32_t TriggerMode;        /*!< Set the comparator output triggering External Interrupt Line (EXTI).
                                    This parameter can be a value of @ref COMP_EXTI_TriggerMode */

} COMP_InitTypeDef;

/**
  * @brief  HAL COMP state machine: HAL COMP states definition
  */
#define COMP_STATE_BITFIELD_LOCK  (0x10U)
typedef enum
{
  HAL_COMP_STATE_RESET             = 0x00U,                                             /*!< COMP not yet initialized                             */
  HAL_COMP_STATE_RESET_LOCKED      = (HAL_COMP_STATE_RESET | COMP_STATE_BITFIELD_LOCK), /*!< COMP not yet initialized and configuration is locked */
  HAL_COMP_STATE_READY             = 0x01U,                                             /*!< COMP initialized and ready for use                   */
  HAL_COMP_STATE_READY_LOCKED      = (HAL_COMP_STATE_READY | COMP_STATE_BITFIELD_LOCK), /*!< COMP initialized but configuration is locked         */
  HAL_COMP_STATE_BUSY              = 0x02U,                                             /*!< COMP is running                                      */
  HAL_COMP_STATE_BUSY_LOCKED       = (HAL_COMP_STATE_BUSY | COMP_STATE_BITFIELD_LOCK)   /*!< COMP is running and configuration is locked          */
} HAL_COMP_StateTypeDef;

/**
  * @brief  COMP Handle Structure definition
  */
#if (USE_HAL_COMP_REGISTER_CALLBACKS == 1)
typedef struct __COMP_HandleTypeDef
#else
typedef struct
#endif
{
  COMP_TypeDef       *Instance;       /*!< Register base address    */
  COMP_InitTypeDef   Init;            /*!< COMP required parameters */
  HAL_LockTypeDef    Lock;            /*!< Locking object           */
  __IO HAL_COMP_StateTypeDef  State;  /*!< COMP communication state */
  __IO uint32_t      ErrorCode;       /*!< COMP error code */
#if (USE_HAL_COMP_REGISTER_CALLBACKS == 1)
  void (* TriggerCallback)(struct __COMP_HandleTypeDef *hcomp);   /*!< COMP trigger callback */
  void (* MspInitCallback)(struct __COMP_HandleTypeDef *hcomp);   /*!< COMP Msp Init callback */
  void (* MspDeInitCallback)(struct __COMP_HandleTypeDef *hcomp); /*!< COMP Msp DeInit callback */
#endif /* USE_HAL_COMP_REGISTER_CALLBACKS */
} COMP_HandleTypeDef;

#if (USE_HAL_COMP_REGISTER_CALLBACKS == 1)
/**
  * @brief  HAL COMP Callback ID enumeration definition
  */
typedef enum
{
  HAL_COMP_TRIGGER_CB_ID                = 0x00U,  /*!< COMP trigger callback ID */
  HAL_COMP_MSPINIT_CB_ID                = 0x01U,  /*!< COMP Msp Init callback ID */
  HAL_COMP_MSPDEINIT_CB_ID              = 0x02U   /*!< COMP Msp DeInit callback ID */
} HAL_COMP_CallbackIDTypeDef;

/**
  * @brief  HAL COMP Callback pointer definition
  */
typedef  void (*pCOMP_CallbackTypeDef)(COMP_HandleTypeDef *hcomp); /*!< pointer to a COMP callback function */

#endif /* USE_HAL_COMP_REGISTER_CALLBACKS */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup COMP_Exported_Constants COMP Exported Constants
  * @{
  */

/** @defgroup COMP_Error_Code COMP Error Code
  * @{
  */
#define HAL_COMP_ERROR_NONE             (0x00UL)  /*!< No error */
#if (USE_HAL_COMP_REGISTER_CALLBACKS == 1)
#define HAL_COMP_ERROR_INVALID_CALLBACK (0x01UL)  /*!< Invalid Callback error */
#endif /* USE_HAL_COMP_REGISTER_CALLBACKS */
/**
  * @}
  */


/** @defgroup COMP_PowerMode COMP power mode
  * @{
  */
/* Note: For the characteristics of comparator power modes                    */
/*       (propagation delay and power consumption),                           */
/*       refer to device datasheet.                                           */
#define COMP_POWERMODE_HIGHSPEED       (0x00000000UL)         /*!< High Speed */
#define COMP_POWERMODE_MEDIUMSPEED     (COMP_CSR_PWRMODE_0)   /*!< Medium Speed */
#define COMP_POWERMODE_ULTRALOWPOWER   (COMP_CSR_PWRMODE)     /*!< Ultra-low power mode */
/**
  * @}
  */

/** @defgroup COMP_InputPlus Comparator inputs plus
  * @{
  */
#if defined(COMP_CSR_INPSEL)
#define COMP_INPUT_PLUS_IO1           (0x00000000UL)                          /*!< Comparator input plus connected to IO1 (pin PA14 for COMP1). */
#define COMP_INPUT_PLUS_IO2           (COMP_CSR_INPSEL_0)                     /*!< Comparator input plus connected to IO2 (pin PB1 for COMP1) */
#define COMP_INPUT_PLUS_IO3           (COMP_CSR_INPSEL_1)                     /*!< Comparator input plus connected to IO3 (pin PB2 for COMP1) */
#endif
/**
  * @}
  */

/** @defgroup COMP_InputMinus COMP input minus (inverting input)
  * @{
  */
#define COMP_INPUT_MINUS_1_4VREFINT    (                                                            COMP_CSR_SCALEN | COMP_CSR_BRGEN)        /*!< Comparator input minus connected to 1/4 VrefInt */
#define COMP_INPUT_MINUS_1_2VREFINT    (                                        COMP_CSR_INMSEL_0 | COMP_CSR_SCALEN | COMP_CSR_BRGEN)        /*!< Comparator input minus connected to 1/2 VrefInt */
#define COMP_INPUT_MINUS_3_4VREFINT    (                    COMP_CSR_INMSEL_1                     | COMP_CSR_SCALEN | COMP_CSR_BRGEN)        /*!< Comparator input minus connected to 3/4 VrefInt */
#define COMP_INPUT_MINUS_VREFINT       (                    COMP_CSR_INMSEL_1 | COMP_CSR_INMSEL_0 | COMP_CSR_SCALEN                 )        /*!< Comparator input minus connected to VrefInt */
#define COMP_INPUT_MINUS_DACOUT        (COMP_CSR_INMSEL_2                                                                           )        /*!< Comparator input minus connected to DAC OUT */
#if defined(CONFIG_DEVICE_SPIRIT3)
#define COMP_INPUT_MINUS_IO1           (COMP_CSR_INMSEL_2 |                     COMP_CSR_INMSEL_0)                                           /*!< Comparator input minus connected to IO1 (pin PA13 for COMP1) */
#define COMP_INPUT_MINUS_IO2           (COMP_CSR_INMSEL_2 | COMP_CSR_INMSEL_1                    )                                           /*!< Comparator input minus connected to IO2 (pin PB0 for COMP1) */
#define COMP_INPUT_MINUS_IO3           (COMP_CSR_INMSEL_2 | COMP_CSR_INMSEL_1 | COMP_CSR_INMSEL_0)                                           /*!< Comparator input minus connected to IO3 (pin PB3 for COMP1) */
#endif
/**
  * @}
  */

/** @defgroup COMP_Hysteresis COMP hysteresis
  * @{
  */
#define COMP_HYSTERESIS_NONE           (0x00000000UL)                       /*!< No hysteresis */
#define COMP_HYSTERESIS_LOW            (                  COMP_CSR_HYST_0)  /*!< Hysteresis level low */
#define COMP_HYSTERESIS_MEDIUM         (COMP_CSR_HYST_1                  )  /*!< Hysteresis level medium */
#define COMP_HYSTERESIS_HIGH           (COMP_CSR_HYST_1 | COMP_CSR_HYST_0)  /*!< Hysteresis level high */
/**
  * @}
  */

/** @defgroup COMP_OutputPolarity COMP output Polarity
  * @{
  */
#define COMP_OUTPUTPOL_NONINVERTED     (0x00000000UL)         /*!< COMP output level is not inverted (comparator output is high when the input plus is at a higher voltage than the input minus) */
#define COMP_OUTPUTPOL_INVERTED        (COMP_CSR_POLARITY)    /*!< COMP output level is inverted     (comparator output is low  when the input plus is at a higher voltage than the input minus) */
/**
  * @}
  */

/** @defgroup COMP_BlankingSrce  COMP blanking source
  * @{
  */
#define COMP_BLANKINGSRC_NONE            (0x00000000UL)          /*!<Comparator output without blanking */
#if defined(CONFIG_DEVICE_SPIRIT3)
#define COMP_BLANKINGSRC_TIM2_OC4        (COMP_CSR_BLANKING_0)   /*!< Comparator output blanking source TIM2 OC4 */
#define COMP_BLANKINGSRC_TIM16_OC1       (COMP_CSR_BLANKING_1)   /*!< Comparator output blanking source TIM16 OC1 */
#endif /* CONFIG_DEVICE_SPIRIT3 */
/**
  * @}
  */

/** @defgroup COMP_OutputLevel COMP Output Level
  * @{
  */
/* Note: Comparator output level values are fixed to "0" and "1",             */
/* corresponding COMP register bit is managed by HAL function to match        */
/* with these values (independently of bit position in register).             */

/* When output polarity is not inverted, comparator output is low when
   the input plus is at a lower voltage than the input minus */
#define COMP_OUTPUT_LEVEL_LOW              (0x00000000UL)
/* When output polarity is not inverted, comparator output is high when
   the input plus is at a higher voltage than the input minus */
#define COMP_OUTPUT_LEVEL_HIGH             (0x00000001UL)
/**
  * @}
  */

/** @defgroup COMP_EXTI_TriggerMode COMP output 
  * @{
  */
#define COMP_TRIGGERMODE_RISING            LL_SYSCFG_MR_SUBG_TRIGGER_RISING_EDGE     /*!< Trigger on Rising Edge  */
#define COMP_TRIGGERMODE_FALLING           LL_SYSCFG_MR_SUBG_TRIGGER_FALLING_EDGE    /*!< Trigger on Falling Edge */
#define COMP_TRIGGERMODE_RISING_FALLING    LL_SYSCFG_MR_SUBG_TRIGGER_BOTH_EDGE       /*!< Trigger on Both Edge    */
#define COMP_TRIGGERMODE_LOW_LEVEL         LL_SYSCFG_MR_SUBG_TRIGGER_LOW_LEVEL       /*!< Trigger on Low Level    */
#define COMP_TRIGGERMODE_HIGH_LEVEL        LL_SYSCFG_MR_SUBG_TRIGGER_HIGH_LEVEL      /*!< Trigger on High Level   */    


/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup COMP_Exported_Macros COMP Exported Macros
  * @{
  */

/** @defgroup COMP_Handle_Management  COMP Handle Management
  * @{
  */

/** @brief  Reset COMP handle state.
  * @param  __HANDLE__  COMP handle
  * @retval None
  */
#if (USE_HAL_COMP_REGISTER_CALLBACKS == 1)
#define __HAL_COMP_RESET_HANDLE_STATE(__HANDLE__) do{                                                  \
                                                      (__HANDLE__)->State = HAL_COMP_STATE_RESET;      \
                                                      (__HANDLE__)->MspInitCallback = NULL;            \
                                                      (__HANDLE__)->MspDeInitCallback = NULL;          \
                                                    } while(0)
#else
#define __HAL_COMP_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_COMP_STATE_RESET)
#endif

/**
  * @brief Clear COMP error code (set it to no error code "HAL_COMP_ERROR_NONE").
  * @param __HANDLE__ COMP handle
  * @retval None
  */
#define COMP_CLEAR_ERRORCODE(__HANDLE__) ((__HANDLE__)->ErrorCode = HAL_COMP_ERROR_NONE)

/**
  * @brief  Enable the specified comparator.
  * @param  __HANDLE__  COMP handle
  * @retval None
  */
#define __HAL_COMP_ENABLE(__HANDLE__)              SET_BIT((__HANDLE__)->Instance->CSR, COMP_CSR_EN)

/**
  * @brief  Disable the specified comparator.
  * @param  __HANDLE__  COMP handle
  * @retval None
  */
#define __HAL_COMP_DISABLE(__HANDLE__)             CLEAR_BIT((__HANDLE__)->Instance->CSR, COMP_CSR_EN)

/**
  * @brief  Lock the specified comparator configuration.
  * @note   Using this macro induce HAL COMP handle state machine being no
  *         more in line with COMP instance state.
  *         To keep HAL COMP handle state machine updated, it is recommended
  *         to use function "HAL_COMP_Lock')".
  * @param  __HANDLE__  COMP handle
  * @retval None
  */
#define __HAL_COMP_LOCK(__HANDLE__)                SET_BIT((__HANDLE__)->Instance->CSR, COMP_CSR_LOCK)

/**
  * @brief  Check whether the specified comparator is locked.
  * @param  __HANDLE__  COMP handle
  * @retval Value 0 if COMP instance is not locked, value 1 if COMP instance is locked
  */
#define __HAL_COMP_IS_LOCKED(__HANDLE__)           (READ_BIT((__HANDLE__)->Instance->CSR, COMP_CSR_LOCK) == COMP_CSR_LOCK)

/**
  * @}
  */

/** @defgroup COMP_Exti_Management  COMP external interrupt line management
  * @{
  */

/**
  * @brief  Enable the COMP1 EXTI line rising edge trigger.
  * @retval None
  */
#define __HAL_COMP_COMP1_EXTI_ENABLE_RISING_EDGE()    LL_SYSCFG_MR_SUBG_INTAI_SetTrigger(LL_SYSCFG_MR_SUBG_TRIGGER_RISING_EDGE, LL_SYSCFG_MR_SUBG_COMPOUT_EVENT)

/**
  * @brief  Enable the COMP1 EXTI line falling edge trigger.
  * @retval None
  */
#define __HAL_COMP_COMP1_EXTI_ENABLE_FALLING_EDGE()   LL_SYSCFG_MR_SUBG_INTAI_SetTrigger(LL_SYSCFG_MR_SUBG_TRIGGER_FALLING_EDGE, LL_SYSCFG_MR_SUBG_COMPOUT_EVENT)

/**
  * @brief  Enable the COMP1 EXTI line rising & falling edge trigger.
  * @retval None
  */
#define __HAL_COMP_COMP1_EXTI_ENABLE_RISING_FALLING_EDGE()   LL_SYSCFG_MR_SUBG_INTAI_SetTrigger(LL_SYSCFG_MR_SUBG_TRIGGER_BOTH_EDGE, LL_SYSCFG_MR_SUBG_COMPOUT_EVENT)


/**
  * @}
  */

/**
  * @}
  */


/* Private types -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup COMP_Private_Constants COMP Private Constants
  * @{
  */


/** @defgroup COMP_ExtiLine COMP EXTI Lines
  * @{
  */
#define COMP_EXTI_IT                        (0x00000001UL)  /*!< EXTI line event with interruption */
#define COMP_EXTI_EVENT                     (0x00000002UL)  /*!< EXTI line event only (without interruption) */
#define COMP_EXTI_RISING                    (0x00000010UL)  /*!< EXTI line event on rising edge */
#define COMP_EXTI_FALLING                   (0x00000020UL)  /*!< EXTI line event on falling edge */
/**
  * @}
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup COMP_Private_Macros COMP Private Macros
  * @{
  */


/** @defgroup COMP_IS_COMP_Private_Definitions COMP private macros to check input parameters
  * @{
  */

#define IS_COMP_POWERMODE(__POWERMODE__)    (((__POWERMODE__) == COMP_POWERMODE_HIGHSPEED)    || \
                                             ((__POWERMODE__) == COMP_POWERMODE_MEDIUMSPEED)  || \
                                             ((__POWERMODE__) == COMP_POWERMODE_ULTRALOWPOWER)  )

#if defined(COMP_INPUT_PLUS_IO1)
#define IS_COMP_INPUT_PLUS(__COMP_INSTANCE__, __INPUT_PLUS__) (((__INPUT_PLUS__) == COMP_INPUT_PLUS_IO1) || \
                                                               ((__INPUT_PLUS__) == COMP_INPUT_PLUS_IO2) || \
                                                               ((__INPUT_PLUS__) == COMP_INPUT_PLUS_IO3))
#else
#define IS_COMP_INPUT_PLUS(__COMP_INSTANCE__, __INPUT_PLUS__) (((__INPUT_PLUS__) == COMP_INPUT_PLUS_IO2) || \
                                                               ((__INPUT_PLUS__) == COMP_INPUT_PLUS_IO3))
#endif

/* Note: On this series, comparator input minus parameters are          */
/*       the same on all COMP instances.                                      */
/*       However, comparator instance kept as macro parameter for             */
/*       compatibility with other families.                             */
#if defined(COMP_INPUT_MINUS_IO2)
#define IS_COMP_INPUT_MINUS(__COMP_INSTANCE__, __INPUT_MINUS__) (((__INPUT_MINUS__) == COMP_INPUT_MINUS_1_4VREFINT)  || \
                                                                 ((__INPUT_MINUS__) == COMP_INPUT_MINUS_1_2VREFINT)  || \
                                                                 ((__INPUT_MINUS__) == COMP_INPUT_MINUS_3_4VREFINT)  || \
                                                                 ((__INPUT_MINUS__) == COMP_INPUT_MINUS_VREFINT)     || \
                                                                 ((__INPUT_MINUS__) == COMP_INPUT_MINUS_IO1)         || \
                                                                 ((__INPUT_MINUS__) == COMP_INPUT_MINUS_IO2)         || \
                                                                 ((__INPUT_MINUS__) == COMP_INPUT_MINUS_IO3)         || \
                                                                 ((__INPUT_MINUS__) == COMP_INPUT_MINUS_IO4)         || \
                                                                 ((__INPUT_MINUS__) == COMP_INPUT_MINUS_IO5))
#else
#define IS_COMP_INPUT_MINUS(__COMP_INSTANCE__, __INPUT_MINUS__) (((__INPUT_MINUS__) == COMP_INPUT_MINUS_1_4VREFINT)  || \
                                                                 ((__INPUT_MINUS__) == COMP_INPUT_MINUS_1_2VREFINT)  || \
                                                                 ((__INPUT_MINUS__) == COMP_INPUT_MINUS_3_4VREFINT)  || \
                                                                 ((__INPUT_MINUS__) == COMP_INPUT_MINUS_VREFINT)     || \
                                                                 ((__INPUT_MINUS__) == COMP_INPUT_MINUS_IO1)         || \
                                                                 ((__INPUT_MINUS__) == COMP_INPUT_MINUS_IO3)         || \
                                                                 ((__INPUT_MINUS__) == COMP_INPUT_MINUS_IO4)         || \
                                                                 ((__INPUT_MINUS__) == COMP_INPUT_MINUS_IO5))
#endif

#define IS_COMP_HYSTERESIS(__HYSTERESIS__)  (((__HYSTERESIS__) == COMP_HYSTERESIS_NONE)   || \
                                             ((__HYSTERESIS__) == COMP_HYSTERESIS_LOW)    || \
                                             ((__HYSTERESIS__) == COMP_HYSTERESIS_MEDIUM) || \
                                             ((__HYSTERESIS__) == COMP_HYSTERESIS_HIGH))

#define IS_COMP_OUTPUTPOL(__POL__)          (((__POL__) == COMP_OUTPUTPOL_NONINVERTED) || \
                                             ((__POL__) == COMP_OUTPUTPOL_INVERTED))

#define IS_COMP_BLANKINGSRCE(__OUTPUT_BLANKING_SOURCE__)                    \
  (   ((__OUTPUT_BLANKING_SOURCE__) == COMP_BLANKINGSRC_NONE)               \
   || ((__OUTPUT_BLANKING_SOURCE__) == COMP_BLANKINGSRC_TIM1_OC5)           \
   || ((__OUTPUT_BLANKING_SOURCE__) == COMP_BLANKINGSRC_TIM2_OC3)           \
  )

/* Note: Output blanking source common to all COMP instances */
/*       Macro kept for compatibility with other series */
#define IS_COMP_BLANKINGSRC_INSTANCE(__INSTANCE__, __OUTPUT_BLANKING_SOURCE__)  \
   (IS_COMP_BLANKINGSRCE(__OUTPUT_BLANKING_SOURCE__))


#define IS_COMP_TRIGGERMODE(__MODE__)       (((__MODE__) == COMP_TRIGGERMODE_RISING)            || \
                                             ((__MODE__) == COMP_TRIGGERMODE_FALLING)           || \
                                             ((__MODE__) == COMP_TRIGGERMODE_RISING_FALLING)    || \
                                             ((__MODE__) == COMP_TRIGGERMODE_LOW_LEVEL)         || \
                                             ((__MODE__) == COMP_TRIGGERMODE_HIGH_LEVEL))

#define IS_COMP_OUTPUT_LEVEL(__OUTPUT_LEVEL__) (((__OUTPUT_LEVEL__) == COMP_OUTPUT_LEVEL_LOW)     || \
                                                ((__OUTPUT_LEVEL__) == COMP_OUTPUT_LEVEL_HIGH))

/**
  * @}
  */

/**
  * @}
  */


/* Exported functions --------------------------------------------------------*/
/** @addtogroup COMP_Exported_Functions
  * @{
  */

/** @addtogroup COMP_Exported_Functions_Group1
  * @{
  */

/* Initialization and de-initialization functions  **********************************/
HAL_StatusTypeDef HAL_COMP_Init(COMP_HandleTypeDef *hcomp);
HAL_StatusTypeDef HAL_COMP_DeInit(COMP_HandleTypeDef *hcomp);
void              HAL_COMP_MspInit(COMP_HandleTypeDef *hcomp);
void              HAL_COMP_MspDeInit(COMP_HandleTypeDef *hcomp);

#if (USE_HAL_COMP_REGISTER_CALLBACKS == 1)
/* Callbacks Register/UnRegister functions  ***********************************/
HAL_StatusTypeDef HAL_COMP_RegisterCallback(COMP_HandleTypeDef *hcomp, HAL_COMP_CallbackIDTypeDef CallbackID,
                                            pCOMP_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_COMP_UnRegisterCallback(COMP_HandleTypeDef *hcomp, HAL_COMP_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_COMP_REGISTER_CALLBACKS */
/**
  * @}
  */

/* IO operation functions  *****************************************************/
/** @addtogroup COMP_Exported_Functions_Group2
  * @{
  */
HAL_StatusTypeDef HAL_COMP_Start(COMP_HandleTypeDef *hcomp);
HAL_StatusTypeDef HAL_COMP_Stop(COMP_HandleTypeDef *hcomp);
void              HAL_COMP_IRQHandler(COMP_HandleTypeDef *hcomp);
/**
  * @}
  */

/* Peripheral Control functions  ************************************************/
/** @addtogroup COMP_Exported_Functions_Group3
  * @{
  */
HAL_StatusTypeDef HAL_COMP_Lock(COMP_HandleTypeDef *hcomp);
uint32_t          HAL_COMP_GetOutputLevel(COMP_HandleTypeDef *hcomp);
/* Callback in interrupt mode */
void              HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp);
/**
  * @}
  */

/* Peripheral State functions  **************************************************/
/** @addtogroup COMP_Exported_Functions_Group4
  * @{
  */
HAL_COMP_StateTypeDef HAL_COMP_GetState(COMP_HandleTypeDef *hcomp);
uint32_t              HAL_COMP_GetError(COMP_HandleTypeDef *hcomp);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* COMP1 */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* RF_DRIVER_HAL_COMP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
