#include <stdio.h>
#include "pico/stdlib.h"
#include "SenTable.h"
#include "TriTable.h"

int main()
{
    stdio_init_all();
    // gpio_set_dir_out_masked (0xFFF); 
    gpio_init_mask (0xFFF);
    gpio_set_dir_out_masked (0xFFF);
    gpio_put_all ( 0 );
    gpio_put_masked ( 0b1001,0b1001 );

    while (1)
    {
        for (int i=0;i<4096;i++)
        {
            gpio_put_masked ( 0b111111111111,SenTable_4096[i] );
            sleep_us ( 50 );
        }
        // for (int i=0;i<=4096;i++)
        // {
        //     gpio_put_masked ( 0b111111111111,Tritable[i] );
        //     sleep_us ( 50 );
        // }
    }    

    return 0;
}
