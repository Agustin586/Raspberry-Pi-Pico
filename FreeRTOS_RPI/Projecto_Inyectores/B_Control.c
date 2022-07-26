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

bool bTemporizador=false;

// ** Menu Inicio ** //

void Control_Mi_escrituraReloj ( void )
{
    DS3231_EnviarTiempo ( 13,33,0 );
    DS3231_EnviarFecha ( 7,17,22 );
    printf ("\nFecha y hora enviada");

    return;
}

// **** //

// ** Menu Reloj ** //

void Control_Mr_lectura ( void )
{
    if ( bLeerReloj )
    {
        printf ("\nLeyendo Reloj");
        DS3231_LeerTiempo ( &strReloj.ucHor, &strReloj.ucMin, &strReloj.ucSeg );
        DS3231_LeerFecha ( &strReloj.ucMes, &strReloj.ucDia, &strReloj.ucAnio );
    }

    return;
}
void Control_Mr_escritura ( void )
{
    if ( bLeerReloj )
    {
        // Nextion_Mr ( strReloj.ucHor, strReloj.ucMin, strReloj.ucDia, strReloj.ucMes, strReloj.ucAnio );
        printf ( "\nHora:%u,\tMin:%u,\tSeg:%u",strReloj.ucHor,strReloj.ucMin,strReloj.ucSeg );
        printf ( "\nDia:%u\tMes:%u,\tAnio:%u",strReloj.ucDia,strReloj.ucMes,strReloj.ucAnio );
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
        // Nextion_Mt ( Adc_lectura() );   // Envia a la pantalla el valor de temperatura
        printf ( "\nTemperatura:%u", Adc_lectura() );
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

    // Cambia la pantalla
    if ( ucEncCont == RPM-1 )
        Nextion_Ms_SMp_Srpm (true);     // Configuracion:Rpm
    else if ( ucEncCont == PWM-1 )
        Nextion_Ms_SMp_Spwm (true);     // Configuracion:Pwm
    else if ( ucEncCont == TIME-1 )
        Nextion_Ms_SMp_Stime (true);    // Configuracion:Tiempo
    else if ( ucEncCont == ATRAS-1 )
        Nextion_Ms_SMx_SAtras ();

    // Selecciona el tipo de configuraccion
    if ( gpio_get ( pinENCODER_PUSH ) )
    {
        *Estado = ucEncCont;            // Cambia el estado
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
        ucEncCont = 0;                  // Reinicia el contador
        Nextion_Ms_SMp_Srpm (false);    // Actualiza la pantalla
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
        return true;
    }
    else    return false;
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

bool Control_Ms_SMp_Aceptar ( void )
{
    if ( gpio_get ( pinENCODER_PUSH ) )
    {
        return true;
    }

    return false;
}

void Control_Salida_Activado ( void )
{
    uint slice_num;
    const uint8_t clk_div=255,fract=15;
    uint16_t wrap;
    float periodo;
    uint16_t pwm_val;

    periodo = 1250000000.0 / ( stDato_Iny.usRpm_val / 60.0 );
    wrap = ( periodo / ( clk_div + fract / 16.0 ) ) - 1;
    pwm_val = (uint16_t) ( ( wrap * stDato_Iny.ucPwm_val ) / 100.0 );

    // Activamos el pwm
    slice_num = pwm_gpio_to_slice_num ( pinINYECTORES );        // Buscamos la parte disponible
    pwm_set_clkdiv_int_frac ( slice_num, clk_div, fract );      // Seteamos el divisor y el fraccional
    pwm_set_wrap ( slice_num, wrap );                           // Seteamos el TOP
    pwm_set_gpio_level ( pinINYECTORES, pwm_val );              // Seteamos el valor del pwm
    pwm_set_enabled ( slice_num, true );                        // Activa la salida

    // Activamos la Bomba 1
    gpio_put ( pinBOMBA1, HIGH );

    // Activamos el temporizador
    bTemporizador = true;

    // Cambiamos de pantalla
    Nextion_Salida_Activado ();

    return;
}
void Control_Salida_Temporizador ( enum Mef_Inicio_est *Estado_Salida )
{

    return;
}
void Control_Salida_Detener ( void )
{

    return;
}
void Control_Salida_Reset ( void )
{

    return;
}

// **** //

void Antirrebote ( void )
{
    gpio_put ( pinBUZZER, HIGH );
    vTaskDelay ( DELAY_60ms );

    while ( gpio_get ( pinENCODER_PUSH ) )
    {
        vTaskDelay ( DELAY_60ms );
    }

    gpio_put ( pinBUZZER, LOW );
    
    return;
}