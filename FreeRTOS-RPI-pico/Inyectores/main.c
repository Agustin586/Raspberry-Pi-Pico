/* Kernel includes. */
#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "queue.h"    /* RTOS queue related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

#ifndef mainRUN_FREE_RTOS_ON_CORE
#define mainRUN_FREE_RTOS_ON_CORE 0
#endif

/* Priorities at which the tasks are created.  The event semaphore task is
given the maximum priority of ( configMAX_PRIORITIES - 1 ) to ensure it runs as
soon as the semaphore is given. */
#define mainSDK_MUTEX_USE_TASK_PRIORITY     ( tskIDLE_PRIORITY + 3 )
#define mainSDK_SEMAPHORE_USE_TASK_PRIORITY ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_RECEIVE_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )
#define mainEVENT_SEMAPHORE_TASK_PRIORITY   ( configMAX_PRIORITIES - 1 )
#define mainSTACKDEPTH  256
#define mainMEF_TASK_PRIORITY               ( tskIDLE_PRIORITY + 2 )
#define mainUARTRX_TASK_PRIORITY            ( tskIDLE_PRIORITY + 1 )
#define mainADC_TASK_PRIORITY               ( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue, specified in milliseconds, and
converted to ticks using the pdMS_TO_TICKS() macro. */
#define mainQUEUE_SEND_PERIOD_MS            pdMS_TO_TICKS( 200 )

/* The period of the example software timer, specified in milliseconds, and
converted to ticks using the pdMS_TO_TICKS() macro. */
#define mainSOFTWARE_TIMER_PERIOD_MS        pdMS_TO_TICKS( 1000 )

/* The number of items the queue can hold.  This is 1 as the receive task
has a higher priority than the send task, so will remove items as they are added,
meaning the send task should always find the queue empty. */
#define mainQUEUE_LENGTH                    ( 1 )

/* Definimos los pines de los perfiricos*/
#define pinINYECTORES   2
#define pinBOMBA1       3
#define pinBOMBA2       6
#define pinENCODER_A    7
#define pinENCODER_B    8
#define pinENCODER_PUSH 9
#define pinBUZZER       10
#define pinRGB_red      11
#define pinRGB_green    12
#define pinRGB_blue     13
#define LED_rpi_pico    25
#define GPIO_INPUT  false
#define HIGH    1
#define LOW     0

#define UART_ID     uart0_t
#define TX_pin      0
#define RX_pin      1
#define I2C_ID      i2c0_t
#define I2C_sda     4
#define I2C_scl     5

/* Definimos delays de rtos */
#define ANT_DELAY   pdMS_TO_TICKS ( 40 )
#define DELAY_500ms pdMS_TO_TICKS ( 500 )
#define DELAY_300ms pdMS_TO_TICKS ( 300 )


/*-----------------------------------------------------------*/

/* Prototipo de funciones de freertos */
static void vUartRx_Task ( void *pvParameter );
static void vMef_Task ( void *pvParameter );
static void vAdc_Task ( void *pvParameter );
/*-----------------------------------------------------------*/

/* Prototipos de funciones no rtos */
void Mef_init ( void );
void Mef ( void );
void Antirrebote ( void );
/*-----------------------------------------------------------*/

/* Callback functions timers */
/**
 * @brief Realiza un blink en el pin 25 
 * 
 * @param xTimer 
 */
static void prvBlinkTimer ( TimerHandle_t xTimer );
/**
 * @brief Reliza un blink cuando se activa la salida pwm
 * 
 * @param xTimer 
 */
static void prvPwmEnableTimer ( TimerHandle_t xTimer );
/*-----------------------------------------------------------*/

typedef enum
{
    SET_DATE,
    MODO_FLUJO,
    MODO_PULV,
    MODO_FUGA,
    IDLE
}Data_est;

typedef struct
{
    uint8_t ucPwm_value;
    uint16_t usRpm_value;
    uint8_t ucSeg;
    uint8_t ucMin;
}Data_inyector;

typedef struct
{
    uint8_t ucDay;
    uint8_t ucMonth;
    uint8_t ucYear;
    uint8_t ucSeg;
    uint8_t ucMin;
}Data_rtc;

Data_est Est_mef;
Data_inyector strData_iny;
Data_rtc strDate;
/*-----------------------------------------------------------*/

volatile bool bEncPush = false;
volatile uint8_t ucEncCont = 0;
/*-----------------------------------------------------------*/

#include "pico/mutex.h"
#include "pico/sem.h"

#if configNUM_CORES > 1
#error Require only one core configured for FreeRTOS
#endif

static void prvNonRTOSWorker() {
    printf("Core %d: Doing regular SDK stuff\n", get_core_num());
    
}

static void prvLaunchRTOS() {
    printf("Core %d: Launching FreeRTOS scheduler\n", get_core_num());
    /* Start the tasks and timer running. */
    vTaskStartScheduler();
    /* should never reach here */
    panic_unsupported();
}

static void prvCore1Entry() {
#if ( mainRUN_FREE_RTOS_ON_CORE == 1 )
    prvLaunchRTOS();
#else
    prvNonRTOSWorker();
#endif
}

int main(void)
{
    TimerHandle_t xBlinkTimer, xPwmEnableTimer;    
    
    /* Creamos las tareas de mef, envio de uart y lectura de adc */
    xTaskCreate ( vMef_Task,"MEF",mainSTACKDEPTH,NULL,mainMEF_TASK_PRIORITY,NULL );
    xTaskCreate ( vUartRx_Task,"UART RX",mainSTACKDEPTH,NULL,mainUARTRX_TASK_PRIORITY,NULL );
    xTaskCreate ( vAdc_Task,"READ ADC",mainSTACKDEPTH,NULL,mainADC_TASK_PRIORITY,NULL );

    /* Creamos los software timers:
        1. Blink funcion durante todo el transcurso del programa y realiza un blink en el pin 25
        2. Se activa cuando la salida de pwm esta activa y realiza un blink en el rgb red
    */
    xBlinkTimer = xTimerCreate ( "BLINK",DELAY_500ms,pdTRUE,0,prvBlinkTimer );
    xPwmEnableTimer = xTimerCreate ( "BLINK_PWM",DELAY_300ms,pdTRUE,0,prvPwmEnableTimer );

    // Inicia el/los timer/s
    if ( xBlinkTimer != NULL )  xTimerStart ( xBlinkTimer,0 );

    multicore_launch_core1(prvCore1Entry);
#if ( mainRUN_FREE_RTOS_ON_CORE == 0 )
    prvLaunchRTOS();
#else
    prvNonRTOSWorker();
#endif
}
/*-----------------------------------------------------------*/

static void vMef_Task ( void *pvParameter )
{
    Data_est Modo;
    
    Mef_init ();

    while (1)
    {
        Mef ();
    }

    vTaskDelay(NULL);
    return;
}
/*-----------------------------------------------------------*/

static void vUartRx_Task ( void *pvParameter )
{
    while (1)
    {
        
    }

    vTaskDelay(NULL);
    return;
}
/*-----------------------------------------------------------*/

static void vAdc_Task ( void *pvParameter )
{
    while (1)
    {
        
    }

    vTaskDelay(NULL);
    return;
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    static uint32_t ulCount = 0;

    /* The RTOS tick hook function is enabled by setting configUSE_TICK_HOOK to
    1 in FreeRTOSConfig.h.

    "Give" the semaphore on every 500th tick interrupt. */

    /* If xHigherPriorityTaskWoken is pdTRUE then a context switch should
    normally be performed before leaving the interrupt (because during the
    execution of the interrupt a task of equal or higher priority than the
    running task was unblocked).  The syntax required to context switch from
    an interrupt is port dependent, so check the documentation of the port you
    are using.

    In this case, the function is running in the context of the tick interrupt,
    which will automatically check for the higher priority task to run anyway,
    so no further action is required. */
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
    /* The malloc failed hook is enabled by setting
    configUSE_MALLOC_FAILED_HOOK to 1 in FreeRTOSConfig.h.

    Called if a call to pvPortMalloc() fails because there is insufficient
    free memory available in the FreeRTOS heap.  pvPortMalloc() is called
    internally by FreeRTOS API functions that create tasks, queues, software
    timers, and semaphores.  The size of the FreeRTOS heap is set by the
    configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
    panic("malloc failed");
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) xTask;

    /* Run time stack overflow checking is performed if
    configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected.  pxCurrentTCB can be
    inspected in the debugger if the task name passed into this function is
    corrupt. */
    for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
    volatile size_t xFreeStackSpace;

    /* The idle task hook is enabled by setting configUSE_IDLE_HOOK to 1 in
    FreeRTOSConfig.h.

    This function is called on each cycle of the idle task.  In this case it
    does nothing useful, other than report the amount of FreeRTOS heap that
    remains unallocated. */
    xFreeStackSpace = xPortGetFreeHeapSize();

    if( xFreeStackSpace > 100 )
    {
        /* By now, the kernel has allocated everything it is going to, so
        if there is a lot of heap remaining unallocated then
        the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
        reduced accordingly. */
    }
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
    /* Want to be able to printf */
    stdio_init_all();
    /* And flash LED */
    gpio_init(LED_rpi_pico);
    gpio_init(pinINYECTORES);
    gpio_init(pinBOMBA1);
    gpio_init(pinBOMBA2);
    gpio_init(pinENCODER_A);
    gpio_init(pinENCODER_B);
    gpio_init(pinENCODER_PUSH);
    gpio_init(pinBUZZER);
    gpio_init(pinRGB_red);
    gpio_init(pinRGB_green);
    gpio_init(pinRGB_blue);

    /* Seteamos direccion */
    gpio_set_dir( LED_rpi_pico, GPIO_OUT );
    gpio_set_dir( pinBOMBA1, GPIO_OUT );
    gpio_set_dir( pinBOMBA2, GPIO_OUT );
    gpio_set_dir( pinENCODER_A, GPIO_INPUT );
    gpio_set_dir( pinENCODER_B, GPIO_INPUT );
    gpio_set_dir( pinENCODER_PUSH, GPIO_INPUT );
    gpio_set_dir( pinRGB_red, GPIO_OUT );
    gpio_set_dir( pinRGB_green, GPIO_OUT );
    gpio_set_dir( pinRGB_blue, GPIO_OUT );

    /* Seteamos la funcion */
    gpio_set_function( pinINYECTORES, GPIO_FUNC_PWM );

    /* Seteamos los estados iniciales de las salidas */
    gpio_put ( LED_rpi_pico, LOW );
    gpio_put ( pinBOMBA1, LOW );
    gpio_put ( pinBOMBA2, LOW );
    gpio_put ( pinRGB_red, LOW );
    gpio_put ( pinRGB_green, LOW );
    gpio_put ( pinRGB_blue, LOW );
}
/*-----------------------------------------------------------*/

static void prvBlinkTimer ( TimerHandle_t xTimer )
{
    gpio_put ( LED_rpi_pico, !gpio_get ( LED_rpi_pico ) );
}
/*-----------------------------------------------------------*/

static void prvPwmEnableTimer ( TimerHandle_t xTimer )
{
    gpio_put ( pinRGB_red, !gpio_get ( pinRGB_red ) );
}
/*-----------------------------------------------------------*/

void Mef_init ( void )
{
    Est_mef = IDLE;
    Modo = IDLE;

    return;
}
/*-----------------------------------------------------------*/

void Mef ( void )
{
    switch (Est_mef)
    {
        case IDLE:
        {
            // Apaga los leds rojo y verde
            if ( gpio_get ( pinRGB_red ) || gpio_get ( pinRGB_green ) )
            {
                gpio_put ( pinRGB_red, LOW );
                gpio_put ( pinRGB_green, LOW );  
            }
            // Prende el lez azul de estado inactivo
            gpio_put ( pinRGB_blue, HIGH );
            // Detecta cuando se pulsa la opcion
            if ( bEncPush )
            {
                if ( ucEncCont == 0)         Modo = MODO_PULV;
                else if ( ucEncCont == 1)    Modo = MODO_FLUJO;
                else if ( ucEncCont == 2 )   Modo = MODO_FUGA;

                Antirrebote ();
            }
            if ( Modo != IDLE )  ucEncCont = 0;
        break;
        }
        case SET_DATE:
        {

        break;
        }
        case MODO_FLUJO:
        {

        break;
        }
        case MODO_PULV:
        {

        break;
        }
        case MODO_FUGA:
        {

        break;
        }
    }
    return;
}
/*-----------------------------------------------------------*/

void Antirrebote ( void )
{
    vTaskDelay ( ANT_DELAY );
    gpio_put ( pinBUZZER,HIGH );
    vTaskDelay ( ANT_DELAY );
    gpio_put ( pinBUZZER,LOW );

    return;
}
/*-----------------------------------------------------------*/
