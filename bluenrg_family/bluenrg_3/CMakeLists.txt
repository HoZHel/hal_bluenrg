# Copyright (c) 2020 STMicroelectronics
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
target_link_libraries(app PUBLIC bluenrg_lp_stack_controller_only.a)

zephyr_library_sources(Drivers/CMSIS/Device/ST/BlueNRG_LP/Source/system_BlueNRG_LP.c)
zephyr_library_sources(Drivers/Peripherals_Drivers/Src/rf_driver_ll_assert.c)
zephyr_library_sources(Drivers/Peripherals_Drivers/Src/rf_driver_ll_rcc.c)
zephyr_library_sources_ifdef(CONFIG_UART_BLUENRG Drivers/Peripherals_Drivers/Src/rf_driver_ll_usart.c)
zephyr_library_sources_ifdef(CONFIG_GPIO_BLUENRG Drivers/Peripherals_Drivers/Src/rf_driver_ll_gpio.c)

if(CONFIG_BT)
# Parameters hard coded to BlueNRG-LP, controller only
zephyr_compile_definitions(
  #-DBLE_STACK_FULL_CONF
  #-DBLE_STACK_BASIC_CONF
  -DBLE_STACK_CUSTOM_CONF
  -DBLESTACK_CONTROLLER_ONLY=1
  -DACI_HAL_GET_FIRMWARE_DETAILS_ENABLED=0
  -DACI_HAL_GET_FIRMWARE_DETAILS_V2_ENABLED=0
  -DACI_HAL_UPDATER_START_ENABLED=0
  -DACI_HAL_TRANSMITTER_TEST_PACKETS_ENABLED=0
  -DCONTROLLER_DATA_LENGTH_EXTENSION_ENABLED=1
  -DCONTROLLER_2M_CODED_PHY_ENABLED=1
  -DCONTROLLER_CHAN_CLASS_ENABLED=0
  -DCONNECTION_SUBRATING_ENABLED=0
  )
if(CONFIG_BT_CENTRAL)
zephyr_compile_definitions(
  -DCONTROLLER_MASTER_ENABLED=1
)
else()
zephyr_compile_definitions(
  -DCONTROLLER_MASTER_ENABLED=0
)
endif()
# This should be OK, but possibly there could be issue if we do simultaneous advertising and connection
if(CONFIG_BT_MAX_CONN)
zephyr_compile_definitions(
  -DCONFIG_NUM_MAX_LINKS=CONFIG_BT_MAX_CONN
)
else()
zephyr_compile_definitions(
  -DCONFIG_NUM_MAX_LINKS=1
)
endif()

if(CONFIG_BT_PRIVACY)
zephyr_compile_definitions(
  -DCONTROLLER_PRIVACY_ENABLED=1
)
else()
zephyr_compile_definitions(
  -DCONTROLLER_PRIVACY_ENABLED=0
)
endif()

if(CONFIG_BT_SMP)
zephyr_compile_definitions(
  -DSECURE_CONNECTIONS_ENABLED=1
)
else()
zephyr_compile_definitions(
  -DSECURE_CONNECTIONS_ENABLED=0
)
endif()

if(CONFIG_BT_EXT_ADV)
zephyr_compile_definitions(
  -DCONTROLLER_EXT_ADV_SCAN_ENABLED=1
)
else()
zephyr_compile_definitions(
  -DCONTROLLER_EXT_ADV_SCAN_ENABLED=0
)
endif()

if(CONFIG_BT_L2CAP_ECRED)
zephyr_compile_definitions(
  -DL2CAP_COS_ENABLED=1
)
else()
zephyr_compile_definitions(
  -DL2CAP_COS_ENABLED=0
)
endif()

if(CONFIG_BT_PER_ADV)
zephyr_compile_definitions(
  -DCONTROLLER_PERIODIC_ADV_ENABLED=1
)
else()
zephyr_compile_definitions(
  -DCONTROLLER_PERIODIC_ADV_ENABLED=0
)
endif()

if(CONFIG_BT_DF)
zephyr_compile_definitions(
  -DCONTROLLER_CTE_ENABLED=1
)
else()
zephyr_compile_definitions(
  -DCONTROLLER_CTE_ENABLED=0
)
endif()

if(CONFIG_BT_TRANSMIT_POWER_CONTROL)
zephyr_compile_definitions(
  -DCONTROLLER_POWER_CONTROL_ENABLED=1
)
else()
zephyr_compile_definitions(
  -DCONTROLLER_POWER_CONTROL_ENABLED=0
)
endif()

if(CONFIG_BT_CONN)
zephyr_compile_definitions(
  -DCONNECTION_ENABLED=1
)
else()
zephyr_compile_definitions(
  -DCONNECTION_ENABLED=0
)
endif()

if(CONFIG_BT_ISO_BROADCAST)
zephyr_compile_definitions(
  -DCONTROLLER_BIS_ENABLED=1
)
else()
zephyr_compile_definitions(
  -DCONTROLLER_BIS_ENABLED=0
)
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
zephyr_compile_definitions(
  -DCONTROLLER_CIS_ENABLED=1
)
else()
zephyr_compile_definitions(
  -DCONTROLLER_CIS_ENABLED=0
)
endif()

if(CONFIG_BT_EXT_ADV_MAX_ADV_SET)
zephyr_compile_definitions(
  -DNUM_ADV_SETS_CONF=CONFIG_BT_EXT_ADV_MAX_ADV_SET
)
else()
zephyr_compile_definitions(
  -DNUM_ADV_SETS_CONF=1
)
endif()

set_source_files_properties(hci_if/DTM/Src/DTM_cmd_db.c PROPERTIES COMPILE_FLAGS -Wno-array-bounds)
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
zephyr_library_sources(Middlewares/ST/Bluetooth_LE/src/stack_user_cfg.c)
zephyr_library_sources(Middlewares/ST/RNGMGR/Src/rng_manager.c)
zephyr_library_sources(hci_if/DTM/Src/DTM_cmd_db.c)
zephyr_library_sources(hci_if/DTM/Src/aci_adv_nwk.c)
zephyr_library_sources(hci_if/DTM/Src/adv_buff_alloc_tiny.c)
zephyr_library_sources(hci_if/DTM/Src/adv_buff_alloc.c)
zephyr_library_sources(hci_if/DTM/Src/aci_l2cap_nwk.c)
zephyr_library_sources(hci_if/DTM/Src/aci_gatt_nwk.c)
zephyr_library_sources(hci_if/DTM/Src/dm_alloc.c)
zephyr_library_sources(hci_if/DTM/Src/hci_parser.c)
endif()
