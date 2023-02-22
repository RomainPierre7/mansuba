#ifndef __PLAY_H__
#define __PLAY_H__

#include "game.h"
#include "neighbors.h"
#include "set.h"
#include "view.h"

#define MAX_PIECES HEIGHT
#define SHUFFLE_ITERATIONS MAX_PIECES*10

// struct of a move
struct move_t {
    unsigned int from;
    unsigned int to;
};

// Return random number between 1 and 2 (Black = 1, White = 2)
enum color_t get_random_player();

// Returns random number between 0 and MAX_RELATION - 1
int get_random_relation();

// Returns the other player number than the current player (Black = 1, White = 2)
enum color_t next_player(enum color_t current_player);

// Returns the index in the list players_pieces of the player player_color
unsigned int pid(enum color_t player_color);

// Returns if the place is a finish line or not, third argument is the left player
int is_finish_line(unsigned int p, enum color_t current_player);

// Returns 1 if the direction is cardinal 0 if not
int is_cardinal(enum dir_t d);

// return if p is a starting position
int is_start_pos(unsigned int p);

// List all moves for a pawn on the place idx
struct set_t list_pawn_moves(struct world_t *w, unsigned int p, enum color_t piece_color, enum board_type_t board_type);

// List all moves for a tower on the place idx
struct set_t list_tower_moves(struct world_t *w, unsigned int p, enum color_t piece_color);

// List all moves for an elephant on the place idx
struct set_t list_elephant_moves(struct world_t *w, unsigned int p, enum color_t piece_color);

// List all attaignable positions for a bishop on the place idx
struct set_t list_bishop_moves(struct world_t *w, unsigned int p, enum color_t piece_color);

// List all attaignable positions for a queen on the place idx
struct set_t list_queen_moves(struct world_t *w, unsigned int p, enum color_t piece_color);

// List all attaignable positions for a king on the place idx
struct set_t list_king_moves(struct world_t *w, unsigned int p, enum color_t piece_color);

// list all possibles moves for a piece in place p
struct set_t list_possibles_moves(struct game_t *g, unsigned int p);

// list al attainable position from a set
struct set_t list_possibles_moves_from_set(struct game_t *g, struct set_t s, unsigned int origin, struct set_t origins[], int explored[]);

// basic IA to randomly choose a good piece to move
unsigned int choose_accurate_piece(struct game_t *g, enum color_t current_player_color);

// Select randomly a move for the piece
struct move_t choose_random_move_for_piece(struct game_t *g, unsigned int p);

// Select nearest move for a piece
struct move_t choose_nearest_move(struct game_t *g, unsigned int p);

// Select best move for a piece
struct move_t choose_best_move(struct game_t *g, unsigned int p);

// Move the piece
void move_piece(struct world_t *w, struct set_t *current_player_pieces, struct set_t *opponent_player_pieces, struct set_captured_t *opponent_capt, struct move_t m);

// Returns if the game is over or not, second argument is the left player
int end_game(struct set_t players_pieces[], char t);

// Add a piece to a set
void add_piece(struct world_t *w, struct set_t player_pieces[], unsigned int pos, enum color_t color, enum sort_t sort);

// Initialize the starting position, second argument is the left one
void set_start(struct world_t *w, struct set_t players_pieces[], enum color_t c, int rel);

// Initialize the starting position with predefined sorts, second argument is the left one
void set_start_predefined(struct world_t *w, struct set_t players_pieces[], enum color_t c, int rel, enum sort_t *sorts);

// Choose a piece and free her with a 50% chance
void free_piece(struct world_t *w, struct set_t *player_pieces, struct set_captured_t *capt, enum color_t c, int print_mode);

// Capture a piece
void capture_piece(struct world_t *w, unsigned int idx, struct set_t *player_pieces, struct set_captured_t *capt);

#endif // __PLAY_H__
