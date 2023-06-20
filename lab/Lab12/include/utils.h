#ifndef __UTILS_H
#define __UTILS_H

#define BOARD_VER 0 /*fill in your board version here. i.e. 0 or 1*/

#if BOARD_VER < 0 || BOARD_VER > 1
#error INVALID_BOARD_VER
#endif

#if BOARD_VER == 0

#define JOY_LEFT GPIO_PIN_0
#define JOY_DOWN GPIO_PIN_1
#define JOY_RIGHT GPIO_PIN_2
#define JOY_UP GPIO_PIN_5
#define JOY_CTR GPIO_PIN_4

#define BUTTON_1 GPIO_PIN_6
#define BUTTON_2 GPIO_PIN_7

#elif BOARD_VER == 1

#define JOY_LEFT GPIO_PIN_13
#define JOY_DOWN GPIO_PIN_0
#define JOY_RIGHT GPIO_PIN_1
#define JOY_UP GPIO_PIN_5
#define JOY_CTR GPIO_PIN_4

#define BUTTON_1 GPIO_PIN_6
#define BUTTON_2 GPIO_PIN_7

#endif


int Get_Button(int ch);
int Get_BOOT0(void);

#endif