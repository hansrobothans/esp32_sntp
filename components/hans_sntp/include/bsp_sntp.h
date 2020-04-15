#ifndef ESP_SNTP
#define ESP_SNTP "ESP_SNTP"


#include <time.h>
#include <string.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/timers.h>
#include <esp_system.h>
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "esp_sntp.h"
#include "nvs_flash.h"


#include "bsp_wifi.h"

#define RETRY_ATTEMPTS 50 // Used to determine the number retry attempts for an ntp service
#define TAG "SNTP"



#define sntp_curl "cn.ntp.org.cn"	//中国提供商
// #define sntp_curl "cn.pool.ntp.org"	//国外提供商
typedef enum {
    EVENT_TYPE_SNTP_CONNECTED,
    EVENT_TYPE_SNTP_DISCONNECTED,
} event_type_t;

typedef void (*sntp_on_connected_cb_t)(void);

/**
 * @brief Create a new event loop.
 *
 * @param[in] event_loop_args configuration structure for the event loop to create
 * @param[out] event_loop handle to the created event loop
 *
 * @return
 *  - ESP_OK: Success
 *  - ESP_ERR_NO_MEM: Cannot allocate memory for event loops list
 *  - ESP_FAIL: Failed to create task loop
 *  - Others: Fail
 */
void _time_sync_notification_cb(struct timeval *tv);
/**
 * @brief Create a new event loop.
 *
 * @param[in] event_loop_args configuration structure for the event loop to create
 * @param[out] event_loop handle to the created event loop
 *
 * @return
 *  - ESP_OK: Success
 *  - ESP_ERR_NO_MEM: Cannot allocate memory for event loops list
 *  - ESP_FAIL: Failed to create task loop
 *  - Others: Fail
 */
void sntp_set_on_connected_cb(sntp_on_connected_cb_t cb);
/**
 * @brief Create a new event loop.
 *
 * @param[in] event_loop_args configuration structure for the event loop to create
 * @param[out] event_loop handle to the created event loop
 *
 * @return
 *  - ESP_OK: Success
 *  - ESP_ERR_NO_MEM: Cannot allocate memory for event loops list
 *  - ESP_FAIL: Failed to create task loop
 *  - Others: Fail
 **/
esp_err_t sntp_helper_init(const char* sntp_pool_uri);
/**
 * @brief Create a new event loop.
 *
 * @param[in] event_loop_args configuration structure for the event loop to create
 * @param[out] event_loop handle to the created event loop
 *
 * @return
 *  - ESP_OK: Success
 *  - ESP_ERR_NO_MEM: Cannot allocate memory for event loops list
 *  - ESP_FAIL: Failed to create task loop
 *  - Others: Fail
 */
uint64_t sntp_get_ms_since_epoch(void);
/**
 * @brief Create a new event loop.
 *
 * @param[in] event_loop_args configuration structure for the event loop to create
 * @param[out] event_loop handle to the created event loop
 *
 * @return
 *  - ESP_OK: Success
 *  - ESP_ERR_NO_MEM: Cannot allocate memory for event loops list
 *  - ESP_FAIL: Failed to create task loop
 *  - Others: Fail
 */
uint64_t sntp_get_sec_since_epoch(void);
/**
 * @brief Create a new event loop.
 *
 * @param[in] event_loop_args configuration structure for the event loop to create
 * @param[out] event_loop handle to the created event loop
 *
 * @return
 *  - ESP_OK: Success
 *  - ESP_ERR_NO_MEM: Cannot allocate memory for event loops list
 *  - ESP_FAIL: Failed to create task loop
 *  - Others: Fail
 */
esp_err_t sntp_get_tz_timeinfo(const char* tz, struct tm *timeinfo);


void sntp_text(void);

#endif