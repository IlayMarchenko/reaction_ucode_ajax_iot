#include <threads_header.h>

void *mainThread(void *arg0) {

    s32 s32_temp;
    float Filter_Buffer_Temperature[BUFFER_LENGTH_TEMPERATURE] = {0,};
    float Filter_Buffer_Lux[BUFFER_LENGTH_LUX] = {0,};

    // Custom functions that will initialize all periphery
    timer_params_init();
    display_params_init();
    i2c_params_init();
    opt3001_params_init();
    bme280_params_init();
    semaphore_params_init();
    trigger_thread_init();

    // Set initial values
    timer_flag = 1;
    current_measurement = 0;

    // Run infinity loop to read values from sensors
    while(1) {
        sem_wait(&semaphore);
        if (timer_flag && current_measurement < MEASUREMENTS_PER_PERIOD) {
            if (OPT3001_getLux(opt3001Handle, &Raw_Lux))
                Display_print1(display, 0, 0, "Raw Lux: %f", Raw_Lux);
            if(BME280_INIT_VALUE == bme280_read_uncomp_temperature(&s32_temp)) {
                Raw_Temperature = (float)bme280_compensate_temperature_int32(s32_temp)/100;
                Display_print1(display, 0, 0, "Raw Temperature: %f", Raw_Temperature);
            }

            lux = filter(Raw_Lux, Filter_Buffer_Lux, BUFFER_LENGTH_LUX);
            temperature = filter(Raw_Temperature, Filter_Buffer_Temperature, BUFFER_LENGTH_TEMPERATURE);
            current_measurement++;
        }
        sem_post(&semaphore);
    }
}

