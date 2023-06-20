#include "utils.h"
#include "gd32vf103_libopt.h"
#include <stdio.h>

/* -----------------------------
 Description: Return 1 if button number ch is pressed
 			  Return 0 otherwise
----------------------------- */
int Get_Button(int ch)
{
    /* hack for new board*/
    if (ch != GPIO_PIN_13)
    {
        int input = (int)(gpio_input_bit_get(GPIOA, ch));

        // discrete movements
        if(input != 0)
            delay_1ms(300);

        return input;
    }
    else
    {
        int input = (int)(gpio_input_bit_get(GPIOC, ch));

        // discrete movements
        if(input != 0)
            delay_1ms(300);

        return input;
    }
}

/* -----------------------------
 Description: Return 1 if button BOOT0 ch is pressed
 			  Return 0 otherwise
----------------------------- */
int Get_BOOT0(void)
{
    int input = (int)(gpio_input_bit_get(GPIOA, GPIO_PIN_8));

    // discrete movements
    if(input != 0)
        delay_1ms(300); 

    return input;
}