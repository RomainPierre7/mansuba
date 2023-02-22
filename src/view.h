#ifndef __VIEW_H__
#define __VIEW_H__

#define COL_NORMAL "\033[00m"
#define COL_RED "\033[31;01m"
#define COL_GREEN "\033[32;01m"
#define COL_BLUE "\033[33;01m"
#define COL_WHITE "\033[37;01m"
#define COL_BLACK "\033[30;01m"

#define CHAR_PAWN "\U000026C2"
#define CHAR_TOWER "\U0000265C"
#define CHAR_ELEPHANT "\U0000265E"
#define CHAR_BISHOP "\U0000265D"
#define CHAR_QUEEN "\U0000265B"
#define CHAR_KING "\U0000265A"

#define BLACK_PAWN "\U000026C0"
#define BLACK_TOWER "\U00002656"
#define BLACK_ELEPHANT "\U00002658"
#define BLACK_BISHOP "\U00002657"
#define BLACK_QUEEN "\U00002655"
#define BLACK_KING "\U00002654"

#define _8D "\U00002735"
#define TRIANGLE "\U000025B3"
#define SQUARE "\U0000229E"

#define TROPHY "\U0001F3C6"
#define CLOCK "\U000023F1"
#define EXCLAMATION "\U0000203C"
#define CHAIN "\U000026D3"
#define UP "\U0001F199"

#include "game.h"

// To print the turn number
void print_turn(int print_mode, unsigned int turn);

// To print the board
void print_board(struct world_t *w, int print_mode, int rel, unsigned int from, unsigned int to);

//To print the captured pieces
void print_captured(struct game_t *g, int print_mode);

// To print the current relation
void print_relation(int print_mode, int relation);

#endif //__VIEW_H__
