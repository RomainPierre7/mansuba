#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#include "play.h"

int main(int argc, char *argv[])
{
    // Get execution options
    int opt;
    unsigned int MAX_TURNS = 2 * WORLD_SIZE;
    char victory_mode = 's';
    int print_mode = -1;
    int game_seed = 0;
    unsigned int delay = 0;
    while ((opt = getopt(argc, argv, "s:m:t:c:pd:")) != -1)
    {
        switch (opt)
        { 
        case 's':
            srand(atoi(optarg));
            break;
        case 'm':
            MAX_TURNS = atoi(optarg);
            break;
        case 't':
            victory_mode = optarg[0];
            break;
        case 'c':
            game_seed = atoi(optarg);
            break;
        case 'p':
            print_mode = 0;
            break;
        case 'd':
            delay = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n",argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Prepare the game
    struct game_t g = init_game(game_seed);
    enum color_t current_player = get_random_player();
    unsigned int turn = 0;

    // Initial print
    printf("\n");
    print_relation(print_mode, g.rel);
    print_turn(print_mode, turn);
    print_board(g.w, print_mode, g.rel, UINT_MAX, UINT_MAX);

    // Game loop
    while (!end_game(g.players_pieces, victory_mode) && turn < MAX_TURNS)
    {
        struct move_t m;
        print_turn(print_mode, turn + 1);

        // TURN BODY
        // either try to free a captured piece 
        if ((g.captured_pieces[pid(current_player)].size > 0) && (((rand() % 2) == 0) || g.players_pieces[pid(current_player)].size == 0))
        {
            free_piece(g.w, &g.players_pieces[pid(current_player)], &g.captured_pieces[pid(current_player)], current_player, print_mode);
        }
        // or move a free piece
        else
        {
            // choose a piece
            int p = choose_accurate_piece(&g, current_player);

            // choose a move for this piece
            m = choose_best_move(&g, p);

            // move the piece
            move_piece(g.w, &g.players_pieces[pid(current_player)], &g.players_pieces[pid(next_player(current_player))], &g.captured_pieces[pid(next_player(current_player))], m);
        }

        // print out the game
        print_board(g.w, print_mode, g.rel, m.from, m.to);
        print_captured(&g, print_mode);

        // change the map every sqrt(MAX_TURNS)
        if (g.change_rel && turn % (int)(floor(sqrt(MAX_TURNS))) == 0)
        {
            g.rel = get_random_relation();
            init_neighbors(g.rel);
            print_relation(print_mode, g.rel);
        }

        // delay option
        usleep(delay);

        current_player = next_player(current_player);
        turn++;
    }

    // print game result
    if (turn >= MAX_TURNS)
    {
        printf(CLOCK"  "EXCLAMATION"  Maximum number of turns exceeded (%d), there is a tie\n", MAX_TURNS);
        return 0;
    }

    current_player = next_player(current_player);
    if (current_player == BLACK) printf(TROPHY" The winner is the player who plays in Black\n");
    else if (current_player == WHITE) printf(TROPHY" The winner is the player who plays in White\n");

    printf(CLOCK"  The game lasted %d turns\n", turn);

    return 0;
}
