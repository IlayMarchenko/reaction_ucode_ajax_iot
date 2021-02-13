#include <stdint.h>
#include <pthread.h>
#include <ti/sysbios/BIOS.h>
#include "ti_drivers_config.h"

#define THREADSTACKSIZE    1024

extern void *mainThread(void *arg0);

int main(void) {
    pthread_t           thread;
    pthread_attr_t      pAttrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;

    Board_initGeneral();

    pthread_attr_init(&pAttrs);
    priParam.sched_priority = 1;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&pAttrs, detachState);
    if (retc != 0) {
        while (1);
    }
    pthread_attr_setschedparam(&pAttrs, &priParam);
    retc |= pthread_attr_setstacksize(&pAttrs, THREADSTACKSIZE);
    if (retc != 0) {
        while (1);
    }

    retc = pthread_create(&thread, &pAttrs, mainThread, NULL);
    if (retc != 0) {
        while (1);
    }

    pthread_attr_destroy(&pAttrs);
    BIOS_start();

    return (0);
}
