/**
 * @file B_Control.h
 * @author Agustin Zuliani (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-08
 * 
 * @copyright Copyright (c) 2022
 * 
 * Este archivo se encarga de habilitar o deshabilitar salidas, controlar pwm, leer adc,...,
 * es decir todo el control directo con el micro por lo tanto muchas de las funciones
 * de otros archivo estaran ligadas a este para verse reflejado en el micro o en su caso
 * en algun componente externo conectado a este.
 */
#ifndef BLOQUE_CONTROL_H
#define BLOQUE_CONTROL_H


// #include "Includes.h"
// #include "stdint.h"
// #include "stdbool.h"
// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "Defines.h"
// #include "ConfigFreeRtos.h"
// #include "B_Nextion.h"
#include "Includes.h"

// *Variables** //
extern volatile uint16_t ucEncCont;     // Contador que se mueve con el encoder
extern TimerHandle_t xConfigTimer;      // Indica que la configuracion esta activada
extern struct Datos_Iny stDato_Iny;     // Registro de datos de los inyectores
extern enum Ms_SMp_est Estado;          // Estado del sub menu pulverizacion
extern struct Data_rtc strReloj;         // Registro de datos del rtc

// **Funciones** //
// --NO FreeRTOS
void Control_Ms_SMr_datos ( void );
uint8_t Control_Ms_Smodo  ( void );                     // Menu:selector --> Selecciona el modo de trabajo
void Control_Ms_SMp_Selec ( enum Ms_SMp_est *Estado );  // Menu:selector --> Pulverizacion --> Selecciona configuracion
bool Control_Ms_SMp_RPMva ( void );                     // Menu:selector --> Pulverizacion --> Seleccion RPM --> Modificar valor
bool Control_Ms_SMp_PWMva ( void );
bool Control_Ms_SMx_TIMEh ( void );
bool Control_Ms_SMx_TIMEm ( void );
bool Control_Ms_SMx_TIMEs ( void );
void Control_Mr_lectura   ( void );
void Control_Mr_escritura ( void );                     // Menu:reloj --> Envia datos de fecha y hora
void Control_Mt_lectura   ( void );                     // Menu:temperatura --> Lee y manda el valor de temperatura
void Antirrebote ( void );                              // Antirrebote de los pulsadores y encoder

#endif /* BLOQUE_CONTROL.H */