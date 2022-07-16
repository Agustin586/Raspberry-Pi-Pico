#ifndef DEFINES_H
#define DEFINES_H

// **Definiciones** //

// --Delays 
#define ANT_DELAY   pdMS_TO_TICKS ( 40 )
#define DELAY_500ms pdMS_TO_TICKS ( 500 )
#define DELAY_300ms pdMS_TO_TICKS ( 300 )
#define DELAY_15ms  pdMS_TO_TICKS ( 15 )
#define DELAY_200ms pdMS_TO_TICKS ( 200 )
#define DELAY_30ms  pdMS_TO_TICKS ( 30 )
#define DELAY_5ms   pdMS_TO_TICKS ( 5 )
#define DELAY_20ms  pdMS_TO_TICKS ( 20 )
#define DELAY_60ms  pdMS_TO_TICKS ( 60 )
#define DELAY_100ms pdMS_TO_TICKS ( 100 )
#define DELAY_1s    pdMS_TO_TICKS ( 1000 )

// --Pines
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

// --Otros
#define Ms_MAXCANTMODO 3
#define Ms_SMp_MAXCANTSELECT 5
#define Ms_SMp_RPM_MAXVAL   10000
#define Ms_SMp_RPM_MINVAL   500
#define Ms_SMp_PWM_MAXVAL   100
#define FUGA_    0
#define FLUJO_           1
#define PULVERIZACION_   2

// --Configuraciones de FreeRTOS
#define mainSDK_MUTEX_USE_TASK_PRIORITY     ( tskIDLE_PRIORITY + 3 )
#define mainSDK_SEMAPHORE_USE_TASK_PRIORITY ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_RECEIVE_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )
#define mainEVENT_SEMAPHORE_TASK_PRIORITY   ( configMAX_PRIORITIES - 1 )
#define mainSTACKDEPTH  256
#define mainMEF_TASK_PRIORITY               ( tskIDLE_PRIORITY + 2 )
#define mainUARTRX_TASK_PRIORITY            ( tskIDLE_PRIORITY + 1 )
#define mainADC_TASK_PRIORITY               ( tskIDLE_PRIORITY + 1 )

// --Pines I2c
#define I2C_ID      i2c0
#define I2C_sda     4
#define I2C_scl     5

// --Pines Uart
#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// --Rectangulos
#define B_RPM_Xinit (16-1)
#define B_RPM_Yinit (14-1)
#define B_RPM_ANCHO 70
#define B_RPM_ALTO  35
#define B_PWM_Xinit (95-1)
#define B_PWM_Yinit (14-1)
#define B_PWM_ANCHO 70
#define B_PWM_ALTO  35
#define B_TIME_Xinit (174-1)
#define B_TIME_Yinit (14-1)
#define B_TIME_ANCHO 70
#define B_TIME_ALTO  35
#define B_ACEPTAR_Xinit (262-1)
#define B_ACEPTAR_Yinit (10-1)
#define B_ACEPTAR_ANCHO 45
#define B_APCETAR_ALTO  45
#define B_ATRAS_Xinit (12-1)
#define B_ATRAS_Yinit (182-1)
#define B_ATRAS_ANCHO 45
#define B_ATRAS_ALTO  45
#define B_VALOR_Xinit (136-1)
#define B_VALOR_Yinit 86
#define B_VALOR_ANCHO (162-1)
#define B_VALOR_ALTO  85

#endif /* DEFINES.H */