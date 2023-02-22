#include "play.h"
#include <stdlib.h>

#include "game.h"

struct game_t init_game(unsigned int seed)
{
    // Game parameters initialization (depending on the seed)
    struct game_t g = {};
    enum sort_t sorts[MAX_PIECES];
    int predefined_sorts = 0;
    rand();
    switch (seed)
    {
        case 0: // standard
            g.left_player_color = get_random_player();
            g.rel = get_random_relation();
            g.change_rel = 1;
            g.capture_activated = 1;
            g.board_type = CHINEESE;
            break;
        case 1: // dames chinoises 
            g.left_player_color = get_random_player();
            g.rel = 1;
            g.change_rel = 0;
            g.capture_activated = 0;
            g.board_type = CHINEESE;
            predefined_sorts = 1;
            for (int i=0; i<MAX_PIECES; i++)
                sorts[i]=PAWN;
            break;
        case 2: // Echecs
            g.left_player_color = get_random_player();
            g.rel = 0;
            g.change_rel = 0;
            g.capture_activated = 1;
            g.board_type = CHESS;
            predefined_sorts = 1;
            sorts[0] = TOWER;
            sorts[MAX_PIECES - 1] = TOWER;
            sorts[1] = ELEPHANT;
            sorts[MAX_PIECES - 2] = ELEPHANT;
            sorts[2] = BISHOP;
            sorts[MAX_PIECES - 3] = BISHOP;
            sorts[3] = KING;
            sorts[MAX_PIECES - 4] = QUEEN;
            break;
        default: // debug and test mode
            g.left_player_color = get_random_player();
            g.rel = 0;
            g.change_rel = 0;
            g.capture_activated = 1;
            g.board_type = CHINEESE;
            predefined_sorts = 1;
            for (int i = 2; i < MAX_PIECES-2; i++)
                sorts[i] = PAWN;
            if (MAX_PIECES > 0)
            {
                sorts[0] = TOWER;
                sorts[MAX_PIECES-1] = TOWER;
            }
            if (MAX_PIECES > 2)
            {
                sorts[1] = ELEPHANT;
                sorts[MAX_PIECES-2] = ELEPHANT;
            }
    }
    
    // World initialization
    g.w = world_init();
    init_neighbors(g.rel);

    // Pieces positioning
    if (predefined_sorts) set_start_predefined(g.w, g.players_pieces, g.left_player_color, g.rel, sorts);
    else set_start(g.w, g.players_pieces, g.left_player_color, g.rel);

    return g;
}
