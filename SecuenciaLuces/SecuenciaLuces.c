#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"

#define HIGH    1
#define LOW     0

int main()
{
    uint8_t GpioSelect[]={18,19,20,21,22}; 

    stdio_init_all();

    // Inicializa los pines //
    gpio_init(21);
    gpio_init(22);
    gpio_init(18);
    gpio_init(19);
    gpio_init(20);
    gpio_init(25);
    // Setea la direccion //
    gpio_set_dir(21,true);
    gpio_set_dir(22,true);
    gpio_set_dir(18,true);
    gpio_set_dir(19,true);
    gpio_set_dir(20,true);
    gpio_set_dir(25,GPIO_OUT);

    gpio_put(25,HIGH);
    sleep_ms(500);
    gpio_put(25,LOW);

    while (1)
    {
        for(uint8_t k=0;k<10;k++)
        {
            for(uint8_t i=0;i<5;i++)
            {
                for(uint8_t j=0;j<5;j++)
                {
                    gpio_put(GpioSelect[j],LOW);
                }
                gpio_put(GpioSelect[i],HIGH);
                sleep_ms(200);
            }
        }
        sleep_ms(1000);
        for(uint8_t i=0;i<5;i++)
        {
            for(uint8_t j=0;j<5;j++)
            {
                gpio_put(GpioSelect[j],HIGH);
            }
            sleep_ms(500);
            for(uint8_t j=0;j<5;j++)
            {
                gpio_put(GpioSelect[j],LOW);
            }
            sleep_ms(500); 
        }
    }

    return 0;
}
