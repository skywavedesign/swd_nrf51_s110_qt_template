//
// nRF51 Template Bluetooth Application - S110
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
// This is the template BLE application used by Sky Wave Design when starting a
// new project.
//
//============================================================================//
// SDK Includes
#include "custom_profile.h"
#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "nrf51_bitfields.h"
#include "nrf_gpio.h"

#include "softdevice_handler.h"
#include "app_timer.h"
#include "app_scheduler.h"
#include "device_manager.h"
#include "pstorage.h"
#include "app_trace.h"
#include "app_gpiote.h"
#include "nrf_delay.h"
#include "app_timer_appsh.h"
//============================================================================//
// Project Includes
#include "custom_board.h"
#include "custom_service.h"
//============================================================================//
//
// Update Interval in milliseconds
#define APP_UPDATE_INTERVAL_MS 1000

#define BSP_APP_TIMERS_NUMBER 2
#define APP_TIMER_PRESCALER 0               // Value of the RTC1 PRESCALER register.
#define APP_TIMER_MAX_TIMERS (6+BSP_APP_TIMERS_NUMBER)
#define APP_TIMER_OP_QUEUE_SIZE 5           // Size of timer operation queues.

#define APP_UPDATE_INTERVAL 	APP_TIMER_TICKS(APP_UPDATE_INTERVAL_MS, APP_TIMER_PRESCALER)

#define SCHED_MAX_EVENT_DATA_SIZE       MAX(APP_TIMER_SCHED_EVT_SIZE,\
                                            BLE_STACK_HANDLER_SCHED_EVT_SIZE)

#define SCHED_QUEUE_SIZE 10
static app_timer_id_t 					_app_update_timer_id;
//============================================================================//
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}
//============================================================================//
static void _app_update_timeout_handler(void* p_context)
{
    UNUSED_PARAMETER(p_context);
}
//============================================================================//
static void timers_init(void)
{
    uint32_t err_code;

    // Initialize timer module.
	APP_TIMER_APPSH_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, true);

    err_code = app_timer_create(&_app_update_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                _app_update_timeout_handler);
    APP_ERROR_CHECK(err_code);
}
//============================================================================//
static void application_timers_start(void)
{
    uint32_t err_code;

    // Start application timers.
    err_code = app_timer_start(_app_update_timer_id, APP_UPDATE_INTERVAL, NULL);
    APP_ERROR_CHECK(err_code);
}
//============================================================================//
static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}
//============================================================================//
// LEDs and Buttons
//============================================================================//
static void _set_led_command(uint8_t* cmd, uint8_t len)
{
    uint8_t bitmask = cmd[0];

    if (bitmask & 0x01)
    {
        // turn on LED 1
        nrf_gpio_pin_clear(LED1_PIN);
    }
    else
    {
        // turn off LED 1
        nrf_gpio_pin_set(LED1_PIN);
    }
    if (bitmask & 0x02)
    {
        // turn on LED 2
        nrf_gpio_pin_clear(LED2_PIN);
    }
    else
    {
        // turn off LED 2
        nrf_gpio_pin_set(LED2_PIN);
    }
    if (bitmask & 0x04)
    {
        // turn on LED 3
        nrf_gpio_pin_clear(LED3_PIN);
    }
    else
    {
        // turn off LED 3
        nrf_gpio_pin_set(LED3_PIN);
    }
    if (bitmask & 0x08)
    {
        // turn on LED 4
        nrf_gpio_pin_clear(LED4_PIN);
    }
    else
    {
        // turn off LED 4
        nrf_gpio_pin_set(LED4_PIN);
    }
}

//============================================================================//
int main(void)
{
    //--------------------------------------------------------------------------
    // Bluetooth Initialization
    CP_StackInit();
    APP_GPIOTE_INIT(3);
	APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
    timers_init();
    CP_Init();
    application_timers_start();
    //--------------------------------------------------------------------------
    // Set up command to enable / disable LEDs
    CS_SetCommand(&_set_led_command, 1);
    nrf_gpio_cfg_output(LED1_PIN);
    nrf_gpio_cfg_output(LED2_PIN);
    nrf_gpio_cfg_output(LED3_PIN);
    nrf_gpio_cfg_output(LED4_PIN);
    //--------------------------------------------------------------------------
    // Enter main loop.
    CP_AdvertisingStart();
    for (;;)
    {
		app_sched_execute();
        power_manage();
    }
}


