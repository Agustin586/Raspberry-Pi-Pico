#include "ConfigFreeRtos.h"
#include "Mef.h"

// --Timers
TimerHandle_t xAntirreboteTimer,xRelojTimer,xAdcTimer;
volatile bool bEncAB = false;
bool bLeerReloj = false;
bool bLeerAdc = false;
// --Semaforos
SemaphoreHandle_t xUartRxSemaphore;
SemaphoreHandle_t xEncA_rising;
SemaphoreHandle_t xEncA_falling;
// --Uart
volatile bool bStringComplete = false;
char cStringRx[20];
volatile uint8_t ucIndice = 0;

// **Configuracion Inicial** //

void vSetupFreeRtos ( void )
{
    prvSetupSemp ();    // Configura los semaforos
    prvSetupTask ();    // Configura las tareas
    prvSetupTimer ();   // Configura los timers
    
    return;
}
// Timers
static void prvSetupTimer ( void )
{
    TimerHandle_t xBlinkTimer, xPwmEnableTimer;

    /* 
    Creamos los software timers:
        1. Blink funcion durante todo el transcurso del programa y realiza un blink en el pin 25
        2. Se activa cuando la salida de pwm esta activa y realiza un blink en el rgb red
        3. Se activa cuando surge el cambio de estado de la interrupcion
        4. Se activa cuando se comienza a configurar los parametros
        5. Cada un segundo activa la lectura del reloj de tiempo real
        6. Temporizador de lectura de temperatura del micro cada 200ms
    */
    xBlinkTimer       = xTimerCreate ( "BLINK",DELAY_500ms,pdTRUE,0,prvBlinkTimer );
    xPwmEnableTimer   = xTimerCreate ( "BLINK_PWM",DELAY_300ms,pdTRUE,0,prvPwmEnableTimer );
    xAntirreboteTimer = xTimerCreate ( "ANTIRREBOTE",DELAY_60ms,pdFALSE,0,prvEncoderAntirrebote );
    xConfigTimer      = xTimerCreate ( "CONFIG",DELAY_200ms,pdTRUE,0,prvConfigTimer );
    xRelojTimer       = xTimerCreate ( "RELOJ", DELAY_1s,pdTRUE,0,prvRelojTimer );
    xAdcTimer         = xTimerCreate ( "ADC",DELAY_200ms,pdTRUE,0,prvAdcTimer);

    // Inicia el/los timer/s
    if ( xBlinkTimer != NULL )  xTimerStart ( xBlinkTimer,0 );
    if ( xRelojTimer != NULL)   xTimerStart ( xRelojTimer, DELAY_100ms );    // Inicia luego de 100ms
    if ( xAdcTimer != NULL )    xTimerStart ( xAdcTimer, DELAY_100ms );

    return;
}
// Tareas
static void prvSetupTask ( void )
{
    xTaskCreate ( vMEF_Task,"MEF",mainSTACKDEPTH,NULL,mainMEF_TASK_PRIORITY,NULL );
    xTaskCreate ( vUartRx_Task,"UART RX",mainSTACKDEPTH,NULL,mainUARTRX_TASK_PRIORITY,NULL );
    xTaskCreate ( vAdc_Task,"READ ADC",mainSTACKDEPTH,NULL,mainADC_TASK_PRIORITY,NULL );

    return;
}
// Semaforos
static void prvSetupSemp ( void )
{
    // Creamos un semaforo para las interrupciones
    // xUartRxSemaphore = xSemaphoreCreateCounting ( 10,0 );   
    xEncA_rising = xSemaphoreCreateCounting ( 5,0 );       // Crea un semaforo con 10 tokens
    xEncA_falling = xSemaphoreCreateCounting ( 5,0 );

    // Creamos las tareas donde utilizan los semaforos
    // if ( xUartRxSemaphore != NULL )
    //     xTaskCreate ( vISR_uartrx_Task,"UART_RX_ISR",mainSTACKDEPTH,NULL,configMAX_PRIORITIES,NULL );
    if ( xEncA_rising != NULL )
        xTaskCreate ( vISR_pinA_edgeRising,"ENC_A_ISR",mainSTACKDEPTH,NULL,configMAX_PRIORITIES-1,NULL );
    if ( xEncA_falling != NULL )
        xTaskCreate ( vISR_pinA_edgeFalling,"ENC_A_ISRfalling",mainSTACKDEPTH,NULL,configMAX_PRIORITIES-1,NULL );

    return;
}

// **** //

// ** Proceso ** //

// Tareas
void vMEF_Task ( void *pvParameter )
{
    MEF_Init ();

    while (1)
    {
        MEF ();
        vTaskDelay ( DELAY_5ms );
    }

    vTaskDelete ( NULL );
    return;
}
static void vUartRx_Task ( void *pvParameter )
{
    while (1)
    {
        vTaskDelay ( DELAY_15ms );
    }

    vTaskDelete ( NULL );
    return;
}
static void vAdc_Task ( void *pvParameter )
{
    while (1)
    {
        vTaskDelay ( DELAY_15ms );
    }

    vTaskDelete ( NULL );
    return;
}
// --Semaforos
static void vISR_pinA_edgeRising ( void *pvParameter )
{
    while (1)
    {
        // Espera a tomar el semaforo dado en la interrupcion    
        if ( xSemaphoreTake ( xEncA_rising,portMAX_DELAY ) == pdPASS )
        {
            if ( !gpio_get ( pinENCODER_B ) && gpio_get ( pinENCODER_A ) )
                ucEncCont++;
            else if ( gpio_get ( pinENCODER_B ) && gpio_get ( pinENCODER_A ) )
                if ( ucEncCont > 0 ) ucEncCont--;
            // Activa el buzzer 
            gpio_put ( pinBUZZER,HIGH );
            vTaskDelay ( DELAY_30ms );
            gpio_put ( pinBUZZER,LOW );
            vTaskDelay ( DELAY_15ms );

            // printf ( "\nencoder:%u",ucEncCont);
        }
    }
    vTaskDelete ( NULL );    
}
static void vISR_pinA_edgeFalling ( void *pvParameter )
{
    while (1)
    {
        // Espera a tomar el semaforo dado en la interrupcion    
        if ( xSemaphoreTake ( xEncA_falling,portMAX_DELAY ) == pdPASS )
        {
            if ( gpio_get ( pinENCODER_B ) && !gpio_get ( pinENCODER_A ) )
                ucEncCont++;
            else if ( !gpio_get ( pinENCODER_B ) && !gpio_get ( pinENCODER_A ) )
                if ( ucEncCont > 0 ) ucEncCont--;
            // Activa el buzzer 
            gpio_put ( pinBUZZER,HIGH );
            vTaskDelay ( DELAY_30ms );
            gpio_put ( pinBUZZER,LOW );
            vTaskDelay ( DELAY_15ms );

            // printf ( "\nencoder:%u",ucEncCont);
        }
    }
    vTaskDelete ( NULL );    
}
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
// --Timers
static void prvBlinkTimer ( TimerHandle_t xTimer )
{
    gpio_put ( LED_rpi_pico, !gpio_get ( LED_rpi_pico ) );
}
static void prvPwmEnableTimer ( TimerHandle_t xTimer )
{
    gpio_put ( pinRGB_red, !gpio_get ( pinRGB_red ) );
}
static void prvEncoderAntirrebote ( TimerHandle_t xTimer )
{
    bEncAB = false;
}
static void prvConfigTimer ( TimerHandle_t xTimer )
{
    if ( gpio_get ( pinRGB_blue ) || gpio_get ( pinRGB_green ) )
        gpio_put ( pinRGB_blue, LOW ),gpio_put ( pinRGB_green, LOW );
    gpio_put ( pinRGB_red, !gpio_get ( pinRGB_red ) );
}
static void prvRelojTimer ( TimerHandle_t xTimer )
{
    bLeerReloj = true;
}
static void prvAdcTimer ( TimerHandle_t xTimer )
{
    bLeerAdc = true;
}

// **** //
