#ifndef __GAME_H__
#define __GAME_H__

#include "set.h"
#include "world.h"

enum board_type_t
{
    NO_BOARD_TYPE = 0,
    CHINEESE = 1,
    CHESS = 2,
    MAX_BOARD_TYPE = 3
};

// struct representing game parameters
struct game_t
{
    struct world_t *w;
    struct set_t players_pieces[MAX_PLAYERS];
    struct set_captured_t captured_pieces[MAX_PLAYERS];
    enum color_t left_player_color;
    int rel;
    int change_rel;
    int capture_activated;
    enum board_type_t board_type;
};

struct game_t init_game(unsigned int seed);

#endif // __GAME_H__
