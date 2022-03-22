#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/rmt.h"
#include "led_strip.h"

#define LED_RGB_GPIO GPIO_NUM_48
#define LED_RMT_CHANNEL RMT_CHANNEL_0

#define BUTTON_BOOT_GPIO GPIO_NUM_0
#define GPIO_INPUT_PIN_SEL 1ULL << BUTTON_BOOT_GPIO
#define ESP_INTR_FLAG_DEFAULT 0

static const char *TAG = "ch03_gpio";

static uint8_t led_state = 0;
static led_strip_t *p_strip_led;

static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task_example(void *arg)
{
    uint32_t io_num;
    for (;;)
    {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY))
        {
            ESP_LOGE(TAG, "GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
        }
    }
}

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
    ESP_LOGI(TAG, "hello gpio!");

    // gpio_reset_pin(LED_RGB_GPIO);
    // gpio_set_direction(LED_RGB_GPIO, GPIO_MODE_OUTPUT);

    led_rmt_init();

    gpio_config_t gpio_conf;
    gpio_conf.intr_type = GPIO_INTR_NEGEDGE;
    gpio_conf.mode = GPIO_MODE_INPUT;
    gpio_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    gpio_conf.pull_up_en = 1;

    gpio_config(&gpio_conf);
    gpio_set_intr_type(BUTTON_BOOT_GPIO, GPIO_INTR_POSEDGE);

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(BUTTON_BOOT_GPIO, gpio_isr_handler, (void *)BUTTON_BOOT_GPIO);

    while (1)
    {
        // led
        led_state = !led_state;
        set_pixel_purple();

        // delay 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
