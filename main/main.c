#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <led_strip.h>
#include "esp_log.h"

#define LED_TYPE LED_STRIP_WS2812
#define LED_GPIO 15

// static const rgb_t colors[] = {
//     { .r = 0x0f, .g = 0x0f, .b = 0x0f },
//     { .r = 0x00, .g = 0x00, .b = 0x2f },
//     { .r = 0x00, .g = 0x2f, .b = 0x00 },
//     { .r = 0x2f, .g = 0x00, .b = 0x00 },
//     { .r = 0x00, .g = 0x00, .b = 0x00 },
// };

#define COLORS_TOTAL (sizeof(colors) / sizeof(rgb_t))

void test(void *pvParameters)
{
    led_strip_t strip = {
        .type = LED_TYPE,
        .length = 32,
        .gpio = LED_GPIO,
        .buf = NULL,
#ifdef LED_STRIP_BRIGHTNESS
        .brightness = 255,
#endif
    };

    ESP_ERROR_CHECK(led_strip_init(&strip));

    size_t c = 0;
    // rgb_t color = {0};
    while (1)
    {
        for (uint8_t i = 0; i < 255; i++)
        {
            ESP_LOGI("main.c", "i: %u", i);
            for (uint8_t j = 0; j < 255; j++)
            {
                ESP_LOGI("main.c", "j: %u", j);
                for (uint8_t k = 0; k < 255; k++)
                {
                    rgb_t color = {
                        .r = i,
                        .g = j,
                        .b = k
                    };
                    ESP_ERROR_CHECK(led_strip_fill(&strip, 0, strip.length, color));
                    ESP_ERROR_CHECK(led_strip_flush(&strip));
                    vTaskDelay(pdMS_TO_TICKS(10));
                }
            }
        }
    }
}

void app_main()
{
    led_strip_install();
    xTaskCreate(test, "test", configMINIMAL_STACK_SIZE * 5, NULL, 5, NULL);
}

