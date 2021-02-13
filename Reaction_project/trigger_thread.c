#include <threads_header.h>

void *trigger_thread_func(void *arg) {

    uint8_t lux_already_on = 0;
    uint8_t temp_already_on = 0;

    // Run infinity loop to check if values from sensors are normal
    while(1) {
        sem_wait(&semaphore);
        if (!timer_flag) {
            if (lux > max_lux && lux_already_on) {
                GPIO_write(GREEN_LED, CONFIG_GPIO_LED_OFF);
                lux_already_on = 0;
            }
            else if (lux < min_lux && !lux_already_on) {
                GPIO_write(GREEN_LED, CONFIG_GPIO_LED_ON);
                lux_already_on = 1;
            }

            if (temperature > max_temperature && temp_already_on) {
                GPIO_write(RED_LED, CONFIG_GPIO_LED_OFF);
                temp_already_on = 0;
            }
            else if (temperature < min_temperature && !temp_already_on) {
                GPIO_write(RED_LED, CONFIG_GPIO_LED_ON);
                temp_already_on = 1;
            }
        }
        sem_post(&semaphore);
    }
}
