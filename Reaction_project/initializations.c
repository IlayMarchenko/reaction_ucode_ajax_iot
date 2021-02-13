#include <threads_header.h>
#include <opt3001_config.h>

void timerCallback(Timer_Handle myHandle) {
    timer_flag = timer_flag == 0 ? 1 : 0;
    current_measurement = 0;
}

void timer_params_init() {
    Timer_init();
    Timer_Params_init(&timer_params);
    timer_params.period = PERIOD_OF_TIMER_IN_SECONDS * 1000000;
    timer_params.periodUnits = Timer_PERIOD_US;
    timer_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    timer_params.timerCallback = timerCallback;
    timer = Timer_open(TIMER, &timer_params);
    if (timer == NULL) {
        while (1);
    }
    if (Timer_start(timer) == Timer_STATUS_ERROR) {
        while (1);
    }
}

void i2c_params_init() {
    I2C_init();
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2cHandle = I2C_open(CONFIG_I2C_OPT, &i2cParams);
    if (i2cHandle == NULL) {
        while (1);
    }
}

void display_params_init() {
    Display_init();
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        while (1);
    }
}

void opt3001_params_init() {
    OPT3001_init();
    OPT3001_Params_init(&opt3001Params);
    opt3001Handle = OPT3001_open(CONFIG_OPT3001_LIGHT, i2cHandle, &opt3001Params);
    if(opt3001Handle == NULL) {
        while(1);
    }
    sleep(1);
}
void bme280_params_init() {
    if(BME280_INIT_VALUE != bme280_data_readout_template(i2cHandle)) {
        while(1);
    }
    bme280_set_power_mode(BME280_NORMAL_MODE);
}

void semaphore_params_init() {
    if(sem_init(&semaphore,0,1) != 0) {
        while (1);
    }
}

void trigger_thread_init() {
    pthread_attr_init(&tt_attrs);
    priParam.sched_priority = 1;
    pthread_attr_setschedparam(&tt_attrs, &priParam);
    pthread_attr_setstacksize(&tt_attrs, TRIGGER_THREAD_STACK_SIZE);
    pthread_create(&trigger_thread, &tt_attrs, &trigger_thread_func, NULL);
    pthread_attr_destroy(&tt_attrs);
}
