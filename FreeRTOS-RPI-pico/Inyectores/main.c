/* Kernel includes. */
#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "queue.h"    /* RTOS queue related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

#ifndef mainRUN_FREE_RTOS_ON_CORE
#define mainRUN_FREE_RTOS_ON_CORE 0
#endif

/* Configuraciones y parametros */
#define mainSDK_MUTEX_USE_TASK_PRIORITY     ( tskIDLE_PRIORITY + 3 )
#define mainSDK_SEMAPHORE_USE_TASK_PRIORITY ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_RECEIVE_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )
#define mainEVENT_SEMAPHORE_TASK_PRIORITY   ( configMAX_PRIORITIES - 1 )
#define mainSTACKDEPTH  256
#define mainMEF_TASK_PRIORITY               ( tskIDLE_PRIORITY + 2 )
#define mainUARTRX_TASK_PRIORITY            ( tskIDLE_PRIORITY + 1 )
#define mainADC_TASK_PRIORITY               ( tskIDLE_PRIORITY + 1 )

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
#define pinSELECT       21
#define LED_rpi_pico    25
#define GPIO_INPUT  false
#define HIGH    1
#define LOW     0

#define I2C_ID      i2c0_t
#define I2C_sda     4
#define I2C_scl     5

// Uart datos
#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE
#define UART_TX_PIN 0
#define UART_RX_PIN 1

/* Definimos delays de rtos */
#define ANT_DELAY   pdMS_TO_TICKS ( 40 )
#define DELAY_500ms pdMS_TO_TICKS ( 500 )
#define DELAY_300ms pdMS_TO_TICKS ( 300 )
#define DELAY_15ms  pdMS_TO_TICKS ( 15 )
#define DELAY_200ms pdMS_TO_TICKS ( 200 )
#define DELAY_30ms  pdMS_TO_TICKS ( 30 )
#define DELAY_5ms   pdMS_TO_TICKS ( 5 )
#define DELAY_20ms  pdMS_TO_TICKS ( 20 )
#define DELAY_60ms  pdMS_TO_TICKS ( 60 )

/*-----------------------------------------------------------*/

/* Prototipo de funciones de freertos */
static void vUartRx_Task ( void *pvParameter );
static void vMef_Task ( void *pvParameter );
static void vAdc_Task ( void *pvParameter );
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/

/* Prototipos de funciones no rtos */
void Mef_init ( void );
void Mef ( void );
void Antirrebote ( void );
void ModSelect ( uint8_t valor );
void SentData_Nextion ( void );
/*-----------------------------------------------------------*/

/* Funciones de interrupcion */
void vISR_uartrx ( void );                              // Hardware interrupt
static void vISR_uartrx_Task ( void *pvParameter );    // Procesa la interrupcion por uart
/*-----------------------------------------------------------*/

/* Callback functions timers */
static void prvBlinkTimer ( TimerHandle_t xTimer );     // Realiza un blink en el pin 25
static void prvPwmEnableTimer ( TimerHandle_t xTimer ); // Realiza un blink cuando se activa la salida pwm
static void prvEncoderAntirrebote ( TimerHandle_t xTimer ); // Antirrebote para el encoder
static void prvConfigTimer ( TimerHandle_t xTimer );
/*-----------------------------------------------------------*/

/* Callbakc functions para las interrupciones por pines */
void vISR_gpioAcallback ( uint gpio, uint32_t events );
void vISR_gpioBcallback ( uint gpio, uint32_t events );
static void vISR_pinA_edgeRising ( void *pvParameter );
static void vISR_pinA_edgeFalling ( void *pvParameter );
/*-----------------------------------------------------------*/

/* Semaforos */
SemaphoreHandle_t xUartRxSemaphore;
SemaphoreHandle_t xEncA_rising;
SemaphoreHandle_t xEncA_falling;
/*-----------------------------------------------------------*/

typedef enum
{
    SET_DATE,
    MODO_FLUJO,
    MODO_PULV,
    MODO_FUGA,
    IDLE
}Data_est;

typedef enum
{
    ATRAS,
    ACEPTAR,
    PWM_CONFIG,
    RPM_CONFIG,
    SEG_CONFIG,
    MIN_CONFIG,
    HOR_CONFIG
}Nextion_est;

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

Data_est Est_mef,Modo;
Data_inyector strData_iny;
Data_rtc strDate;
Nextion_est Select;
/*-----------------------------------------------------------*/

volatile bool bEncPush = false, bStringComplete = false, bEncAB = false;
volatile uint16_t ucEncCont = 0;
char cStringRx[20];
volatile uint8_t ucIndice = 0;
TimerHandle_t xAntirreboteTimer,xConfigTimer;
/*-----------------------------------------------------------*/

#include "pico/mutex.h"
#include "pico/sem.h"

#if configNUM_CORES > 1
#error Require only one core configured for FreeRTOS
#endif

static void prvNonRTOSWorker() {
    // printf("Core %d: Doing regular SDK stuff\n", get_core_num());
    
}

static void prvLaunchRTOS() {
    // printf("Core %d: Launching FreeRTOS scheduler\n", get_core_num());
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
    
    prvSetupHardware();
    
    // Creamos un semaforo para las interrupciones
    xUartRxSemaphore = xSemaphoreCreateCounting ( 10,0 );
    xEncA_rising = xSemaphoreCreateCounting ( 10,0 );
    xEncA_falling = xSemaphoreCreateCounting ( 10,0 );

    // Creamos la funcion que procesa la interrupcion por uart
    // if ( xUartRxSemaphore != NULL )
    //     xTaskCreate ( vISR_uartrx_Task,"UART_RX_ISR",mainSTACKDEPTH,NULL,configMAX_PRIORITIES,NULL );
    if ( xEncA_rising != NULL )
        xTaskCreate ( vISR_pinA_edgeRising,"ENC_A_ISR",mainSTACKDEPTH,NULL,configMAX_PRIORITIES-1,NULL );
    if ( xEncA_falling != NULL )
        xTaskCreate ( vISR_pinA_edgeFalling,"ENC_A_ISRfalling",mainSTACKDEPTH,NULL,configMAX_PRIORITIES-1,NULL );

    /* Creamos las tareas de mef, envio de uart y lectura de adc */
    xTaskCreate ( vMef_Task,"MEF",mainSTACKDEPTH,NULL,mainMEF_TASK_PRIORITY,NULL );
    xTaskCreate ( vUartRx_Task,"UART RX",mainSTACKDEPTH,NULL,mainUARTRX_TASK_PRIORITY,NULL );
    xTaskCreate ( vAdc_Task,"READ ADC",mainSTACKDEPTH,NULL,mainADC_TASK_PRIORITY,NULL );

    /* Creamos los software timers:
        1. Blink funcion durante todo el transcurso del programa y realiza un blink en el pin 25
        2. Se activa cuando la salida de pwm esta activa y realiza un blink en el rgb red
        3. Se activa cuando surge el cambio de estado de la interrupcion
        4. Se activa cuando se comienza a configurar los parametros
    */
    xBlinkTimer = xTimerCreate ( "BLINK",DELAY_500ms,pdTRUE,0,prvBlinkTimer );
    xPwmEnableTimer = xTimerCreate ( "BLINK_PWM",DELAY_300ms,pdTRUE,0,prvPwmEnableTimer );
    xAntirreboteTimer = xTimerCreate ( "ANTIRREBOTE",DELAY_15ms,pdFALSE,0,prvEncoderAntirrebote );
    xConfigTimer = xTimerCreate ( "CONFIG",DELAY_200ms,pdTRUE,0,prvConfigTimer );

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
    Mef_init ();

    while (1)
    {
        Mef ();
        vTaskDelay ( DELAY_15ms );
    }

    vTaskDelete ( NULL );
    return;
}
/*-----------------------------------------------------------*/

static void vUartRx_Task ( void *pvParameter )
{
    while (1)
    {
        vTaskDelay ( DELAY_15ms );
    }

    vTaskDelete ( NULL );
    return;
}
/*-----------------------------------------------------------*/

static void vAdc_Task ( void *pvParameter )
{
    while (1)
    {
        vTaskDelay ( DELAY_15ms );
    }

    vTaskDelete ( NULL );
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
    gpio_init( LED_rpi_pico );
    gpio_init( pinINYECTORES );
    gpio_init( pinBOMBA1 );
    gpio_init( pinBOMBA2 );
    // gpio_init( pinENCODER_A );
    // gpio_init( pinENCODER_B );
    gpio_init( pinENCODER_PUSH );
    gpio_init( pinBUZZER );
    gpio_init( pinRGB_red );
    gpio_init( pinRGB_green );
    gpio_init( pinRGB_blue );
    gpio_init( pinSELECT );

    /* Seteamos direccion */
    gpio_set_dir ( LED_rpi_pico, GPIO_OUT );
    gpio_set_dir ( pinBOMBA1, GPIO_OUT );
    gpio_set_dir ( pinBOMBA2, GPIO_OUT );
    // gpio_set_dir ( pinENCODER_A, GPIO_INPUT );
    // gpio_set_dir ( pinENCODER_B, GPIO_INPUT );
    gpio_set_dir ( pinENCODER_PUSH, GPIO_INPUT );
    gpio_set_dir ( pinRGB_red, GPIO_OUT );
    gpio_set_dir ( pinRGB_green, GPIO_OUT );
    gpio_set_dir ( pinRGB_blue, GPIO_OUT );
    gpio_set_dir ( pinSELECT, GPIO_INPUT );
    gpio_set_dir ( pinBUZZER, GPIO_OUT);

    /* Seteamos la funcion */
    gpio_set_function ( pinINYECTORES, GPIO_FUNC_PWM );

    /* Seteamos los estados iniciales de las salidas */
    gpio_put ( LED_rpi_pico, HIGH );
    gpio_put ( pinBOMBA1, LOW );
    gpio_put ( pinBOMBA2, LOW );
    gpio_put ( pinRGB_red, LOW );
    gpio_put ( pinRGB_green, LOW );
    gpio_put ( pinRGB_blue, LOW );
    gpio_put ( pinBUZZER, LOW);
    
    /* Configuramos las interrupciones */
    // UART init
    uart_init ( UART_ID, BAUD_RATE );
    gpio_set_function ( UART_TX_PIN, GPIO_FUNC_UART );
    gpio_set_function ( UART_RX_PIN, GPIO_FUNC_UART );
    uart_set_format ( UART_ID, DATA_BITS, STOP_BITS, PARITY );
    uart_set_fifo_enabled ( UART_ID, false );
    // irq_set_exclusive_handler ( UART0_IRQ, vISR_uartrx );
    // irq_set_enabled ( UART0_IRQ, true);
    // uart_set_irq_enables ( UART_ID, true, false );

    // // Interrupciones por cambio de flanco en pines
    gpio_pull_up ( pinENCODER_A );
    gpio_pull_up ( pinENCODER_B );
    gpio_set_irq_enabled_with_callback ( pinENCODER_A, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &vISR_gpioAcallback );
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

static void prvEncoderAntirrebote ( TimerHandle_t xTimer )
{
    bEncAB = false;
}
/*-----------------------------------------------------------*/

static void prvConfigTimer ( TimerHandle_t xTimer )
{
    if ( gpio_get ( pinRGB_blue ) || gpio_get ( pinRGB_green ) )
        gpio_put ( pinRGB_blue, LOW ),gpio_put ( pinRGB_green, LOW );
    gpio_put ( pinRGB_red, !gpio_get ( pinRGB_red ) );
}
/*-----------------------------------------------------------*/

void Mef_init ( void )
{
    uart_puts ( UART_ID, "page pMenuInicio" );
    SentData_Nextion ();
    vTaskDelay ( DELAY_15ms );
    Est_mef = IDLE;
    Modo = IDLE;

    return;
}
/*-----------------------------------------------------------*/

void Mef ( void )
{
    switch ( Est_mef )
    {
        // Entra en modo de reposo
        case IDLE:
        {
            Modo = IDLE;

            // Apaga los leds rojo y verde
            if ( gpio_get ( pinRGB_red ) || gpio_get ( pinRGB_green ) )
            {
                gpio_put ( pinRGB_red, LOW );
                gpio_put ( pinRGB_green, LOW );  
            }

            // Prende el lez azul de estado inactivo
            gpio_put ( pinRGB_blue, HIGH );

            // Detecta cuando se pulsa la opcion
            if ( gpio_get ( pinENCODER_PUSH ) )
            {
                if ( ucEncCont == 0)         Est_mef = MODO_PULV;   // Modo de pulverizacion
                else if ( ucEncCont == 1)    Est_mef = MODO_FLUJO;  // Modo de flujo
                else if ( ucEncCont == 2 )   Est_mef = MODO_FUGA;   // Modo de fuga
                
                Select = RPM_CONFIG;            // Seteamos en la nextion la pagina de rpm config
                ucEncCont = 0;                  // Limpiamos el contador o selector

                gpio_put ( pinRGB_blue, LOW );  // Apaga el led azul
                xTimerStart ( xConfigTimer,0 ); // Activamos el timer
                
                // Envia datos a la nextion
                uart_puts ( UART_ID, "page pMenuRPM" );     // Cambia de pagina
                SentData_Nextion ();

                Antirrebote ();
            }
        break;
        }
        // Configura los parametros para el modo flujo
        case MODO_PULV:
        {
            Modo = MODO_PULV;

            switch ( Select )
            {
                case RPM_CONFIG:
                {
                    // Carga el valor de rpm
                    strData_iny.usRpm_value = ucEncCont;
                    
                    // Detecta pulsador del encoder
                    if ( gpio_get ( pinENCODER_PUSH ) )
                    {
                        Select = PWM_CONFIG;    // Configura el pwm
                        ucEncCont = 0;          // Reinicia el contador
                        Antirrebote ();
                    }
                break;
                }
                case PWM_CONFIG:
                {
                    strData_iny.ucPwm_value = ucEncCont;    // Carga el valor del pwm
                    if ( gpio_get ( pinENCODER_PUSH ) )
                        Select = SEG_CONFIG, ucEncCont = 0, Antirrebote ();
                break;
                }
                
            }
            
        break;
        }
        case MODO_FLUJO:
        {
            Modo = MODO_PULV;

        break;
        }
        case MODO_FUGA:
        {
            Modo = MODO_FUGA;


        break;
        }
    }
    return;
}
/*-----------------------------------------------------------*/

void Antirrebote ( void )
{
    gpio_put ( pinBUZZER, HIGH );
    vTaskDelay ( DELAY_60ms );
    gpio_put ( pinBUZZER, LOW );

    while ( gpio_get ( pinENCODER_PUSH ) )
    {
        vTaskDelay ( DELAY_60ms );
    }

    return;
}
/*-----------------------------------------------------------*/

void SentData_Nextion ( void )
{
    printf ( "\xFF\xFF\xFF" );
}
/*-----------------------------------------------------------*/

void vISR_uartrx ( void )
{
    BaseType_t xHigherPriorityTaskWoken;

    xHigherPriorityTaskWoken = pdFALSE;

    xSemaphoreGiveFromISR ( xUartRxSemaphore,&xHigherPriorityTaskWoken );
    // portYIELD_FROM_ISR ();
    portYIELD_FROM_ISR ( xHigherPriorityTaskWoken );
}
/*-----------------------------------------------------------*/

static void vISR_uartrx_Task ( void *pvParameter )
{
    while (1)
    {
        if ( xSemaphoreTake ( xUartRxSemaphore,DELAY_500ms ) == pdPASS )
        {
            while (uart_is_readable(UART_ID))
            {
                char cRx;
                cRx = uart_getc(UART_ID);
                if(cRx != 0x0A )    // Ultimo valor leido es el salto de linea ( 0x0A o 10 )
                {
                    cStringRx[ucIndice] = cRx;
                    ucIndice++;
                }
                else
                    bStringComplete = true;
                // uart_putc(UART_ID, cRx);
            }
        }
    }
    vTaskDelete ( NULL );
}
/*-----------------------------------------------------------*/

void vISR_gpioAcallback ( uint gpio, uint32_t events )
{
    BaseType_t xHigherPriorityTaskWoken;
    
    xHigherPriorityTaskWoken = pdFALSE;

    if ( !bEncAB )
    {
        if ( events == GPIO_IRQ_EDGE_RISE )
            xSemaphoreGiveFromISR ( xEncA_rising, &xHigherPriorityTaskWoken );
        else
            xSemaphoreGiveFromISR ( xEncA_falling, &xHigherPriorityTaskWoken );
        
        bEncAB = true;
    }

    xTimerResetFromISR ( xAntirreboteTimer, &xHigherPriorityTaskWoken );
    xTimerStartFromISR ( xAntirreboteTimer, &xHigherPriorityTaskWoken );
    
    portYIELD_FROM_ISR ( xHigherPriorityTaskWoken );
}
/*-----------------------------------------------------------*/

static void vISR_pinA_edgeRising ( void *pvParameter )
{
    while (1)
    {
        // Espera a tomar el semaforo dado en la interrupcion    
        if ( xSemaphoreTake ( xEncA_rising,portMAX_DELAY ) == pdPASS )
        {
                if ( !gpio_get ( pinENCODER_B ) && gpio_get ( pinENCODER_A ) )
                    ModSelect ( true );
                else if ( gpio_get ( pinENCODER_B ) && gpio_get ( pinENCODER_A ) )
                    ModSelect ( false );
        }
    }
    vTaskDelete ( NULL );    
}
/*-----------------------------------------------------------*/

static void vISR_pinA_edgeFalling ( void *pvParameter )
{
    while (1)
    {
        // Espera a tomar el semaforo dado en la interrupcion    
        if ( xSemaphoreTake ( xEncA_falling,portMAX_DELAY ) == pdPASS )
        {
            if ( gpio_get ( pinENCODER_B ) && !gpio_get ( pinENCODER_A ) )
                ModSelect ( true );
            else if ( !gpio_get ( pinENCODER_B ) && !gpio_get ( pinENCODER_A ) )
                ModSelect ( false );
        }
    }
    vTaskDelete ( NULL );    
}
/*-----------------------------------------------------------*/

void ModSelect ( uint8_t valor )
{
    char buffer_send[30];

    if ( Modo == IDLE )
    {
        // // Aumenta o disminuye el valor de la seleccion
        if ( valor )
            ucEncCont++;
        else
            ucEncCont--;
        
        if ( ucEncCont > 2 )    ucEncCont = 0;  // Reinicia la cuenta
        
        // sprintf ( buffer_send, "Valor:%d\n", ucEncCont );
        // uart_puts ( UART_ID, buffer_send );
        // uart_putc ( UART_ID, 13 );
        // Envia datos segun se encuentra en cada seleccion
        if ( ucEncCont == 0 )
        {
            uart_puts ( UART_ID, "ref 0" );
            SentData_Nextion ();
            vTaskDelay ( DELAY_15ms );
            uart_puts ( UART_ID, "draw 18,129,109,200,RED" );
            SentData_Nextion ();
            vTaskDelay ( DELAY_15ms );
        }
        else if ( ucEncCont == 1 )
        {
            uart_puts ( UART_ID, "ref 0" );
            SentData_Nextion ();
            vTaskDelay ( DELAY_15ms );
            uart_puts ( UART_ID, "draw 116,128,207,200,RED" );
            SentData_Nextion ();
            vTaskDelay ( DELAY_15ms );
        }
        else if ( ucEncCont == 2 )
        {
            uart_puts ( UART_ID, "ref 0" );
            SentData_Nextion ();
            vTaskDelay ( DELAY_15ms );
            uart_puts ( UART_ID, "draw 212,129,303,200,RED" );
            SentData_Nextion ();
            vTaskDelay ( DELAY_15ms );
        }
    }
    else if ( Modo == MODO_PULV )
    {
        // Configuracion de RPM
        if ( Select == RPM_CONFIG )
        {
            if ( valor )
            {
                if ( ucEncCont <= 9900 )
                    ucEncCont+=100;
            }
            else
            {   
                if ( ucEncCont >= 100 )
                    ucEncCont-=100;
            }
        }
        // Configuracion PWM
        if ( Select == PWM_CONFIG )
        {
            if ( valor )
            {
                if ( ucEncCont <= 90 )
                    ucEncCont+=10;
            }
            else
            {   
                if ( ucEncCont >= 10 )
                    ucEncCont-=10;
            }
        }

        // Envia los datos al puerto serial
        sprintf ( buffer_send, "PWM set:%d\n", strData_iny.ucPwm_value );
        uart_puts ( UART_ID, buffer_send );
        sprintf ( buffer_send, "RPM set:%d\n", strData_iny.usRpm_value );
        uart_puts ( UART_ID, buffer_send );
    }
    
    // Activa el buzzer 
    gpio_put ( pinBUZZER,HIGH );
    vTaskDelay ( DELAY_30ms );
    gpio_put ( pinBUZZER,LOW );
    vTaskDelay ( DELAY_15ms );
    
    return;
}
/*-----------------------------------------------------------*/

