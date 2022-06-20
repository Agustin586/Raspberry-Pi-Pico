#include "ConfigFreeRtos.h"
#include "hardware/i2c.h"
#include "Defines.h"

#ifndef mainRUN_FREE_RTOS_ON_CORE
#define mainRUN_FREE_RTOS_ON_CORE 0
#endif

// **Funciones** //
// --Inicializacion
static void prvSetupHardware( void );
// --Interrupcion
void vISR_uartrx ( void );                              // Hardware interrupt
// --Callback functions
void vISR_gpioAcallback ( uint gpio, uint32_t events );
void vISR_gpioBcallback ( uint gpio, uint32_t events );
// **** //

// **Variables** //
volatile uint16_t ucEncCont = 0;
// **** //

#if configNUM_CORES > 1
#error Require only one core configured for FreeRTOS
#endif

// --Nucleo 1 corriendo
static void prvNonRTOSWorker ()
{
    /* Codigo del nucleo 1 */
    while ( true )
    {
        
    }   
}
// --Nucleo 0 corriendo
static void prvLaunchRTOS () {
    /* Codigo del nucleo 0 */
    /* Start the tasks and timer running. */
    vTaskStartScheduler ();
    /* should never reach here */
    panic_unsupported ();
    // printf("Core %d: Launching FreeRTOS scheduler\n", get_core_num()); 
}

static void prvCore1Entry() {
#if ( mainRUN_FREE_RTOS_ON_CORE == 1 )
    prvLaunchRTOS();
#else
    prvNonRTOSWorker ();
#endif
}

// **Funciones** //

// --Principal
int main(void)
{
    prvSetupHardware ();    // Configuraciones inciales
    vSetupFreeRtos();       // Configura el freertos
    multicore_launch_core1( prvCore1Entry );  // Habilita el nucleo 1

#if ( mainRUN_FREE_RTOS_ON_CORE == 0 )
    prvLaunchRTOS();
#else
    prvNonRTOSWorker();
#endif
}
// --Inicializacion
static void prvSetupHardware( void )
{
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
    // Interrupciones por cambio de flanco en pines
    gpio_pull_up ( pinENCODER_A );
    gpio_pull_up ( pinENCODER_B );
    gpio_set_irq_enabled_with_callback ( pinENCODER_A, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &vISR_gpioAcallback );
    // I2C
    i2c_init( I2C_ID, 100 * 1000 );
    gpio_set_function( I2C_sda, GPIO_FUNC_I2C );
    gpio_set_function( I2C_scl, GPIO_FUNC_I2C );
    gpio_pull_up( I2C_sda );
    gpio_pull_up( I2C_scl );
}
// --Interrupciones
void vISR_uartrx ( void )
{
    BaseType_t xHigherPriorityTaskWoken;

    xHigherPriorityTaskWoken = pdFALSE;

    xSemaphoreGiveFromISR ( xUartRxSemaphore,&xHigherPriorityTaskWoken );
    // portYIELD_FROM_ISR ();
    portYIELD_FROM_ISR ( xHigherPriorityTaskWoken );
}
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
// **Freertos** //
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

// **** //