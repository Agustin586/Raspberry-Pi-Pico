#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include <stdint.h>

// I2C defines
// This example will use I2C0 on GPIO14 (SDA) and GPIO15 (SCL) running at 100KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 19
#define I2C_SCL 20
#define GPIO_RED    19
#define GPIO_BLUE   16
#define GPIO_GREEN  17

void Init(void);

int main()
{
    uint16_t  pwm_duty=0;

    stdio_init_all();

    // I2C Initialisation. Using it at 100Khz.
    //i2c_init(I2C_PORT, 100*1000);
    
    // gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    // gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    // gpio_pull_up(I2C_SDA);
    // gpio_pull_up(I2C_SCL);
    gpio_init(GPIO_RED);
    gpio_set_dir(GPIO_RED,GPIO_OUT);
    gpio_put(GPIO_RED,1);
    sleep_ms(1000);

    Init();
    
    while(1)
    {
        if (pwm_duty <= 65535)  pwm_duty++;
        else pwm_duty = 0;
        
        pwm_set_gpio_level(GPIO_RED, 65535-(pwm_duty*pwm_duty));
        pwm_set_gpio_level(GPIO_GREEN, 65535/pwm_duty);
        pwm_set_gpio_level(GPIO_BLUE, pwm_duty*pwm_duty);
    }

    return 0;
}

void Init(void)
{
    int slice_num;
    // Config pwm //
    pwm_config config = pwm_get_default_config();
    // Setup color in led //
    gpio_set_function(GPIO_RED, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(GPIO_RED);
    pwm_init(slice_num, &config, true);
    
    gpio_set_function(GPIO_GREEN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(GPIO_GREEN);
    pwm_init(slice_num, &config, false);

    gpio_set_function(GPIO_BLUE, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(GPIO_BLUE);
    pwm_init(slice_num, &config, true);

    return;
}
