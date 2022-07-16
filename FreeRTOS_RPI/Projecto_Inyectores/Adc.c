#include "Adc.h"

uint16_t Adc_lectura ( void )
{
    float fAdcTemp_Voltage;
    uint16_t TempSensor;

    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);
    fAdcTemp_Voltage = ( adc_read() * 3.3) / 4096;
    TempSensor = (uint16_t) ( fAdcTemp_Voltage * 27/0.706 );

    return TempSensor;
}