#ifndef __CUSTOM_PROFILE_H__
#define __CUSTOM_PROFILE_H__

#include <stdint.h>
#include "app_util.h"
#if defined(__cplusplus)
extern "C" {
#endif
//------------------------------------------------------------------------------
// Custom Profile
//
// Author: Jacob Portukalian
//
// Sky Wave Design, LLC
// 7083 Hollywood Blvd
// Los Angeles, CA 90028
// jacob@skywavedesign.com
// www.skywavedesign.com
//
// Sky Wave Design, LLC is a consulting firm which provides engineering design
// services.  We provide turnkey electronics design, from schematics and
// circuit designs to PCB layouts, mechanical enclosures, firmware, software,
// and volume manufacturing.
//
// This code is heavily modified from examples provided by Nordic Semiconductor.
//
// The custom profile module is more than just a profile, it also is responisble
// for setting up the stack.  It also serves as the custom profile, as it loads
// the custom service as well as the adopted services for device information and
// battery.
//
// This code gives you an example of how to set up your own profile with your
// own custom service, however the custom service itself is defined in a
// separate module.
//------------------------------------------------------------------------------
// Bluetooth Configuration Defines
#define IS_SRVC_CHANGED_CHARACT_PRESENT  1

#define DEVICE_NAME                     "Template App Build 0001"
#define MANUFACTURER_NAME               "ACME BLE Design"

#define MODEL_NUMBER					"PCA00028"
#define SERIAL_NUMBER					"0000-0001"
#define HARDWARE_REVISION				"Rev 1"
#define FIRMWARE_REVISION				"0.0.1 Build 0003"

#define APP_ADV_INTERVAL                 500
#define APP_ADV_TIMEOUT_IN_SECONDS       180
	
#define MIN_CONN_INTERVAL                MSEC_TO_UNITS(20, UNIT_1_25_MS)
#define MAX_CONN_INTERVAL                MSEC_TO_UNITS(40, UNIT_1_25_MS)
#define SLAVE_LATENCY                    0
#define CONN_SUP_TIMEOUT                 MSEC_TO_UNITS(4000, UNIT_10_MS)

#define FIRST_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(500, 0)
#define NEXT_CONN_PARAMS_UPDATE_DELAY    APP_TIMER_TICKS(30000, 0)
#define MAX_CONN_PARAMS_UPDATE_COUNT     3

#define SEC_PARAM_BOND                   1
#define SEC_PARAM_MITM                   0
#define SEC_PARAM_IO_CAPABILITIES        BLE_GAP_IO_CAPS_NONE
#define SEC_PARAM_OOB                    0
#define SEC_PARAM_MIN_KEY_SIZE           7
#define SEC_PARAM_MAX_KEY_SIZE           16

#define DEAD_BEEF                        0xDEADBEEF
#ifdef BLE_DFU_APP_SUPPORT
#define DFU_REV_MAJOR                    0x00
#define DFU_REV_MINOR                    0x01
#define DFU_REVISION                     ((DFU_REV_MAJOR << 8) | DFU_REV_MINOR)
#define APP_SERVICE_HANDLE_START         0x000C
#define BLE_HANDLE_MAX                   0xFFFF

STATIC_ASSERT(IS_SRVC_CHANGED_CHARACT_PRESENT);
#endif // BLE_DFU_APP_SUPPORT
//------------------------------------------------------------------------------
// Stack Init initializes the stack.
void CP_StackInit();
//------------------------------------------------------------------------------
void CP_Init();
//------------------------------------------------------------------------------
void CP_AdvertisingStart();
//------------------------------------------------------------------------------
void CP_BatteryServiceUpdate(uint8_t level);
//------------------------------------------------------------------------------
void CP_SetStatus(uint8_t);
//------------------------------------------------------------------------------
#if defined(__cplusplus)
}
#endif
	
#endif
