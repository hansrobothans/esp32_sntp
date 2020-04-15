#include "bsp_sntp.h"


// static const char *TAG = "SNTP";
static sntp_on_connected_cb_t on_connected_cb = NULL;

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
void _time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Notification of a time synchronization event");
    on_connected_cb();
}

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
void sntp_set_on_connected_cb(sntp_on_connected_cb_t cb)
{
    on_connected_cb = cb;
}

/**
 * A brief history of JavaDoc-style (C-style) comments.
 *
 * This is the typical JavaDoc-style C-style comment. It starts with two
 * asterisks.
 *
 * @param void 
 * @sa https://www.timecalculator.net/milliseconds-to-date
 */
uint64_t sntp_get_sec_since_epoch(void)
{
    uint64_t secondsSinceEpoch = 0; // return value in milliseconds
    struct timeval tv; // struct timeval {
                       // time_t      tv_sec;     /* seconds */
                       // suseconds_t tv_usec;    /* microseconds */
                       // };

    gettimeofday(&tv, NULL); //  and gives the number of seconds and microseconds since the Epoch (see
                             // time(2)).  The tz argument is a struct timezone:

    secondsSinceEpoch = (uint64_t)(tv.tv_sec); 

    // printf("%llu\n", secondsSinceEpoch); // TODO: debug delete
    return secondsSinceEpoch;
}

/**
 * A brief history of JavaDoc-style (C-style) comments.
 *
 * This is the typical JavaDoc-style C-style comment. It starts with two
 * asterisks.
 *
 * @param void 
 * @sa https://www.timecalculator.net/milliseconds-to-date
 */   
uint64_t sntp_get_ms_since_epoch(void)
{
    uint64_t millisecondsSinceEpoch = 0; // return value in milliseconds
    struct timeval tv; // struct timeval {
                       // time_t      tv_sec;     /* seconds */
                       // suseconds_t tv_usec;    /* microseconds */
                       // };

    gettimeofday(&tv, NULL); //  and gives the number of seconds and microseconds since the Epoch (see
                             // time(2)).  The tz argument is a struct timezone:

    millisecondsSinceEpoch =
            (uint64_t)(tv.tv_sec) * 1000 +
            (uint64_t)(tv.tv_usec) / 1000;

    // printf("%llu\n", millisecondsSinceEpoch); // TODO: debug delete
    return millisecondsSinceEpoch;
}

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
esp_err_t sntp_get_tz_timeinfo(const char* tz, struct tm *timeinfo) 
{
    time_t now;
    // struct tm timeinfo;
    time(&now);
    setenv("TZ", tz, 1);
    tzset();
    localtime_r(&now, timeinfo);
    return ESP_OK;
}

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
esp_err_t sntp_helper_init(const char* sntp_pool_uri)
{
    int retry = 0;
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, sntp_pool_uri); // connect to ntp server pool via URI
    sntp_set_time_sync_notification_cb(_time_sync_notification_cb); // set the cb function linked in main
    sntp_init(); //Initialize this sntp module.

    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < RETRY_ATTEMPTS) 
    {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, RETRY_ATTEMPTS);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        if(retry >= RETRY_ATTEMPTS)
        {
            return ESP_FAIL; // Couldn't connect, check LAN connection
        }
    }
    
    return ESP_OK; // connection success
}

static void _sntp_on_connected(void)
{
    // event_t *event = malloc(sizeof(*event));

    // event->type = EVENT_TYPE_SNTP_CONNECTED;

    // ESP_LOGD(TAG, "Queuing event WIFI_CONNECTED");
    // xQueueSend(event_queue, &event, portMAX_DELAY);
    ESP_LOGI(TAG,"SNTP CONNECTED");
}


/* Variable holding number of times ESP32 restarted since first boot.
 * It is placed into RTC memory using RTC_DATA_ATTR and
 * maintains its value when ESP32 wakes from deep sleep.
 */
RTC_DATA_ATTR static int boot_count = 0;


void sntp_text(void)
{
    struct tm timeinfo;
    char strftime_buf[64];

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing d
     * Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */

    /* utilize callback functionality to drive event cb */
    sntp_set_on_connected_cb(_sntp_on_connected);

    /* sntp_helper_init to connect to SNTP service and do 
     * a connection check to ensure status
     * is OK.
     */
    ESP_ERROR_CHECK(sntp_helper_init("cn.pool.ntp.org")); 

    while(1)
    {  
        printf("milliseconds: %llu\n",sntp_get_ms_since_epoch());
        printf("seconds: %llu \n", sntp_get_sec_since_epoch());
        
        // 设置为eastern standard时间
        // sntp_get_tz_timeinfo("EST5EDT,M3.2.0/2,M11.1.0", &timeinfo);
        // 设置为中国（上海）时间
        sntp_get_tz_timeinfo("CST-8", &timeinfo);
        
        strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
        ESP_LOGI(TAG, "The current date/time : %s", strftime_buf);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}