#include "lcd/lcd.h"
#include <string.h>
#include "utils.h"
#include "gamedrivers.h"

void Inp_init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOC);

    gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6);
    gpio_init(GPIOC, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
}

void IO_init(void)
{
    Inp_init(); // inport init
    Lcd_Init(); // LCD init
}

int main(void)
{
    IO_init();         // init OLED
    // testing_code();
    
    // YOUR CODE HERE
    /*
    set 19260817 as unplayed status
    */
    int score_board[3][3]; // 3 * 3 score board
    score_board[0][0] = 19260817, score_board[0][1] = 19260817, score_board[0][2] = 19260817;
    score_board[1][0] = 19260817, score_board[1][1] = 19260817, score_board[1][2] = 19260817;
    score_board[2][0] = 19260817, score_board[2][1] = 19260817, score_board[2][2] = 19260817;
    // game starter
    game_starter();
    while(1)
    {   
        // generate param
        LevelParameters* param = paras_generator(20, 10);
        // level_selection(param, score_board);
        int is_set = parameters_setting(param, score_board);
        if (is_set == 0)
            break;
        // generate level
        level_generate(param, score_board);
        level_starter(param, score_board);
        // if leave
        int is_leave = level_closer(param, score_board);
        if(is_leave)
            break;
    }
    // free
    game_closer();

    return 0;
}