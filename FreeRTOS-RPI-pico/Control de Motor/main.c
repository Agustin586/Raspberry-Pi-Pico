/*
 * FreeRTOS V202107.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/******************************************************************************
 * This project provides two demo applications.  A simple blinky style project,
 * and a more comprehensive test and demo application.  The
 * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting (defined in this file) is used to
 * select between the two.  The simply blinky demo is implemented and described
 * in main_blinky.c.  The more comprehensive test and demo application is
 * implemented and described in main_full.c.
 *
 * This file implements the code that is not demo specific, including the
 * hardware setup and standard FreeRTOS hook functions.
 *
 * ENSURE TO READ THE DOCUMENTATION PAGE FOR THIS PORT AND DEMO APPLICATION ON
 * THE http://www.FreeRTOS.org WEB SITE FOR FULL INFORMATION ON USING THIS DEMO
 * APPLICATION, AND ITS ASSOCIATE FreeRTOS ARCHITECTURE PORT!
 *
 */

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h"

/* Library includes. */
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

/*
 *  DEFINES PINS 
 */
// I2C
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9
// RGB
#define RGB_RED     2   // PWM_A[1]
#define RGB_GREEN   3   // PWM_B[1]
#define RGB_BLUE    4   // PWM_A[2]
// RELAYS
#define RELAY1      5   // GPIO 5
#define RELAY2      6   // GPIO 6
// ADC
#define ADC0        21  // ADC0
#define ADC_TEMPint // Buscar en los ejemplos
// UART
#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE
#define UART_TX_PIN 0
#define UART_RX_PIN 1
// TIMER
#define DELAY_500ms pdMS_TO_TICKS(500)
#define DELAY_200ms pdMS_TO_TICKS(200)
#define DELAY_100ms pdMS_TO_TICKS(100)

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

/* Prototypes for the standard FreeRTOS callback/hook functions implemented
within this file. */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationTickHook( void );

/**
 * Prototypes for funtions
 */
void vUartProc_Task( void *pvParameters );
void vRGB_Task( void *pvParameters );
void vLcd_Task( void *pvParameters );
void vMot_Task( void *pvParameters );
static void prvBlinkTimer_Callback( TimerHandle_t xTimer );
void vCopyString ( char *Task[], char *Cmd[], char *Date[] );

/*-----------------------------------------------------------*/

volatile char cStringRx[40];
volatile uint8_t cIndice=0;
volatile bool bStringComplete=false;
const char mot_task[] = "MOT", rgb_task[] = "RGB", lcd_task[] = "LCD";
const char mot_pwr[] = "PWR", mot_vel[] = "PWM", mot_freq[] = "FRQ";
const char rgb_red[] = "RPW", rgb_blue[] = "BPW", rgb_green[] = "GPW", rgb_pwr[] = "PWR";
const char lcd_clear[] = "CLR", lcd_write[] = "WRT";

TaskHandle_t xUartProc_Handle;
TimerHandle_t xBlink_SoftTmr;

/*-----------------------------------------------------------*/

void on_uart_rx( void )
{
    while (uart_is_readable(UART_ID))
    {
        char cRx;
        cRx = uart_getc(UART_ID);
        if(cRx != '\n')
        {
            cStringRx[cIndice] = cRx;
            cIndice++;
        }
        else
            bStringComplete = true;
        uart_putc(UART_ID, cRx);
    }

    return;
}

/*-----------------------------------------------------------*/

int main( void )
{
    /* Configure the hardware ready to run the demo. */
    prvSetupHardware();
    
    xTaskCreate( vUartProc_Task,"Uart_Processing",128,NULL,2,&xUartProc_Handle );
    xTaskCreate( vRGB_Task,"Rgb_control",128,NULL,1,NULL );
    xTaskCreate( vLcd_Task,"Lcd_control",128,NULL,1,NULL );
    xTaskCreate( vMot_Task,"Motor_control",128,NULL,1,NULL );

    xBlink_SoftTmr = xTimerCreate( "Blink",DELAY_500ms,pdTRUE,0,prvBlinkTimer_Callback );
    if ( xBlink_SoftTmr != NULL )
        xTimerStart( xBlink_SoftTmr,0 );

    vTaskStartScheduler();

    while (1) {}
    
    return 0;
}

/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
    stdio_init_all();
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, 1);
    gpio_put(PICO_DEFAULT_LED_PIN, !PICO_DEFAULT_LED_PIN_INVERTED);
    
    // RGB PINS init
    gpio_init(RGB_RED);
    gpio_set_dir(RGB_RED, true);
    gpio_set_function(RGB_RED, GPIO_FUNC_PWM);
    gpio_init(RGB_GREEN);
    gpio_set_dir(RGB_GREEN, true);
    gpio_set_function(RGB_GREEN, GPIO_FUNC_PWM);
    gpio_init(RGB_BLUE);
    gpio_set_dir(RGB_BLUE, true);
    gpio_set_function(RGB_BLUE, GPIO_FUNC_PWM);

    // RELAY PINS init
    gpio_init(RELAY1);
    gpio_set_dir(RELAY1, true);
    gpio_put(RELAY1, false);
    gpio_init(RELAY2);
    gpio_set_dir(RELAY2, true);
    gpio_put(RELAY2, false);

    // I2C init
    i2c_init(I2C_PORT, 100*1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // UART init
    // uart_init(UART_ID, BAUD_RATE);
    // gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    // gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    // uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    // uart_set_fifo_enabled(UART_ID, false);
    // irq_set_exclusive_handler(UART0_IRQ, on_uart_rx);
    // irq_set_enabled(UART0_IRQ, true);
    // uart_set_irq_enables(UART_ID, true, false);

    return;
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
    /* Called if a call to pvPortMalloc() fails because there is insufficient
    free memory available in the FreeRTOS heap.  pvPortMalloc() is called
    internally by FreeRTOS API functions that create tasks, queues, software
    timers, and semaphores.  The size of the FreeRTOS heap is set by the
    configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */

    /* Force an assert. */
    configASSERT( ( volatile void * ) NULL );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */

    /* Force an assert. */
    configASSERT( ( volatile void * ) NULL );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
    volatile size_t xFreeHeapSpace;

    /* This is just a trivial example of an idle hook.  It is called on each
    cycle of the idle task.  It must *NOT* attempt to block.  In this case the
    idle task just queries the amount of FreeRTOS heap that remains.  See the
    memory management section on the http://www.FreeRTOS.org web site for memory
    management options.  If there is a lot of heap memory free then the
    configTOTAL_HEAP_SIZE value in FreeRTOSConfig.h can be reduced to free up
    RAM. */
    xFreeHeapSpace = xPortGetFreeHeapSize();

    /* Remove compiler warning about xFreeHeapSpace being set but never used. */
    ( void ) xFreeHeapSpace;
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
#if mainCREATE_SIMPLE_BLINKY_DEMO_ONLY == 0
    {
        /* The full demo includes a software timer demo/test that requires
        prodding periodically from the tick interrupt. */
        #if (mainENABLE_TIMER_DEMO == 1)
        vTimerPeriodicISRTests();
        #endif

        /* Call the periodic queue overwrite from ISR demo. */
        #if (mainENABLE_QUEUE_OVERWRITE == 1)
        vQueueOverwritePeriodicISRDemo();
        #endif

        /* Call the periodic event group from ISR demo. */
        #if (mainENABLE_EVENT_GROUP == 1)
        vPeriodicEventGroupsProcessing();
        #endif

        /* Call the code that uses a mutex from an ISR. */
        #if (mainENABLE_INTERRUPT_SEMAPHORE == 1)
        vInterruptSemaphorePeriodicTest();
        #endif

        /* Call the code that 'gives' a task notification from an ISR. */
        #if (mainENABLE_TASK_NOTIFY == 1)
        xNotifyTaskFromISR();
        #endif
    }
#endif
}

/*-----------------------------------------------------------*/

// FreeRtos functions
/*-----------------------------------------------------------*/

void vUartProc_Task( void *pvParameters )
{
    struct rx_buffer
    {
        char cTask[3];
        char cCmd[3];
        char cDate[20];
    }rx_data;

    while (1)
    {
        if ( bStringComplete )
        {
            vCopyString ( &rx_data.cTask,&rx_data.cCmd,&rx_data.cDate );
            if ( strcmp ( rx_data.cTask,mot_task ) == 0 )
            {
                if ( strcmp ( rx_data.cCmd,mot_pwr ) == 0)
                {

                }
                else if ( strcmp ( rx_data.cCmd,mot_vel ) == 0)
                {

                }
                else if ( strcmp ( rx_data.cCmd,mot_freq ) == 0 )
                {

                }
            }
            if ( strcmp ( rx_data.cTask,rgb_task ) == 0)
            {
                if ( strcmp ( rx_data.cCmd,rgb_pwr ) == 0)
                {

                }
                else if ( strcmp ( rx_data.cCmd,rgb_red ) == 0 )
                {

                }
                else if ( strcmp ( rx_data.cCmd,rgb_blue ) == 0)
                {

                }
                else if ( strcmp ( rx_data.cCmd,rgb_green ) == 0 )
                {

                }
            }
            if ( strcmp ( rx_data.cTask,lcd_task ) == 0 )
            {
                if ( strcmp ( rx_data.cCmd,lcd_write ) == 0 )
                {

                }
                else if ( strcmp ( rx_data.cCmd,lcd_clear ) == 0 )
                {
                    
                }
            }
        }
        vTaskDelay( DELAY_200ms );
    }
    
    vTaskDelete(NULL);
    return;
}

/*-----------------------------------------------------------*/

void vRGB_Task( void *pvParameters )
{

    while (1)
    {
        vTaskDelay ( DELAY_500ms );
    }
    
    vTaskDelete(NULL);
    return;
}

/*-----------------------------------------------------------*/

void vLcd_Task( void *pvParameters )
{

    while (1)
    {
        vTaskDelay ( DELAY_500ms );
    }
    
    vTaskDelete(NULL);
    return;
}

/*-----------------------------------------------------------*/

void vMot_Task( void *pvParameters )
{

    while (1)
    {
        vTaskDelay ( DELAY_500ms );
    }
    
    vTaskDelete( NULL );
    return;
}

/*-----------------------------------------------------------*/

static void prvBlinkTimer_Callback( TimerHandle_t xTimer )
{
    gpio_put( PICO_DEFAULT_LED_PIN, !gpio_get( PICO_DEFAULT_LED_PIN ) );

    return;
}

/*-----------------------------------------------------------*/

void vCopyString ( char *Task[], char *Cmd[], char *Date[] )
{
    if ( cStringRx[0] == '#' )
    {
        for( uint8_t i=1;i<=3;i++ )
        {
            *Task[i-1] = cStringRx[i];
            *Cmd[i-1] = cStringRx[i+4];
        }
        for( uint8_t i=9;i<=cIndice;i++ )
        {
            *Date[i-9] = cStringRx[i];
        }
        for ( uint8_t i=0;i<cIndice;i++)
        {
            cStringRx[i] = 0;
        }
    }

    return;
}
