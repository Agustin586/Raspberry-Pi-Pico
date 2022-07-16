#include "Mef.h"
#include "B_Control.h"
#include "B_Nextion.h"

typedef enum Data_est
{
    SET_DATE,
    MODO_FLUJO,
    MODO_PULV,
    MODO_FUGA,
    IDLE
}Data_est;
typedef enum Mef_est
{
    INIT,
    RELOJ,
    TEMPETURA,
    CONFIGURACION,
    SELECTOR_MODO
}Mef_est;
typedef enum Ms_est
{
    FUGA,
    FLUJO,
    PULVERIZACION,
    RESET
}Ms_est;
typedef enum Ms_SMp_est
{
    SELECTOR,
    RPM,
    PWM,
    TIME,
    ACEPTAR,
    ATRAS
}Ms_SMp_est;
typedef enum Ms_SMx_est
{
    HORA,
    MINUTO,
    SEGUNDO
}Ms_SMx_est;

Data_est Est_mef;               // Estado de la maquina de estado
Mef_est Estado_M;               // Estado Menu
Ms_est Estado_Ms;               // Estado SubMenu
Ms_SMp_est Estado_Ms_SMp;       // Estado 
Ms_SMx_est Estado_Ms_SMx_Time;

// **Configuracion Inicial** //

void MEF_Init ( void )
{
    uart_puts ( UART_ID, "page pMenuInicio" );
    SentData_Nextion ();
    vTaskDelay ( DELAY_15ms );
    Estado_M = INIT;
    Estado_Ms = RESET;
    Estado_Ms_SMp = SELECTOR;
    Estado_Ms_SMx_Time = HORA;

    return;
}

// **** //

// **Menus**//

// --Seleccion Menus
void MEF ( void )
{
    switch ( Estado_M )
    {
        case INIT:              // Menu de configuracion inicial
        {
            MEF_Mi ();              // Menu: inicio (configuraciones iniciales)
            Estado_M = RELOJ;
        break;
        }
        case RELOJ:             // Menu de reloj
        {
            MEF_Mr ();              // Menu: reloj
            Estado_M = TEMPETURA;
        break;
        }
        case TEMPETURA:         // Menu de temperatura
        {
            MEF_Mt ();              // Menu: temperatura
            Estado_M = SELECTOR_MODO;
        break;
        }
        case SELECTOR_MODO:     // Menu de selector de modos de trabajo
        {
            MEF_Ms ();              // Menu: seleccion modo de trabajo
            Estado_M = RELOJ;
        break;
        }
    }

    return;
}
// --Menu inicio
static void MEF_Mi ( void )
{
    return;
}
// --Menu reloj
static void MEF_Mr ( void )
{
    Control_Mr_lectura ();
    Control_Mr_escritura ();

    return;
}
// --Menu temperatura
static void MEF_Mt ( void )
{
    Control_Mt_lectura ();
    
    return;
}
// --Menu configuracion
static void MEF_Mc ( void )
{
    return;
}
// --Menu seleccion modo de trabajo
static void MEF_Ms ( void )
{
    switch ( Estado_Ms )
    {
        case FUGA:
        {
            Ms_SMfu ();         // Menu:selector; Submenu:fuga
        break;
        }
        case FLUJO:
        {
            Ms_SMfl ();         // Menu:selector; Submenu:flujo
        break;
        }
        case PULVERIZACION:
        {
            Ms_SMp ();          // Menu:selector; Submenu:pulverizacion
        break;
        }
        case RESET:
        {
            Ms_SMr ();
            if ( Control_Ms_Smodo () == FUGA )                  Estado_Ms = FUGA;
            else if ( Control_Ms_Smodo () == FLUJO )            Estado_Ms = FLUJO;
            else if ( Control_Ms_Smodo () == PULVERIZACION )    Estado_Ms = PULVERIZACION;
        break;
        }
    }

    return;
}
// **** //

// ** SubMenus ** //

// --SubMenu fuga
static void Ms_SMfu ( void )
{
    return;
}
// --SubMenu flujo
static void Ms_SMfl ( void )
{
    return;
}
// --SubMenu pulverizacion
static void Ms_SMp ( void )
{
    switch ( Estado_Ms_SMp )
    {
        case SELECTOR:
        {
            Control_Ms_SMp_Selec ( &Estado_Ms_SMp );
        break;
        }
        case RPM:
        {
            Ms_SMp_RPM ();
        break;
        }
        case PWM:
        {
            Ms_SMp_PWM ();
        break;
        }
        case TIME:
        {
            Ms_SMx_TIME ();
        break;
        }
        case ACEPTAR:
        {

        break;
        }
        case ATRAS:
        {
            Ms_SMx_Atras ();
        break;
        }
    }
    return;
}
static void Ms_SMr ( void )
{
    Control_Ms_SMr_datos ();

    return;
}
static void Ms_SMp_RPM ( void )
{
    // Modifica el valor del rpm
    if ( Control_Ms_SMp_RPMva () ) Estado_Ms_SMp = SELECTOR;

    return;
}
static void Ms_SMp_PWM ( void )
{
    if ( Control_Ms_SMp_PWMva () )  Estado_Ms_SMp = SELECTOR;

    return;
}
static void Ms_SMx_TIME ( void )
{
    switch ( Estado_Ms_SMx_Time )
    {
        case HORA:
        {
            if ( Control_Ms_SMx_TIMEh () ) Estado_Ms_SMx_Time = MINUTO;
        break;
        }
        case MINUTO:
        {
            if ( Control_Ms_SMx_TIMEm () ) Estado_Ms_SMx_Time = SEGUNDO;
        break;
        }
        case SEGUNDO:
        {
            if ( Control_Ms_SMx_TIMEs () ) Estado_Ms_SMx_Time = HORA,Estado_Ms_SMp = SELECTOR;
        break;
        }
    }

    return;
}
static void Ms_SMx_Atras ( void )
{
    Estado_Ms_SMp = SELECTOR;
    Estado_Ms = RESET;

    return;
}
// **** //

// void MEF ( void )
// {
//     switch ( Est_mef )
//     {
//         // Entra en modo de reposo
//         case IDLE:
//         {
//             Modo = IDLE;

//             // Apaga los leds rojo y verde
//             if ( gpio_get ( pinRGB_red ) || gpio_get ( pinRGB_green ) )
//             {
//                 gpio_put ( pinRGB_red, LOW );
//                 gpio_put ( pinRGB_green, LOW );  
//             }

//             // Prende el lez azul de estado inactivo
//             gpio_put ( pinRGB_blue, HIGH );

//             // Detecta cuando se pulsa la opcion
//             if ( gpio_get ( pinENCODER_PUSH ) )
//             {
//                 if ( ucEncCont == 0)         Est_mef = MODO_PULV;   // Modo de pulverizacion
//                 else if ( ucEncCont == 1)    Est_mef = MODO_FLUJO;  // Modo de flujo
//                 else if ( ucEncCont == 2 )   Est_mef = MODO_FUGA;   // Modo de fuga
                
//                 Select = RPM_CONFIG;            // Seteamos en la nextion la pagina de rpm config
//                 ucEncCont = 0;                  // Limpiamos el contador o selector

//                 gpio_put ( pinRGB_blue, LOW );  // Apaga el led azul
//                 xTimerStart ( xConfigTimer,0 ); // Activamos el timer
                
//                 // Envia datos a la nextion
//                 uart_puts ( UART_ID, "page pMenuRPM" );     // Cambia de pagina
//                 SentData_Nextion ();

//                 Antirrebote ();
//             }
//         break;
//         }
//         // Configura los parametros para el modo flujo
//         case MODO_PULV:
//         {
//             Modo = MODO_PULV;

//             switch ( Select )
//             {
//                 case RPM_CONFIG:
//                 {
//                     // Carga el valor de rpm
//                     strData_iny.usRpm_value = ucEncCont;
                    
//                     // Detecta pulsador del encoder
//                     if ( gpio_get ( pinENCODER_PUSH ) )
//                     {
//                         Select = PWM_CONFIG;    // Configura el pwm
//                         ucEncCont = 0;          // Reinicia el contador
//                         Antirrebote ();
//                     }
//                 break;
//                 }
//                 case PWM_CONFIG:
//                 {
//                     strData_iny.ucPwm_value = ucEncCont;    // Carga el valor del pwm
//                     if ( gpio_get ( pinENCODER_PUSH ) )
//                         Select = SEG_CONFIG, ucEncCont = 0, Antirrebote ();
//                 break;
//                 }
                
//             }
            
//         break;
//         }
//         case MODO_FLUJO:
//         {
//             Modo = MODO_PULV;

//         break;
//         }
//         case MODO_FUGA:
//         {
//             Modo = MODO_FUGA;


//         break;
//         }
//     }
//     return;
// }