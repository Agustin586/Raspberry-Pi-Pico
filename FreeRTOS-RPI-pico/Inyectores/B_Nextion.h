/**
 * @file B_Nextion.h
 * @author Agustin Zuliani (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-08
 * 
 * @copyright Copyright (c) 2022
 * 
 * Este archivo se encarga de procesar todo lo relacionado con la pantalla
 * haciendo uso del puerto serie del microcontrolador rp2040 y enviar
 * datos hacia otro archivo en caso de procesar algo en el microcontrolador.
 */

#ifndef BLOQUE_NEXTION_H
#define BLOQUE_NEXTION_H

#include "Includes.h"
// #include "Defines.h"
// #include "hardware/uart.h"
// #include "string.h"
// #include "stdio.h"
// #include "B_Control.h"

// **Variables**//
extern volatile uint16_t ucEncCont;
extern struct Nextion_pos Rectangulo;

// **Funciones** //

// --NO FreeRTOS
void Ms_NextionAct ( void );    // Menu selector --> Actualizacion display
void Ms_SMp_SeleccionConfig ( void );       // Menu selector --> Pulv --> Seleccion
void Ms_SMrAModo ( void );
void Nextion_Ms_SMp_Srpm ( bool color );
void Nextion_Ms_SMp_Spwm ( bool color );
void Nextion_Ms_SMp_Stime ( bool color );
void Nextion_Ms_SMp_RPMva ( uint16_t val ); // Menu selector --> Pulv --> Modifica valor rpm
void Nextion_Ms_SMp_PWMva ( uint8_t val );
void Nextion_Ms_SMx_TIMEh ( uint8_t hora );
void Nextion_Ms_SMx_TIMEm ( uint8_t min );
void Nextion_Ms_SMx_TIMEs ( uint8_t seg );
void Nextion_Ms_SMx_SAtras ( void );
void SentData_Nextion ( void );

#endif /* BLOQUE_NEXTION.H */