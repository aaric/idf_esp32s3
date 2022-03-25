#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

static const char *TAG = "ch04_timer";

static esp_timer_handle_t timer_once_handle;

static void timer_once_callback(void *arg)
{
    int64_t interval = esp_timer_get_time();
    ESP_LOGI(TAG, "callback=%s, interval=%lld", __func__, interval);
    esp_err_t err = esp_timer_delete(timer_once_handle);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "timer_once delete ok");
    }
}

static esp_timer_create_args_t timer_once_args =
{
    .callback = &timer_once_callback,
    .arg = NULL,
    .name = "timer_once"
};

static void timer_once_init()
{
    esp_timer_create(&timer_once_args, &timer_once_handle);
    // wait 10s
    esp_timer_start_once(timer_once_handle, 10 * 1000 * 1000);
}

static esp_timer_handle_t timer_periodic_handle;

static void timer_periodic_callback(void *arg)
{
    int64_t interval = esp_timer_get_time();
    ESP_LOGI(TAG, "callback=%s, interval=%lld", __func__, interval);

    if (interval > 30 * 1000 * 1000) {
        esp_err_t err = esp_timer_stop(timer_periodic_handle);
        if (err == ESP_OK)
        {
            ESP_LOGI(TAG, "timer_periodic stop ok");
        }
        err = esp_timer_delete(timer_periodic_handle);
        if (err == ESP_OK)
        {
            ESP_LOGI(TAG, "timer_periodic delete ok");
        }
    }
}

static esp_timer_create_args_t timer_periodic_args =
{
    .callback = &timer_periodic_callback,
    .arg = NULL,
    .name = "timer_periodic"
};

static void timer_periodic_init()
{
    esp_timer_create(&timer_periodic_args, &timer_periodic_handle);
    // interval 3s
    esp_timer_start_periodic(timer_periodic_handle, 3 * 1000 * 1000);
}

void app_main(void)
{
    ESP_LOGI(TAG, "hello timer!");

    esp_timer_init();

    timer_once_init();

    timer_periodic_init();

    while (1)
    {
        // delay 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
