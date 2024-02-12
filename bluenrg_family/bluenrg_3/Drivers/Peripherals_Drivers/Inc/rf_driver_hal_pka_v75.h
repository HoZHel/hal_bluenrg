/**
  ******************************************************************************
  * @file    rf_driver_hal_pka_v75.h
  * @author  RF Application Team
  * @brief   Header file of PKA HAL module.
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
#ifndef RF_DRIVER_HAL_PKA_H
#define RF_DRIVER_HAL_PKA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "rf_driver_hal_def.h"
#include "rf_driver_ll_pka_v7c.h"

/** @addtogroup RF_DRIVER_HAL_Driver
 * @{
 */

#if defined(PKA) && defined(HAL_PKA_MODULE_ENABLED)

/** @addtogroup PKA
 * @{
 */

/* Exported types ------------------------------------------------------------*/
/** @defgroup PKA_Exported_Types PKA Exported Types
 * @{
 */

/** @defgroup HAL_state_structure_definition HAL state structure definition
  * @brief  HAL State structures definition
  * @{
  */
typedef enum
{
  HAL_PKA_STATE_RESET   = 0x00U,  /*!< PKA not yet initialized or disabled  */
  HAL_PKA_STATE_READY   = 0x01U,  /*!< PKA initialized and ready for use    */
  HAL_PKA_STATE_BUSY    = 0x02U,  /*!< PKA internal processing is ongoing   */
  HAL_PKA_STATE_ERROR   = 0x03U,  /*!< PKA error state                      */
}
HAL_PKA_StateTypeDef;

/**
  * @}
  */

#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
/** @defgroup HAL_callback_id HAL callback ID enumeration
  * @{
  */
typedef enum
{
  HAL_PKA_OPERATION_COMPLETE_CB_ID      = 0x00U,    /*!< PKA End of operation callback ID  */
  HAL_PKA_ERROR_CB_ID                   = 0x01U,    /*!< PKA Error callback ID             */
  HAL_PKA_MSPINIT_CB_ID                 = 0x02U,    /*!< PKA Msp Init callback ID          */
  HAL_PKA_MSPDEINIT_CB_ID               = 0x03U     /*!< PKA Msp DeInit callback ID        */
} HAL_PKA_CallbackIDTypeDef;

/**
  * @}
  */

#endif /* USE_HAL_PKA_REGISTER_CALLBACKS */

/** @defgroup PKA_Error_Code_definition PKA Error Code definition
  * @brief  PKA Error Code definition
  * @{
  */
#define HAL_PKA_ERROR_NONE      (0x00000000U)
#define HAL_PKA_ERROR_ADDRERR   (0x00000001U)
#define HAL_PKA_ERROR_RAMERR    (0x00000002U)
#define HAL_PKA_ERROR_TIMEOUT   (0x00000004U)
#define HAL_PKA_ERROR_OPERATION (0x00000008U)
#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
#define HAL_PKA_ERROR_INVALID_CALLBACK  (0x00000010U)    /*!< Invalid Callback error */
#endif /* USE_HAL_PKA_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup PKA_handle_Structure_definition PKA handle Structure definition
  * @brief  PKA handle Structure definition
  * @{
  */
#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
typedef struct __PKA_HandleTypeDef
#else
typedef struct
#endif  /* USE_HAL_PKA_REGISTER_CALLBACKS */
{
  PKA_TypeDef                   *Instance;              /*!< Register base address */
  __IO HAL_PKA_StateTypeDef     State;                  /*!< PKA state */
  __IO uint32_t                 ErrorCode;              /*!< PKA Error code */
#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
  void (* OperationCpltCallback)(struct __PKA_HandleTypeDef *hpka); /*!< PKA End of operation callback */
  void (* ErrorCallback)(struct __PKA_HandleTypeDef *hpka);         /*!< PKA Error callback            */
  void (* MspInitCallback)(struct __PKA_HandleTypeDef *hpka);       /*!< PKA Msp Init callback         */
  void (* MspDeInitCallback)(struct __PKA_HandleTypeDef *hpka);     /*!< PKA Msp DeInit callback       */
#endif  /* USE_HAL_PKA_REGISTER_CALLBACKS */
} PKA_HandleTypeDef;
/**
  * @}
  */

#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
/** @defgroup PKA_Callback_definition PKA Callback pointer definition
  * @brief  PKA Callback pointer definition
  * @{
  */
typedef  void (*pPKA_CallbackTypeDef)(PKA_HandleTypeDef *hpka); /*!< Pointer to a PKA callback function */
/**
  * @}
  */
#endif /* USE_HAL_PKA_REGISTER_CALLBACKS */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup PKA_Exported_Constants PKA Exported Constants
  * @{
  */

/** @defgroup PKA_Mode PKA mode
  * @{
  */

#define PKA_MODE_MONTGOMERY_PARAM_MOD_EXP  LL_PKA_MODE_MONTGOMERY_PARAM_MOD_EXP  /*!< Compute Montgomery parameter and modular exponentiation */
#define PKA_MODE_MONTGOMERY_PARAM          LL_PKA_MODE_MONTGOMERY_PARAM          /*!< Compute Montgomery parameter only */
#define PKA_MODE_MODULAR_EXP               LL_PKA_MODE_MODULAR_EXP               /*!< Compute modular exponentiation only (Montgomery parameter should be loaded) */
#define PKA_MODE_MONTGOMERY_PARAM_ECC      LL_PKA_MODE_MONTGOMERY_PARAM_ECC      /*!< Compute Montgomery parameter and compute ECC kP operation */
#define PKA_MODE_ECC_KP_PRIMITIVE          LL_PKA_MODE_ECC_KP_PRIMITIVE          /*!< Compute the ECC kP primitive only (Montgomery parameter should be loaded) */
#define PKA_MODE_ECDSA_SIGNATURE           LL_PKA_MODE_ECDSA_SIGNATURE           /*!< ECDSA signature */
#define PKA_MODE_ECDSA_VERIFICATION        LL_PKA_MODE_ECDSA_VERIFICATION        /*!< ECDSA verification */
#define PKA_MODE_POINT_CHECK               LL_PKA_MODE_POINT_CHECK               /*!< Point check */
#define PKA_MODE_RSA_CRT_EXP               LL_PKA_MODE_RSA_CRT_EXP               /*!< RSA CRT exponentiation */
#define PKA_MODE_MODULAR_INV               LL_PKA_MODE_MODULAR_INV               /*!< Modular inversion */
#define PKA_MODE_ARITHMETIC_ADD            LL_PKA_MODE_ARITHMETIC_ADD            /*!< Arithmetic addition */
#define PKA_MODE_ARITHMETIC_SUB            LL_PKA_MODE_ARITHMETIC_SUB            /*!< Arithmetic subtraction */
#define PKA_MODE_ARITHMETIC_MUL            LL_PKA_MODE_ARITHMETIC_MUL            /*!< Arithmetic multiplication */
#define PKA_MODE_COMPARISON                LL_PKA_MODE_COMPARISON                /*!< Comparison */
#define PKA_MODE_MODULAR_RED               LL_PKA_MODE_MODULAR_REDUC             /*!< Modular reduction */
#define PKA_MODE_MODULAR_ADD               LL_PKA_MODE_MODULAR_ADD               /*!< Modular addition */
#define PKA_MODE_MODULAR_SUB               LL_PKA_MODE_MODULAR_SUB               /*!< Modular subtraction */
#define PKA_MODE_MONTGOMERY_MUL            LL_PKA_MODE_MONTGOMERY_MUL            /*!< Montgomery multiplication */
#define PKA_MODE_ECC_COMPLETE_ADDITION     LL_PKA_MODE_ECC_COMPLETE_ADDITION     /*!< Compute the ECC complete addition */
#define PKA_MODE_ECC_DOUBLE_BASE_LADDER    LL_PKA_MODE_ECC_DOUBLE_BASE_LADDER    /*!< Compute the ECC double base ladder */
/**
  * @}
  */

/** @defgroup PKA_Interrupt_configuration_definition PKA Interrupt configuration definition
  * @brief PKA Interrupt definition
  * @{
  */
#ifdef PKA_CR_FAULTERRORCODEIE
#define PKA_IT_FAULTERRORCODE                     PKA_CR_FAULTERRORCODEIE
#endif
#ifdef PKA_CR_FAULTFSMIE
#define PKA_IT_FAULTFSM                           PKA_CR_FAULTFSMIE
#endif
#define PKA_IT_PROCEND                            PKA_CR_PROCENDIE
#define PKA_IT_ADDRERR                            PKA_CR_ADDRERRIE
#define PKA_IT_RAMERR                             PKA_CR_RAMERRIE

/**
  * @}
  */

/** @defgroup PKA_Flag_definition PKA Flag definition
  * @{
  */
#ifdef PKA_SR_FAULTERRORCODEF
#define PKA_FLAG_FAULTERRORCODE                   PKA_SR_FAULTERRORCODEF
#endif
#ifdef PKA_FLAG_FAULTFSMF
#define PKA_FLAG_FAULTFSM                         PKA_SR_FAULTFSMF
#endif
#define PKA_FLAG_PROCEND                          PKA_SR_PROCENDF
#define PKA_FLAG_ADDRERR                          PKA_SR_ADDRERRF
#define PKA_FLAG_RAMERR                           PKA_SR_RAMERRF

/**
  * @}
  */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/

/** @defgroup PKA_Exported_Macros PKA Exported Macros
  * @{
  */

/** @brief  Reset PKA handle state.
  * @param  __HANDLE__ specifies the PKA Handle
  * @retval None
  */
#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
#define __HAL_PKA_RESET_HANDLE_STATE(__HANDLE__)                do{                                                   \
                                                                    (__HANDLE__)->State = HAL_PKA_STATE_RESET;       \
                                                                    (__HANDLE__)->MspInitCallback = NULL;            \
                                                                    (__HANDLE__)->MspDeInitCallback = NULL;          \
                                                                  } while(0)
#else
#define __HAL_PKA_RESET_HANDLE_STATE(__HANDLE__)                ((__HANDLE__)->State = HAL_PKA_STATE_RESET)
#endif

/** @brief  Enable the specified PKA interrupt.
  * @param  __HANDLE__ specifies the PKA Handle
  * @param  __INTERRUPT__ specifies the interrupt source to enable.
  *        This parameter can be one of the following values:
  *            @arg @ref PKA_IT_FAULTERRORCODE Fault Error code enable
  *            @arg @ref PKA_IT_FAULTFSM Fault FSM enable 
  *            @arg @ref PKA_IT_PROCEND End Of Operation interrupt enable
  *            @arg @ref PKA_IT_ADDRERR Address error interrupt enable
  *            @arg @ref PKA_IT_RAMERR RAM error interrupt enable
  * @retval None
  */
#define __HAL_PKA_ENABLE_IT(__HANDLE__, __INTERRUPT__)          ((__HANDLE__)->Instance->CR |= (__INTERRUPT__))

/** @brief  Disable the specified PKA interrupt.
  * @param  __HANDLE__ specifies the PKA Handle
  * @param  __INTERRUPT__ specifies the interrupt source to disable.
  *        This parameter can be one of the following values:
  *            @arg @ref PKA_IT_FAULTERRORCODE Fault Error code disable
  *            @arg @ref PKA_IT_FAULTFSM Fault FSM disable 
  *            @arg @ref PKA_IT_PROCEND End Of Operation interrupt disable
  *            @arg @ref PKA_IT_ADDRERR Address error interrupt disable
  *            @arg @ref PKA_IT_RAMERR RAM error interrupt disable
  * @retval None
  */
#define __HAL_PKA_DISABLE_IT(__HANDLE__, __INTERRUPT__)         ((__HANDLE__)->Instance->CR &= (~(__INTERRUPT__)))

/** @brief  Check whether the specified PKA interrupt source is enabled or not.
  * @param  __HANDLE__ specifies the PKA Handle
  * @param  __INTERRUPT__ specifies the PKA interrupt source to check.
  *        This parameter can be one of the following values:
  *            @arg @ref PKA_IT_FAULTERRORCODE Fault Error code
  *            @arg @ref PKA_IT_FAULTFSM Fault FSM 
  *            @arg @ref PKA_IT_PROCEND End Of Operation interrupt
  *            @arg @ref PKA_IT_ADDRERR Address error interrupt 
  *            @arg @ref PKA_IT_RAMERR RAM error interrupt 
  * @retval The new state of __INTERRUPT__ (SET or RESET)
  */
#define __HAL_PKA_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__)      ((((__HANDLE__)->Instance->CR & (__INTERRUPT__)) == (__INTERRUPT__)) ? SET : RESET)

/** @brief  Check whether the specified PKA flag is set or not.
  * @param  __HANDLE__ specifies the PKA Handle
  * @param  __FLAG__ specifies the flag to check.
  *        This parameter can be one of the following values:
  *            @arg @ref PKA_FLAG_FAULTERRORCODE Fault Error code
  *            @arg @ref PKA_FLAG_FAULTFSM Fault FSM 
  *            @arg @ref PKA_FLAG_PROCEND End Of Operation
  *            @arg @ref PKA_FLAG_ADDRERR Address error
  *            @arg @ref PKA_FLAG_RAMERR RAM error
  * @retval The new state of __FLAG__ (SET or RESET)
  */
#define __HAL_PKA_GET_FLAG(__HANDLE__, __FLAG__)                (((((__HANDLE__)->Instance->SR) & (__FLAG__)) == (__FLAG__)) ? SET : RESET)

/** @brief  Clear the PKA pending flags which are cleared by writing 1 in a specific bit.
  * @param  __HANDLE__ specifies the PKA Handle
  * @param  __FLAG__ specifies the flag to clear.
  *          This parameter can be any combination of the following values:
  *            @arg @ref PKA_FLAG_FAULTERRORCODE Fault Error code
  *            @arg @ref PKA_FLAG_FAULTFSM Fault FSM   
  *            @arg @ref PKA_FLAG_PROCEND End Of Operation
  *            @arg @ref PKA_FLAG_ADDRERR Address error
  *            @arg @ref PKA_FLAG_RAMERR RAM error
  * @retval None
  */
#define __HAL_PKA_CLEAR_FLAG(__HANDLE__, __FLAG__)              ((__HANDLE__)->Instance->CLRFR = (__FLAG__))

/** @brief  Enable the specified PKA peripheral.
  * @param  __HANDLE__ specifies the PKA Handle
  * @retval None
  */
#define __HAL_PKA_ENABLE(__HANDLE__)                            (SET_BIT((__HANDLE__)->Instance->CR,  PKA_CR_EN))

/** @brief  Disable the specified PKA peripheral.
  * @param  __HANDLE__ specifies the PKA Handle
  * @retval None
  */
#define __HAL_PKA_DISABLE(__HANDLE__)                           (CLEAR_BIT((__HANDLE__)->Instance->CR, PKA_CR_EN))

/** @brief  Start a PKA operation.
  * @param  __HANDLE__ specifies the PKA Handle
  * @retval None
  */
#define __HAL_PKA_START(__HANDLE__)                             (SET_BIT((__HANDLE__)->Instance->CR,  PKA_CR_START))


/**
  * @}
  */

/* Private macros --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @addtogroup PKA_Exported_Functions
  * @{
  */

/** @addtogroup PKA_Exported_Functions_Group1
  * @{
  */
/* Initialization and de-initialization functions *****************************/
HAL_StatusTypeDef HAL_PKA_Init(PKA_HandleTypeDef *hpka);
HAL_StatusTypeDef HAL_PKA_DeInit(PKA_HandleTypeDef *hpka);
void              HAL_PKA_MspInit(PKA_HandleTypeDef *hpka);
void              HAL_PKA_MspDeInit(PKA_HandleTypeDef *hpka);

#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
/* Callbacks Register/UnRegister functions  ***********************************/
HAL_StatusTypeDef HAL_PKA_RegisterCallback(PKA_HandleTypeDef *hpka, HAL_PKA_CallbackIDTypeDef CallbackID, pPKA_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_PKA_UnRegisterCallback(PKA_HandleTypeDef *hpka, HAL_PKA_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_PKA_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @addtogroup PKA_Exported_Functions_Group2
  * @{
  */
/* IO operation functions *****************************************************/
/* High Level Functions *******************************************************/

HAL_StatusTypeDef HAL_PKA_Abort(PKA_HandleTypeDef *hpka);
void HAL_PKA_RAMReset(PKA_HandleTypeDef *hpka);
void HAL_PKA_OperationCpltCallback(PKA_HandleTypeDef *hpka);
void HAL_PKA_ErrorCallback(PKA_HandleTypeDef *hpka);
void HAL_PKA_IRQHandler(PKA_HandleTypeDef *hpka);
/**
  * @}
  */

/** @addtogroup PKA_Exported_Functions_Group3
  * @{
  */
/* Peripheral State and Error functions ***************************************/
HAL_PKA_StateTypeDef HAL_PKA_GetState(PKA_HandleTypeDef *hpka);
uint32_t             HAL_PKA_GetError(PKA_HandleTypeDef *hpka);
void HAL_PKA_WriteSingleInput( uint32_t index,  int size, const uint32_t* word );
void HAL_PKA_WriteOperand( uint32_t index, int size, const uint32_t* in );
void HAL_PKA_ReadResult( uint32_t index, int size, uint32_t* out );
void HAL_PKA_ReadSingleOutput( uint32_t index, uint32_t* out);
HAL_StatusTypeDef PKA_Process(PKA_HandleTypeDef *hpka, uint32_t mode, uint32_t Timeout);
HAL_StatusTypeDef PKA_Process_IT(PKA_HandleTypeDef *hpka, uint32_t mode);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* defined(PKA) && defined(HAL_PKA_MODULE_ENABLED) */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* RF_DRIVER_HAL_PKA_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
