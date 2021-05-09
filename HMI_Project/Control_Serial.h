#ifndef CONTROL_SERIAL
#define CONTROL_SERIAL

// Funciones para escribir en el serial //
void MicroToHmi(void);      // Envia datos necesarios desde el microcontrolador al hmi
void HmiToMicro(void);      // Recive datos necesario desde el hmi al microcontrolador

#endif