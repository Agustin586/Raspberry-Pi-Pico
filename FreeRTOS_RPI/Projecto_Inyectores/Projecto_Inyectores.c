#include "ConfigFreeRtos.h"
#include "hardware/i2c.h"
#include "Defines.h"
#include <math.h>
#include <stdio.h>
#include "pico/stdlib.h"

#ifndef mainRUN_FREE_RTOS_ON_CORE
#define mainRUN_FREE_RTOS_ON_CORE 0
#endif

// Declaraciones iniciales //

// Funciones
static void prvSetupHardware( void );                   // Inicializacion
void vISR_uartrx ( void );                              // Hardware interrupt
void vISR_gpioAcallback ( uint gpio, uint32_t events ); // Funcion Timer
void vISR_gpioBcallback ( uint gpio, uint32_t events ); // Funcion Timer
static void prvLaunchRTOS ( void );                     // Codigo donde corre el FreeRTOS (nucleo 0)
static void prvCore1Entry ( void );                     // Codigo donde corre el nucleo 1
uint32_t ulConversorMascara ( uint8_t gpio );           // Realiza la conversion de gpio a bit 
uint32_t potencia ( uint32_t base, uint8_t exponente );
// Variables
volatile uint16_t ucEncCont = 0;

// Fin declaraciones //

// Cuerpo de funciones //

// Principal
int main(void)
{
    uint32_t input=0,output=0,total=0,aux=0;

    prvSetupHardware ();                        // Configuraciones inciales
    vSetupFreeRtos();                           // Configura el freertos
    multicore_launch_core1( prvCore1Entry );    // Habilita el nucleo 1

    #if ( mainRUN_FREE_RTOS_ON_CORE == 0 )
        prvLaunchRTOS();
    #else
        prvNonRTOSWorker();
    #endif
}

// Codigo de Nucleo 1:
static void prvNonRTOSWorker ()
{
    /* Codigo del nucleo 1 */
    while ( true )
    {
        
    }   
}

// Codigo de Nucleo 0:
static void prvLaunchRTOS () 
{
    vTaskStartScheduler ();     // Habilita el kernel
    panic_unsupported ();
}

// Eleccion del nucleo que corre FreeRtos
static void prvCore1Entry() 
{
    #if ( mainRUN_FREE_RTOS_ON_CORE == 1 )
        prvLaunchRTOS();
    #else
        prvNonRTOSWorker ();
    #endif
}

// Inicializacion
static void prvSetupHardware( void )
{
    uint32_t input=0,output=0,total=0,aux=0;

    stdio_init_all();
    
    // Cargamos las salidas como una mascara bits
    aux = ulConversorMascara ( pinINYECTORES );
    output = aux;
    aux = ulConversorMascara ( pinBOMBA1 );
    output = output | aux;
    aux = ulConversorMascara ( pinBOMBA2 );
    output = output | aux;
    aux = ulConversorMascara ( pinBUZZER );
    output = output | aux;
    aux = ulConversorMascara ( pinRGB_red );
    output = output | aux;
    aux = ulConversorMascara ( pinRGB_blue );
    output = output | aux;
    aux = ulConversorMascara ( pinRGB_green );
    output = output | aux;
    aux = ulConversorMascara ( LED_rpi_pico );
    output = output | aux;

    // Cargamos las entradas como una mascara de bits
    aux = ulConversorMascara ( pinENCODER_PUSH );
    input = input | aux;
    aux = ulConversorMascara ( pinSELECT );
    input = input | aux;
    aux = ulConversorMascara ( pinENCODER_A );
    input = input | aux;
    aux = ulConversorMascara ( pinENCODER_B );

    // Total de pines utilizados
    total = input | output;

    // Inicializamos
    gpio_init_mask ( total );

    // Seteamos las entradas y salidas
    gpio_set_dir_masked ( total, output );

    // Test de salidas para el correcto funcionamiento
    // gpio_put_masked ( output, 0xffffffff );
    gpio_put_all ( output );
    sleep_ms(1500);
    
    // Estado inicial de gpios
    gpio_put_masked ( output, LOW );
    // gpio_put ( pinBUZZER, HIGH );
    
    // Funciones de gpios
    gpio_set_function ( pinINYECTORES, GPIO_FUNC_PWM );
    
    // UART init
    uart_init ( UART_ID, BAUD_RATE );
    gpio_set_function ( UART_TX_PIN, GPIO_FUNC_UART );
    gpio_set_function ( UART_RX_PIN, GPIO_FUNC_UART );
    uart_set_format ( UART_ID, DATA_BITS, STOP_BITS, PARITY );
    uart_set_fifo_enabled ( UART_ID, false );
    
    // Interrupciones por cambio de flanco en pines
    gpio_pull_up ( pinENCODER_A );
    gpio_pull_up ( pinENCODER_B );
    gpio_set_irq_enabled_with_callback ( pinENCODER_A, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &vISR_gpioAcallback );

    // I2C
    i2c_init ( I2C_ID, 100 * 1000 );
    gpio_set_function ( I2C_sda, GPIO_FUNC_I2C );
    gpio_set_function( I2C_scl, GPIO_FUNC_I2C );
    gpio_pull_up ( I2C_sda );
    gpio_pull_up ( I2C_scl );

    // ADC
    adc_init();
}
uint32_t ulConversorMascara ( uint8_t gpio )
{
    return potencia ( 2, gpio );
}
uint32_t potencia ( uint32_t base, uint8_t exponente )
{
    uint32_t aux=base;

    if ( exponente != 0 )
    {
        while ( exponente > 1 )
        {
            exponente--;
            base = base * aux;
        }
    }

    return base;
}
// Interrupciones
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

// FreeRtos
void vApplicationTickHook( void )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    static uint32_t ulCount = 0;
}
void vApplicationMallocFailedHook( void )
{
    panic("malloc failed");
}
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) xTask;
    for( ;; );
}
void vApplicationIdleHook( void )
{
    volatile size_t xFreeStackSpace;
    xFreeStackSpace = xPortGetFreeHeapSize();

    if( xFreeStackSpace > 100 )
    {
    }
}

// Fin cuerpo de funciones //


// Inicializacion de gpios
    // gpio_init ( LED_rpi_pico );
    // gpio_init ( pinINYECTORES );
    // gpio_init ( pinBOMBA1 );
    // gpio_init ( pinBOMBA2 );
    // gpio_init ( pinENCODER_PUSH );
    // gpio_init ( pinBUZZER );
    // gpio_init ( pinRGB_red );
    // gpio_init ( pinRGB_green );
    // gpio_init ( pinRGB_blue );
    // gpio_init ( pinSELECT );
    // gpio_set_dir ( pinENCODER_A, GPIO_INPUT );
    // gpio_set_dir ( pinENCODER_B, GPIO_INPUT );

    // irq_set_exclusive_handler ( UART0_IRQ, vISR_uartrx );
    // irq_set_enabled ( UART0_IRQ, true);
    // uart_set_irq_enables ( UART_ID, true, false );

    // gpio_set_dir ( LED_rpi_pico, GPIO_OUT );
    // gpio_set_dir ( pinBOMBA1, GPIO_OUT );
    // gpio_set_dir ( pinBOMBA2, GPIO_OUT );
    
    // gpio_set_dir ( pinENCODER_PUSH, GPIO_INPUT );
    // gpio_set_dir ( pinRGB_red, GPIO_OUT );
    // gpio_set_dir ( pinRGB_green, GPIO_OUT );
    // gpio_set_dir ( pinRGB_blue, GPIO_OUT );
    // gpio_set_dir ( pinSELECT, GPIO_INPUT );
    // gpio_set_dir ( pinBUZZER, GPIO_OUT);

    // gpio_put ( LED_rpi_pico, HIGH );
    // gpio_put ( pinBOMBA1, LOW );
    // gpio_put ( pinBOMBA2, LOW );
    // gpio_put ( pinRGB_red, LOW );
    // gpio_put ( pinRGB_green, LOW );
    // gpio_put ( pinRGB_blue, LOW );
    // gpio_put ( pinBUZZER, LOW);