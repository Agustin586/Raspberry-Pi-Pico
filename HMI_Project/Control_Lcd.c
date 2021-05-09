/*
    LCD,Pantalla de Inicio:
        Linea 1:
            Zuliani,Agustin
        Linea 2:
            Practica HMI
    -------------------------------------------------------------------------
    LCD,Pantalla Principal:
        Linea 1:
            Mensaje enviado desde el hmi al micro (no mas de 16 caracteres) 
        Linea 2:
            Informacion sobre el duty del motor y el valor de la temperatura
*/

/*
    Algoritmo:

    Incio Intro_Lcd:

        Escribir_Lcd("Zuliani,Agustin")
        Escribir_Lcd("Practica HMI")

    Fin Intro_Lcd.

    // Esta funcion ingresa cada 100ms //
    Inicio Main_Lcd:

        Variables:
            String: BufferToSend[20]

        LimpiarDisplay()                    // Limpia el display
        
        BufferToSend <- MsjToLcd
        Escribir_Lcd(BufferToSend)    // Mensaje de prueba

        BufferToSend <- Porcentaje y temperatura
        Escribir_Lcd("20% 30°C")            // Acompañado de su simbolo, Porcentaje --> Barra, Temperatura --> Simbolo de temperatura
        Dibujar_Lcd(Simbolos de porcentaje y temperatura)

    Fin Main_Lcd.

*/

#include "Control_Lcd.h"