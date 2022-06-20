#ifndef RTC_DS3231_H
#define RTC_DS3231_H

#include "stdint.h"

// **Funciones** //
// --Enviar datos
void DS3231_EnviarTiempo ( uint8_t hora, uint8_t min, uint8_t seg );
void DS3231_EnviarFecha ( uint8_t mes, uint8_t dia, uint8_t anio );
// --Recibir datos
void DS3231_LeerTiempo ( uint8_t *hora, uint8_t *min, uint8_t *seg );
void DS3231_LeerFecha ( uint8_t *mes, uint8_t *dia, uint8_t *anio );

#endif /* RTC_DS3231_H */