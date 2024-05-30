#ifndef DTM_CONFIG_H
#define DTM_CONFIG_H

#if defined(CONFIG_BLE_STACK_VERSION_3_2a)
#include <DTM_config_3_2a.h>
#else
#include "DTM_config_4.h"
#endif

#endif // DTM_CONFIG_H