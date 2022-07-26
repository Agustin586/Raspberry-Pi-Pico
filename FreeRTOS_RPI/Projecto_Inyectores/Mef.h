/**
 * @file Mef.h
 * @author Agustin Zuliani (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-08
 * 
 * @copyright Copyright (c) 2022
 * 
 * Este archivo se encarga de generar el codigo necesario para la parte 
 * de la maquina de estado, teniendo como parte principal el switch 
 * que conmutara segun sea el caso. Tambien puede existir mas de un
 * switch si existiera mas de un menu, o en su caso submenus.
 */
#ifndef MEF_H
#define MEF_H

#include <stdbool.h>

// **Variables** //
extern enum Data_est Est_mef;
extern enum Mef_est Estado_M;                   // Estado del selector de menu
extern enum Ms_est Estado_Ms;                   // Estado del menu selector de modo de trabajo
extern enum Ms_SMp_est Estado_Ms_SMp;
extern enum Ms_SMx_est Estado_Ms_SMx_Time;
extern enum Mef_Inicio_est Estado_Salida;
extern bool bInicio;                            // Activa la salida y el conteo

// **Funciones** //

// --No FreeRTOS
void MEF_Init ( void );
void MEF ( void );
static void MEF_Mi ( void );            // Configuracion inicial de los menu
static void MEF_Mr ( void );            // Menu reloj
static void MEF_Mt ( void );            // Menu temperatura
static void MEF_Ms ( void );            // Menu selector
static void MEF_Mc ( void );            // Menu configuracion
static void PROCESO_SALIDA ( void );       // Menu:selector; MEF:proceso de activado
static void Ms_SMfu ( void );           // Menu selector --> Sub menu de trabajo fuga
static void Ms_SMfl ( void );           // Menu selector --> Sub menu de trabajo flujo
static void Ms_SMp ( void );            // Menu selector --> Sub menu de trabajo pulverizacion
static void Ms_SMr ( void );            // Menu selector --> Sub menu de reseteo de datos
static void Ms_SMp_RPM ( void );        // Menu selector --> Sub menu pulverizacion --> Seleccion RPM
static void Ms_SMp_PWM ( void );        // Menu selector --> Sub menu pulverizacion --> Seleccion PWM
static void Ms_SMx_TIME ( void );       // Menu selector --> Sub menu pulverizacion --> Seleccion TIME
static void Ms_SMp_Aceptar ( void );    // Menu selector --> Sub menu pulverizacion --> Aceptar
static void Ms_SMsalida ( void );       // Menu selector --> Sub menu inicio
static void Ms_SMx_Atras ( void );

// --FreeRTOS
void vMEF_Task ( void *pvParameter );

#endif /* MEF.H */