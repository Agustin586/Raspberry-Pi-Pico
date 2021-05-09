/*
    Comunicacion Microcontrolador con el Hmi:
    *Cabe aclarar que solo habra una sola comunicacion entre estos dos dispositivos
    por lo tanto no existirá un 3er dispositivos conectado a este bus de datos.

    ===================================================================================
    Maestro: Microcontrolador Raspberry Pi Pico
    Esclavo: Hmi

    Para que el esclavo pueda responder al mensaje, primero el maestro
    debe dirijirse a el con el mensaje de inicio de recepcion.

    Mensaje de inicio de recepcion: [Comando]\n

    Comandos:
        EVD: con este comando el maestro indica al esclavo que el siguiente mensaje
             serán datos que tendrá que procesar el esclavo.

        RPD: con este comando el maestro indica al esclavo que el siguiente mensaje 
             recivirá datos desde el esclavo para que el maestro los procese.

        MFL: con este comando el maestro indica al esclavo que envie el mensaje
             que tenga, si es que tiene, para imprimirse por el lcd. 
    ===================================================================================
    
    ===================================================================================
    Mensaje desde el Hmi al micro: [AC,XX,YYYY]\n

    Valor AC(tipo de accion): 0-9
    Valor XX: 0-99
    Valor YYYY: 0-9999

    -----------------------------------------------------------------------------------

    Mensaje desde el Hmi al micro para la pantalla lcd: [0123456789012345]\n

    -----------------------------------------------------------------------------------

    Mensaje desde el micro al Hmi: [AC,XX,YY]\n

    Valor AC(tipo de accion): 0-9
    Valor XX: 0-99
    Valor YY: 0-99
    ===================================================================================
*/

/*
    Algoritmo:

    Variables:
        Entero sin signo: Temperatura
        String: CmdToSend:"[EVD]\n",CmdToRecep:"[RPD]\n",CmdToRecepMsj:"[MLF]\n",Buffer_HmiToMicro[25]
        Caracter volatile: Rx
        Entero sin signo volatile: Index_HTM
        Logico volatile: End_M
    
    Inicio MicroToHmi_Update:

        Send_DateToHMi(Temperatura)

    Fin MicroToHmi.

    Inicio HmiToMicro:

        Segun Sea(Estado_Recepcion):

            caso Datos:     Get_DateToHmi(),    Estado_Recepcion <- Mensaje
            caso Mensaje:   Get_MsjToHmi(),     Estado_Recepcion <- Datos

        Fin Segun Sea.

    Fin HmiToMicro.

    Inicio Send_DateToHmi:

        Variables:
            String: BufferToSend

        // Envio de datos //
        Escribir_Serial(CmdToSend)      // Inicio de la comunicacion
        BufferToSend <- Cargar datos    // Carga de datos en el buffer
        Escribir_Serial(BufferToSend)   // Envio de datos del buffer

        // Asi sucesivamente se realiza
        // para enviar datos el esclavo // 
  
    Fin Send_DateToHmi

    Inicio Get_DateToHmi:

        Variables:
            Entero: AC_Int,XX_Int,YYYY_Int
            String: AC_Strg,XX_Strg,YYYY_Strg

        Escribir_Serial(CmdToRecep)
        Si (End_M)
            entonces    Si (Buffer_HmiToMicro[0]=='[' AND Buffer_HmiToMicro[Indice_HTM-1] == ']')
                            entonces    AC_Strg   <- Buffer_HmitToMicro[1,2]
                                        AC_Int    <- atoi(AC_Strg)
                                        XX_Strg   <- Buffer_HmiToMicro[4,5]
                                        XX_Int    <- atoi(XX_Strg)
                                        YYYY_Strg <- Buffer_HmiToMicro[7,8,9,10]
                                        YYYY_Int  <- atoi(YYYY_Strg)
                                        Procesar_DatosRecep(&AC_Int,&XX_Int,&YYYY_Int)
                                        Clear_BufferHmiToMicro()
                            Sino        Clear_BufferHmiToMicro()
                        Fin Si
            End_M <-false
        Fin Si

    Fin Get_DateToHmi.

    Inicio Procesar_DatosRecep:

        Segun Sea (*AC_Int):

            // Procesa datos para el pwm //
            caso 0:
                Si (*XX_Int == 1)
                    entonces Pwm1_Value <- *YYYY_Int
                    Sino     Pwm2_Value <- *YYYY_Int
                Fin Si.
            caso 1:
            caso 2:
            caso 3:
            caso 4:
            caso 5:
            caso 6:
            caso 7:
            caso 8:
            caso 9:

        Fin Segun Sea.

    Fin Procesar_DatosRecep.

    Inicio Get_MsjToHmi:

        Escribir_Serial(CmdToRecepMsj)
        Si (End_M)
            
            entonces    Si (Buffer_HmiToMicro[0]=='[' AND Buffer_HmiToMicro[Indice_HTM-1] == ']')
            
                            entonces    MsjToLcd <- Copiar cadena desde [1 al 17]
                                        Clear_BufferHmiToMicro()
                            Sino        Clear_BufferHmiToMicro()
            
                        Fin Si.
            End_M <- false

        Fin Si

    Fin Get_MsjToHmi.
*/

#include "Control_Serial.h"

void MicroToHmi(void)
{


    return;
}

void HmiToMicro(void)
{


    return;
}
