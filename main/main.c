#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"


static const char* TAG = "ch02_log"

void app_main(void)
{
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    ESP_LOGI(TAG, "This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    ESP_LOGI(TAG, "silicon revision %d, ", chip_info.revision);

    ESP_LOGI(TAG, "%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    /* ESP_LOGx testing */
    while(1) {
        // error level
        ESP_LOGE(TAG, "error log");

        // warn level
        ESP_LOGW(TAG, "warn log");

        // info level
        ESP_LOGI(TAG, "info log");

        // error level
        ESP_LOGD(TAG, "debug log");

        // verbose
        ESP_LOGV(TAG, "verbose log");

        // delay 10s
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}
