#ifndef MEF_H
#define MEF_H

    // Variables para maquina de estados//
    typedef enum
    {
        Est_Display,
        Est_Pwm1,
        Est_Pwm2,
        Est_Temperature,
        Est_Serial
    }Mef_estate;

    Mef_estate Actual_State;
    // -- //

    void Mef_Updated(void); // Actualiza el valor de las maquinas de estado

#endif