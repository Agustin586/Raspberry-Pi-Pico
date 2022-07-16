#include "B_Control.h"
#include "B_Nextion.h"
#include "RTC_DS3231.h"
#include "ConfigFreeRtos.h"
#include "stdbool.h"
#include "Adc.h"

TimerHandle_t xConfigTimer;
typedef struct Datos_Iny      // Datos de salida del pwm
{
    uint8_t  ucPwm_val;
    uint16_t usRpm_val;
    uint8_t  ucSeg;
    uint8_t  ucMin;
    uint8_t  ucHora;
}Datos_Iny;
Datos_Iny stDato_Iny;
typedef enum Ms_SMp_est
{
    SELECTOR,
    RPM,
    PWM,
    TIME,
    ACEPTAR,
    ATRAS
}Ms_SMp_est;
Ms_SMp_est Estado;
typedef struct Data_rtc
{
    uint8_t ucDia;
    uint8_t ucMes;
    uint8_t ucAnio;
    uint8_t ucSeg;
    uint8_t ucMin;
    uint8_t ucHor;
}Data_rtc;
Data_rtc strReloj;

// ** Menu Reloj ** //

void Control_Mr_lectura ( void )
{
    if ( bLeerReloj )
    {
        DS3231_LeerTiempo ( &strReloj.ucHor, &strReloj.ucMin, &strReloj.ucSeg );
        DS3231_LeerFecha ( &strReloj.ucMes, &strReloj.ucDia, &strReloj.ucAnio );
    }

    return;
}
void Control_Mr_escritura ( void )
{
    if ( bLeerReloj )
    {
        Nextion_Mr ( strReloj.ucHor, strReloj.ucMin, strReloj.ucDia, strReloj.ucMes, strReloj.ucAnio );
        bLeerReloj = false;  
    }
    
    return;
}

// **** //

// ** Menu Temperatura ** //

void Control_Mt_lectura ( void )
{
    if ( bLeerAdc )
    {
        Nextion_Mt ( Adc_lectura() );   // Envia a la pantalla el valor de temperatura
        bLeerAdc = false;
    }

    return;
}

// **** //

// **MENU SELECTOR DE MODO DE TRABAJO** //
void Control_Ms_SMr_datos ( void )
{
    // Apaga los led rojo y verde
    if ( gpio_get ( pinRGB_red ) || gpio_get ( pinRGB_green ) )
    {
        gpio_put ( pinRGB_red, LOW );
        gpio_put ( pinRGB_green, LOW );  
    }
    // Prende el lez azul de estado inactivo
    if ( !gpio_get ( pinRGB_blue ) ) gpio_put ( pinRGB_blue, HIGH );

    return;
}

uint8_t Control_Ms_Smodo ( void )
{
    // Pone un tope maximo de valor
    if ( ucEncCont > Ms_MAXCANTMODO-1 )    ucEncCont--;

    // Actualiza el display segun el modo pre-selecionado
    Ms_NextionAct ();

    // Ingresa al modo selecionado
    if ( gpio_get ( pinENCODER_PUSH ) )
    {
        gpio_put ( pinRGB_blue, LOW );  // Apaga el led azul
        xTimerStart ( xConfigTimer,0 );

        // Actualiza la pantalla
        Ms_SMrAModo ();                 // Actualiza el display

        Antirrebote ();                 // Espera de tiempo por el rebote del pulsador
        
        if ( ucEncCont == FUGA_ )                
        {
            ucEncCont=0;
            return (FUGA_);
        } 
        else if ( ucEncCont == FLUJO_ )      
        {
            ucEncCont=0;
            return (FLUJO_);
        }
        else if ( ucEncCont == PULVERIZACION_ )  
        {
            ucEncCont=0;
            return (PULVERIZACION_);
        }
    }
    else return 3;
}

void Control_Ms_SMp_Selec ( Ms_SMp_est *Estado )
{
    // Setea el limite de selecciones
    if ( ucEncCont > Ms_SMp_MAXCANTSELECT ) ucEncCont--;

    if ( ucEncCont == RPM-1 )
        Nextion_Ms_SMp_Srpm (true);     // Actualiza la pantalla
    else if ( ucEncCont == PWM-1 )
        Nextion_Ms_SMp_Spwm (true);     // Actualiza la pantalla
    else if ( ucEncCont == TIME-1 )
        Nextion_Ms_SMp_Stime (true);    // Actualiza la pantalla
    else if ( ucEncCont == ATRAS-1 )
        Nextion_Ms_SMx_SAtras ();

    if ( gpio_get ( pinENCODER_PUSH ) )
    {
        *Estado = ucEncCont;
        ucEncCont = 0;
        Antirrebote ();
    }

    return;
}

bool Control_Ms_SMp_RPMva ( void )
{
    uint16_t usRpmVal;
    
    usRpmVal = ucEncCont * 100;

    // Establece el limite de valor maximo y minimo de Rpm
    if ( usRpmVal < Ms_SMp_RPM_MINVAL ) usRpmVal = Ms_SMp_RPM_MINVAL, ucEncCont = usRpmVal/100;
    if ( usRpmVal > Ms_SMp_RPM_MAXVAL ) usRpmVal = Ms_SMp_RPM_MAXVAL, ucEncCont = usRpmVal/100;

    stDato_Iny.usRpm_val = usRpmVal;                // Actualiza el valor en la variable
    Nextion_Ms_SMp_RPMva ( stDato_Iny.usRpm_val );  // Actualiza la pantalla

    // Detecta si quiere salir de la configuracion de rpm
    if ( gpio_get ( pinENCODER_PUSH ) )
    {
        ucEncCont = 0;              // Reinicia el contador
        Nextion_Ms_SMp_Srpm (false);// Actualiza la pantalla
        Antirrebote ();

        return true;
    }

    return false;
}
bool Control_Ms_SMp_PWMva ( void )
{
    uint8_t ucPwmVal;

    ucPwmVal = ucEncCont * 10;

    // Establece los limites superior e inferior del valor
    if ( ucPwmVal > Ms_SMp_PWM_MAXVAL ) ucPwmVal = Ms_SMp_PWM_MAXVAL, ucEncCont = ucPwmVal/10;

    stDato_Iny.ucPwm_val = ucPwmVal;
    Nextion_Ms_SMp_PWMva ( stDato_Iny.ucPwm_val );

    if ( gpio_get ( pinENCODER_PUSH ) )
    {
        ucEncCont = 0;
        Nextion_Ms_SMp_Spwm (0);
        Antirrebote ();
        return true;
    }

    return false;
}
bool Control_Ms_SMx_TIMEh ( void )
{
    uint8_t Hora;

    // Establece los limites de la hora
    if ( ucEncCont > 60 )   ucEncCont--;

    Hora = ucEncCont;
    stDato_Iny.ucHora = Hora;
    Nextion_Ms_SMx_TIMEh ( stDato_Iny.ucHora );

    if ( gpio_get ( pinENCODER_PUSH ) )
    {
        ucEncCont = 0;
        Antirrebote ();
        return 1;
    }
    else    return 0;
}
bool Control_Ms_SMx_TIMEm ( void )
{
    uint8_t Min;

    // Establece los limites de la hora
    if ( ucEncCont > 60 )   ucEncCont--;

    Min = ucEncCont;
    stDato_Iny.ucMin = Min;
    Nextion_Ms_SMx_TIMEm ( stDato_Iny.ucMin );

    if ( gpio_get ( pinENCODER_PUSH ) )
    {
        ucEncCont = 0;
        Antirrebote ();
        return 1;
    }
    else    return 0;
}
bool Control_Ms_SMx_TIMEs ( void )
{
    uint8_t Seg;

    // Establece los limites de la hora
    if ( ucEncCont > 60 )   ucEncCont--;

    Seg = ucEncCont;
    stDato_Iny.ucSeg = Seg;
    Nextion_Ms_SMx_TIMEs ( stDato_Iny.ucSeg );

    if ( gpio_get ( pinENCODER_PUSH ) )
    {
        ucEncCont = 0;
        Antirrebote ();
        return 1;
    }
    else    return 0;
}

// **** //

void Antirrebote ( void )
{
    gpio_put ( pinBUZZER, HIGH );
    vTaskDelay ( DELAY_60ms );
    gpio_put ( pinBUZZER, LOW );

    while ( gpio_get ( pinENCODER_PUSH ) )
    {
        vTaskDelay ( DELAY_60ms );
    }

    return;
}