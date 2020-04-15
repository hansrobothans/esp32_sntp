/*
* @file         esp.c 
* @brief        用户应用程序入口
* @details      用户应用程序的入口文件,用户所有要实现的功能逻辑均是从该文件开始或者处理
* @author       hans
* @par Copyright (c):  
*               个人开发库，QQ：304872739
* @par History:          
*               Ver0.0.1:
                     hans, 2019/09/02, 初始化版本\n 
*/

#include <esp32.h>

void app_main()
{
    // 初始化flash
    // 确认flash没有问题
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    printf("haha\n");

    bsp_wifi_init();

    
    sntp_text();


    while(1)
    {
        printf("haha\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }



}   

