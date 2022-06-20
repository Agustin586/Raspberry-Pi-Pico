#ifndef INCLUDE_H
#define INCLUDE_H

#include "pico/mutex.h"
#include "pico/sem.h"
#include "pico/multicore.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include <stdio.h>
#include "pico/stdlib.h"

#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "queue.h"    /* RTOS queue related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */

#include "Defines.h"
// #include "ConfigFreeRtos.h"

#endif /* INCLUDE_H */