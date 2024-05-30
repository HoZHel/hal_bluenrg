# Copyright (c) 2023 STMicroelectronics
#
# SPDX-License-Identifier: Apache-2.0

zephyr_library()

if(CONFIG_SOC_BLUENRG_LP)
zephyr_compile_definitions(
  -DCONFIG_DEVICE_BLUENRG_LP
  )
endif()

if(CONFIG_SOC_BLUENRG_LPS)
zephyr_compile_definitions(
  -DCONFIG_DEVICE_BLUENRG_LPS
  )
endif()

if(CONFIG_SOC_BLUENRG_LPF)
zephyr_compile_definitions(
  -DCONFIG_DEVICE_BLUENRG_LPF
  )
endif()

zephyr_compile_definitions(
  -DUSE_FULL_LL_DRIVER
  -DCONFIG_HW_LS_XTAL
  -DCONFIG_HW_SMPS_10uH
  )
zephyr_include_directories(Drivers/CMSIS/Device/ST/BlueNRG_LP/Include)
zephyr_include_directories(Drivers/Peripherals_Drivers/Inc)
zephyr_include_directories(Drivers/BSP/Inc)
zephyr_include_directories(Drivers/BSP/Components/lsm6dsox_STdC/driver)
zephyr_include_directories(Drivers/BSP/Components/lps22hh_STdC/driver)
zephyr_include_directories(Middlewares/ST/BLECNTR/Inc)
zephyr_include_directories(Middlewares/ST/Bluetooth_LE/inc)
zephyr_include_directories(Middlewares/ST/hal/Inc)
zephyr_include_directories(Middlewares/ST/BLE_Application/layers_inc)
zephyr_include_directories(Middlewares/ST/BLE_Application/Profiles/Inc)
zephyr_include_directories(Middlewares/ST/BLE_Application/Queued_Write/Inc)
zephyr_include_directories(Middlewares/ST/RNGMGR/Inc)
zephyr_include_directories(Middlewares/ST/AESMGR/Inc)
zephyr_include_directories(Middlewares/ST/PKAMGR/Inc)
zephyr_include_directories(Middlewares/ST/cryptolib/inc)
zephyr_include_directories(hci_if/DTM/Inc)

target_link_directories(app PUBLIC Middlewares/ST/Bluetooth_LE/library)
if(CONFIG_BLE_STACK_VERSION_3_2a)
target_link_libraries(app PUBLIC bluenrg_lp_stack_controller_only.a)
elseif(CONFIG_BLE_STACK_VERSION_4)
target_link_libraries(app PUBLIC stm32wb0x_ble_stack_controller_only.a)
endif()

zephyr_library_sources(Drivers/CMSIS/Device/ST/BlueNRG_LP/Source/system_BlueNRG_LP.c)
zephyr_library_sources(Drivers/Peripherals_Drivers/Src/rf_driver_ll_assert.c)
zephyr_library_sources(Drivers/Peripherals_Drivers/Src/rf_driver_ll_rcc.c)
zephyr_library_sources_ifdef(CONFIG_UART_BLUENRG Drivers/Peripherals_Drivers/Src/rf_driver_ll_usart.c)
zephyr_library_sources_ifdef(CONFIG_GPIO_BLUENRG Drivers/Peripherals_Drivers/Src/rf_driver_ll_gpio.c)

########### Begining of modularity selection ###########
set(CFG_BLE_CONTROLLER_MASTER_ENABLED "0")
set(CFG_BLE_CONTROLLER_SCAN_ENABLED "0")
set(CFG_BLE_CONTROLLER_PRIVACY_ENABLED "0")
set(CFG_BLE_SECURE_CONNECTIONS_ENABLED "0")
set(CFG_BLE_CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED "0")
set(CFG_BLE_CONTROLLER_2M_CODED_PHY_ENABLED "1")
set(CFG_BLE_CONTROLLER_EXT_ADV_SCAN_ENABLED "0")
set(CFG_BLE_L2CAP_COS_ENABLED "0")
set(CFG_BLE_CONTROLLER_PERIODIC_ADV_ENABLED "0")
set(CFG_BLE_CONTROLLER_PERIODIC_ADV_WR_ENABLED "0")
set(CFG_BLE_CONTROLLER_CTE_ENABLED "0")
set(CFG_BLE_CONTROLLER_POWER_CONTROL_ENABLED "0")
set(CFG_BLE_CONNECTION_ENABLED "0")
set(CFG_BLE_CONTROLLER_CHAN_CLASS_ENABLED "0")
set(CFG_BLE_CONTROLLER_BIS_ENABLED "0")
set(CFG_BLE_CONNECTION_SUBRATING_ENABLED "0")
set(CFG_BLE_CONTROLLER_CIS_ENABLED "0")

if(CONFIG_BT)
# Parameters hard coded to BlueNRG-LP, controller only
zephyr_compile_definitions(
  -DBLESTACK_CONTROLLER_ONLY=1
  -DACI_HAL_GET_FIRMWARE_DETAILS_ENABLED=0
  -DACI_HAL_GET_FIRMWARE_DETAILS_V2_ENABLED=0
  -DACI_HAL_UPDATER_START_ENABLED=0
  -DACI_HAL_TRANSMITTER_TEST_PACKETS_ENABLED=0
  -DACI_HAL_WRITE_RADIO_REG_ENABLED=0
  -DACI_HAL_READ_RADIO_REG_ENABLED=0
  -DCONFIG_HW_HSE_TUNE=32
)
if(CONFIG_BLE_STACK_VERSION_3_2a)
zephyr_compile_definitions(
  -DBLE_STACK_CUSTOM_CONF
)
endif()

if(CONFIG_BT_OBSERVER)
	#the first one belongs to stack v3.2a
	set(CFG_BLE_CONTROLLER_MASTER_ENABLED "1")
	set(CFG_BLE_CONTROLLER_SCAN_ENABLED "1")
endif()

if(CONFIG_BT_PRIVACY)
	set(CFG_BLE_CONTROLLER_PRIVACY_ENABLED "1")
endif()

if(CONFIG_BT_SMP)
	set(CFG_BLE_SECURE_CONNECTIONS_ENABLED "1")
endif()

if(CONFIG_BT_CTLR_DATA_LENGTH GREATER 27)
	set(CFG_BLE_CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED "1")
endif()

if(CONFIG_BT_EXT_ADV)
	set(CFG_BLE_CONTROLLER_EXT_ADV_SCAN_ENABLED "1")
endif()

if(CONFIG_BT_L2CAP_ECRED)
	set(CFG_BLE_L2CAP_COS_ENABLED "1")
endif()

if(CONFIG_BT_PER_ADV OR CONFIG_BT_PER_ADV_SYNC)
	set(CFG_BLE_CONTROLLER_PERIODIC_ADV_ENABLED "1")
endif()

if(CONFIG_BT_PER_ADV_RSP AND CONFIG_BLE_STACK_VERSION_3_2a)
	message(SEND_ERROR "PAwR Not supported on stack version 3.2a")
elseif(CONFIG_BT_PER_ADV_RSP)
  set(CFG_BLE_CONTROLLER_PERIODIC_ADV_WR_ENABLED "1")
endif()

if(CONFIG_BT_DF)
	set(CFG_BLE_CONTROLLER_CTE_ENABLED "1")
endif()

if(CONFIG_BT_TRANSMIT_POWER_CONTROL)
	set(CFG_BLE_CONTROLLER_POWER_CONTROL_ENABLED "1")
endif()

if(CONFIG_BT_CONN)
	set(CFG_BLE_CONNECTION_ENABLED "1")
endif()

if(CONFIG_BT_ISO_BROADCAST)
	set(CFG_BLE_CONTROLLER_BIS_ENABLED "1")
endif()

if(CONFIG_BT_EATT)
zephyr_compile_definitions(
  -DEATT_ENABLED=1
)
else()
zephyr_compile_definitions(
  -DEATT_ENABLED=0
)
endif()

if(CONFIG_BT_ISO_UNICAST)
	set(CFG_BLE_CONTROLLER_CIS_ENABLED "1")
endif()

message(STATUS "BLE Stack modularity configuration:")
message(STATUS "CONTROLLER_SCAN_ENABLED=${CFG_BLE_CONTROLLER_SCAN_ENABLED}")
message(STATUS "CONTROLLER_PRIVACY_ENABLED=${CFG_BLE_CONTROLLER_PRIVACY_ENABLED}")
message(STATUS "SECURE_CONNECTIONS_ENABLED=${CFG_BLE_SECURE_CONNECTIONS_ENABLED}")
message(STATUS "CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED=${CFG_BLE_CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED}")
message(STATUS "CONTROLLER_2M_CODED_PHY_ENABLED=${CFG_BLE_CONTROLLER_2M_CODED_PHY_ENABLED}")
message(STATUS "CONTROLLER_EXT_ADV_SCAN_ENABLED=${CFG_BLE_CONTROLLER_EXT_ADV_SCAN_ENABLED}")
message(STATUS "L2CAP_COS_ENABLED=${CFG_BLE_L2CAP_COS_ENABLED}")
message(STATUS "CONTROLLER_PERIODIC_ADV_ENABLED=${CFG_BLE_CONTROLLER_PERIODIC_ADV_ENABLED}")
message(STATUS "CONTROLLER_PERIODIC_ADV_WR_ENABLED=${CFG_BLE_CONTROLLER_PERIODIC_ADV_WR_ENABLED}")
message(STATUS "CONTROLLER_CTE_ENABLED=${CFG_BLE_CONTROLLER_CTE_ENABLED}")
message(STATUS "CONTROLLER_POWER_CONTROL_ENABLED=${CFG_BLE_CONTROLLER_POWER_CONTROL_ENABLED}")
message(STATUS "CONNECTION_ENABLED=${CFG_BLE_CONNECTION_ENABLED}")
message(STATUS "CONTROLLER_CHAN_CLASS_ENABLED=${CFG_BLE_CONTROLLER_CHAN_CLASS_ENABLED}")
message(STATUS "CONTROLLER_BIS_ENABLED=${CFG_BLE_CONTROLLER_BIS_ENABLED}")
message(STATUS "CONNECTION_SUBRATING_ENABLED=${CFG_BLE_CONNECTION_SUBRATING_ENABLED}")
message(STATUS "CONTROLLER_CIS_ENABLED=${CFG_BLE_CONTROLLER_CIS_ENABLED}")

zephyr_compile_definitions(-DCONTROLLER_MASTER_ENABLED=${CFG_BLE_CONTROLLER_MASTER_ENABLED})
zephyr_compile_definitions(-DCONTROLLER_SCAN_ENABLED=${CFG_BLE_CONTROLLER_SCAN_ENABLED})
zephyr_compile_definitions(-DCONTROLLER_PRIVACY_ENABLED=${CFG_BLE_CONTROLLER_PRIVACY_ENABLED})
zephyr_compile_definitions(-DSECURE_CONNECTIONS_ENABLED=${CFG_BLE_SECURE_CONNECTIONS_ENABLED})
zephyr_compile_definitions(-DCONTROLLER_DATA_LENGTH_EXTENSION_ENABLED=${CFG_BLE_CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED})
zephyr_compile_definitions(-DCONTROLLER_2M_CODED_PHY_ENABLED=${CFG_BLE_CONTROLLER_2M_CODED_PHY_ENABLED})
zephyr_compile_definitions(-DCONTROLLER_EXT_ADV_SCAN_ENABLED=${CFG_BLE_CONTROLLER_EXT_ADV_SCAN_ENABLED})
zephyr_compile_definitions(-DL2CAP_COS_ENABLED=${CFG_BLE_L2CAP_COS_ENABLED})
zephyr_compile_definitions(-DCONTROLLER_PERIODIC_ADV_ENABLED=${CFG_BLE_CONTROLLER_PERIODIC_ADV_ENABLED})
zephyr_compile_definitions(-DCONTROLLER_PERIODIC_ADV_WR_ENABLED=${CFG_BLE_CONTROLLER_PERIODIC_ADV_WR_ENABLED})
zephyr_compile_definitions(-DCONTROLLER_CTE_ENABLED=${CFG_BLE_CONTROLLER_CTE_ENABLED})
zephyr_compile_definitions(-DCONTROLLER_POWER_CONTROL_ENABLED=${CFG_BLE_CONTROLLER_POWER_CONTROL_ENABLED})
zephyr_compile_definitions(-DCONNECTION_ENABLED=${CFG_BLE_CONNECTION_ENABLED})
zephyr_compile_definitions(-DCONTROLLER_CHAN_CLASS_ENABLED=${CFG_BLE_CONTROLLER_CHAN_CLASS_ENABLED})
zephyr_compile_definitions(-DCONTROLLER_BIS_ENABLED=${CFG_BLE_CONTROLLER_BIS_ENABLED})
zephyr_compile_definitions(-DCONNECTION_SUBRATING_ENABLED=${CFG_BLE_CONNECTION_SUBRATING_ENABLED})
zephyr_compile_definitions(-DCONTROLLER_CIS_ENABLED=${CFG_BLE_CONTROLLER_CIS_ENABLED})
########### End of modularity selection ###########

########### Beginning of numeric value section ###########
set(CFG_BLE_NUM_SYNC_SLOTS "0")
set(CFG_BLE_NUM_ADV_SETS "1")
set(bt_max_conn "0")

if(CONFIG_BT_PER_ADV_SYNC_MAX)
	set(CFG_BLE_NUM_SYNC_SLOTS "${CONFIG_BT_PER_ADV_SYNC_MAX}")
endif()

if(CONFIG_BT_EXT_ADV_MAX_ADV_SET)
	set(CFG_BLE_NUM_ADV_SETS "${CONFIG_BT_EXT_ADV_MAX_ADV_SET}")
endif()

if (CONFIG_BT_MAX_CONN)
	#Maximum number of simultaneous connections
	set(bt_max_conn "${CONFIG_BT_MAX_CONN}")
endif()

zephyr_compile_definitions(-DCFG_BLE_NUM_SYNC_SLOTS=${CFG_BLE_NUM_SYNC_SLOTS})

zephyr_compile_definitions(
  -DNUM_ADV_SETS_CONF=${CFG_BLE_NUM_ADV_SETS}
  -DCFG_BLE_NUM_ADV_SETS=${CFG_BLE_NUM_ADV_SETS}
)
########### End of numeric value section ###########

########### Beginning of number of radio tasks calculation ###########
if(${CFG_BLE_CONTROLLER_EXT_ADV_SCAN_ENABLED})
set(one_adv_tasks "2")
else()
set(one_adv_tasks "1")
endif()

set(tasks "0")

MATH(EXPR tasks "${tasks} + (${one_adv_tasks} * ${CFG_BLE_NUM_ADV_SETS})")
MATH(EXPR sub_opr "${bt_max_conn} - ${one_adv_tasks}")
MATH(EXPR mul_opr "${one_adv_tasks} * (${CFG_BLE_NUM_ADV_SETS} - 1)")
# Calculate max(bt_max_conn - one_adv_tasks, one_adv_tasks * (CFG_BLE_NUM_ADV_SETS - 1))
if(${sub_opr} GREATER ${mul_opr})
MATH(EXPR tasks "${tasks} + ${sub_opr}")
else()
MATH(EXPR tasks "${tasks} + ${mul_opr}")
endif()

if(${CFG_BLE_CONTROLLER_BIS_ENABLED})
MATH(EXPR tasks "${tasks} + 1")
endif()

if(${CFG_BLE_CONTROLLER_CIS_ENABLED})
MATH(EXPR tasks "${tasks} + 1")
endif()

if(${CFG_BLE_CONTROLLER_PERIODIC_ADV_WR_ENABLED})
MATH(EXPR tasks "${tasks} + 1")
endif()

message(STATUS "Number of computed radio tasks: ${tasks}")

zephyr_compile_definitions(
  -DCFG_BLE_NUM_RADIO_TASKS=${tasks}
)
########### End of number of radio tasks calculation ###########
set_source_files_properties(hci_if/DTM/Src/DTM_cmd_db.c PROPERTIES COMPILE_FLAGS -Wno-array-bounds)
set_source_files_properties(Middlewares/ST/Bluetooth_LE/src/stack_user_cfg.c PROPERTIES COMPILE_FLAGS -Wno-array-parameter)
zephyr_library_sources(Middlewares/ST/BLECNTR/Src/ble_controller_bluenrg_lp.c)
zephyr_library_sources(Middlewares/ST/BLECNTR/Src/ble_controller.c)
zephyr_library_sources(Drivers/Peripherals_Drivers/Src/rf_driver_hal_vtimer.c)
zephyr_library_sources(Drivers/Peripherals_Drivers/Src/rf_driver_ll_timer.c)
zephyr_library_sources(Middlewares/ST/hal/Src/miscutil.c)
zephyr_library_sources(Middlewares/ST/hal/Src/blue_unit_conversion.s)
zephyr_library_sources(Middlewares/ST/hal/Src/osal_memcpy.s)
zephyr_library_sources(Middlewares/ST/hal/Src/osal.c)
zephyr_library_sources(Middlewares/ST/hal/Src/hal_miscutil.c)
zephyr_library_sources(Middlewares/ST/PKAMGR/Src/pka_manager.c)

zephyr_library_sources_ifdef(CONFIG_SOC_BLUENRG_LP Middlewares/ST/PKAMGR/Src/pka_manager_bluenrg_lp.c)
zephyr_library_sources_ifdef(CONFIG_SOC_BLUENRG_LP Drivers/Peripherals_Drivers/Src/rf_driver_ll_pka_v7b.c)
zephyr_library_sources_ifdef(CONFIG_SOC_BLUENRG_LP Middlewares/ST/RNGMGR/Src/rng_manager_bluenrg_lp.c)

zephyr_library_sources_ifdef(CONFIG_SOC_BLUENRG_LPS Middlewares/ST/PKAMGR/Src/pka_manager_bluenrg_lps.c)
zephyr_library_sources_ifdef(CONFIG_SOC_BLUENRG_LPS Drivers/Peripherals_Drivers/Src/rf_driver_ll_pka_v7c.c)
zephyr_library_sources_ifdef(CONFIG_SOC_BLUENRG_LPS Middlewares/ST/RNGMGR/Src/rng_manager_bluenrg_lp.c)

zephyr_library_sources_ifdef(CONFIG_SOC_BLUENRG_LPF Middlewares/ST/PKAMGR/Src/pka_manager_bluenrg_lpf.c)
zephyr_library_sources_ifdef(CONFIG_SOC_BLUENRG_LPF Drivers/Peripherals_Drivers/Src/rf_driver_ll_pka_v7c.c)
zephyr_library_sources_ifdef(CONFIG_SOC_BLUENRG_LPF Middlewares/ST/RNGMGR/Src/rng_manager_bluenrg_lpf.c)

zephyr_library_sources(Middlewares/ST/AESMGR/Src/aes_manager_bluenrg_lp.c)
zephyr_library_sources_ifdef(CONFIG_BLE_STACK_VERSION_4 Middlewares/ST/Bluetooth_LE/src/stack_user_cfg.c)
zephyr_library_sources_ifdef(CONFIG_BLE_STACK_VERSION_3_2a Middlewares/ST/Bluetooth_LE/src/stack_user_cfg_3_2a.c)
zephyr_library_sources(Middlewares/ST/RNGMGR/Src/rng_manager.c)
zephyr_library_sources(hci_if/DTM/Src/DTM_cmd_db.c)
zephyr_library_sources(hci_if/DTM/Src/aci_adv_nwk.c)
# With legacy advertising only, alloc_tiny API will be used
zephyr_library_sources(hci_if/DTM/Src/adv_buff_alloc_tiny.c)
# With extended advertising, alloc API will be used
zephyr_library_sources(hci_if/DTM/Src/adv_buff_alloc.c)
zephyr_library_sources_ifdef(CONFIG_BLE_STACK_VERSION_4 hci_if/DTM/Src/pawr_buff_alloc.c)
zephyr_library_sources(hci_if/DTM/Src/aci_l2cap_nwk.c)
zephyr_library_sources(hci_if/DTM/Src/aci_gatt_nwk.c)
zephyr_library_sources(hci_if/DTM/Src/dm_alloc.c)
zephyr_library_sources(hci_if/DTM/Src/hci_parser.c)

else()
zephyr_compile_definitions(
  -DCFG_BLE_NUM_RADIO_TASKS=0
)
endif()
