#include "B_Nextion.h"

typedef struct Nextion_pos
{
    uint16_t PosXInit;
    uint16_t PosYInit;
    uint16_t PosXFinal;
    uint16_t PosYFinal;
}Nextion_pos;
Nextion_pos Rectangulo;

// **MENU SELECTOR DE MODO DE TRABAJO** //

void Ms_NextionAct ( void )
{
    if ( ucEncCont == 0 )
    {
        uart_puts ( UART_ID, "ref 0" );
        SentData_Nextion ();
        uart_puts ( UART_ID, "draw 18,129,109,200,RED" );
        SentData_Nextion ();
    }
    else if ( ucEncCont == 1 )
    {
        uart_puts ( UART_ID, "ref 0" );
        SentData_Nextion ();
        uart_puts ( UART_ID, "draw 116,128,207,200,RED" );
        SentData_Nextion ();
    }
    else if ( ucEncCont == 2 )
    {
        uart_puts ( UART_ID, "ref 0" );
        SentData_Nextion ();
        uart_puts ( UART_ID, "draw 212,129,303,200,RED" );
        SentData_Nextion ();
    }

    return;
}

void Ms_SMrAModo ( void )
{
    /* 
    Segun el caso del contador del encoder
    Si: contador=0 o contador=1 --> Solo configura el tiempo
    Si: contador=2 --> Configrua todo, iniciando desde el rpm
    */
    if ( !ucEncCont || ucEncCont )              uart_puts ( UART_ID, "page pMenuTIME" );
    else if ( ucEncCont == PULVERIZACION_ )     uart_puts ( UART_ID, "page pMenuRPM" );
    SentData_Nextion ();

    return;
}

void Nextion_Ms_SMp_Srpm ( bool color )
{
    uart_puts ( UART_ID, "page pMenuRPM");
    SentData_Nextion ();
    uart_puts ( UART_ID, "ref 0" );
    SentData_Nextion ();
    
    Rectangulo.PosXInit  = B_RPM_Xinit;
    Rectangulo.PosYInit  = B_RPM_Yinit;
    Rectangulo.PosXFinal = B_RPM_ANCHO + Rectangulo.PosXInit;
    Rectangulo.PosYFinal = B_RPM_ALTO  + Rectangulo.PosYInit;
    if ( color )        printf ( "draw %d,%d,%d,%d,BLUE",Rectangulo.PosXInit,Rectangulo.PosYInit,Rectangulo.PosXFinal,Rectangulo.PosYFinal);
    else if ( !color )  printf ( "draw %d,%d,%d,%d,RED",Rectangulo.PosXInit,Rectangulo.PosYInit,Rectangulo.PosXFinal,Rectangulo.PosYFinal);
    SentData_Nextion ();
    // uart_puts ( UART_ID, "draw 212,129,303,200,RED" );

    return;
}
void Nextion_Ms_SMp_Spwm ( bool color )
{
    uart_puts ( UART_ID, "page pMenuPWM");
    SentData_Nextion ();
    uart_puts ( UART_ID, "ref 0" );
    SentData_Nextion ();
    
    Rectangulo.PosXInit  = B_PWM_Xinit;
    Rectangulo.PosYInit  = B_PWM_Yinit;
    Rectangulo.PosXFinal = B_PWM_ANCHO + Rectangulo.PosXInit;
    Rectangulo.PosYFinal = B_PWM_ALTO  + Rectangulo.PosYInit;
    if ( color )        printf ( "draw %d,%d,%d,%d,BLUE",Rectangulo.PosXInit,Rectangulo.PosYInit,Rectangulo.PosXFinal,Rectangulo.PosYFinal);
    else if ( !color )  printf ( "draw %d,%d,%d,%d,RED",Rectangulo.PosXInit,Rectangulo.PosYInit,Rectangulo.PosXFinal,Rectangulo.PosYFinal);
    SentData_Nextion ();

    return;
}
void Nextion_Ms_SMp_Stime ( bool color )
{
    uart_puts ( UART_ID, "page pMenuTIME");
    SentData_Nextion ();
    uart_puts ( UART_ID, "ref 0" );
    SentData_Nextion ();
    
    Rectangulo.PosXInit  = B_TIME_Xinit;
    Rectangulo.PosYInit  = B_TIME_Yinit;
    Rectangulo.PosXFinal = B_TIME_ANCHO + Rectangulo.PosXInit;
    Rectangulo.PosYFinal = B_TIME_ALTO  + Rectangulo.PosYInit;
    if ( color )        printf ( "draw %d,%d,%d,%d,BLUE",Rectangulo.PosXInit,Rectangulo.PosYInit,Rectangulo.PosXFinal,Rectangulo.PosYFinal);
    else if ( !color )  printf ( "draw %d,%d,%d,%d,RED",Rectangulo.PosXInit,Rectangulo.PosYInit,Rectangulo.PosXFinal,Rectangulo.PosYFinal);
    SentData_Nextion ();

    return;
}
void Nextion_Ms_SMp_RPMva ( uint16_t val )
{
    printf ( "nRPM.val=%u",val );
    SentData_Nextion ();

    return;
}
void Nextion_Ms_SMp_PWMva ( uint8_t val )
{
    printf ( "nPWM.val=%d",val );
    SentData_Nextion();

    return;
}
void Nextion_Ms_SMx_TIMEh ( uint8_t hora )
{
    printf ( "nHora.val=%u",hora );
    SentData_Nextion ();
}
void Nextion_Ms_SMx_TIMEm ( uint8_t min )
{
    printf ( "nMin.val=%u",min );
    SentData_Nextion ();
}
void Nextion_Ms_SMx_TIMEs ( uint8_t seg )
{
    printf ( "nSeg.val=%u",seg );
    SentData_Nextion ();
}
void Nextion_Ms_SMx_SAtras ( void )
{
    uart_puts ( UART_ID, "page pMenuInicio");
    SentData_Nextion ();
}
// **** //

void SentData_Nextion ( void )
{
    printf ( "\xFF\xFF\xFF" );
    vTaskDelay ( DELAY_30ms );
}