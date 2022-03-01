#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/rmt.h"
#include "led_strip.h"

#define LED_RGB_GPIO 48
#define LED_RMT_CHANNEL RMT_CHANNEL_0

static const char *TAG = "ch03_led";

static uint8_t led_state = 0;
static led_strip_t *p_strip_led;

static void led_rmt_init()
{
    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(LED_RGB_GPIO, LED_RMT_CHANNEL);
    config.clk_div = 2;

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(1, (led_strip_dev_t)config.channel);
    p_strip_led = led_strip_new_rmt_ws2812(&strip_config);

    if (!p_strip_led)
    {
        ESP_LOGE(TAG, "-- init ws2812 error ---");
    }

    ESP_ERROR_CHECK(p_strip_led->clear(p_strip_led, 100));

    ESP_LOGI(TAG, "--- init ok ---");
}

static void set_pixel_purple()
{
    if (0 == led_state)
    {
        ESP_LOGD(TAG, "rgb led on");
        p_strip_led->set_pixel(p_strip_led, 0, 128, 0, 128);
        p_strip_led->refresh(p_strip_led, 100);
    }
    else
    {
        ESP_LOGD(TAG, "rgb led off");
        p_strip_led->clear(p_strip_led, 50);
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Hello led!");

    // gpio_reset_pin(LED_RGB_GPIO);
    // gpio_set_direction(LED_RGB_GPIO, GPIO_MODE_OUTPUT);

    led_rmt_init();

    while (1)
    {
        // led
        led_state = !led_state;
        set_pixel_purple();

        // delay 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
