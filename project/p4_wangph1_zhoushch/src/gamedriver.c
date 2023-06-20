#include "lcd/lcd.h"
#include <string.h>
#include "utils.h"
#include "gamedrivers.h"
#include "game.h"
#include "animation.h"

void testing_code()
{
    // testing code
    while (1)
    {
        LCD_Clear(BLACK);
        LCD_ShowString(60,25,"TEST",WHITE);
        if (Get_Button(JOY_LEFT))
        {
            LCD_ShowString(5,25,"L", BLUE);
            //continue;
        }
        if (Get_Button(JOY_DOWN))
        {
            LCD_ShowString(25,45,"D", BLUE);
        }
        // update
        LCD_ShowString(5,5,"U:INOP",RED);
        if (Get_Button(JOY_RIGHT))
        {
            LCD_ShowString(45,25,"R", BLUE);
        }
        // if ctr
        if (Get_Button(JOY_CTR))
        {
            LCD_ShowString(25,25,"C", BLUE);
        }
        // if button1
        if (Get_Button(BUTTON_1))
        {
            LCD_ShowString(60,5,"SW1", BLUE);
        }
        // test
        LCD_ShowString(60,45,"SW2:INOP",RED);
    }
}

void test_clear_area(u16 xsta,u16 ysta)
{
    // below: testing which area to clear
    u16 xend = xsta, yend = ysta;
    while(1)
    {
        // test show
        LCD_ShowNum(0,0,xend,4,WHITE);
        LCD_ShowNum(0,15,yend,4,WHITE);

        // LCD_Fill(xsta,ysta,xend,yend,color)
        if (Get_Button(JOY_DOWN))
        {
            yend ++;
            LCD_Fill(xsta,ysta,xend,yend,BLACK);
        }
        // if get right
        if (Get_Button(JOY_RIGHT))
        {
            xend ++;
            LCD_Fill(xsta,ysta,xend,yend,BLACK);
        }
    }
}


// draw the player with the given position
void draw_player(int x, int y) {
    // loop the 2d array
    for (int i = 0; i < PLAYER_SIZE_X; i++) {
        // loop the 2d array
        for (int j = 0; j < PLAYER_SIZE_Y; j++) {
            // draw the player
            LCD_DrawPoint(x + i + ((10 - PLAYER_SIZE_X) / 2) - 1, y + j + ((10 - PLAYER_SIZE_Y) / 2) - 1, user_image[i][j]);
        }
    }
}

/*
This function is to pause the game while playing
*/
void game_pause(LevelParameters* param, int* pause_status, int (*score_board)[3])
{
    // if get buttion
    if(Get_Button(BUTTON_1))
    {
        // clear
        LCD_Clear(BLACK);
        delay_1ms(300);
        // show the pause
        LCD_ShowString(0,0,"Game Pause, Press:", WHITE);
        LCD_ShowString(0,20,"SW1 to continue", WHITE);
        LCD_ShowString(0,40,"SW2 to restart", WHITE);
        LCD_ShowString(0,60,"BOOT0 to quit", WHITE);
        // stop
        delay_1ms(100);

        while(1)
        {
            if(Get_Button(BUTTON_1)) //SW1
            {
                // continue the game
                *pause_status = 2;
                return;
            }
            if(Get_Button(BUTTON_2)) // SW2
            {
                // restart this level
                *pause_status = 3;
                return;
            }
            if(Get_BOOT0())
            {
                // quit the level(go to level selection part)
                *pause_status = 4;
                return;
            }
            // end
        }
    }
}

/*
This function is to start the whole game
*/
void game_starter()
{
    IO_init();
    LCD_Clear(BLACK);
    delay_1ms(300);

    // for(int t = 1; t <= 3; ++t)
    // {
    //     // LCD_ShowNum(0,0,t,1,WHITE);

    //     // LCD_Fill(xsta,ysta,xend,yend,color)
    //     LCD_Fill(120,40,145,45,BLACK); // partial clear
    //     LCD_ShowString(20,30,"Game Starting", WHITE);
    //     delay_1ms(300);
    //     LCD_ShowString(20,30,"Game Starting.", WHITE);
    //     delay_1ms(300);
    //     LCD_ShowString(20,30,"Game Starting..", WHITE);
    //     delay_1ms(300);
    //     LCD_ShowString(20,30,"Game Starting...", WHITE);
    //     delay_1ms(300);

    //     // Capital letter requires 7  pixels to clear for x direction
    //     // Capital letter requires 13 pixels to clear for y direction
    // }

    // test_clear_area(20,30);

    // TODO: add some opening animation
    // const char* opening = "Hello";
    const char* GameName = "Sokoban Nano";
    Animate(GameName, 35, 30);
}

/*
This function is used to generate the parameters of the current level
*/
LevelParameters* paras_generator(int x_size, int y_size)
{
    LevelParameters* param = malloc(sizeof(LevelParameters));
    param->x_size = x_size;
    param->y_size = y_size;
    // init level
    param->level = 1;
    param->stones = 0;
    param->boxes = 1;
    param->steps = 0;
    // init the player
    param->player_x = 0;
    param->player_y = 0;
    // calloc the board
    param->board = calloc(x_size, sizeof(int*));
    for(int i = 0; i < x_size; ++i)
        param->board[i] = calloc(y_size, sizeof(int));
    // calloc the origin
    param->origin_board = calloc(x_size, sizeof(int*));
    for(int i = 0; i < x_size; ++i)
        param->origin_board[i] = calloc(y_size, sizeof(int));
    // calloc the target
    param->target = calloc(x_size, sizeof(int*));
    for(int i = 0; i < x_size; ++i)
        param->target[i] = calloc(y_size, sizeof(int));
    // return
    return param;
}

/*
This function is used to select the level
*/
void level_select(LevelParameters* param)
{
    // select level
    LCD_Clear(BLACK);
    // select
    LCD_ShowString(10,0,"Please select the level of game", WHITE);
    LCD_ShowString(0,45,"Please press SW1 to confirm", WHITE);
    // stop
    delay_1ms(300);
    LCD_ShowString(0,45,"Please press SW1 to confirm.", WHITE);
    delay_1ms(300);
    // sw1
    LCD_ShowString(0,45,"Please press SW1 to confirm..", WHITE);
    delay_1ms(300);
    // loading
    LCD_ShowString(0,45,"Please press SW1 to confirm...", WHITE);
    delay_1ms(2000);
    // clear
    LCD_Clear(BLACK);
    level_shown(param);
}

/*
This function is used to show the level we chosen on the screen
*/
void level_shown(LevelParameters* param)
{
    // show level
    LCD_ShowString(20,30,"Level:", WHITE);
    LCD_ShowNum(70,30,param->level,2,WHITE);
}

/*
This function is used to select the number of boxes
*/
void box_select(LevelParameters* param)
{
    // select boxes
    LCD_Clear(BLACK);
    LCD_ShowString(10,0,"Please select the number of boxes", WHITE);
    LCD_ShowString(0,45,"Please press SW1 to confirm", WHITE);
    delay_1ms(300);
    // press sw1
    LCD_ShowString(0,45,"Please press SW1 to confirm.", WHITE);
    delay_1ms(300);
    // press sw1
    LCD_ShowString(0,45,"Please press SW1 to confirm..", WHITE);
    delay_1ms(300);
    // press sw1
    LCD_ShowString(0,45,"Please press SW1 to confirm...", WHITE);
    delay_1ms(2000);
    LCD_Clear(BLACK);
    box_shown(param);
}

/*
This function is used to show the number of boxes on the screen
*/
void box_shown(LevelParameters* param)
{
    // show boxes
    LCD_ShowString(20,30,"Boxes:", WHITE);
    LCD_ShowNum(70,30,param->boxes,2,WHITE);
}

/*
This function is used to select the number of stones
*/
void stone_select(LevelParameters* param)
{
    // select stones
    LCD_Clear(BLACK);
    LCD_ShowString(10,0,"Please select the number of stones", WHITE);
    LCD_ShowString(0,45,"Please press SW1 to confirm", WHITE);
    delay_1ms(300);
    // press sw1
    LCD_ShowString(0,45,"Please press SW1 to confirm.", WHITE);
    delay_1ms(300);
    // press sw1
    LCD_ShowString(0,45,"Please press SW1 to confirm..", WHITE);
    delay_1ms(300);
    // press sw1
    LCD_ShowString(0,45,"Please press SW1 to confirm...", WHITE);
    delay_1ms(2000);
    // clear
    LCD_Clear(BLACK);
    stone_shown(param);
}

/*
This function is used to show the number of stones on the screen
*/
void stone_shown(LevelParameters* param)
{
    // show stones
    LCD_ShowString(20,30,"Stones:", WHITE);
    LCD_ShowNum(70,30,param->stones,2,WHITE);
}

/*
update:
This is to meet the requirement of the project
Modified to make it looks like what the demo like.
the function ```original``` is the previous version
the function ```modified``` is the modified version
*/
int original(LevelParameters* param, int (*score_board)[3])
{
    // select the level
    level_select(param);
    while(1)
    {
        if(Get_Button(JOY_LEFT)) // A
        {
            // if level > 1
            if(param->level > 1)
            {
                param->level--;
                param->boxes = param->level;
                // LCD_Clear(BLACK);
                LCD_Fill(70,30,65,45,BLACK); // partial clear
                // LCD_Clear(BLACK);
                level_shown(param);
            }
        }
        if(Get_Button(JOY_RIGHT)) // D
        {
            // if < 4
            if(param->level < 4)
            {
                param->level++;
                param->boxes = param->level;
                LCD_Fill(70,30,65,45,BLACK); // partial clear
                level_shown(param);
            }
        }
        // sw 1
        if(Get_Button(BUTTON_1)) // SW1
            break;
        if(Get_BOOT0()) // BOOT0
        {
            // quit
            LCD_Clear(BLACK);
            return 0;
        }
    }
    // if level 4
    if (param->level == 4) {
        return 1;
    }

    // set boxes
    LCD_Clear(BLACK);
    box_select(param);
    while(1)
    {
        // number of boxes: [level, level + 3]
        if(Get_Button(JOY_LEFT)) // A
        {
            if(param->boxes > param->level)
            {
                // box--
                param->boxes--;
                LCD_Fill(70,30,65,45,BLACK); // partial clear
                box_shown(param);
            }
        }
        if(Get_Button(JOY_RIGHT)) // D
        {
            if(param->boxes < param->level + 3)
            {
                param->boxes++;
                LCD_Fill(70,30,65,45,BLACK); // partial clear
                box_shown(param);
            }
        }
        if(Get_BOOT0()) // BOOT0
            break;
        // if get sw1
        if(Get_Button(BUTTON_1)) // SW1
        {
            // quit
            LCD_Clear(BLACK);
            return 0;
        }
    }

    // set stones
    LCD_Clear(BLACK);
    stone_select(param);
    while(1)
    {
        // number of stones: [0, level]
        if(Get_Button(JOY_LEFT)) // A
        {
            if(param->stones > 0)
            {
                param->stones--;
                LCD_Fill(70,30,65,45,BLACK); // partial clear
                stone_shown(param);
            }
        }
        if(Get_Button(JOY_RIGHT)) // D
        {
            if(param->stones < param->level)
            {
                param->stones++;
                LCD_Fill(70,30,65,45,BLACK); // partial clear
                stone_shown(param);
            }
        }
        if(Get_Button(BUTTON_1)) // SW1
            break;
        if(Get_BOOT0()) // BOOT0
        {
            // quit
            LCD_Clear(BLACK);
            return 0;
        }
    }
    // return 
    return 1;
}

int modified(LevelParameters* param, int (*score_board)[3])
{
    // clear
    LCD_Clear(BLACK);
    LCD_ShowString(0,20,"Level  (D- U+):", WHITE);
    LCD_ShowNum(120,20,param->level,2,WHITE);
    LCD_ShowString(0,40,"Stones (L- R+):", WHITE);
    LCD_ShowNum(120,40,param->stones,2,WHITE);
    // while loop
    while(1)
    {
        // number of level: [1, 4]
        if(Get_Button(JOY_DOWN)) // D
        {
            // level > 1
            if(param->level > 1)
            {
                // level --
                param->level--;
                param->boxes = param->level;
                // if stone > 
                if(param->stones > param->level)
                {
                    // reinit the stone
                    param->stones = param->level;
                    LCD_ShowNum(120,40,param->stones,2,WHITE);
                }
                // show the level
                LCD_ShowNum(120,20,param->level,2,WHITE);
            }
        }
        // if button ctr
        if(Get_Button(JOY_CTR)) // U
        {
            // if level < 4
            if(param->level < 4)
            {
                // ++ level
                param->level++;
                param->boxes = param->level;
                LCD_ShowNum(120,20,param->level,2,WHITE);
            }
        }
        // number of stones: [0, level]
        if(Get_Button(JOY_LEFT)) // L
        {
            if(param->stones > 0)
            {
                // -- stone
                param->stones--;
                LCD_ShowNum(120,40,param->stones,2,WHITE);
            }
        }
        // if r
        if(Get_Button(JOY_RIGHT)) // R
        {
            if(param->stones < param->level + 3)
            {
                // ++ stone
                param->stones++;
                LCD_ShowNum(120,40,param->stones,2,WHITE);
            }
        }
        if(Get_Button(BUTTON_1)) // SW1
            break;
        if(Get_BOOT0()) // BOOT0
        {
            // quit
            LCD_Clear(BLACK);
            return 0;
        }
    }
    // reinit the box
    param->boxes = param->level;
    return 1;
}

/*
This function is used to set the parameters of the current level
*/
int parameters_setting(LevelParameters* param, int (*score_board)[3])
{
    // test_clear_area(50,30);

    // set level
    LCD_Clear(BLACK);

    // return original(param, score_board);
    return modified(param, score_board);
}

/*
This function is used to generate the imformations of the current level
*/
void level_generate(LevelParameters* param, int (*score_board)[3])
{
    LCD_Clear(BLACK);
    delay_1ms(300);
    for(int t = 1; t <= 3; ++t)
    {
        // level animation
        LCD_Fill(125,40,150,45,BLACK); // partial clear
        LCD_ShowString(0,30,"Level Generating", WHITE);
        delay_1ms(300);
        // first stage
        LCD_ShowString(0,30,"Level Generating.", WHITE);
        delay_1ms(300);
        // second stage
        LCD_ShowString(0,30,"Level Generating..", WHITE);
        delay_1ms(300);
        // third stage
        LCD_ShowString(0,30,"Level Generating...", WHITE);
        delay_1ms(300);
    }
    // test_clear_area(120,40);
    LCD_Clear(BLACK);
    score_board = score_board;
    board_generate(param);
}

/*
This function is used to start the current level
*/
void level_starter(LevelParameters* param, int (*score_board)[3])
{
    score_board = score_board;

    int pause_status = 1;
    // 1 continue
    // 2 restart
    // 3 quit

    int status = 1;
    // 1 : playing
    // 2 : win

    print_board(param);

    while(1)
    {
        // pause
        game_pause(param, &pause_status, score_board);
        // if status 2
        if(pause_status == 2)
        {
            // if status 1
            pause_status = 1;
            print_board(param);
        }
        if(pause_status == 3) // restart
        {
            // for the array
            for(int i = 0; i < param->x_size; ++i)
                for(int j = 0;j < param->y_size; ++j)
                    param->board[i][j] = param->origin_board[i][j];
            // update the player
            param->player_x = param->player_origin_x;
            param->player_y = param->player_origin_y;
            param->steps = 0;
            // print the board
            print_board(param);
            // re value the pause
            pause_status = 1;
        }
        if(pause_status == 4) // quit the level
        {
            break;
        }

        player_move(param);
        // print_board(param);
        check(param, &status);

        if(status == 2) // win
        {   
            // s1
            int s1 = score_board[param->level - 1][0];
            int s2 = score_board[param->level - 1][1];
            int s3 = score_board[param->level - 1][2];
            // get list
            int list[4] = {s1, s2, s3, param->steps};
            int tmp;
            // for loop bubble
            for(int i = 0; i < 4; ++i)
                for(int j = i + 1; j < 4; ++j)
                // if > 
                    if(list[i] > list[j])
                    {
                        tmp = list[i];
                        list[i] = list[j];
                        list[j] = tmp;
                    }
            // get score board
            score_board[param->level - 1][0] = list[0];
            score_board[param->level - 1][1] = list[1];
            score_board[param->level - 1][2] = list[2];
            // generate cong
            const char* celebrate = "Congratulations";
            Animate(celebrate, 18, 30);
            // break
            break;
        }
    }
}

/*
This function is used to show the steps we have taken for each level
*/
void show_score(int score, int x, int y)
{
    if(score == 19260817)
        LCD_ShowString(x, y, "---", WHITE);
    else
        LCD_ShowNum(x, y, score, 3, WHITE);
}

/*
This function is used to close the current level
*/
int level_closer(LevelParameters* param, int (*score_board)[3])
{
    // free
    for(int i = 0; i < param->y_size; ++i)
        free(param->board[i]);
    free(param->board);
    free(param);
    // clear
    LCD_Clear(BLACK);
    // show scoreboard
    LCD_ShowString(0,0,"Level   1    2    3", WHITE);
    LCD_ShowString(0,40,"Steps", WHITE);
    // show the first line
    show_score(score_board[0][0], 45, 20);
    show_score(score_board[0][1], 45, 40);
    show_score(score_board[0][2], 45, 60);
    // show the second line
    show_score(score_board[1][0], 80, 20);
    show_score(score_board[1][1], 80, 40);
    show_score(score_board[1][2], 80, 60);
    // show the third line
    show_score(score_board[2][0], 125, 20);
    show_score(score_board[2][1], 125, 40);
    show_score(score_board[2][2], 125, 60);
    // while
    while(1)
    {
        // sw1
        if(Get_Button(BUTTON_1)) // SW1
        {
            return 0;
        }
        // boot0
        if(Get_BOOT0()) // BOOT0
        {
            return 1;
        }
    }
}

/*
This function is used to start the whole game
*/
void game_closer()
{
    LCD_Clear(BLACK);
    delay_1ms(300);
    // closer
    for(int t = 1; t <= 3; ++t)
    {
        LCD_Fill(120,40,145,45,BLACK); // partial clear
        LCD_ShowString(20,30,"Game Closing", WHITE);
        delay_1ms(300);
        // first stage
        LCD_ShowString(20,30,"Game Closing.", WHITE);
        delay_1ms(300);
        // first stage
        LCD_ShowString(20,30,"Game Closing..", WHITE);
        delay_1ms(300);
        // first stage
        LCD_ShowString(20,30,"Game Closing...", WHITE);
        delay_1ms(300);
    }

    // TODO: add some opening animation
    // Bye Thanks for playing
    const char* closing = "Bye";
    Animate(closing, 65, 30);
}