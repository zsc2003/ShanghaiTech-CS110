#include "lcd/lcd.h"
#include <string.h>
#include "utils.h"
#include "gamedrivers.h"
#include "game.h"

// __####___
// ###--####
// #-----$-#
// #-#--#$-#
// #-.-.#@-#
// #########
const char* level_2_map[] = {
    // map
    "__####___",
    "###--####",
    "#-----$-#",
    "#-#--#$-#",
    // map
    "#-.-.#@-#",
    "#########",
};

/*
This function is used to update the steps
shown on the left up corner of the screen
*/
void update_step(LevelParameters *param)
{
    // fill the origin step place to black
    LCD_Fill(0, 0, 20, 20, BLACK);
    // update the step
    LCD_ShowNum(0, 0, param->steps, 2, WHITE);
}

/*
This function is used to control the player's movements
*/
void player_move(LevelParameters* param)
{
    // 1. get the direction of moving
    // 2. check whether it can be moved
    // 3. move and modify the param->board
    // 4. partly update the scene

    // Attention!!!
    // The Up botton is broken
    // Use SW2(BOTTON_2) to instead
    // param = param;

    // 1. get the direction of moving
    int direction = 0;
    // button left
    if (Get_Button(JOY_LEFT) == 1) {
        // direction 1
        direction = 1;
    // button right
    } else if (Get_Button(JOY_RIGHT) == 1) {
        // direction 2
        direction = 2;
    // button press
    } else if (Get_Button(JOY_CTR) == 1) {
        // direction 3
        direction = 3;
    // button down
    } else if (Get_Button(JOY_DOWN) == 1) {
        // direction 4
        direction = 4;
    } else {
        // no button is pressed
        return;
    }
    // unit width
    int unit_width = SCREEN_WIDTH / param->x_size;
    // unit height
    int unit_height = SCREEN_HEIGHT / param->y_size;

    // define the new position
    int new_x = 0;
    int new_y = 0;
    // define the new box position
    int new_box_x = 0;
    int new_box_y = 0;
    // with direction to deal with

    int old_x = param->player_x;
    int old_y = param->player_y;

    switch (direction)
    {
    // direction left
    case 1:
        // cal the new position
        new_x = param->player_x - 1;
        new_y = param->player_y;
        new_box_x = param->player_x - 2;
        new_box_y = param->player_y;
        break;
    // direction right
    case 2:
        // cal the new position
        new_x = param->player_x + 1;
        new_y = param->player_y;
        new_box_x = param->player_x + 2;
        new_box_y = param->player_y;
        break;
    // direction up
    case 3:
        // cal the new position
        new_x = param->player_x;
        new_y = param->player_y - 1;
        new_box_x = param->player_x;
        new_box_y = param->player_y - 2;
        break;
    // direction down
    case 4:
        // cal the new position
        new_x = param->player_x;
        new_y = param->player_y + 1;
        new_box_x = param->player_x;
        new_box_y = param->player_y + 2;
        break;
    default:
        break;
    }
    // check if the new position is valid
    if (new_x < 0 || new_x >= param->x_size || new_y < 0 || new_y >= param->y_size) {
        // invalid
        return;
    }
    // check if the new position is a wall
    if (param->board[new_x][new_y] == 1) {
        // wall
        return;
    }
    // check if the new position is a target or a free area
    if (param->board[new_x][new_y] == 0) {

        // draw the new player
        // firstly update the position of the player
        param->player_x = new_x;
        param->player_y = new_y;
        // update the step
        param->steps++;
        // update_step(param);
    }
    // if the new position is a box
    else if (param->board[new_x][new_y] == 3) {
        // check if new box position is valid
        if (new_box_x < 0 || new_box_x >= param->x_size || new_box_y < 0 || new_box_y >= param->y_size) {
            // invalid
            return;
        }
        // if new box position is a wall or box
        if (param->board[new_box_x][new_box_y] == 1 || param->board[new_box_x][new_box_y] == 3) {
            // LCD_Clear(GREEN);
            return;
        }
        // if new box position is a free area or target
        if (param->board[new_box_x][new_box_y] == 0 || param->target[new_box_x][new_box_y] == 2) {
            // update box position
            param->board[new_box_x][new_box_y] = 3;
            // update the origin position of box
            param->board[new_x][new_y] = 0;
            // update the player position
            param->player_x = new_x;
            param->player_y = new_y;
            // update the step
            param->steps++;
            // update_step(param);
        }
    }
    // update the screen
    update_board(param, old_x, old_y, direction);
    // print_board(param);
}

/*
This function is used to check whether the player
has won the current level
*/
void check(LevelParameters* param, int* status)
{
    param = param;
    status = status;
    int box_target = 0;
    // status
    // win      : *status = 2
    // continue : *status = 0
    // chech if all the target's position is a box
    for (int i = 0; i < param->x_size; i ++) {
        for (int j = 0; j < param->y_size; j ++) {
            // if 2
            if (param->target[i][j] == 2) {
                // if not 3
                if (param->board[i][j] != 3) {
                    return;
                }
            }
        }
    }
    // update status
    *status = 2;
}

/*
This function is used to convert the man-made
map to the game map
*/
void map_translate(LevelParameters* param, char* map[], int size_x, int size_y)
{
    // define the game map template
    // # : wall
    // . : target
    // $ : box
    // @ : player
    // 20 * 10
    int num_boxes = 0;
    // for the array
    for (int i = 0; i < size_x; i++) {
        for (int j = 0; j < size_y; j++) {
            // translate the map
            // note that the map order is diff
            switch (map[j][i]) {
                // if the map is a wall
                case '#':
                    // set the board
                    param->board[3 + i][j] = 1;
                    break;
                // if the map is a target
                case '.':
                    // set  the board
                    param->target[3 + i][j] = 2;
                    break;
                // if the map is a box
                case '$':
                    // set the board
                    param->board[3 + i][j] = 3;
                    num_boxes++;
                    break;
                // if the map is a player
                case '@':
                    // set the board
                    param->player_x = 3 + i;
                    // set the board
                    param->player_y = j;
                    break;
                // else break
                default:
                    break;
            }
        }
    }
    // set the boxes
    param->boxes = num_boxes;
}


/*
This function is used to generate the board
*/
void board_generate(LevelParameters* param)
{
    int stone_index_X[] = {4, 15, 4, 4, 15, 4, 4};
    int stone_index_Y[] = {4, 2, 6, 7, 8, 3, 5};
    // generate random x and y in board size
    if (param->level != 4) {
        // set player
        // param->board[5][10] = 5;
        // param->board[5][11] = 5;
        for (int i = 0; i < param->stones; i++) {
            param->board[stone_index_X[i]][stone_index_Y[i]] = 1;
        }
        // generate the wall
        for (int i = 0; i < 10; i++) {
            param->board[0][i] = 1;
            param->board[19][i] = 1;
        }
        // generate the wall
        for (int j = 0; j < 20; j++) {
            param->board[j][0] = 1;
            param->board[j][9] = 1;
        }
        // set the box
        param->board[5][4] = 3;
        if (param->boxes == 2) {
            param->board[10][2] = 3;
        }
        // generate the box
        if (param->boxes == 3) {
            param->board[10][2] = 3;
            param->board[14][5] = 3;
        }
        // set the target to the target array
        param->target[5][7] = 2;
        if (param->boxes == 2) {
            param->target[14][2] = 2;
        }
        // generate the box
        if (param->boxes == 3) {
            param->target[14][7] = 2;
            param->target[14][2] = 2;
        }

        // set the player
        param->player_x = 5;
        param->player_y = 2;
    } else {
        map_translate(param, level_2_map, 10, 10);
    }
    // for the array
    for(int i = 0; i < param->x_size; ++i)
        for(int j = 0;j < param->y_size; ++j)
            // update the board
            param->origin_board[i][j] = param->board[i][j];
    // update the player
    param->player_origin_x = param->player_x;
    param->player_origin_y = param->player_y;
}

/*
This function is used to show the whole game board
So it might be shine if it is used to frequently
*/
void print_board(LevelParameters* param)
{
    LCD_Clear(BLACK);
    // param = param;
    // LCD_Clear(BLACK);
    // LCD_ShowString(0,0,"The game should be playing, but to be continued", WHITE);

    // define the unit_width and unit_height
    int unit_width = SCREEN_WIDTH / param->x_size;
    int unit_height = SCREEN_HEIGHT / param->y_size;

    // LCD_ShowString(0,0,"The game should be playing, but to be continued", WHITE);
    // loop the board and print
    for (int i = 0; i < param->x_size; ++i) {
        for (int j = 0; j < param->y_size; ++j) {
            // LCD_DrawRectangle
            // LCD_ShowString(0, 0, char(param->board[i][j]), WHITE);
            // LCD_ShowChar(0, 0, (char)param->board[i][j], 1, WHITE);
            // print the board
            // define with number
            // 0 : free area
            // 1 : wall (can not be removed when generate) at the edge of the screen
            // 2 : obstacle (can be removed when generate)
            // 3 : target
            // 4 : box
            // 5 : player
            switch (param->board[i][j]) {
                case 0:
                    // free area
                    // LCD_ShowString(j * 10, i * 10, " ", WHITE);
                    break;
                case 1:
                    // wall
                    // LCD_DrawRectangle(j * unit_width, i * unit_height, (j + 1) * unit_width, (i + 1) * unit_height, WHITE);
                    LCD_Fill(i * unit_width, j * unit_height, (i + 1) * unit_width - 1, (j + 1) * unit_height - 1, WHITE);
                    break;
                case 3:
                    // box
                    LCD_Fill(i * unit_width, j * unit_height, (i + 1) * unit_width - 1, (j + 1) * unit_height - 1, RED);
                    break;
                default:
                    break;
            }
            // if there is a target
            if (param->target[i][j] == 2) {
                LCD_Fill(i * unit_width, j * unit_height, (i + 1) * unit_width - 1, (j + 1) * unit_height - 1, BLUE);
            }
            // if there is a box on the target
            if (param->board[i][j] == 3 && param->target[i][j] == 2) {
                LCD_Fill(i * unit_width, j * unit_height, (i + 1) * unit_width - 1, (j + 1) * unit_height - 1, GREEN);
            }
        }
    }
    // consider the player as a single point
    // LCD_Fill(param->player_x * unit_width, param->player_y * unit_height, (param->player_x + 1) * unit_width - 1, (param->player_y + 1) * unit_height - 1, YELLOW);
    draw_player(param->player_x * unit_width, param->player_y * unit_height);
    // show the number of step
    LCD_ShowNum(0, 0, param->steps, 3, WHITE);
}

/*
This function only partially modify the board
So it will not be shine even if it is used to frequently
*
* Incremental Rendering
*
*/
void update_board(LevelParameters* param, int old_x, int old_y, int direction)
{
    // define the unit_width and unit_height
    int unit_width = SCREEN_WIDTH / param->x_size;
    int unit_height = SCREEN_HEIGHT / param->y_size;

    // show the number of step
    LCD_ShowNum(0, 0, param->steps, 3, WHITE);

    // update the position and its neibors
    int move_x[5] = {0, -1, 1, 0, 0}; // L, R, U, D
    int move_y[5] = {0, 0, 0, -1, 1}; // L, R, U, D


    for(int t = 0; t <= 2; ++t)
    {
        int i = old_x + move_x[direction] * t;
        int j = old_y + move_y[direction] * t;
        LCD_Fill(i * unit_width, j * unit_height, (i + 1) * unit_width - 1, (j + 1) * unit_height - 1, BLACK);

        // print the board
        // define with number
        // 0 : free area
        // 1 : wall (can not be removed when generate) at the edge of the screen
        // 2 : obstacle (can be removed when generate)
        // 3 : target
        // 4 : box
        // 5 : player
        switch (param->board[i][j]) {
            case 0:
                // free area
                // LCD_ShowString(j * 10, i * 10, " ", WHITE);
                break;
            case 1:
                // wall
                // LCD_DrawRectangle(j * unit_width, i * unit_height, (j + 1) * unit_width, (i + 1) * unit_height, WHITE);
                LCD_Fill(i * unit_width, j * unit_height, (i + 1) * unit_width - 1, (j + 1) * unit_height - 1, WHITE);
                break;
            case 3:
                // box
                LCD_Fill(i * unit_width, j * unit_height, (i + 1) * unit_width - 1, (j + 1) * unit_height - 1, RED);
                break;
            default:
                break;
        }
        // if there is a target
        if (param->target[i][j] == 2) {
            LCD_Fill(i * unit_width, j * unit_height, (i + 1) * unit_width - 1, (j + 1) * unit_height - 1, BLUE);
        }
        // if there is a box on the target
        if (param->board[i][j] == 3 && param->target[i][j] == 2) {
            LCD_Fill(i * unit_width, j * unit_height, (i + 1) * unit_width - 1, (j + 1) * unit_height - 1, GREEN);
        }

    }

    // consider the player as a single point
    // LCD_Fill(param->player_x * unit_width, param->player_y * unit_height, (param->player_x + 1) * unit_width - 1, (param->player_y + 1) * unit_height - 1, YELLOW);
    draw_player(param->player_x * unit_width, param->player_y * unit_height);
}