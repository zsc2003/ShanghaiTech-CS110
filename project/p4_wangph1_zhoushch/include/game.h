#ifndef GAME_H
#define GAME_H

#include "lcd/lcd.h"
#include <string.h>
#include "utils.h"
#include "gamedrivers.h"

// define with number
// 0 : free area
// 1 : wall
// 2 : target, store it in the target array
// 3 : box

// define the game map template
// _ : outside area
// - : inside free area
// # : wall
// . : target
// $ : box
// 20 * 10

// level
extern const char *level_1_map[];
// map
extern const char *level_2_map[];

// player move
void player_move(LevelParameters* param);
// check win
void check(LevelParameters* param, int* status);
// generate board
void board_generate(LevelParameters* param);
// print the board
void print_board(LevelParameters* param);

#endif // GAME_H