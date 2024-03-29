/**
  ******************************************************************************
  * @file    rf_driver_ll_pka_v7c.h
  * @author  RF Application Team
  * @brief   Header file of PKA LL module.
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
#ifndef RF_DRIVER_LL_PKA_H
#define RF_DRIVER_LL_PKA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#if defined(CONFIG_DEVICE_BLUENRG_LPS) || defined(CONFIG_DEVICE_BLUENRG_LPF)
#include "bluenrg_lpx.h"
#endif

/** @addtogroup RF_DRIVER_LL_Driver
  * @{
  */

#if defined(PKA)

/** @defgroup PKA_LL PKA
  * @{
  */

/* Private variables ---------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
#if defined(USE_FULL_LL_DRIVER)
/** @defgroup PKA_LL_ES_INIT PKA Exported Init structure
  * @{
  */

/**
  * @brief  PKA Init structures definition
  */
typedef struct
{
  uint32_t Mode;          /*!< Specifies the PKA operation mode.
                               This parameter can be a value of @ref PKA_LL_EC_MODE.

                               This feature can be modified afterwards using unitary function @ref LL_PKA_SetMode(). */
} LL_PKA_InitTypeDef;

/**
  * @}
  */
#endif /* USE_FULL_LL_DRIVER */

/* Exported constants --------------------------------------------------------*/
/** @defgroup PKA_LL_Exported_Constants PKA Exported Constants
  * @{
  */

/** @defgroup PKA_RAM_ADDR RAM Address Definitions
  * @{
  */
#define PKA_RAM_ECC_ADDR_KP_ERROR   (PKA_RAM_BASE)
#define PKA_RAM_ECC_ADDR_K          (PKA_RAM_BASE + 0x6C)
#define PKA_RAM_ECC_ADDR_PX         (PKA_RAM_BASE + 0x90)
#define PKA_RAM_ECC_ADDR_PY         (PKA_RAM_BASE + 0xB4)
/**
  * @}
  */

/** @defgroup PKA_LL_EC_GET_FLAG Get Flags Defines
  * @brief    Flags defines which can be used with LL_PKA_ReadReg function
  * @{
  */
#ifdef PKA_SR_FAULTERRORCODEF
#define LL_PKA_SR_FAULTERRORCODEF          PKA_SR_FAULTERRORCODEF
#endif
#ifdef PKA_SR_FAULTFSMF
#define LL_PKA_SR_FAULTFSMF                PKA_SR_FAULTFSMF
#endif
#define LL_PKA_SR_ADDRERRF                 PKA_SR_ADDRERRF
#define LL_PKA_SR_RAMERRF                  PKA_SR_RAMERRF
#define LL_PKA_SR_PROCENDF                 PKA_SR_PROCENDF
#define LL_PKA_SR_BUSY                     PKA_SR_BUSY
/**
  * @}
  */

/** @defgroup PKA_LL_EC_IT IT Defines
  * @brief    IT defines which can be used with LL_PKA_ReadReg and  LL_PKA_WriteReg functions
  * @{
  */
#ifdef PKA_CR_FAULTERRORCODEIE
#define LL_PKA_CR_FAULTERRORCODEIE         PKA_CR_FAULTERRORCODEIE
#endif
#ifdef PKA_CR_FAULTFSMIE
#define LL_PKA_CR_FAULTFSMIE               PKA_CR_FAULTFSMIE
#endif
#define LL_PKA_CR_ADDRERRIE                PKA_CR_ADDRERRIE
#define LL_PKA_CR_RAMERRIE                 PKA_CR_RAMERRIE
#define LL_PKA_CR_PROCENDIE                PKA_CR_PROCENDIE

#ifdef LL_PKA_CLRFR_FAULTERRORCODEFC
#define LL_PKA_CLRFR_FAULTERRORCODEFC      PKA_CLRFR_FAULTERRORCODEFC
#endif
#ifdef LL_PKA_CLRFR_FAULTFSMFC
#define LL_PKA_CLRFR_FAULTFSMFC            PKA_CLRFR_FAULTFSMFC
#endif
#define LL_PKA_CLRFR_PROCENDFC             PKA_CLRFR_PROCENDFC
#define LL_PKA_CLRFR_RAMERRFC              PKA_CLRFR_RAMERRFC
#define LL_PKA_CLRFR_ADDRERRFC             PKA_CLRFR_ADDRERRFC
/**
  * @}
  */

/** @defgroup PKA_LL_SECLVL Value Defines
  * @brief    IT defines which can be used with LL_PKA_SetSecurityLevel and  LL_PKA_GetSecurityLevel functions
  * @{
  */
#define LL_PKA_SECURITY_LEVEL_NONE              0x00000000U
#define LL_PKA_SECURITY_LEVEL_ENABLE            0x00000001U
/**
  * @}
  */

/** @defgroup PKA_LL_EC_MODE Operation Mode
  * @brief    List of opearation mode.
  * @{
  */
#if defined(CONFIG_DEVICE_BLUENRG_LPS) ||  defined(CONFIG_DEVICE_BLUENRG_LPF)
#define LL_PKA_MODE_MONTGOMERY_PARAM_MOD_EXP ((uint32_t)0x00000000U) /*!< Compute Montgomery parameter and modular exponentiation */
#define LL_PKA_MODE_MONTGOMERY_PARAM         ((uint32_t)0x00000001U) /*!< Compute Montgomery parameter only */
#define LL_PKA_MODE_MODULAR_EXP              ((uint32_t)0x00000002U) /*!< Compute modular exponentiation only (Montgomery parameter should be loaded) */
#define LL_PKA_MODE_MONTGOMERY_PARAM_ECC     ((uint32_t)0x00000020U) /*!< Compute Montgomery parameter and compute ECC kP operation */
#define LL_PKA_MODE_ECC_KP_PRIMITIVE         ((uint32_t)0x00000022U) /*!< Compute the ECC kP primitive only (Montgomery parameter should be loaded) */
#define LL_PKA_MODE_ECDSA_SIGNATURE          ((uint32_t)0x00000024U) /*!< ECDSA signature */
#define LL_PKA_MODE_ECDSA_VERIFICATION       ((uint32_t)0x00000026U) /*!< ECDSA verification */
#define LL_PKA_MODE_POINT_CHECK              ((uint32_t)0x00000028U) /*!< Point check */
#define LL_PKA_MODE_RSA_CRT_EXP              ((uint32_t)0x00000007U) /*!< RSA CRT exponentiation */
#define LL_PKA_MODE_MODULAR_INV              ((uint32_t)0x00000008U) /*!< Modular inversion */
#define LL_PKA_MODE_ARITHMETIC_ADD           ((uint32_t)0x00000009U) /*!< Arithmetic addition */
#define LL_PKA_MODE_ARITHMETIC_SUB           ((uint32_t)0x0000000AU) /*!< Arithmetic subtraction */
#define LL_PKA_MODE_ARITHMETIC_MUL           ((uint32_t)0x0000000BU) /*!< Arithmetic multiplication */
#define LL_PKA_MODE_COMPARISON               ((uint32_t)0x0000000CU) /*!< Comparison */
#define LL_PKA_MODE_MODULAR_REDUC            ((uint32_t)0x0000000DU) /*!< Modular reduction */
#define LL_PKA_MODE_MODULAR_ADD              ((uint32_t)0x0000000EU) /*!< Modular addition */
#define LL_PKA_MODE_MODULAR_SUB              ((uint32_t)0x0000000FU) /*!< Modular subtraction */
#define LL_PKA_MODE_MONTGOMERY_MUL           ((uint32_t)0x00000010U) /*!< Montgomery multiplication */
#endif

#if defined(CONFIG_DEVICE_BLUENRG_LPF)
#define LL_PKA_MODE_ECC_COMPLETE_ADDITION    ((uint32_t)0x00000023U) /*!< Compute the ECC complete addition */
#define LL_PKA_MODE_ECC_DOUBLE_BASE_LADDER   ((uint32_t)0x00000027U) /*!< Compute the ECC double base ladder */
#endif

/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup PKA_LL_Exported_Macros PKA Exported Macros
  * @{
  */

/** @defgroup PKA_LL_EM_WRITE_READ Common Write and read registers Macros
  * @{
  */

/**
  * @brief  Write a value in PKA register
  * @param  __INSTANCE__ PKA Instance
  * @param  __REG__ Register to be written
  * @param  __VALUE__ Value to be written in the register
  * @retval None
  */
#define LL_PKA_WriteReg(__INSTANCE__, __REG__, __VALUE__) WRITE_REG(__INSTANCE__->__REG__, (__VALUE__))

/**
  * @brief  Read a value in PKA register
  * @param  __INSTANCE__ PKA Instance
  * @param  __REG__ Register to be read
  * @retval Register value
  */
#define LL_PKA_ReadReg(__INSTANCE__, __REG__) READ_REG(__INSTANCE__->__REG__)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup PKA_LL_Exported_Functions PKA Exported Functions
  * @{
  */

/** @defgroup PKA_LL_EF_Configuration Configuration
  * @{
  */

/**
  * @brief  Configure PKA peripheral.
  * @brief  Set PKA operating mode.
  * @rmtoll CR           MODE          LL_PKA_Config
  * @param  PKAx PKA Instance.
  * @param  Mode This parameter can be one of the following values for BlueNRG-LPS and BlueNRG-LPF where applicable:
  * @arg LL_PKA_MODE_MONTGOMERY_PARAM_MOD_EXP
  * @arg LL_PKA_MODE_MONTGOMERY_PARAM
  * @arg LL_PKA_MODE_MODULAR_EXP
  * @arg LL_PKA_MODE_MONTGOMERY_PARAM_ECC
  * @arg LL_PKA_MODE_ECC_KP_PRIMITIVE
  * @arg LL_PKA_MODE_ECDSA_SIGNATURE
  * @arg LL_PKA_MODE_ECDSA_VERIFICATION
  * @arg LL_PKA_MODE_POINT_CHECK
  * @arg LL_PKA_MODE_RSA_CRT_EXP
  * @arg LL_PKA_MODE_MODULAR_INV
  * @arg LL_PKA_MODE_ARITHMETIC_ADD
  * @arg LL_PKA_MODE_ARITHMETIC_SUB
  * @arg LL_PKA_MODE_ARITHMETIC_MUL
  * @arg LL_PKA_MODE_COMPARISON
  * @arg LL_PKA_MODE_MODULAR_REDUC
  * @arg LL_PKA_MODE_MODULAR_ADD
  * @arg LL_PKA_MODE_MODULAR_SUB
  * @arg LL_PKA_MODE_MONTGOMERY_MUL
  * @arg LL_PKA_MODE_ECC_COMPLETE_ADDITION
  * @arg LL_PKA_MODE_ECC_DOUBLE_BASE_LADDER
  */
__STATIC_INLINE void LL_PKA_Config(PKA_TypeDef *PKAx, uint32_t Mode)
{
  MODIFY_REG(PKAx->CR, (PKA_CR_MODE), (Mode << PKA_CR_MODE_Pos));
}

/**
  * @brief  Enable PKA peripheral.
  * @rmtoll CR           EN            LL_PKA_Enable
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_Enable(PKA_TypeDef *PKAx)
{
  SET_BIT(PKAx->CR, PKA_CR_EN);
}

/**
  * @brief  Disable PKA peripheral.
  * @rmtoll CR           EN            LL_PKA_Disable
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_Disable(PKA_TypeDef *PKAx)
{
  CLEAR_BIT(PKAx->CR, PKA_CR_EN);
}

/**
  * @brief  Check if the PKA peripheral is enabled or disabled.
  * @rmtoll CR           EN            LL_PKA_IsEnabled
  * @param  PKAx PKA Instance.
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PKA_IsEnabled(PKA_TypeDef *PKAx)
{
  return ((READ_BIT(PKAx->CR, PKA_CR_EN) == (PKA_CR_EN)) ? 1UL : 0UL);
}

/**
  * @brief  Set PKA operating mode.
  * @rmtoll CR           MODE          LL_PKA_SetMode
  * @param  PKAx PKA Instance.
  * @param  Mode This parameter can be one of the following values for BlueNRG-LPS and BlueNRG-LPF where applicable:
  * @arg LL_PKA_MODE_MONTGOMERY_PARAM_MOD_EXP
  * @arg LL_PKA_MODE_MONTGOMERY_PARAM
  * @arg LL_PKA_MODE_MODULAR_EXP
  * @arg LL_PKA_MODE_MONTGOMERY_PARAM_ECC
  * @arg LL_PKA_MODE_ECC_KP_PRIMITIVE
  * @arg LL_PKA_MODE_ECDSA_SIGNATURE
  * @arg LL_PKA_MODE_ECDSA_VERIFICATION
  * @arg LL_PKA_MODE_POINT_CHECK
  * @arg LL_PKA_MODE_RSA_CRT_EXP
  * @arg LL_PKA_MODE_MODULAR_INV
  * @arg LL_PKA_MODE_ARITHMETIC_ADD
  * @arg LL_PKA_MODE_ARITHMETIC_SUB
  * @arg LL_PKA_MODE_ARITHMETIC_MUL
  * @arg LL_PKA_MODE_COMPARISON
  * @arg LL_PKA_MODE_MODULAR_REDUC
  * @arg LL_PKA_MODE_MODULAR_ADD
  * @arg LL_PKA_MODE_MODULAR_SUB
  * @arg LL_PKA_MODE_MONTGOMERY_MUL
  * @arg LL_PKA_MODE_ECC_COMPLETE_ADDITION
  * @arg LL_PKA_MODE_ECC_DOUBLE_BASE_LADDER
  * @retval None
  */
__STATIC_INLINE void LL_PKA_SetMode(PKA_TypeDef *PKAx, uint32_t Mode)
{
  MODIFY_REG(PKAx->CR, PKA_CR_MODE, Mode << PKA_CR_MODE_Pos);
}

/**
  * @brief  Get PKA operating mode.
  * @rmtoll CR           MODE          LL_PKA_GetMode
  * @param  PKAx PKA Instance.
  * @retval Returned value can be one of the following values for BlueNRG-LPS and BlueNRG-LPF where applicable:
  * @arg LL_PKA_MODE_MONTGOMERY_PARAM_MOD_EXP
  * @arg LL_PKA_MODE_MONTGOMERY_PARAM
  * @arg LL_PKA_MODE_MODULAR_EXP
  * @arg LL_PKA_MODE_MONTGOMERY_PARAM_ECC
  * @arg LL_PKA_MODE_ECC_KP_PRIMITIVE
  * @arg LL_PKA_MODE_ECDSA_SIGNATURE
  * @arg LL_PKA_MODE_ECDSA_VERIFICATION
  * @arg LL_PKA_MODE_POINT_CHECK
  * @arg LL_PKA_MODE_RSA_CRT_EXP
  * @arg LL_PKA_MODE_MODULAR_INV
  * @arg LL_PKA_MODE_ARITHMETIC_ADD
  * @arg LL_PKA_MODE_ARITHMETIC_SUB
  * @arg LL_PKA_MODE_ARITHMETIC_MUL
  * @arg LL_PKA_MODE_COMPARISON
  * @arg LL_PKA_MODE_MODULAR_REDUC
  * @arg LL_PKA_MODE_MODULAR_ADD
  * @arg LL_PKA_MODE_MODULAR_SUB
  * @arg LL_PKA_MODE_MONTGOMERY_MUL
  * @arg LL_PKA_MODE_ECC_COMPLETE_ADDITION
  * @arg LL_PKA_MODE_ECC_DOUBLE_BASE_LADDER
  */
__STATIC_INLINE uint32_t LL_PKA_GetMode(PKA_TypeDef *PKAx)
{
  return (uint32_t)(READ_BIT(PKAx->CR, PKA_CR_MODE) >> PKA_CR_MODE_Pos);
}

/**
  * @brief  Set the Security level.
  * @rmtoll CR           SECLVL         LL_PKA_SetSecurityLevel
  * @param  arg This parameter can be one of the following values:
  * @arg LL_PKA_SECURITY_LEVEL_NONE
  * @arg LL_PKA_SECURITY_LEVEL_ENABLE
  * @param  PKAx PKA Instance.
  * @retval NONE
  */
__STATIC_INLINE void LL_PKA_SetSecurityLevel(PKA_TypeDef *PKAx, uint32_t arg)
{
  MODIFY_REG(PKAx->CR, PKA_CR_SECLVL, (arg << PKA_CR_SECLVL_Pos));
}

/**
  * @brief  Get the Security level.
  * @rmtoll CR           SECLVL         LL_PKA_GetSecurityLevel
  * @param  PKAx PKA Instance.
  * @retval Returned value can be one of the following values:
  * @arg LL_PKA_SECURITY_LEVEL_NONE
  * @arg LL_PKA_SECURITY_LEVEL_ENABLE
  */
__STATIC_INLINE void LL_PKA_GetSecurityLevel(PKA_TypeDef *PKAx)
{
  SET_BIT(PKAx->CR, PKA_CR_SECLVL);
}

/**
  * @brief  Start the operation selected using LL_PKA_SetMode.
  * @rmtoll CR           START         LL_PKA_Start
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_Start(PKA_TypeDef *PKAx)
{
  SET_BIT(PKAx->CR, PKA_CR_START);
}

/**
  * @}
  */

/** @defgroup PKA_LL_EF_IT_Management IT_Management
  * @{
  */

#ifdef PKA_CR_FAULTERRORCODEIE
/**
  * @brief  Enable Fault error code interrupt.
  * @rmtoll CR           FAULTERRORCODEIE   LL_PKA_EnableIT_FAULTERRORCODE
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_EnableIT_FAULTERRORCODE(PKA_TypeDef *PKAx)
{
  SET_BIT(PKAx->CR, PKA_CR_FAULTERRORCODEIE);
}
#endif

#ifdef PKA_CR_FAULTFSMIE
/**
  * @brief  Enable Fault FSM interrupt.
  * @rmtoll CR           FAULTFSMIE    LL_PKA_EnableIT_FAULTFSM
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_EnableIT_FAULTFSM(PKA_TypeDef *PKAx)
{
  SET_BIT(PKAx->CR, PKA_CR_FAULTERRORCODEIE);
}
#endif

/**
  * @brief  Enable address error interrupt.
  * @rmtoll CR           ADDRERRIE     LL_PKA_EnableIT_ADDRERR
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_EnableIT_ADDRERR(PKA_TypeDef *PKAx)
{
  SET_BIT(PKAx->CR, PKA_CR_ADDRERRIE);
}

/**
  * @brief  Enable RAM error interrupt.
  * @rmtoll CR           RAMERRIE      LL_PKA_EnableIT_RAMERR
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_EnableIT_RAMERR(PKA_TypeDef *PKAx)
{
  SET_BIT(PKAx->CR, PKA_CR_RAMERRIE);
}


/**
  * @brief  Enable end of operation interrupt.
  * @rmtoll CR           PROCENDIE     LL_PKA_EnableIT_PROCEND
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_EnableIT_PROCEND(PKA_TypeDef *PKAx)
{
  SET_BIT(PKAx->CR, PKA_CR_PROCENDIE);
}

#ifdef PKA_CR_FAULTERRORCODEIE
/**
  * @brief  Disable Fault error code interrupt.
  * @rmtoll CR           FAULTERRORCODEIE   LL_PKA_DisableIT_FAULTERRORCODE
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_DisableIT_FAULTERRORCODE(PKA_TypeDef *PKAx)
{
  CLEAR_BIT(PKAx->CR, PKA_CR_FAULTERRORCODEIE);
}
#endif

#ifdef PKA_CR_FAULTFSMIE
/**
  * @brief  Disable Fault FSM interrupt.
  * @rmtoll CR           FAULTFSMIE    LL_PKA_DisableIT_FAULTFSM
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_DisableIT_FAULTFSM(PKA_TypeDef *PKAx)
{
  CLEAR_BIT(PKAx->CR, PKA_CR_FAULTERRORCODEIE);
}
#endif

/**
  * @brief  Disable address error interrupt.
  * @rmtoll CR           ADDRERRIE     LL_PKA_DisableIT_ADDRERR
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_DisableIT_ADDRERR(PKA_TypeDef *PKAx)
{
  CLEAR_BIT(PKAx->CR, PKA_CR_ADDRERRIE);
}

/**
  * @brief  Disable RAM error interrupt.
  * @rmtoll CR           RAMERRIE      LL_PKA_DisableIT_RAMERR
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_DisableIT_RAMERR(PKA_TypeDef *PKAx)
{
  CLEAR_BIT(PKAx->CR, PKA_CR_RAMERRIE);
}

/**
  * @brief  Disable End of operation interrupt.
  * @rmtoll CR           PROCENDIE     LL_PKA_DisableIT_PROCEND
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_DisableIT_PROCEND(PKA_TypeDef *PKAx)
{
  CLEAR_BIT(PKAx->CR, PKA_CR_PROCENDIE);
}


#ifdef PKA_CR_FAULTERRORCODEIE
/**
  * @brief  Check if Fault error code interrupt is enabled.
  * @rmtoll CR           FAULTERRORCODEIE   LL_PKA_IsEnabledIT_FAULTERRORCODE
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE uint32_t LL_PKA_IsEnabledIT_FAULTERRORCODE(PKA_TypeDef *PKAx)
{
  return ((READ_BIT(PKAx->CR, PKA_CR_FAULTERRORCODEIE) == (PKA_CR_FAULTERRORCODEIE)) ? 1UL : 0UL);
}
#endif

#ifdef PKA_CR_FAULTFSMIE
/**
  * @brief  Check if Fault FSM interrupt is enabled.
  * @rmtoll CR           FAULTFSMIE    LL_PKA_IsEnabledIT_FAULTFSM
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE uint32_t LL_PKA_IsEnabledIT_FAULTFSM(PKA_TypeDef *PKAx)
{
  return ((READ_BIT(PKAx->CR, PKA_CR_FAULTERRORCODEIE) == (PKA_CR_FAULTERRORCODEIE)) ? 1UL : 0UL);
}
#endif

/**
  * @brief  Check if address error interrupt is enabled.
  * @rmtoll CR           ADDRERRIE     LL_PKA_IsEnabledIT_ADDRERR
  * @param  PKAx PKA Instance.
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PKA_IsEnabledIT_ADDRERR(PKA_TypeDef *PKAx)
{
  return ((READ_BIT(PKAx->CR, PKA_CR_ADDRERRIE) == (PKA_CR_ADDRERRIE)) ? 1UL : 0UL);
}

/**
  * @brief  Check if RAM error interrupt is enabled.
  * @rmtoll CR           RAMERRIE      LL_PKA_IsEnabledIT_RAMERR
  * @param  PKAx PKA Instance.
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PKA_IsEnabledIT_RAMERR(PKA_TypeDef *PKAx)
{
  return ((READ_BIT(PKAx->CR, PKA_CR_RAMERRIE) == (PKA_CR_RAMERRIE)) ? 1UL : 0UL);
}


/**
  * @brief  Check if end of operation interrupt is enabled.
  * @rmtoll CR           PROCENDIE     LL_PKA_IsEnabledIT_PROCEND
  * @param  PKAx PKA Instance.
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PKA_IsEnabledIT_PROCEND(PKA_TypeDef *PKAx)
{
  return ((READ_BIT(PKAx->CR, PKA_CR_PROCENDIE) == (PKA_CR_PROCENDIE)) ? 1UL : 0UL);
}

/**
  * @}
  */

/** @defgroup PKA_LL_EF_FLAG_Management PKA flag management
  * @{
  */

/**
  * @brief  Get PKA address error flag.
  * @rmtoll SR           ADDRERRF      LL_PKA_IsActiveFlag_ADDRERR
  * @param  PKAx PKA Instance.
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PKA_IsActiveFlag_ADDRERR(PKA_TypeDef *PKAx)
{
  return ((READ_BIT(PKAx->SR, PKA_SR_ADDRERRF) == (PKA_SR_ADDRERRF)) ? 1UL : 0UL);
}

/**
  * @brief  Get PKA RAM error flag.
  * @rmtoll SR           RAMERRF       LL_PKA_IsActiveFlag_RAMERR
  * @param  PKAx PKA Instance.
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PKA_IsActiveFlag_RAMERR(PKA_TypeDef *PKAx)
{
  return ((READ_BIT(PKAx->SR, PKA_SR_RAMERRF) == (PKA_SR_RAMERRF)) ? 1UL : 0UL);
}


/**
  * @brief  Get PKA end of operation flag.
  * @rmtoll SR           PROCENDF      LL_PKA_IsActiveFlag_PROCEND
  * @param  PKAx PKA Instance.
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PKA_IsActiveFlag_PROCEND(PKA_TypeDef *PKAx)
{
  return ((READ_BIT(PKAx->SR, PKA_SR_PROCENDF) == (PKA_SR_PROCENDF)) ? 1UL : 0UL);
}

/**
  * @brief  Get PKA busy flag.
  * @rmtoll SR           BUSY          LL_PKA_IsActiveFlag_BUSY
  * @param  PKAx PKA Instance.
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PKA_IsActiveFlag_BUSY(PKA_TypeDef *PKAx)
{
  return ((READ_BIT(PKAx->SR, PKA_SR_BUSY) == (PKA_SR_BUSY)) ? 1UL : 0UL);
}

#ifdef LL_PKA_CLRFR_FAULTERRORCODEFC
/**
  * @brief  Clear PKA Fault error code flag.
  * @rmtoll CLRFR        FAULTERRORCODEFC LL_PKA_ClearFlag_FAULTERRORCODE
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_ClearFlag_FAULTERRORCODE(PKA_TypeDef *PKAx)
{
  SET_BIT(PKAx->CLRFR, PKA_CLRFR_FAULTERRORCODEFC);
}
#endif
#ifdef LL_PKA_CLRFR_FAULTFSMFC
/**
  * @brief  Clear PKA Fault error code flag.
  * @rmtoll CLRFR        FAULTFSMFC    LL_PKA_ClearFlag_FAULTFSM
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_ClearFlag_FAULTFSM(PKA_TypeDef *PKAx)
{
  SET_BIT(PKAx->CLRFR, PKA_CLRFR_FAULTFSMFC);
}
#endif


/**
  * @brief  Clear PKA address error flag.
  * @rmtoll CLRFR        ADDRERRFC     LL_PKA_ClearFlag_ADDRERR
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_ClearFlag_ADDRERR(PKA_TypeDef *PKAx)
{
  SET_BIT(PKAx->CLRFR, PKA_CLRFR_ADDRERRFC);
}

/**
  * @brief  Clear PKA RAM error flag.
  * @rmtoll CLRFR        RAMERRFC      LL_PKA_ClearFlag_RAMERR
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_ClearFlag_RAMERR(PKA_TypeDef *PKAx)
{
  SET_BIT(PKAx->CLRFR, PKA_CLRFR_RAMERRFC);
}


/**
  * @brief  Clear PKA end of operation flag.
  * @rmtoll CLRFR        PROCENDFC     LL_PKA_ClearFlag_PROCEND
  * @param  PKAx PKA Instance.
  * @retval None
  */
__STATIC_INLINE void LL_PKA_ClearFlag_PROCEND(PKA_TypeDef *PKAx)
{
  SET_BIT(PKAx->CLRFR, PKA_CLRFR_PROCENDFC);
}

/**
  * @}
  */

#if defined(USE_FULL_LL_DRIVER)

/** @defgroup PKA_LL_EF_Init Initialization and de-initialization functions
  * @{
  */

ErrorStatus LL_PKA_DeInit(PKA_TypeDef *PKAx);
ErrorStatus LL_PKA_Init(PKA_TypeDef *PKAx, LL_PKA_InitTypeDef *PKA_InitStruct);
void LL_PKA_StructInit(LL_PKA_InitTypeDef *PKA_InitStruct);
#if defined CONFIG_DEVICE_BLUENRG_LPF
void LL_PKA_WriteSingleInput( uint32_t index, int size, const uint32_t* word );
#elif defined CONFIG_DEVICE_BLUENRG_LPS
void LL_PKA_WriteSingleInput( uint32_t index, uint32_t word );
#endif
void LL_PKA_WriteOperand( uint32_t index, int size, const uint32_t* in );
void LL_PKA_WriteOperandByAddress( uint32_t address, int size, const uint32_t* in );
void LL_PKA_ReadResult( uint32_t index, int size, uint32_t* out );
#if defined CONFIG_DEVICE_BLUENRG_LPF
void LL_PKA_ReadSingleOutput( uint32_t index, uint32_t* out);
#elif defined CONFIG_DEVICE_BLUENRG_LPS
uint32_t LL_PKA_ReadSingleOutput( uint32_t index );
#endif

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */
/**
  * @}
  */

/**
  * @}
  */

#endif /* defined(PKA) */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* RF_DRIVER_LL_PKA_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
