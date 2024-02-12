/******************** (C) COPYRIGHT 2020 STMicroelectronics ********************
* File Name          : dm_alloc.h
* Author             : SRA - BLE stack team
* Description        : Dinamic Memory Allocator
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifndef __DM_ALLOC_H__
#define __DM_ALLOC_H__
/******************************************************************************
 * Includes
 *****************************************************************************/
/******************************************************************************
 * CONSTANT SYMBOLS
 *****************************************************************************/
/******************************************************************************
 * LOCAL MACROS
 *****************************************************************************/
/******************************************************************************
 * TYPES
 *****************************************************************************/
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
void dm_init(uint16_t buffer_size, uint32_t *buffer_p);
void *dm_alloc(uint16_t size);
void *dm_realloc(void *buffer_p, uint16_t size);
void dm_free(void *buffer_p);

#endif
