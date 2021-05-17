/*
    Autor: Zuliani, Agustin
    Fecha de inicio: 03/05/21 00:09
    Programa: Este programa controla diferentes hardware del micro, con el objetivo de 
    aprender a utilizarlos y manejarlos mediante un hmi en c#.
    Controles y funciones:
        Core 0:
            Serial: 
                - Control pwm 1 - Unidireccional
                - Control pwm 2 - Unidireccional
                - Datos por pantalla (opcional, segun el tipo de pantalla) - Unidereccional
                - Lectura Adc0 (Temperatura interna del micro) - HMI (Lectura) <- Micro (Datos)
            I2C:
                - Control de pantalla lcd i2c
                - Control de RTC (opcional)
            Pwm 1:
                - Control duty Led 1
            Pwm 2:
                - Control Motor 1
            Adc0:
                - Lectura de temperatura interna del micro
            TMRx:
                - Generacion de ticks
                - Modifica la frecuencia del led de estado
            Frecuencia: reloj:
                - 133 MHz
        Core 1:

*/

/*
    Algoritmo: Projecto 1 Raspberry Pi Pico
    
    Variables:
        Caracter: Rx
        String: RxBuffer[25],

    Inicio Main:

        Inicializacion_I2C()
        Inicializacion_Lcd()
        Inicializacion_Serial()
        Inicializacion_Pwm1()
        Inicializacion_Pwm2()
        Inicializacion_Adc()
        Inicializacion_Core1()
        Inicializacion_Timer()

        Escribri_Serial("Autor:Zualiani,Agustin\n")
        Escribir_Serial("Programa:Practica 1 con manejo de HMI")
        Escribir_Serial("-------------------------------------")
        Escribir_Serial("Programa Inciado")
        Escribir_Serial("-------------------------------------")
        Retardo(1500ms)

        Bucle (infinito)

            Segun sea (Estado_Actual)
                
                caso Estado_Pantalla:
                    Actualizacion_Pantalla()
                caso Estado_Pwm1:
                    Actualizacion_Pwm1()
                caso Estado_Pwm2:
                    Actualizacion_Pwm2()
                caso Estado_Adc:
                    Lectura_datos_Adc()
                caso Estado_Serial:
                    Envia_datos_hmi()
                
            Fin Segun Sea

        Fin Bucle

    Fin Main.


*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include <stdint.h>
#include "Mef.h"
#include "Control_Serial.h"
#include "Control_Lcd.h"


// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9
#define PWM1_PIN    20
#define PWM2_PIN    21
#define LED_BLINK   25

// ================================================================================= //

// --------------------------------------------------------------------------------- //
//                                  VARIABLES                                        //
// --------------------------------------------------------------------------------- //

// Variables para Serial //
volatile char Buffer_HmiToMicro[25],Rx;
volatile bool End_M;
volatile uint8_t Index_HTM;
char CmdToSend[]={"[EVD]\n"},CmdToRecep[]={"[RPD]\n"},CmdToRecepMsj[]={"[MLF]\n"};
// -- //

// Variables para Pwm //
uint16_t Pwm1_Value=0,Pwm2_Value=0;
// -- //

// Variables para la temperatura //
uint16_t Temp;
// -- //

// ================================================================================= //

// ================================================================================= //

// --------------------------------------------------------------------------------- //
//                                  FUNCIONES                                        //
// --------------------------------------------------------------------------------- //

void Perf_init(void);   // Inicializa todos los perifericos

// ================================================================================= //

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}

void Perf_init(void)
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Timer example code - This example fires off the callback after 15ms
    add_alarm_in_ms(15, alarm_callback, NULL, false);

    return;
}

int main()
{
    Perf_init();

    while (1)
    {
        Mef_Updated();
    }

    return 0;
}