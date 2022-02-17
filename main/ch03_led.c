#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"


static const char* TAG = "ch03_led";

void app_main(void) {
    ESP_LOGI(TAG, "Hello world!");

    uint16_t count = 0;
    while(1) {
        // TODO
        ESP_LOGI(TAG, "count %d", ++count);

        // delay 10s
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}
