/**
******************************************************************************
* @file    pka_manager_bluenrg_lp.c
* @author  AMS - RF Application Team
* @brief   This file provides functions to implement sequential databases.
*
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2019 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "pka_manager_bluenrg_lpf.h"
#include "pka_manager.h"


/** @defgroup PKA_Manager_BlueNRG_LP  PKA Manager
* @{
*/

/** @defgroup PKA_Manager_BlueNRG_LP_TypesDefinitions Private Type Definitions
* @{
*/
/* Internal state*/
typedef enum
{
  PKAMGR_STEP_0 = 0x0,
  PKAMGR_STEP_1,
  PKAMGR_STEP_2,
  PKAMGR_STEP_3,
  PKAMGR_STEP_4,
  PKAMGR_STEP_5,
  PKAMGR_STEP_6,
  PKAMGR_STEP_7,
  PKAMGR_STEP_8,
  PKAMGR_STEP_9,
  PKAMGR_STEP_10,
  PKAMGR_STEP_11,
  PKAMGR_STEP_12,
  PKAMGR_STEP_13,
  PKAMGR_STEP_END_SUCCESS
} StateMachine_Step;

typedef enum
{
  PKAMGR_OPERATION_NONE     =  0,
  PKAMGR_OPERATION_P256
} StateMachine_operation;

/**
* @}
*/

/** @defgroup PKA_Manager_BlueNRG_LP_Private_Defines Private Defines
* @{
*/

/**
* @}
*/

/** @defgroup PKA_Manager_BlueNRG_LP_Private_Macros Private Macros
* @{
*/



/**
* @}
*/

/** @defgroup PKA_Manager_BlueNRG_LP_Private_Variables Private Variables
* @{
*/
static volatile uint32_t internalStateMachine_Operation = PKAMGR_OPERATION_NONE;
static volatile uint32_t internalStateMachine_Step = 0;  

static volatile uint32_t bufferSecretKey[8] = {0};
static volatile uint32_t bufferPublicKey[16] = {0};


static uint32_t ret[24] = {0}; 

#define VERIFY_ECC_SCALAR_MUL_RESULT 1


static const uint32_t PKAMGR_oplen[2] = {
0x100,
0x0
};

static const uint32_t PKAMGR_nbits_k[2] = {
0x100,
0x0
};
   
static const uint32_t PKAMGR_nbits_m[2] = {
0x100,
0x0
};
                                
static const uint32_t PKAMGR_a_coeff_sign[2] = {
0x1,
0x0
};

static const uint32_t PKAMGR_P256_gfp[8] =
{
  0xFFFFFFFF, /* LSB */
  0xFFFFFFFF,
  0xFFFFFFFF,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000001,
  0xFFFFFFFF,
};

static const uint32_t PKAMGR_P256_r2[8] =
{
  0x00000002, /* LSB */
  0x00000005,
  0x00000003,
  0xFFFFFFFE,
  0xFFFFFFF9,
  0xFFFFFFFB,
  0xFFFFFFFC,
  0xFFFFFFFC,
};


static const uint32_t PKAMGR_P256_a[8] =
{
  0x00000003, /* LSB */
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
};

static const uint32_t PKAMGR_P256_b[8] =
{
  0x27D2604B, /* LSB */
  0x3BCE3C3E,
  0xCC53B0F6,
  0x651D06B0,
  0x769886BC,
  0xB3EBBD55,
  0xAA3A93E7,
  0x5AC635D8,
};

/**
* @}
*/

/** @defgroup PKA_Manager_BlueNRG_LP_External_Variables External Variables
* @{
*/



/**
* @}
*/

/** @defgroup PKA_Manager_BlueNRG_LP_Private_FunctionPrototypes Private Function Prototypes
* @{
*/
PKAMGR_ResultStatus PKAMGR_PrivateInit(void);
PKAMGR_ResultStatus PKAMGR_PrivateDeinit(void);
void (*PKAMGR_funcCB_LP)(PKAMGR_ResultStatus error_code, void *args_p);
void PKAMGR_ExitWithError(uint32_t errorCode);
void PKAMGR_ProcEnd_StateMachine(void);
void PKAMGR_Comparison( const uint32_t* opLen, const uint32_t* op1 , const uint32_t* op2 );
void PKAMGR_P256_StartPointCheck( const uint32_t* x, const uint32_t* y );
uint32_t PKAMGR_IsPointCheckOk(void);
uint32_t PKAMGR_IsRangeCheckOk(void);
void PKAMGR_P256_StartEccScalarMul( const uint32_t* k, const uint32_t* pX, const uint32_t* pY );
void PKAMGR_P256_ReadEccScalarMul(uint32_t* pX, uint32_t* pY);
uint32_t PKAMGR_isStartPoint(const uint32_t* p);
/**
* @}
*/

/** @defgroup PKA_Manager_BlueNRG_LP_Private_Functions Private Functions
* @{
*/

/** 
  * @brief  Init function fot BlueNRG-LPF PKA version. Enable peripheral clock, configure the NVIC, enable PKA and all PKA interrupd sources.
  * @param  none
  * @param  none
  * 
  */
PKAMGR_ResultStatus PKAMGR_PrivateInit(void)
{
  /* Peripheral clock enable */
  LL_AHB_EnableClock(LL_AHB_PERIPH_PKA);

  /* Configure NVIC for PKA interrupts */
  /*   Set priority for PKA_IRQn */
  /*   Enable PKA_IRQn */
  NVIC_SetPriority(PKA_IRQn, IRQ_LOW_PRIORITY);  
  NVIC_EnableIRQ(PKA_IRQn);
  
  LL_PKA_Enable(PKA);
  
  LL_PKA_EnableIT_ADDRERR(PKA);
  LL_PKA_EnableIT_RAMERR(PKA);
  LL_PKA_EnableIT_PROCEND(PKA);
  
  return PKAMGR_SUCCESS;
}

/** 
  * @brief  
  * @param  
  * @param  
  * 
  */
PKAMGR_ResultStatus PKAMGR_PrivateDeinit(void)
{
  /* Force PKA reset */
  LL_AHB_ForceReset(LL_AHB_PERIPH_PKA);

  /* Release PKA reset */
  LL_AHB_ReleaseReset(LL_AHB_PERIPH_PKA);
    /* Disable PKA clock */
  LL_AHB_DisableClock(LL_AHB_PERIPH_PKA);
  
  return PKAMGR_SUCCESS;
}

uint8_t PKAMGR_PowerSaveLevelCheck(uint8_t x){
  if(PKAMGR_SleepCheck()==PKAMGR_SUCCESS)
  {
    return POWER_SAVE_LEVEL_STOP_NOTIMER;
  }
  else // PKA on going
  {
    return POWER_SAVE_LEVEL_CPU_HALT;
  }
}

/** 
  * @brief  
  * @param  
  * @param  
  * 
  */
PKAMGR_ResultStatus PKAMGR_StartP256DHkeyGeneration(const uint32_t* secretKey, const uint32_t* publicKey, PKAMGR_funcCB funcCB)
{  
  /* Set the PKA internal state to busy */
  if(PKAMGR_Lock()!=PKAMGR_SUCCESS)
    return PKAMGR_ERR_BUSY;
 
  PKAMGR_funcCB_LP = funcCB;

  /* Save input data */
  for(int i=0;i<8;i++)
    bufferSecretKey[i] = secretKey[i];
  for(int i=0;i<16;i++)
    bufferPublicKey[i] = publicKey[i];
  
  if(PKAMGR_isStartPoint(publicKey))
  {
    /* p256 public key generation */
    internalStateMachine_Step = PKAMGR_STEP_3;
    internalStateMachine_Operation = PKAMGR_OPERATION_P256;
    PKAMGR_P256_StartEccScalarMul( secretKey, (uint32_t *)&publicKey[0], (uint32_t *)&publicKey[8] );       
  }
  else
  { 
    /* p256 dhkey generation */
    internalStateMachine_Step = PKAMGR_STEP_0;
    internalStateMachine_Operation = PKAMGR_OPERATION_P256;
    /* Call the PKA range check operation for public key X coordinate */
    PKAMGR_Comparison(PKAMGR_oplen, (uint32_t *)&publicKey[0], PKAMGR_P256_gfp);
    
#if !defined(ASYNC_MODE)
    // blocking mode (intial check steps interval time: 0.476ms)
    while(internalStateMachine_Step!=PKAMGR_STEP_1);
    
    /* Test result of range check operation for public key X coordinate */
    if( !PKAMGR_IsRangeCheckOk() )
    {
      internalStateMachine_Operation = PKAMGR_OPERATION_NONE;  
      internalStateMachine_Step = PKAMGR_STEP_0;     
      PKAMGR_Unlock();
      return PKAMGR_ERR_PARAM;
    }
    else
    {
      /* Call the PKA range check operation for public key Y coordinate */
      PKAMGR_Comparison(PKAMGR_oplen, (uint32_t *)&bufferPublicKey[8], PKAMGR_P256_gfp);
    }
    
    while(internalStateMachine_Step!=PKAMGR_STEP_2);
    
    /* Test result of range check operation for public key X coordinate */
    if( !PKAMGR_IsRangeCheckOk() )
    {
      internalStateMachine_Operation = PKAMGR_OPERATION_NONE;  
      internalStateMachine_Step = PKAMGR_STEP_0;     
      PKAMGR_Unlock();
      return PKAMGR_ERR_PARAM;
    }
    else
    {
      /* Call the PKA point check operation for remote public key */
      PKAMGR_P256_StartPointCheck( (uint32_t *)&bufferPublicKey[0], (uint32_t *)&bufferPublicKey[8] );
    }
    
    while(internalStateMachine_Step!=PKAMGR_STEP_3);
    
    /* Test result of point check operation for remote public key */
    if ( !PKAMGR_IsPointCheckOk() )
    {
      internalStateMachine_Operation = PKAMGR_OPERATION_NONE;  
      internalStateMachine_Step = PKAMGR_STEP_0;     
      PKAMGR_Unlock();
      return PKAMGR_ERR_PARAM;
    }
    else
    {
      /* Call the PKA scalar multiplication with the local private key
         as k and the remote public key as starting point,
         in order to compute the DH key */
      PKAMGR_P256_StartEccScalarMul( (uint32_t *)&bufferSecretKey[0], (uint32_t *)&bufferPublicKey[0], (uint32_t *)&bufferPublicKey[8] );
    }
    
#endif /* !defined(ASYNC_MODE) */  
  }
  
  return PKAMGR_SUCCESS;
}

/** 
  * @brief  
  * @param  
  * @param  
  * 
  */
void PKAMGR_ExitWithError(uint32_t errorCode)
{ 
  internalStateMachine_Operation = 0;
  internalStateMachine_Step = 0;
  PKAMGR_Unlock();
  ret[0]=errorCode;
  for(int i=0; i<8; i++)
  {
    ret[i+16] = bufferSecretKey[i];
  }
  PKAMGR_funcCB_LP(PKAMGR_ERROR, ret); 
}

/** 
  * @brief  
  * @param  
  * @param  
  * 
  */
void PKAMGR_ProcEnd_StateMachine(void)
{
  if(internalStateMachine_Operation == PKAMGR_OPERATION_P256)
  {
    switch(internalStateMachine_Step)
    {
      case PKAMGR_STEP_0:
        internalStateMachine_Step = PKAMGR_STEP_1;
       
      break;
      case PKAMGR_STEP_1:
        internalStateMachine_Step = PKAMGR_STEP_2;  
        break;
      case PKAMGR_STEP_2:
          internalStateMachine_Step = PKAMGR_STEP_3; 
          break;
      case PKAMGR_STEP_3:
            /* Read the PKA scalar multiplication result which is the DH key */
            for(int i=0;i<8;i++)
              ret[i] = bufferSecretKey[i];
            PKAMGR_P256_ReadEccScalarMul( (uint32_t *)&ret[8], (uint32_t *)&ret[16] );

            internalStateMachine_Step = PKAMGR_STEP_END_SUCCESS;
            PKAMGR_ProcEnd_StateMachine();

          break;
       case PKAMGR_STEP_END_SUCCESS:
          internalStateMachine_Operation = PKAMGR_OPERATION_NONE;  
          internalStateMachine_Step = PKAMGR_STEP_0;     
          PKAMGR_Unlock();
          for(int i=0;i<8;i++)
            ret[i] = bufferSecretKey[i];
          PKAMGR_funcCB_LP(PKAMGR_SUCCESS, ret);
          break;
       default:
          PKAMGR_ExitWithError(0xA0);
    }
  }
}

/**
  * Brief   This function handles PKA Instance interrupt request.
  * Param   None
  * Retval  None
  */
void PKAMGR_IRQHandler(void)
{
   /* Manage the PKA RAM error flag */
  if(LL_PKA_IsActiveFlag_RAMERR(PKA) == 1)
  {
    LL_PKA_ClearFlag_RAMERR(PKA);
    PKAMGR_ExitWithError(0xB1);
  }
  
  /* Manage the Address error flag */
  if(LL_PKA_IsActiveFlag_ADDRERR(PKA) == 1)
  {
    LL_PKA_ClearFlag_ADDRERR(PKA);
    PKAMGR_ExitWithError(0xB2);
  }
  
  /* Manage the PKA End of Operation flag */
  if(LL_PKA_IsActiveFlag_PROCEND(PKA) == 1)
  {
    LL_PKA_ClearFlag_PROCEND(PKA);
    PKAMGR_ProcEnd_StateMachine();
  }
}

/** 
  * @brief  
  * @param  
  * @param  
  * 
  */
uint32_t PKAMGR_IsPointCheckOk(void)
{
  uint32_t result[2] = {0};
  LL_PKA_ReadResult( PKA_POINT_CHECK_OUT_ERROR, 2, &result[0] );
  return ( result[0] == 0xD60D);
}

/** 
  * @brief  
  * @param  
  * @param  
  * 
  */
uint32_t PKAMGR_IsRangeCheckOk(void)
{
  uint32_t result[2] = {0};
  LL_PKA_ReadResult( PKA_COMPARISON_OUT_RESULT, 2, &result[0] );
  return ( result[0] == 0x916A);
}

/** 
  * @brief  
  * @param  
  * @param  
  * 
  */
void PKAMGR_P256_StartEccScalarMul( const uint32_t* k, const uint32_t* pX, const uint32_t* pY )
{
  /* Set the configuration */
  LL_PKA_Config( PKA, LL_PKA_MODE_ECC_KP_PRIMITIVE );
  
  /* Enable the Security level */
  LL_PKA_SetSecurityLevel(PKA, LL_PKA_SECURITY_LEVEL_ENABLE);
  
  /* Set the scalar multiplier k length */
  LL_PKA_WriteSingleInput( PKA_ECC_SCALAR_MUL_IN_EXP_NB_BITS, 2, PKAMGR_nbits_k);
  /* Set the modulus length */
  LL_PKA_WriteSingleInput( PKA_ECC_SCALAR_MUL_IN_OP_NB_BITS, 2, PKAMGR_nbits_m);
  /* Set the coefficient a sign */
  LL_PKA_WriteSingleInput( PKA_ECC_SCALAR_MUL_IN_A_COEFF_SIGN, 2, PKAMGR_a_coeff_sign);
  /* Set the coefficient |a| */
  LL_PKA_WriteOperand( PKA_ECC_SCALAR_MUL_IN_A_COEFF, 8, PKAMGR_P256_a );
  /* Set the modulus value p */
  LL_PKA_WriteOperand( PKA_ECC_SCALAR_MUL_IN_MOD_GF, 8, PKAMGR_P256_gfp );
  /* Set the Montgomery parameter */
  LL_PKA_WriteOperand( PKA_ECC_SCALAR_MUL_IN_MONTGOMERY_PARAM, 8, PKAMGR_P256_r2 );
  /* Set the scalar multiplier k */
  LL_PKA_WriteOperand( PKA_ECC_SCALAR_MUL_IN_K, 8, k );
  /* Set the point P coordinate x */
  LL_PKA_WriteOperand( PKA_ECC_SCALAR_MUL_IN_INITIAL_POINT_X, 8, pX );
  /* Set the point P coordinate y */
  LL_PKA_WriteOperand( PKA_ECC_SCALAR_MUL_IN_INITIAL_POINT_Y, 8, pY );

  /* Wait for  PKA busy flag equeal to 0 */
  while(LL_PKA_IsActiveFlag_BUSY(PKA));
    
  /* Start the PKA processing */
  LL_PKA_Start( PKA );
}

/** 
  * @brief  
  * @param  
  * @param  
  * 
  */                   
void PKAMGR_Comparison( const uint32_t* opLen, const uint32_t* op1 , const uint32_t* op2 )
{
  LL_PKA_SetMode(PKA, LL_PKA_MODE_COMPARISON );
  
  /* Loads the input buffers to PKA RAM */
   /* Set the muber of bits of P */
  LL_PKA_WriteSingleInput( PKA_COMPARISON_NB_BITS, 2, opLen );
  /* Set the op1 */
  LL_PKA_WriteOperand( PKA_COMPARISON_IN_OP1, 8, op1 );
  /* Set the op2 */
  LL_PKA_WriteOperand( PKA_COMPARISON_IN_OP2, 8, op2 );

  /* Wait for  PKA busy flag equeal to 0 */
  while(LL_PKA_IsActiveFlag_BUSY(PKA));
  
  /* Launch the computation in interrupt mode */
  LL_PKA_Start(PKA);
}

void PKAMGR_P256_StartPointCheck( const uint32_t* x, const uint32_t* y )
{  
  /* Set the configuration */
  LL_PKA_Config( PKA, LL_PKA_MODE_POINT_CHECK );

  /* Set the muber of bits of p */
  LL_PKA_WriteSingleInput(  PKA_POINT_CHECK_IN_MOD_NB_BITS, 2, PKAMGR_nbits_m);
  /* Set the coefficient a sign */
  LL_PKA_WriteSingleInput( PKA_POINT_CHECK_IN_A_COEFF_SIGN, 2, PKAMGR_a_coeff_sign);
  /* Set the coefficient |a| */
  LL_PKA_WriteOperand( PKA_POINT_CHECK_IN_A_COEFF, 8, PKAMGR_P256_a );
  /* Set the modulus value p */
  LL_PKA_WriteOperand( PKA_POINT_CHECK_IN_MOD_GF, 8, PKAMGR_P256_gfp );
  /* Set the point coordinate x */
  LL_PKA_WriteOperand( PKA_POINT_CHECK_IN_INITIAL_POINT_X, 8, x );
  /* Set the point coordinate y */
  LL_PKA_WriteOperand( PKA_POINT_CHECK_IN_INITIAL_POINT_Y, 8, y );
  /* Set the Montgomery parameter */
  LL_PKA_WriteOperand( PKA_ECC_SCALAR_MUL_IN_MONTGOMERY_PARAM, 8, PKAMGR_P256_r2 );
  /* Set the coefficient |b| */
  LL_PKA_WriteOperand( PKA_POINT_CHECK_IN_B_COEFF, 8, PKAMGR_P256_b );

  /* Wait for  PKA busy flag equeal to 0 */
  while(LL_PKA_IsActiveFlag_BUSY(PKA));

  /* Start the PKA processing */
  LL_PKA_Start( PKA );
}


/** 
  * @brief  
  * @param  
  * @param  
  * 
  */
void PKAMGR_P256_ReadEccScalarMul( uint32_t* pX, uint32_t* pY )
{
  /* Read the output point X */
  if( pX )
  {
    LL_PKA_ReadResult( PKA_ECC_SCALAR_MUL_OUT_RESULT_X, 8, pX );
  }
  /* Read the output point Y as the second half of the result */
  if( pY )
  {
    LL_PKA_ReadResult( PKA_ECC_SCALAR_MUL_OUT_RESULT_Y, 8, pY );
  }
}

/**
  * Brief   This function checks if the param p is equal to the Start Point.
  * Param   Publick key p
  * Retval  1 if p is equal to the Start Point. 0 in other cases.
  */
uint32_t PKAMGR_isStartPoint(const uint32_t* p)
{
  for(int i=0; i<16; i++)
  {
    if(PKAStartPoint[i]!=p[i])
      return 0;
  }
  return 1;
}

/**
* @}
*/

/** @defgroup PKA_Manager_BlueNRG_LP_Public_Functions Public Functions
* @{
*/


/**
* @}
*/

/**
* @}
*/
