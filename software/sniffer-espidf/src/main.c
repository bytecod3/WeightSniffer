#include <stddef.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define GPIO_BUTTON             12
#define GPIO_BUTTON_PIN_SEL     (1UL << GPIO_BUTTON)     
#define BUZZ_INTERVAL           200
#define ESP_INTR_FLAG_DEFAULT   0

static void button_handler(void *arg);

/**
 * hardware initialization function
 */
static void init_hw(void) {
    gpio_config_t io_conf;

    // configure the button as input
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = GPIO_BUTTON_PIN_SEL;
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    // attach the ISR
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(GPIO_BUTTON, button_handler, NULL);
}

static TickType_t next = 0;
static bool led_state = false;

/**
 * Interrupt service routine for the button
 */
static void IRAM_ATTR button_handler(void* arg) {
    TickType_t now = xTaskGetTickCountFromISR();

    // debounce button
    if(now > next) {
        gpio_set_level(GPIO_LED, led_state);
        next = now + 500 / portTICK_PERIOD_MS
    }
}

void app_main() {


}