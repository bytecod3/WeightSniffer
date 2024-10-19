#include <stddef.h>
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define GPIO_BUTTON             12
#define GPIO_BUTTON_PIN_SEL     (1UL << GPIO_BUTTON)   
#define GPIO_BUZZ               4
#define GPIO_BUZZ_PIN_SEL       (1UL << GPIO_BUZZ)  
#define BUZZ_INTERVAL           100
#define ESP_INTR_FLAG_DEFAULT   0

static TickType_t next = 0;
double weight = 0.0;

/**
 * hardware initialization function
 */
static void init_hw() {
    gpio_config_t io_conf; 
    
    // init the buzzer as output
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_BUZZ_PIN_SEL;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    // configure the button as input
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = GPIO_BUTTON_PIN_SEL;
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    // attach the ISR
    // gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    // gpio_isr_handler_add(GPIO_BUTTON, button_handler, NULL);
}

/**
 * buzz
 */
void buzz(void) {
    gpio_set_level(GPIO_BUZZ, 1);
    vTaskDelay(BUZZ_INTERVAL / portTICK_PERIOD_MS);
    gpio_set_level(GPIO_BUZZ, 0);
    vTaskDelay(BUZZ_INTERVAL / portTICK_PERIOD_MS);
}

/**
 * read the weight from load cell
 */
// double read_load_cell(void) {

// }

/**
 * read push button
 * 
 */

uint8_t read_button() {
    TickType_t now = xTaskGetTickCount();
    uint8_t bv = 1;
    if(now - next ) {
        bv = gpio_get_level(GPIO_BUTTON);
        printf("%d\n", bv);
        next = now + 500 / portTICK_PERIOD_MS;
    }

    return bv;
}

int app_main() {

    // system setup 
    init_hw();

    while(1) {
        uint8_t c = read_button();

        if(!c) {
            buzz();
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);

    }
   

}