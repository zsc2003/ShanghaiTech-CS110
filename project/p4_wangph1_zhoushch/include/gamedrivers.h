#ifndef __GAMEDRIVERS_H__
#define __GAMEDRIVERS_H__

#include "lcd/lcd.h"
#include <string.h>
#include "utils.h"

// screen size
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 80

// player size
#define PLAYER_SIZE_X 4
#define PLAYER_SIZE_Y 4

// user image
const static uint16_t user_image[PLAYER_SIZE_X][PLAYER_SIZE_Y] = {
    // first line
	{RED,BLUE,BLUE,RED},
    // second line
	{GREEN,RED,RED,YELLOW},
    // third line
	{YELLOW,RED,RED,GREEN},
    // fourth line
	{RED,BLUE,BLUE,RED},
};

typedef struct LevelParameters
{
    // level
    int level;
    int stones;
    int boxes;
    int x_size;
    int y_size;
    int steps;
    // board array
    int** origin_board;
    int** board;
    // player position
    int player_x;
    int player_y;
    // origin player position
    int player_origin_x;
    int player_origin_y;
    // target array
    int** target;
}LevelParameters;

// test
void testing_code();
void test_clear_area();
// draw
void draw_player(int x, int y);
void game_starter();
// helper func
LevelParameters* paras_generator(int x_size, int y_size);
int parameters_setting(LevelParameters* param, int (*score_board)[3]);
void level_generate(LevelParameters* param, int (*score_board)[3]);
void level_starter(LevelParameters* param, int (*score_board)[3]);
int level_closer(LevelParameters* param, int (*score_board)[3]);
// free

void game_closer();

#endif // __GAMEDRIVERS_H__