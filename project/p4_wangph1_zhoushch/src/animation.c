#include "lcd/lcd.h"
#include <string.h>
#include "utils.h"
#include "gamedrivers.h"
#include "game.h"
#include "animation.h"

/*
This funciton is used to show the game name and some shiny cubes
The beginning, ending and celebrating animation
*/
void Animate(const char* str, int pos_x, int pos_y)
{
    LCD_Clear(BLACK);
    // LCD_ShowString(55,30,str, WHITE);
    // delay_1ms(3000);
    // TODO
    // show game name
    LCD_ShowString(pos_x, pos_y , str, WHITE);
    // Add some shiny cubes
    // generate 1000 cubes
    for (int i = 0; i < 1000; i++)
    {
        // rand x and y
        int x = rand() % 160;
        int y = rand() % 80;
        // rand color
        int color = rand() % 0xffffff;
        LCD_DrawPoint(x, y, color);
        // delay 3ms
        delay_1ms(3);
    }
    // stop
    delay_1ms(1000);
    // clear screen
    LCD_Clear(BLACK);
    delay_1ms(1000);
}