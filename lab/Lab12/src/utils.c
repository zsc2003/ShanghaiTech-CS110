#include "utils.h"
#include "gd32vf103_libopt.h"

/* -----------------------------
 Description: Return 1 if button number ch is pressed
 			  Return 0 otherwise
----------------------------- */
int Get_Button(int ch)
{
    /* hack for new board*/
    if (ch != GPIO_PIN_13)
        return (int)(gpio_input_bit_get(GPIOA, ch));
    else
        return (int)(gpio_input_bit_get(GPIOC, ch));
}

/* -----------------------------
 Description: Return 1 if button BOOT0 ch is pressed
 			  Return 0 otherwise
----------------------------- */
int Get_BOOT0(void)
{
    return (int)(gpio_input_bit_get(GPIOA, GPIO_PIN_8));
}