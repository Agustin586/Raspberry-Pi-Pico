#include "RTC_DS3231.h"
#include "stdint.h"
#include "Includes.h"

void DS3231_EnviarTiempo ( uint8_t hora, uint8_t min, uint8_t seg )
{
    uint8_t Decena,Unidad,*Dato;

    i2c_write_blocking ( I2C_ID, 0xD0, 0x00, 1, true );
    Decena = (uint8_t)( seg / 10 );
    Unidad = seg - ( Decena*10 );
    *Dato = ( Decena<<4 ) | Unidad;
    i2c_write_blocking ( I2C_ID, 0xD0, Dato, 1, true );
    Decena = (uint8_t)( min/10 );
    Unidad = min - ( Decena*10 );
    *Dato = ( Decena<<4 ) | Unidad;
    i2c_write_blocking ( I2C_ID, 0xD0, Dato, 1, true );
    Decena = (uint8_t)( hora/10 );
    Unidad = hora - ( Decena*10 );
    *Dato = ( Decena<<4 ) | Unidad;
    i2c_write_blocking ( I2C_ID, 0xD0, Dato, 1, false );

    return;
}
void DS3231_EnviarFecha ( uint8_t mes, uint8_t dia, uint8_t anio )
{
    uint8_t Decena,Unidad,*Dato;

    *Dato = 0x04;
    i2c_write_blocking ( I2C_ID, 0xD0, Dato, 1, true );
    Decena = (uint8_t)( dia / 10 );
    Unidad = dia - ( Decena*10 );
    *Dato = ( Decena<<4 ) | Unidad;
    i2c_write_blocking ( I2C_ID, 0xD0, Dato, 1, true );
    Decena = (uint8_t)( mes/10 );
    Unidad = mes - ( Decena*10 );
    *Dato = ( Decena<<4 ) | Unidad;
    i2c_write_blocking ( I2C_ID, 0xD0, Dato, 1, true );
    Decena = (uint8_t)( anio/10 );
    Unidad = anio - ( Decena*10 );
    *Dato = ( Decena<<4 ) | Unidad;
    i2c_write_blocking ( I2C_ID, 0xD0, Dato, 1, false );

    return;
}
void DS3231_LeerTiempo ( uint8_t *hora, uint8_t *min, uint8_t *seg )
{
    uint8_t aux1,aux2;

    i2c_write_blocking ( I2C_ID, 0xD0, 0x00, 1, false );
    i2c_read_blocking ( I2C_ID, 0xD1, seg,  1, true );
    i2c_read_blocking ( I2C_ID, 0xD1, min,  1, true );
    i2c_read_blocking ( I2C_ID, 0xD1, hora, 1, false );

    aux1 = ( *seg>>4 ) * 10;
    aux2 = *seg & 0x0F;
    *seg = aux1 + aux2;
    
    aux1 = ( *min>>4 ) * 10;
    aux2 = *min & 0x0F;
    *min = aux1 + aux2;
    
    aux1 = ( *hora>>4 ) * 10;
    aux2 = *hora & 0x0F;
    *hora = aux1 + aux2;

    return;
}
void DS3231_LeerFecha ( uint8_t *mes, uint8_t *dia, uint8_t *anio )
{
    uint8_t aux1,aux2,*Dato;

    *Dato = 0x04;
    i2c_write_blocking ( I2C_ID, 0xD0, Dato, 1, false );
    i2c_read_blocking ( I2C_ID, 0xD1, dia,  1, true );
    i2c_read_blocking ( I2C_ID, 0xD1, mes,  1, true );
    i2c_read_blocking ( I2C_ID, 0xD1, anio, 1, false );

    aux1 = ( *dia>>4 ) * 10;
    aux2 = *dia & 0x0F;
    *dia = aux1 + aux2;
    
    aux1 = ( *mes>>4 ) * 10;
    aux2 = *mes & 0x0F;
    *mes = aux1 + aux2;
    
    aux1 = ( *anio>>4 ) * 10;
    aux2 = *anio & 0x0F;
    *anio = aux1 + aux2;

    return;
}
