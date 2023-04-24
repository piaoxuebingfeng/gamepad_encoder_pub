/* encoder Test Example
*/
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/queue.h"

#define ENCODER_PIN_A 34
#define ENCODER_PIN_B 35
// static u_int8_t count = 0;
static volatile int16_t EncoderDiff = 0;
// static xQueueHandle gpioEventQueue = NULL;


// static void time_out_task(void *arg)
// {
//     vTaskDelay(20 / portTICK_PERIOD_MS);
//     count = 0;
//     vTaskDelete(NULL);
// }

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    static volatile int count, countLast;
    static volatile uint8_t a0, b0;
    static volatile uint8_t ab0;
    uint8_t a = gpio_get_level(ENCODER_PIN_A);
    uint8_t b = gpio_get_level(ENCODER_PIN_B);
    if (a != a0)
    {
        a0 = a;
        if (b != b0)
        {
            b0 = b;
            count += ((a == b) ? 1 : -1);
            if ((a == b) != ab0)
            {
                count += ((a == b) ? 1 : -1);
            }
            ab0 = (a == b);
        }
    }

    if (count != countLast)
    {
        EncoderDiff += (count - countLast) > 0 ? 1 : -1;
        countLast = count;
    }
    // uint32_t gpio_num = (uint32_t) arg;

    // xQueueSendFromISR(gpioEventQueue, &EncoderDiff, NULL);
}

static void gpio_task_example(void* arg)
{
    // int16_t encoder_diff;
    volatile int16_t diff;
    for(;;) {
        // if(xQueueReceive(gpioEventQueue, &encoder_diff, portMAX_DELAY)) {
        //          printf("encoder_diff %d\n",encoder_diff);
        // }
        
        //diff = -EncoderDiff / 2;
        diff = EncoderDiff;
        if (diff != 0)
        {
            // EncoderDiff是实际的脉冲数；把本次变量用掉了，需要重新置0
            printf("diff %d\n",diff);
            EncoderDiff = 0;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    printf("esp32 encoder test!\n");
    static int tc=0;
    gpio_config_t gpio_encoder_A = {
        .pin_bit_mask = 1ULL << ENCODER_PIN_A,
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_NEGEDGE,
        .pull_up_en = 1,
    };
    // gpio_config_t gpio_35_encoder_B = {
    //     .pin_bit_mask = 1ULL << ENCODER_PIN_B,
    //     .mode = GPIO_MODE_INPUT,
    //     .intr_type = GPIO_INTR_NEGEDGE,
    //     .pull_up_en = 1,
    // };
    gpio_config(&gpio_encoder_A);
    // gpio_config(&gpio_35_encoder_B);

    gpio_set_direction(ENCODER_PIN_B,GPIO_MODE_INPUT);
    gpio_set_pull_mode(ENCODER_PIN_B,GPIO_PULLUP_ONLY);

    // gpioEventQueue = xQueueCreate(10, sizeof(uint32_t));

    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 1, NULL);    
    gpio_install_isr_service(0);
    gpio_isr_handler_add(ENCODER_PIN_A, gpio_isr_handler, (void*)ENCODER_PIN_A);
    // gpio_isr_handler_add(ENCODER_PIN_B, gpio_isr_handler, (void*)ENCODER_PIN_B);
    while(1)
    {
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        //printf("main task running %d...",tc);
        tc++;
    }
}
