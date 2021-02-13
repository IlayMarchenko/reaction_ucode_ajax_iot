#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>
#include "ti_drivers_config.h"
#include <ti/display/Display.h>
#include <ti/sail/bme280/bme280.h>
#include <ti/sail/opt3001/opt3001.h>

#define TRIGGER_THREAD_STACK_SIZE 512 // size of trigger thread stuck
#define PERIOD_OF_TIMER_IN_SECONDS 1 // period of timer in seconds
#define MEASUREMENTS_PER_PERIOD 10 // how much measurements will be measured in one period of timer
#define BUFFER_LENGTH_TEMPERATURE 100 // length of buffer for temperature values
#define BUFFER_LENGTH_LUX 100 // length of buffer for lux values

// ---------------------------------- HANDLES ------------------------------------------------
Timer_Handle    timer;
Timer_Params    timer_params;

I2C_Handle      i2cHandle;
I2C_Params      i2cParams;

pthread_t       trigger_thread;
pthread_attr_t  tt_attrs;
struct sched_param   priParam;

OPT3001_Handle opt3001Handle;
OPT3001_Params opt3001Params;

// ---------------------------------- DECLARATION OF FUNCTIONS -------------------------------
extern s32 bme280_data_readout_template(I2C_Handle i2cHndl);
extern void *trigger_thread_func(void *arg);
extern float filter(float raw_value, float buffer[], uint8_t length_of_buffer);

extern void i2c_params_init();
extern void timer_params_init();
extern void bme280_params_init();
extern void opt3001_params_init();
extern void trigger_thread_init();
extern void display_params_init();
extern void semaphore_params_init();

// ---------------------------------- GLOBAL VARIABLES FOR MEASUREMENTS ----------------------
float lux;
float temperature;
float Raw_Lux;
float Raw_Temperature;

// ---------------------------------- ADDITINAL GLOBAL VARIABLES -----------------------------
uint8_t timer_flag;
sem_t semaphore;
uint8_t current_measurement;
Display_Handle  display;

// ---------------------------------- ENUMERATION FOR THRESHOLDS -----------------------------
enum thresholds {
    max_temperature = 34,
    min_temperature = 30,
    max_lux = 25,
    min_lux = 15
};





