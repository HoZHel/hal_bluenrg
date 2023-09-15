/**
  ******************************************************************************
  * @file    rf_driver_hal_pka_v75.c
  * @author  RF Application Team
  * @brief   PKA HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of public key accelerator(PKA):
  *           + Initialization and de-initialization functions
  *           + Start an operation
  *           + Retrieve the operation result
  *
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
    [..]
    The PKA HAL driver can be used as follows:

    (#) Declare a PKA_HandleTypeDef handle structure, for example: PKA_HandleTypeDef  hpka;

    (#) Initialize the PKA low level resources by implementing the HAL_PKA_MspInit() API:
        (##) Enable the PKA interface clock
        (##) NVIC configuration if you need to use interrupt process
            (+++) Configure the PKA interrupt priority
            (+++) Enable the NVIC PKA IRQ Channel

    (#) Initialize the PKA registers by calling the HAL_PKA_Init() API which trig
        HAL_PKA_MspInit().

    (#) Fill entirely the input structure corresponding to your operation:
        For instance: PKA_ModExpInTypeDef for HAL_PKA_ModExp().

    (#) Execute the operation (in polling or interrupt) and check the returned value.

    (#) Retrieve the result of the operation (For instance, HAL_PKA_ModExp_GetResult for
        HAL_PKA_ModExp operation). The function to gather the result is different for each
        kind of operation. The correspondence can be found in the following section.

    (#) Call the function HAL_PKA_DeInit() to restore the default configuration which trig
        HAL_PKA_MspDeInit().

    *** Polling mode operation ***
    ===================================
    [..]
      (+) When an operation is started in polling mode, the function returns when:
      (++) A timeout is encounter.
      (++) The operation is completed.

    *** Interrupt mode operation ***
    ===================================
    [..]
      (+) Add HAL_PKA_IRQHandler to the IRQHandler of PKA.
      (+) Enable the IRQ using HAL_NVIC_EnableIRQ().
      (+) When an operation is started in interrupt mode, the function returns immediatly.
      (+) When the operation is completed, the callback HAL_PKA_OperationCpltCallback is called.
      (+) When an error is encountered, the callback HAL_PKA_ErrorCallback is called.
      (+) To stop any operation in interrupt mode, use HAL_PKA_Abort().

    *** Utilities ***
    ===================================
    [..]
      (+) To clear the PKA RAM, use HAL_PKA_RAMReset().
      (+) To get current state, use HAL_PKA_GetState().
      (+) To get current error, use HAL_PKA_GetError().

    *** Callback registration ***
    =============================================
    [..]

     The compilation flag USE_HAL_PKA_REGISTER_CALLBACKS, when set to 1,
     allows the user to configure dynamically the driver callbacks.
     Use Functions @ref HAL_PKA_RegisterCallback()
     to register an interrupt callback.
    [..]

     Function @ref HAL_PKA_RegisterCallback() allows to register following callbacks:
       (+) OperationCpltCallback : callback for End of operation.
       (+) ErrorCallback         : callback for error detection.
       (+) MspInitCallback       : callback for Msp Init.
       (+) MspDeInitCallback     : callback for Msp DeInit.
     This function takes as parameters the HAL peripheral handle, the Callback ID
     and a pointer to the user callback function.
    [..]

     Use function @ref HAL_PKA_UnRegisterCallback to reset a callback to the default
     weak function.
    [..]

     @ref HAL_PKA_UnRegisterCallback takes as parameters the HAL peripheral handle,
     and the Callback ID.
     This function allows to reset following callbacks:
       (+) OperationCpltCallback : callback for End of operation.
       (+) ErrorCallback         : callback for error detection.
       (+) MspInitCallback       : callback for Msp Init.
       (+) MspDeInitCallback     : callback for Msp DeInit.
     [..]

     By default, after the @ref HAL_PKA_Init() and when the state is @ref HAL_PKA_STATE_RESET
     all callbacks are set to the corresponding weak functions:
     examples @ref HAL_PKA_OperationCpltCallback(), @ref HAL_PKA_ErrorCallback().
     Exception done for MspInit and MspDeInit functions that are
     reset to the legacy weak functions in the @ref HAL_PKA_Init()/ @ref HAL_PKA_DeInit() only when
     these callbacks are null (not registered beforehand).
    [..]

     If MspInit or MspDeInit are not null, the @ref HAL_PKA_Init()/ @ref HAL_PKA_DeInit()
     keep and use the user MspInit/MspDeInit callbacks (registered beforehand) whatever the state.
     [..]

     Callbacks can be registered/unregistered in @ref HAL_PKA_STATE_READY state only.
     Exception done MspInit/MspDeInit functions that can be registered/unregistered
     in @ref HAL_PKA_STATE_READY or @ref HAL_PKA_STATE_RESET state,
     thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
    [..]

     Then, the user first registers the MspInit/MspDeInit user callbacks
     using @ref HAL_PKA_RegisterCallback() before calling @ref HAL_PKA_DeInit()
     or @ref HAL_PKA_Init() function.
     [..]

     When the compilation flag USE_HAL_PKA_REGISTER_CALLBACKS is set to 0 or
     not defined, the callback registration feature is not available and all callbacks
     are set to the corresponding weak functions.

  @endverbatim
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

/* Includes ------------------------------------------------------------------*/
#include "rf_driver_hal.h"

/** @addtogroup RF_DRIVER_HAL_Driver
  * @{
  */

#if defined(PKA) && defined(HAL_PKA_MODULE_ENABLED)

/** @defgroup PKA PKA
  * @brief PKA HAL module driver.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup PKA_Private_Define PKA Private Define
  * @{
  */
#define PKA_RAM_SIZE 894U

/* Private macro -------------------------------------------------------------*/
#define __PKA_RAM_PARAM_END(TAB,INDEX)                do{                                   \
                                                                    TAB[INDEX] = 0UL;       \
                                                                  } while(0)
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup PKA_Private_Functions PKA Private Functions
  * @{
  */
uint32_t PKA_GetMode(PKA_HandleTypeDef *hpka);
HAL_StatusTypeDef PKA_PollEndOfOperation(PKA_HandleTypeDef *hpka, uint32_t Timeout, uint32_t Tickstart);
uint32_t PKA_CheckError(PKA_HandleTypeDef *hpka, uint32_t mode);
uint32_t PKA_GetBitSize_u8(uint32_t byteNumber);
uint32_t PKA_GetOptBitSize_u8(uint32_t byteNumber, uint8_t msb);
uint32_t PKA_GetBitSize_u32(uint32_t wordNumber);
uint32_t PKA_GetArraySize_u8(uint32_t bitSize);
void PKA_Memcpy_u32_to_u8(uint8_t dst[], __IO const uint32_t src[], size_t n);
void PKA_Memcpy_u8_to_u32(__IO uint32_t dst[], const uint8_t src[], size_t n);
void PKA_Memcpy_u32_to_u32(__IO uint32_t dst[], __IO const uint32_t src[], size_t n);
HAL_StatusTypeDef PKA_Process(PKA_HandleTypeDef *hpka, uint32_t mode, uint32_t Timeout);
HAL_StatusTypeDef PKA_Process_IT(PKA_HandleTypeDef *hpka, uint32_t mode);

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @defgroup PKA_Exported_Functions PKA Exported Functions
  * @{
  */

/** @defgroup PKA_Exported_Functions_Group1 Initialization and de-initialization functions
 *  @brief   Initialization and de-initialization functions
 *
@verbatim
 ===============================================================================
             ##### Initialization and de-initialization functions  #####
 ===============================================================================
    [..]  This subsection provides a set of functions allowing to initialize and
          deinitialize the PKAx peripheral:

      (+) User must implement HAL_PKA_MspInit() function in which he configures
          all related peripherals resources (CLOCK, IT and NVIC ).

      (+) Call the function HAL_PKA_Init() to configure the device.

      (+) Call the function HAL_PKA_DeInit() to restore the default configuration
          of the selected PKAx peripheral.

@endverbatim
  * @{
  */

/**
  * @brief  Initialize the PKA according to the specified
  *         parameters in the PKA_InitTypeDef and initialize the associated handle.
  * @param  hpka PKA handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PKA_Init(PKA_HandleTypeDef *hpka)
{
  HAL_StatusTypeDef err = HAL_OK;

  /* Check the PKA handle allocation */
  if (hpka != NULL)
  {
    /* Check the parameters */
    assert_param(IS_PKA_ALL_INSTANCE(hpka->Instance));

    if (hpka->State == HAL_PKA_STATE_RESET)
    {

#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
      /* Init the PKA Callback settings */
      hpka->OperationCpltCallback = HAL_PKA_OperationCpltCallback; /* Legacy weak OperationCpltCallback */
      hpka->ErrorCallback         = HAL_PKA_ErrorCallback;         /* Legacy weak ErrorCallback         */

      if (hpka->MspInitCallback == NULL)
      {
        hpka->MspInitCallback = HAL_PKA_MspInit; /* Legacy weak MspInit  */
      }

      /* Init the low level hardware */
      hpka->MspInitCallback(hpka);
#else
      /* Init the low level hardware */
      HAL_PKA_MspInit(hpka);
#endif /* USE_HAL_PKA_REGISTER_CALLBACKS */
    }

    /* Set the state to busy */
    hpka->State = HAL_PKA_STATE_BUSY;

    SET_BIT(hpka->Instance->CR, PKA_CR_EN);

    /* Reset any pending flag */
    SET_BIT(hpka->Instance->CLRFR, PKA_CLRFR_PROCENDFC | PKA_CLRFR_RAMERRFC | PKA_CLRFR_ADDRERRFC);

    /* Initialize the error code */
    hpka->ErrorCode = HAL_PKA_ERROR_NONE;

    /* Set the state to ready */
    hpka->State = HAL_PKA_STATE_READY;
  }
  else
  {
    err = HAL_ERROR;
  }

  return err;
}

/**
  * @brief  DeInitialize the PKA peripheral.
  * @param  hpka PKA handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PKA_DeInit(PKA_HandleTypeDef *hpka)
{
  HAL_StatusTypeDef err = HAL_OK;

  /* Check the PKA handle allocation */
  if (hpka != NULL)
  {
    /* Check the parameters */
    assert_param(IS_PKA_ALL_INSTANCE(hpka->Instance));

    /* Set the state to busy */
    hpka->State = HAL_PKA_STATE_BUSY;

    /* Reset the control register */
    /* This abort any operation in progress (PKA RAM content is not guaranted in this case) */
    hpka->Instance->CR = 0;

    /* Reset any pending flag */
    SET_BIT(hpka->Instance->CLRFR, PKA_CLRFR_PROCENDFC | PKA_CLRFR_RAMERRFC | PKA_CLRFR_ADDRERRFC);

#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
    if (hpka->MspDeInitCallback == NULL)
    {
      hpka->MspDeInitCallback = HAL_PKA_MspDeInit; /* Legacy weak MspDeInit  */
    }

    /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
    hpka->MspDeInitCallback(hpka);
#else
    /* DeInit the low level hardware: CLOCK, NVIC */
    HAL_PKA_MspDeInit(hpka);
#endif /* USE_HAL_PKA_REGISTER_CALLBACKS */

    /* Reset the error code */
    hpka->ErrorCode = HAL_PKA_ERROR_NONE;

    /* Reset the state */
    hpka->State = HAL_PKA_STATE_RESET;
  }
  else
  {
    err = HAL_ERROR;
  }

  return err;
}

/**
  * @brief  Initialize the PKA MSP.
  * @param  hpka PKA handle
  * @retval None
  */
__weak void HAL_PKA_MspInit(PKA_HandleTypeDef *hpka)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpka);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PKA_MspInit can be implemented in the user file
   */
}

/**
  * @brief  DeInitialize the PKA MSP.
  * @param  hpka PKA handle
  * @retval None
  */
__weak void HAL_PKA_MspDeInit(PKA_HandleTypeDef *hpka)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpka);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PKA_MspDeInit can be implemented in the user file
   */
}

#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
/**
  * @brief  Register a User PKA Callback
  *         To be used instead of the weak predefined callback
  * @param  hpka Pointer to a PKA_HandleTypeDef structure that contains
  *                the configuration information for the specified PKA.
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_PKA_OPERATION_COMPLETE_CB_ID End of operation callback ID
  *          @arg @ref HAL_PKA_ERROR_CB_ID Error callback ID
  *          @arg @ref HAL_PKA_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_PKA_MSPDEINIT_CB_ID MspDeInit callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PKA_RegisterCallback(PKA_HandleTypeDef *hpka, HAL_PKA_CallbackIDTypeDef CallbackID, pPKA_CallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (pCallback == NULL)
  {
    /* Update the error code */
    hpka->ErrorCode |= HAL_PKA_ERROR_INVALID_CALLBACK;

    return HAL_ERROR;
  }

  if (HAL_PKA_STATE_READY == hpka->State)
  {
    switch (CallbackID)
    {
      case HAL_PKA_OPERATION_COMPLETE_CB_ID :
        hpka->OperationCpltCallback = pCallback;
        break;

      case HAL_PKA_ERROR_CB_ID :
        hpka->ErrorCallback = pCallback;
        break;

      case HAL_PKA_MSPINIT_CB_ID :
        hpka->MspInitCallback = pCallback;
        break;

      case HAL_PKA_MSPDEINIT_CB_ID :
        hpka->MspDeInitCallback = pCallback;
        break;

      default :
        /* Update the error code */
        hpka->ErrorCode |= HAL_PKA_ERROR_INVALID_CALLBACK;

        /* Return error status */
        status = HAL_ERROR;
        break;
    }
  }
  else if (HAL_PKA_STATE_RESET == hpka->State)
  {
    switch (CallbackID)
    {
      case HAL_PKA_MSPINIT_CB_ID :
        hpka->MspInitCallback = pCallback;
        break;

      case HAL_PKA_MSPDEINIT_CB_ID :
        hpka->MspDeInitCallback = pCallback;
        break;

      default :
        /* Update the error code */
        hpka->ErrorCode |= HAL_PKA_ERROR_INVALID_CALLBACK;

        /* Return error status */
        status = HAL_ERROR;
        break;
    }
  }
  else
  {
    /* Update the error code */
    hpka->ErrorCode |= HAL_PKA_ERROR_INVALID_CALLBACK;

    /* Return error status */
    status =  HAL_ERROR;
  }

  return status;
}

/**
  * @brief  Unregister a PKA Callback
  *         PKA callback is redirected to the weak predefined callback
  * @param  hpka Pointer to a PKA_HandleTypeDef structure that contains
  *                the configuration information for the specified PKA.
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_PKA_OPERATION_COMPLETE_CB_ID End of operation callback ID
  *          @arg @ref HAL_PKA_ERROR_CB_ID Error callback ID
  *          @arg @ref HAL_PKA_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_PKA_MSPDEINIT_CB_ID MspDeInit callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PKA_UnRegisterCallback(PKA_HandleTypeDef *hpka, HAL_PKA_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (HAL_PKA_STATE_READY == hpka->State)
  {
    switch (CallbackID)
    {
      case HAL_PKA_OPERATION_COMPLETE_CB_ID :
        hpka->OperationCpltCallback = HAL_PKA_OperationCpltCallback; /* Legacy weak OperationCpltCallback */
        break;

      case HAL_PKA_ERROR_CB_ID :
        hpka->ErrorCallback = HAL_PKA_ErrorCallback;                 /* Legacy weak ErrorCallback        */
        break;

      case HAL_PKA_MSPINIT_CB_ID :
        hpka->MspInitCallback = HAL_PKA_MspInit;                     /* Legacy weak MspInit              */
        break;

      case HAL_PKA_MSPDEINIT_CB_ID :
        hpka->MspDeInitCallback = HAL_PKA_MspDeInit;                 /* Legacy weak MspDeInit            */
        break;

      default :
        /* Update the error code */
        hpka->ErrorCode |= HAL_PKA_ERROR_INVALID_CALLBACK;

        /* Return error status */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (HAL_PKA_STATE_RESET == hpka->State)
  {
    switch (CallbackID)
    {
      case HAL_PKA_MSPINIT_CB_ID :
        hpka->MspInitCallback = HAL_PKA_MspInit;                   /* Legacy weak MspInit              */
        break;

      case HAL_PKA_MSPDEINIT_CB_ID :
        hpka->MspDeInitCallback = HAL_PKA_MspDeInit;               /* Legacy weak MspDeInit            */
        break;

      default :
        /* Update the error code */
        hpka->ErrorCode |= HAL_PKA_ERROR_INVALID_CALLBACK;

        /* Return error status */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* Update the error code */
    hpka->ErrorCode |= HAL_PKA_ERROR_INVALID_CALLBACK;

    /* Return error status */
    status =  HAL_ERROR;
  }

  return status;
}

#endif /* USE_HAL_PKA_REGISTER_CALLBACKS */

/**
  * @}
  */


/**
  * @brief  Abort any ongoing operation.
  * @param  hpka PKA handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PKA_Abort(PKA_HandleTypeDef *hpka)
{
  HAL_StatusTypeDef err = HAL_OK;

  /* Clear EN bit */
  /* This abort any operation in progress (PKA RAM content is not guaranted in this case) */
  CLEAR_BIT(hpka->Instance->CR, PKA_CR_EN);
  SET_BIT(hpka->Instance->CR, PKA_CR_EN);

  /* Reset any pending flag */
  SET_BIT(hpka->Instance->CLRFR, PKA_CLRFR_PROCENDFC | PKA_CLRFR_RAMERRFC | PKA_CLRFR_ADDRERRFC);

  /* Reset the error code */
  hpka->ErrorCode = HAL_PKA_ERROR_NONE;

  /* Reset the state */
  hpka->State = HAL_PKA_STATE_READY;

  return err;
}

/**
  * @brief  Reset the PKA RAM.
  * @param  hpka PKA handle
  * @retval None
  */
void HAL_PKA_RAMReset(PKA_HandleTypeDef *hpka)
{
  uint32_t index;

  /* For each element in the PKA RAM */
  for (index = 0; index < PKA_RAM_SIZE; index++)
  {
    /* Clear the content */
    PKA_RAM->RAM[index] = 0UL;
  }
}

/**
  * @brief  This function handles PKA event interrupt request.
  * @param  hpka PKA handle
  * @retval None
  */
void HAL_PKA_IRQHandler(PKA_HandleTypeDef *hpka)
{
  uint32_t mode = PKA_GetMode(hpka);
  FlagStatus addErrFlag = __HAL_PKA_GET_FLAG(hpka, PKA_FLAG_ADDRERR);
  FlagStatus ramErrFlag = __HAL_PKA_GET_FLAG(hpka, PKA_FLAG_RAMERR);
  FlagStatus procEndFlag = __HAL_PKA_GET_FLAG(hpka, PKA_FLAG_PROCEND);

  /* Address error interrupt occurred */
  if ((__HAL_PKA_GET_IT_SOURCE(hpka, PKA_IT_ADDRERR) == SET) && (addErrFlag == SET))
  {
    hpka->ErrorCode |= HAL_PKA_ERROR_ADDRERR;

    /* Clear ADDRERR flag */
    __HAL_PKA_CLEAR_FLAG(hpka, PKA_FLAG_ADDRERR);
  }

  /* RAM access error interrupt occurred */
  if ((__HAL_PKA_GET_IT_SOURCE(hpka, PKA_IT_RAMERR) == SET) && (ramErrFlag == SET))
  {
    hpka->ErrorCode |= HAL_PKA_ERROR_RAMERR;

    /* Clear RAMERR flag */
    __HAL_PKA_CLEAR_FLAG(hpka, PKA_FLAG_RAMERR);
  }

  /* Check the operation success in case of ECDSA signature */
  if (mode == PKA_MODE_ECDSA_SIGNATURE)
  {
    /* If error output result is different from 0, ecdsa sign operation need to be repeated */
    if (PKA_RAM->RAM[PKA_ECDSA_SIGN_OUT_ERROR] != 0UL)
    {
      hpka->ErrorCode |= HAL_PKA_ERROR_OPERATION;
    }
  }
  /* Trigger the error callback if an error is present */
  if (hpka->ErrorCode != HAL_PKA_ERROR_NONE)
  {
#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
    hpka->ErrorCallback(hpka);
#else
    HAL_PKA_ErrorCallback(hpka);
#endif /* USE_HAL_PKA_REGISTER_CALLBACKS */
  }

  /* End Of Operation interrupt occurred */
  if ((__HAL_PKA_GET_IT_SOURCE(hpka, PKA_IT_PROCEND) == SET) && (procEndFlag == SET))
  {
    /* Clear PROCEND flag */
    __HAL_PKA_CLEAR_FLAG(hpka, PKA_FLAG_PROCEND);

    /* Set the state to ready */
    hpka->State = HAL_PKA_STATE_READY;

#if (USE_HAL_PKA_REGISTER_CALLBACKS == 1)
    hpka->OperationCpltCallback(hpka);
#else
    HAL_PKA_OperationCpltCallback(hpka);
#endif /* USE_HAL_PKA_REGISTER_CALLBACKS */
  }
}

/**
  * @brief  Process completed callback.
  * @param  hpka PKA handle
  * @retval None
  */
__weak void HAL_PKA_OperationCpltCallback(PKA_HandleTypeDef *hpka)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpka);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PKA_OperationCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  Error callback.
  * @param  hpka PKA handle
  * @retval None
  */
__weak void HAL_PKA_ErrorCallback(PKA_HandleTypeDef *hpka)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hpka);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PKA_ErrorCallback could be implemented in the user file
   */
}



void HAL_PKA_WriteSingleInput( uint32_t index,  int size, const uint32_t* word )
{
  /* Write the single word into PKA RAM */
  for(int i=0; i<size; i++)
  {
    PKA_RAM->RAM[index++] = *word++;
  }
}


void HAL_PKA_WriteOperand( uint32_t index, int size, const uint32_t* in )
{
  uint32_t* pka_ram = (uint32_t*)&PKA_RAM->RAM[index];
  /* Write the input data into PKA RAM */
  for ( ; size > 0; size-- )
  {
    *pka_ram++ = *in++;
  }
  /* Write extra zeros into PKA RAM */
  *pka_ram++ = 0;
  *pka_ram = 0;
}


void HAL_PKA_ReadResult( uint32_t index, int size, uint32_t* out )
{
  uint32_t* pka_ram = (uint32_t*)&PKA_RAM->RAM[index];
  /* Read from PKA RAM */
  for ( ; size > 0; size-- )
  {
    *out++ = *pka_ram++;
  }
}


void HAL_PKA_ReadSingleOutput( uint32_t index, uint32_t* out)
{
  HAL_PKA_ReadResult(index, 2, out);
}











/**
  * @}
  */

/** @defgroup PKA_Exported_Functions_Group3 Peripheral State and Error functions
 *  @brief   Peripheral State and Error functions
 *
  @verbatim
 ===============================================================================
            ##### Peripheral State and Error functions #####
 ===============================================================================
    [..]
    This subsection permit to get in run-time the status of the peripheral.

@endverbatim
  * @{
  */

/**
  * @brief  Return the PKA handle state.
  * @param  hpka PKA handle
  * @retval HAL status
  */
HAL_PKA_StateTypeDef HAL_PKA_GetState(PKA_HandleTypeDef *hpka)
{
  /* Return PKA handle state */
  return hpka->State;
}

/**
  * @brief  Return the PKA error code.
  * @param  hpka PKA handle
  * @retval PKA error code
*/
uint32_t HAL_PKA_GetError(PKA_HandleTypeDef *hpka)
{
  /* Return PKA handle error code */
  return hpka->ErrorCode;
}

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup PKA_Private_Functions
  * @{
  */

/**
  * @brief  Get PKA operating mode.
  * @param  hpka PKA handle
  * @retval Return the current mode
  */
uint32_t PKA_GetMode(PKA_HandleTypeDef *hpka)
{
  /* return the shifted PKA_CR_MODE value */
  return (uint32_t)(READ_BIT(hpka->Instance->CR, PKA_CR_MODE) >> PKA_CR_MODE_Pos);
}

/**
  * @brief  Wait for operation completion or timeout.
  * @param  hpka PKA handle
  * @param  Timeout Timeout duration in millisecond.
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
HAL_StatusTypeDef PKA_PollEndOfOperation(PKA_HandleTypeDef *hpka, uint32_t Timeout, uint32_t Tickstart)
{
  /* Wait for the end of operation or timeout */
  while ((hpka->Instance->SR & PKA_SR_PROCENDF) == 0UL)
  {
    /* Check if timeout is disabled (set to infinite wait) */
    if (Timeout != HAL_MAX_DELAY)
    {
      if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0UL))
      {
        return HAL_TIMEOUT;
      }
    }
  }
  return HAL_OK;
}

/**
  * @brief  Return a hal error code based on PKA error flags.
  * @param  hpka PKA handle
  * @param  mode PKA operating mode
  * @retval error code
  */
uint32_t PKA_CheckError(PKA_HandleTypeDef *hpka, uint32_t mode)
{
  uint32_t err = HAL_PKA_ERROR_NONE;

  /* Check RAMERR error */
  if (__HAL_PKA_GET_FLAG(hpka, PKA_FLAG_RAMERR) == SET)
  {
    err |= HAL_PKA_ERROR_RAMERR;
  }

  /* Check ADDRERR error */
  if (__HAL_PKA_GET_FLAG(hpka, PKA_FLAG_ADDRERR) == SET)
  {
    err |= HAL_PKA_ERROR_ADDRERR;
  }

  /* Check the operation success in case of ECDSA signature */
  if (mode == PKA_MODE_ECDSA_SIGNATURE)
  {
#define EDCSA_SIGN_NOERROR 0UL
    /* If error output result is different from no error, ecsa sign operation need to be repeated */
    if (PKA_RAM->RAM[PKA_ECDSA_SIGN_OUT_ERROR] != EDCSA_SIGN_NOERROR)
    {
      err |= HAL_PKA_ERROR_OPERATION;
    }
  }

  return err;
}

/**
  * @brief  Get number of bits inside an array of u8.
  * @param  byteNumber Number of u8 inside the array
  */
uint32_t PKA_GetBitSize_u8(uint32_t byteNumber)
{
  /* Convert from number of uint8_t in an array to the associated number of bits in this array */
  return byteNumber * 8UL;
}

/**
  * @brief  Get optimal number of bits inside an array of u8.
  * @param  byteNumber Number of u8 inside the array
  * @param  msb Most significant uint8_t of the array
  */
uint32_t PKA_GetOptBitSize_u8(uint32_t byteNumber, uint8_t msb)
{
  uint32_t position;

  position = 32UL - __CLZ(msb);

  return (((byteNumber - 1UL) * 8UL) + position);
}

/**
  * @brief  Get number of bits inside an array of u32.
  * @param  wordNumber Number of u32 inside the array
  */
uint32_t PKA_GetBitSize_u32(uint32_t wordNumber)
{
  /* Convert from number of uint32_t in an array to the associated number of bits in this array */
  return wordNumber * 32UL;
}

/**
  * @brief  Get number of uint8_t element in an array of bitSize bits.
  * @param  bitSize Number of bits in an array
  */
uint32_t PKA_GetArraySize_u8(uint32_t bitSize)
{
  /* Manage the non aligned on uint8_t bitsize: */
  /*   512 bits requires 64 uint8_t             */
  /*   521 bits requires 66 uint8_t             */
  return ((bitSize + 7UL) / 8UL);
}

/**
  * @brief  Copy uint32_t array to uint8_t array to fit PKA number representation.
  * @param  dst Pointer to destination
  * @param  src Pointer to source
  * @param  n Number of uint8_t to copy
  * @retval dst
  */
void PKA_Memcpy_u32_to_u8(uint8_t dst[], __IO const uint32_t src[], size_t n)
{
  if (dst != NULL)
  {
    if (src != NULL)
    {
      uint32_t index_uint32_t = 0UL; /* This index is used outside of the loop */

      for (; index_uint32_t < (n / 4UL); index_uint32_t++)
      {
        /* Avoid casting from uint8_t* to uint32_t* by copying 4 uint8_t in a row */
        /* Apply __REV equivalent */
        uint32_t index_uint8_t = n - 4UL - (index_uint32_t * 4UL);
        dst[index_uint8_t + 3UL] = (uint8_t)((src[index_uint32_t] & 0x000000FFU));
        dst[index_uint8_t + 2UL] = (uint8_t)((src[index_uint32_t] & 0x0000FF00U) >> 8UL);
        dst[index_uint8_t + 1UL] = (uint8_t)((src[index_uint32_t] & 0x00FF0000U) >> 16UL);
        dst[index_uint8_t + 0UL] = (uint8_t)((src[index_uint32_t] & 0xFF000000U) >> 24UL);
      }

      /* Manage the buffers not aligned on uint32_t */
      if ((n % 4UL) == 1UL)
      {
        dst[0UL] = (uint8_t)((src[index_uint32_t] & 0x000000FFU));
      }
      else if ((n % 4UL) == 2UL)
      {
        dst[1UL] = (uint8_t)((src[index_uint32_t] & 0x000000FFU));
        dst[0UL] = (uint8_t)((src[index_uint32_t] & 0x0000FF00U) >> 8UL);
      }
      else if ((n % 4UL) == 3UL)
      {
        dst[2UL] = (uint8_t)((src[index_uint32_t] & 0x000000FFU));
        dst[1UL] = (uint8_t)((src[index_uint32_t] & 0x0000FF00U) >> 8UL);
        dst[0UL] = (uint8_t)((src[index_uint32_t] & 0x00FF0000U) >> 16UL);
      }
      else
      {
        /* The last element is already handle in the loop */
      }
    }
  }
}

/**
  * @brief  Copy uint8_t array to uint32_t array to fit PKA number representation.
  * @param  dst Pointer to destination
  * @param  src Pointer to source
  * @param  n Number of uint8_t to copy (must be multiple of 4)
  * @retval dst
  */
void PKA_Memcpy_u8_to_u32(__IO uint32_t dst[], const uint8_t src[], size_t n)
{
  if (dst != NULL)
  {
    if (src != NULL)
    {
      uint32_t index = 0UL; /* This index is used outside of the loop */

      for (; index < (n / 4UL); index++)
      {
        /* Apply the equivalent of __REV from uint8_t to uint32_t */
        dst[index] = ((uint32_t)src[(n - (index * 4UL) - 1UL)]) \
                     | ((uint32_t)src[(n - (index * 4UL) - 2UL)] << 8UL) \
                     | ((uint32_t)src[(n - (index * 4UL) - 3UL)] << 16UL) \
                     | ((uint32_t)src[(n - (index * 4UL) - 4UL)] << 24UL);
      }

      /* Manage the buffers not aligned on uint32_t */
      if ((n % 4UL) == 1UL)
      {
        dst[index] = (uint32_t)src[(n - (index * 4UL) - 1UL)];
      }
      else if ((n % 4UL) == 2UL)
      {
        dst[index] = ((uint32_t)src[(n - (index * 4UL) - 1UL)]) \
                     | ((uint32_t)src[(n - (index * 4UL) - 2UL)] << 8UL);
      }
      else if ((n % 4UL) == 3UL)
      {
        dst[index] = ((uint32_t)src[(n - (index * 4UL) - 1UL)]) \
                     | ((uint32_t)src[(n - (index * 4UL) - 2UL)] << 8UL) \
                     | ((uint32_t)src[(n - (index * 4UL) - 3UL)] << 16UL);
      }
      else
      {
        /* The last element is already handle in the loop */
      }
    }
  }
}

/**
  * @brief  Copy uint32_t array to uint32_t array.
  * @param  dst Pointer to destination
  * @param  src Pointer to source
  * @param  n Number of u32 to be handled
  * @retval dst
  */
void PKA_Memcpy_u32_to_u32(__IO uint32_t dst[], __IO const uint32_t src[], size_t n)
{
  /* If a destination buffer is provided */
  if (dst != NULL)
  {
    /* If a source buffer is provided */
    if (src != NULL)
    {
      /* For each element in the array */
      for (uint32_t index = 0UL; index < n; index++)
      {
        /* Copy the content */
        dst[index] = src[index];
      }
    }
  }
}

/**
  * @brief  Generic function to start a PKA operation in blocking mode.
  * @param  hpka PKA handle
  * @param  mode PKA operation
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef PKA_Process(PKA_HandleTypeDef *hpka, uint32_t mode, uint32_t Timeout)
{
  HAL_StatusTypeDef err = HAL_OK;
  uint32_t tickstart;

  if (hpka->State == HAL_PKA_STATE_READY)
  {
    /* Set the state to busy */
    hpka->State = HAL_PKA_STATE_BUSY;

    /* Clear any pending error */
    hpka->ErrorCode = HAL_PKA_ERROR_NONE;

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();

    /* Set the mode and deactivate the interrupts */
    MODIFY_REG(hpka->Instance->CR, PKA_CR_MODE | PKA_CR_PROCENDIE | PKA_CR_RAMERRIE | PKA_CR_ADDRERRIE, mode << PKA_CR_MODE_Pos);

    /* Start the computation */
    hpka->Instance->CR |= PKA_CR_START;

    /* Wait for the end of operation or timeout */
    if (PKA_PollEndOfOperation(hpka, Timeout, tickstart) != HAL_OK)
    {
      /* Abort any ongoing operation */
      CLEAR_BIT(hpka->Instance->CR, PKA_CR_EN);

      hpka->ErrorCode |= HAL_PKA_ERROR_TIMEOUT;

      /* Make ready for the next operation */
      SET_BIT(hpka->Instance->CR, PKA_CR_EN);
    }

    /* Check error */
    hpka->ErrorCode |= PKA_CheckError(hpka, mode);

    /* Clear all flags */
    hpka->Instance->CLRFR |= (PKA_CLRFR_PROCENDFC | PKA_CLRFR_RAMERRFC | PKA_CLRFR_ADDRERRFC);

    /* Set the state to ready */
    hpka->State = HAL_PKA_STATE_READY;

    /* Manage the result based on encountered errors */
    if (hpka->ErrorCode != HAL_PKA_ERROR_NONE)
    {
      err = HAL_ERROR;
    }
  }
  else
  {
    err = HAL_ERROR;
  }
  return err;
}

/**
  * @brief  Generic function to start a PKA operation in non-blocking mode with Interrupt.
  * @param  hpka PKA handle
  * @param  mode PKA operation
  * @retval HAL status
  */
HAL_StatusTypeDef PKA_Process_IT(PKA_HandleTypeDef *hpka, uint32_t mode)
{
  HAL_StatusTypeDef err = HAL_OK;

  if (hpka->State == HAL_PKA_STATE_READY)
  {
    /* Set the state to busy */
    hpka->State = HAL_PKA_STATE_BUSY;

    /* Clear any pending error */
    hpka->ErrorCode = HAL_PKA_ERROR_NONE;

    /* Set the mode and activate interrupts */
    MODIFY_REG(hpka->Instance->CR, PKA_CR_MODE | PKA_CR_PROCENDIE | PKA_CR_RAMERRIE | PKA_CR_ADDRERRIE, (mode << PKA_CR_MODE_Pos) | PKA_CR_PROCENDIE | PKA_CR_RAMERRIE | PKA_CR_ADDRERRIE);

    /* Start the computation */
    hpka->Instance->CR |= PKA_CR_START;
  }
  else
  {
    err = HAL_ERROR;
  }
  return err;
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
