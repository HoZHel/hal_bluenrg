/**
 ******************************************************************************
 * @file    att_pwrq.h
 * @author  RF Application team
 * @version V1.0.0
 * @date    31 March 2020
 * @brief   Header file for EATT Prepare Write Queue implementation
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
 * <h2><center>&copy; COPYRIGHT STMicroelectronics</center></h2>
 ******************************************************************************
 */
#ifndef __EATT_PWRQ_H__
#define __EATT_PWRQ_H__

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "bluenrg_lp_gatt.h"
/******************************************************************************
 * MACROS
 *****************************************************************************/
 #define EATT_PWRQ_CID_ALL                          (0x0000U)
 #define EATT_PWRQ_ATTR_HANDLE_INVALID              (0x0000U)
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
/**
 *@defgroup PWRQ_APIs Prepare Write Queue support module
 *@brief Module to help managing prepare write queue
 *@{
 */

/**
 * @brief This function is used to initialize the PWRQ module
 *
 * @param queue_length[in] the queue buffer size.
 * @param queue_buffer_p[in] pointer to queue buffer memory area.
 *
 * @return
 * - BLE_STATUS_SUCCESS: the PWRQ module is initialized correctly.
 * - BLE_STATUS_FAILED: an error arise on initialization.
 *
 */
tBleStatus EATT_pwrq_init(uint16_t queue_length,
                          uint8_t *queue_buffer_p);

/**
 * @brief Reset PWRQ module flushing any present Prepare Writes.
 *
 * @return void
 *
 */
void EATT_pwrq_reset(void);

/**
 * @brief This function is used to store a prepare write request in the queue.
 *
 * @param conn_handle[in] The connection handle from which the request is received.
 * @param cid[in] The channel ID from which the request is received.
 * @param attr_h[in] Handle of attribute to write.
 * @param data_offset[in] Offset from which the write has to be start.
 * @param data_length[in] Length of data.
 * @param data[in] Data to write.
 *
 * @return
 * - BLE_STATUS_SUCCESS: the prepare write request is queued correctly
 * - BLE_STATUS_ERROR: the queue is full.
 */
tBleStatus EATT_pwrq_push(uint16_t conn_handle,
                          uint16_t cid,
                          uint16_t attr_h,
                          uint16_t data_offset,
                          uint16_t data_length,
                          uint8_t *data);

/**
 * @brief Flush prepared write for the given connection handle.
 *
 * @param conn_handle[in] The connection handle of the prepare write to flush.
 * @param cid[in] The channel ID of the prepare write to flush. If set to 0x0000 means
 *                flush any prepared write received from the indicated connection
 *                handle.
 *
 * @return void
 */
void EATT_pwrq_flush(uint16_t conn_handle,
                     uint16_t cid);

/**
 * @brief This function is used to read a prepare write from the queue.
 *
 * @param conn_handle[in] The connection handle from which the request is received.
 * @param cid[in] The channel ID of the prepare write to read. If set to EATT_PWRQ_CID_ALL
 *                means read any prepared write received from the indicated connection
 *                handle.
 * @param idx[in] The index of the prepare write in the queue.
 * @param wr_ops_p[in] The prepare write pointer.
 *
 * @return
 * - BLE_STATUS_SUCCESS: the prepare write request is found and returned correctly
 * - BLE_STATUS_ERROR: no prepare write request was found.
 */
tBleStatus EATT_pwrq_read(uint16_t conn_handle,
                          uint16_t cid,
                          uint16_t idx,
                          ble_gatt_clt_write_ops_t *wr_ops_p);

/**
 * @brief This function is used to extract a prepare write from the queue.
 *
 * @param conn_handle[in] The connection handle from which the request is received.
 * @param cid[in] The channel ID of the prepare write to pop. If set to EATT_PWRQ_CID_ALL
 *                means pop any prepared write received from the indicated connection
 *                handle.
 * @param attr_handle[in] The requested attribute handle to find. If it is set to
 *                        0x0000 then the first found prepare write entry will
 *                        be returned.
 * @param wr_ops_p[out] The prepare write pointer.
 *
 * @return
 * - BLE_STATUS_SUCCESS: the prepare write request is found and returned correctly
 * - BLE_STATUS_ERROR: no prepare write request was found.
 */
tBleStatus EATT_pwrq_pop(uint16_t conn_handle,
                         uint16_t cid,
                         uint16_t attr_handle,
                         ble_gatt_clt_write_ops_t *wr_ops_p);

/**
 *@}
 */

#endif

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/
