#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/rmt.h"
#include "sdkconfig.h"

#define BLINK_GPIO 48

static const char *TAG = "ch03_led";

static uint8_t led_state = 0;
static led_strip_t *p_strip_p48;

void app_main(void)
{
    ESP_LOGI(TAG, "Hello led!");

    // gpio_reset_pin(BLINK_GPIO);
    // gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    while (1)
    {
        // close
        ESP_LOGD(TAG, "gpio-48 off");
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // open
        gpio_set_level(BLINK_GPIO, 1);
        ESP_LOGD(TAG, "gpio-48 on");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
