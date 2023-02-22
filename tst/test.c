#include <stdio.h>
#include <stdlib.h>
#include "geometry.h"
#include "world.h"
#include "neighbors.h"
#include "play.h"  
#include "game.h"
#include "view.h"
#include "set.h"

/* init_game(-1) donne un monde où les noirs sont à gauche et les pièces de chaque joueurs
sont disposées sur leur première colonne avec de haut en bas :
Tour Elephant Pion ... Pioon Elephant Tour*/

// Fonctions aidant aux tests

void reset_world_test(struct game_t *g)
{
    enum sort_t sorts[MAX_PIECES];
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        world_set(g->w, i, NO_COLOR);
        world_set_sort(g->w, i, NO_SORT);
    }
    for (unsigned int i = 2; i < MAX_PIECES - 2; i++) sorts[i] = PAWN;
    sorts[0] = TOWER;
    sorts[MAX_PIECES-1] = TOWER;
    sorts[1] = ELEPHANT;
    sorts[MAX_PIECES-2] = ELEPHANT;
    set_start_predefined(g->w, g->players_pieces, g->left_player_color, g->rel, sorts);
    g->captured_pieces[0].size = 0;
    g->captured_pieces[1].size = 0;
}

// Les tests sont prévus pour une grille d'au moins 6x6

//TESTS

int test1_move_piece_capture(struct game_t g)
{
    struct move_t m = {.from = 0, .to = WIDTH - 1};
    move_piece(g.w, &g.players_pieces[pid(BLACK)], &g.players_pieces[pid(WHITE)], &g.captured_pieces[pid(WHITE)], m);
    if (g.captured_pieces[pid(WHITE)].size != 1) return 0;
    else if (g.captured_pieces[pid(WHITE)].places[0] != WIDTH - 1) return 0;
    else if (g.captured_pieces[pid(WHITE)].sorts[0] != TOWER) return 0;
    else if (g.players_pieces[pid(WHITE)].size != HEIGHT - 1) return 0;
    else if (g.players_pieces[pid(BLACK)].size != HEIGHT) return 0;
    else if (world_get(g.w, WIDTH - 1) != BLACK) return 0;
    else if (world_get_sort(g.w, WIDTH - 1) != TOWER) return 0;
    else if (world_get(g.w, 0) != NO_COLOR) return 0;
    else if (world_get_sort(g.w, 0) != NO_SORT) return 0;
    return 1;
}

int test2_free_piece_free_place(struct game_t g)
{
    world_set(g.w, WIDTH - 1, NO_COLOR);
    world_set_sort(g.w, WIDTH - 1, NO_SORT);
    g.captured_pieces[pid(WHITE)].size = 1;
    g.captured_pieces[pid(WHITE)].places[0] = WIDTH - 1;
    g.captured_pieces[pid(WHITE)].sorts[0] = TOWER;
    remove_place(&g.players_pieces[pid(WHITE)], HEIGHT - 1);
    for (int i = 0; i < 1000; i++)
    {
        free_piece(g.w, &g.players_pieces[pid(WHITE)], &g.captured_pieces[pid(WHITE)], WHITE, -1);
    }
    if (g.captured_pieces[pid(WHITE)].size != 0) return 0;
    else if (g.players_pieces[pid(WHITE)].size != HEIGHT) return 0;
    else if (!is_in(g.players_pieces[pid(WHITE)], WIDTH - 1)) return 0;
    else if (world_get(g.w, WIDTH - 1) != WHITE) return 0;
    else if (world_get_sort(g.w, WIDTH - 1) != TOWER) return 0;
    return 1;
}

int test3_free_piece_occuped_place(struct game_t g)
{
    world_set(g.w, WIDTH - 1, BLACK);
    world_set_sort(g.w, WIDTH - 1, PAWN);
    g.captured_pieces[pid(WHITE)].size = 1;
    g.captured_pieces[pid(WHITE)].places[0] = WIDTH - 1;
    g.captured_pieces[pid(WHITE)].sorts[0] = TOWER;
    g.players_pieces[pid(WHITE)].size = HEIGHT - 1;
    for (int i = 0; i < 1000; i++)
    {
        free_piece(g.w, &g.players_pieces[pid(WHITE)], &g.captured_pieces[pid(WHITE)], WHITE, -1);
    }
    if (g.captured_pieces[pid(WHITE)].size != 1) return 0;
    else if (g.players_pieces[pid(WHITE)].size != HEIGHT - 1) return 0;
    else if (world_get(g.w, WIDTH - 1) != BLACK) return 0;
    else if (world_get_sort(g.w, WIDTH - 1) != PAWN) return 0;
    return 1;
}

int test4_list_tower_moves(struct game_t g)
{
    world_set(g.w, WIDTH + 3, BLACK);
    world_set_sort(g.w, WIDTH + 3, TOWER);
    world_set(g.w, 3, BLACK);
    world_set_sort(g.w, 3, PAWN);
    world_set(g.w, WIDTH, WHITE);
    world_set_sort(g.w, WIDTH, PAWN);
    world_set(g.w, WIDTH + 4, WHITE);
    world_set_sort(g.w, WIDTH + 4, PAWN);
    world_set(g.w, 3 + 3 * WIDTH, BLACK);
    world_set_sort(g.w, 3 + 3 * WIDTH, PAWN);
    struct set_t possibles = list_possibles_moves(&g, WIDTH + 3);
    struct set_t verif = {.places = {9, 10, 12, 19}, .size = 4};
    for (unsigned int i = 0; i < verif.size; i++)
    {
        if (possibles.places[i] != verif.places[i]) return 0;
    }
    if (possibles.size != verif.size) return 0;
    return 1;
}

int test4_list_bishop_moves(struct game_t g)
{
    world_set(g.w, 2 * WIDTH + 2, BLACK);
    world_set_sort(g.w, 2 * WIDTH + 2, BISHOP);
    world_set(g.w, WIDTH + 1, WHITE);
    world_set_sort(g.w, WIDTH + 1, PAWN);
    world_set(g.w, WIDTH + 3, BLACK);
    world_set_sort(g.w, WIDTH + 3, PAWN);
    world_set(g.w, 4 * WIDTH + 4, BLACK);
    world_set_sort(g.w, 4 * WIDTH + 4, PAWN);
    world_set(g.w, 4 * WIDTH, NO_COLOR);
    world_set_sort(g.w, 4 * WIDTH, NO_SORT);
    struct set_t possibles = list_possibles_moves(&g, 2 * WIDTH + 2);
    struct set_t verif = {.places = {9, 25, 27, 32}, .size = 4};
    for (unsigned int i = 0; i < verif.size; i++)
    {
        if (possibles.places[i] != verif.places[i]) return 0;
    }
    if (possibles.size != verif.size) return 0;
    return 1;
}

/* int testX_nom_de_fonctionnalite()
{
} */

//MAIN

int main()
{
    struct game_t g = init_game(-1);

    if (test1_move_piece_capture(g)) printf("TEST move_piece (with capture): OK\n");
    else printf("TEST move_piece (with capture) : ERROR\n");

    reset_world_test(&g);
    if (test2_free_piece_free_place(g)) printf("TEST free_piece (free place) : OK\n");
    else printf("TEST free_piece (free place) : ERROR\n");

    reset_world_test(&g);
    if (test3_free_piece_occuped_place(g)) printf("TEST free_piece (occuped place) : OK\n");
    else printf("TEST free_piece (occuped place) : ERROR\n");

    reset_world_test(&g);
    if (test4_list_tower_moves(g)) printf("TEST list_tower_moves : OK\n");
    else printf("TEST list_tower_moves : ERROR\n");

    reset_world_test(&g);
    if (test4_list_bishop_moves(g)) printf("TEST list_bishop_moves : OK\n");
    else printf("TEST list_bishop_moves : ERROR\n");

    /* reset_world_test(&g);
    if (testX_nom_de_fonctionnalite()) printf("TEST fonctionnalitéX : OK\n");
    else printf("TEST fonctionnalitéX : ERROR\n");*/

    return 0;
}