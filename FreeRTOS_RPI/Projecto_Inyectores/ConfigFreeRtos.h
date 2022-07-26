#ifndef CONFIG_FREERTOS_H
#define CONFIG_FREERTOS_H

#include "Includes.h"

// **Variables** //

// --Tareas
extern volatile uint16_t ucEncCont;
// --Timers
extern TimerHandle_t xAntirreboteTimer,xConfigTimer,xAdcTimer,xRelojTimer;
extern volatile bool bEncAB;
extern bool bLeerReloj;
extern bool bLeerAdc;
// --Semaforos
extern SemaphoreHandle_t xUartRxSemaphore;
extern SemaphoreHandle_t xEncA_rising;
extern SemaphoreHandle_t xEncA_falling;
// --Uart
extern volatile bool bStringComplete;
extern char cStringRx[20];
extern volatile uint8_t ucIndice;

// **Funciones** //

// --Tareas
static void vMEF_Task ( void *pvParameter );
static void vUartRx_Task ( void *pvParameter );
static void vAdc_Task ( void *pvParameter );
// --Configuracion Inicial
void vSetupFreeRtos ( void );           // Configuraciones inicial de freertos
static void prvSetupTimer ( void );
static void prvSetupTask ( void );
static void prvSetupSemp ( void );
// --Timers
static void prvBlinkTimer ( TimerHandle_t xTimer );         // Realiza un blink en el pin 25
static void prvPwmEnableTimer ( TimerHandle_t xTimer );     // Realiza un blink cuando se activa la salida pwm
static void prvEncoderAntirrebote ( TimerHandle_t xTimer ); // Antirrebote para el encoder
static void prvConfigTimer ( TimerHandle_t xTimer );        // 
static void prvRelojTimer ( TimerHandle_t xTimer );         // Temporizador de lectura de reloj
static void prvAdcTimer ( TimerHandle_t xTimer );           // Temporizador de lectura de adc
// --Semaforos
static void vISR_pinA_edgeRising ( void *pvParameter ); // Procesa la interrupcion por flanco ascendente
static void vISR_pinA_edgeFalling ( void *pvParameter );// Procesa la interrupcion por flanco descendente
static void vISR_uartrx_Task ( void *pvParameter );     // Procesa la interrupcion por uart

#endif /* CONFIG_FREERTOS.H */