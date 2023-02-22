#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include "play.h"

#include "view.h"

void print_turn(int print_mode, unsigned int turn)
{
  if (print_mode == 0)
  {
    printf("---------------- turn : %d -----------------\n", turn);
  }
}

void print_board (struct world_t *w, int print_mode, int rel, unsigned int from, unsigned int to)
{
    if (print_mode == 0)
    {
      for (unsigned int i = 0; i < WORLD_SIZE; i++)
        {
            // grid type specific prints
            if (i % WIDTH == 0) 
            {
              if (rel == 0 && i/WIDTH == 1)
                printf("         Grid type : "_8D);
              else if (rel == 1 && i/WIDTH == 1)
                printf("         Grid type : "TRIANGLE);
              else if (rel == 2 && i/WIDTH == 1)
                printf("         Grid type : "SQUARE);
              printf("\n");

              // triangle effect
              if (rel == 1)
                for (unsigned int j=0; j<i/WIDTH; j++)
                  printf(" ");
            }

            // BODY
            if (i == to)
            {
              // print the recently moved piece in color
              printf(COL_GREEN);
              if ((world_get(w, i) == BLACK) && (world_get_sort(w, i) == PAWN)) printf(BLACK_PAWN);
              else if ((world_get(w, i) == WHITE) && (world_get_sort(w, i) == PAWN)) printf(CHAR_PAWN);
              else if ((world_get(w, i) == BLACK) && (world_get_sort(w, i) == TOWER)) printf(BLACK_TOWER);
              else if ((world_get(w, i) == WHITE) && (world_get_sort(w, i) == TOWER)) printf(CHAR_TOWER);
              else if ((world_get(w, i) == BLACK) && (world_get_sort(w, i) == ELEPHANT)) printf(BLACK_ELEPHANT);
              else if ((world_get(w, i) == WHITE) && (world_get_sort(w, i) == ELEPHANT)) printf(CHAR_ELEPHANT);
              else if ((world_get(w, i) == BLACK) && (world_get_sort(w, i) == BISHOP)) printf(BLACK_BISHOP);
              else if ((world_get(w, i) == WHITE) && (world_get_sort(w, i) == BISHOP)) printf(CHAR_BISHOP);
              else if ((world_get(w, i) == BLACK) && (world_get_sort(w, i) == QUEEN)) printf(BLACK_QUEEN);
              else if ((world_get(w, i) == WHITE) && (world_get_sort(w, i) == QUEEN)) printf(CHAR_QUEEN);
              else if ((world_get(w, i) == BLACK) && (world_get_sort(w, i) == KING)) printf(BLACK_KING);
              else if ((world_get(w, i) == WHITE) && (world_get_sort(w, i) == KING)) printf(CHAR_KING);
              else if (i == from) printf(COL_GREEN"."COL_NORMAL);
              else printf(".");

              if (i == to) printf(COL_NORMAL);
              printf(" ");
            }

            // print other pieces
            else
            {
              if ((world_get(w, i) == BLACK) && (world_get_sort(w, i) == PAWN)) printf(COL_WHITE BLACK_PAWN COL_NORMAL);
              else if ((world_get(w, i) == WHITE) && (world_get_sort(w, i) == PAWN)) printf(COL_WHITE CHAR_PAWN COL_NORMAL);
              else if ((world_get(w, i) == BLACK) && (world_get_sort(w, i) == TOWER)) printf(COL_WHITE BLACK_TOWER COL_NORMAL);
              else if ((world_get(w, i) == WHITE) && (world_get_sort(w, i) == TOWER)) printf(COL_WHITE CHAR_TOWER COL_NORMAL);
              else if ((world_get(w, i) == BLACK) && (world_get_sort(w, i) == ELEPHANT)) printf(COL_WHITE BLACK_ELEPHANT COL_NORMAL);
              else if ((world_get(w, i) == WHITE) && (world_get_sort(w, i) == ELEPHANT)) printf(COL_WHITE CHAR_ELEPHANT COL_NORMAL);
              else if ((world_get(w, i) == BLACK) && (world_get_sort(w, i) == BISHOP)) printf(COL_WHITE BLACK_BISHOP COL_NORMAL);
              else if ((world_get(w, i) == WHITE) && (world_get_sort(w, i) == BISHOP)) printf(COL_WHITE CHAR_BISHOP COL_NORMAL);
              else if ((world_get(w, i) == BLACK) && (world_get_sort(w, i) == QUEEN)) printf(COL_WHITE BLACK_QUEEN COL_NORMAL);
              else if ((world_get(w, i) == WHITE) && (world_get_sort(w, i) == QUEEN)) printf(COL_WHITE CHAR_QUEEN COL_NORMAL);
              else if ((world_get(w, i) == BLACK) && (world_get_sort(w, i) == KING)) printf(COL_WHITE BLACK_KING COL_NORMAL);
              else if ((world_get(w, i) == WHITE) && (world_get_sort(w, i) == KING)) printf(COL_WHITE CHAR_KING COL_NORMAL);
              else if (i == from) printf(COL_GREEN"."COL_NORMAL);
              else printf(".");
              printf(" ");
            }
        }
        printf("\n\n");
    }
}

void print_captured(struct game_t *g, int print_mode)
{
  if ((print_mode == 0) && (g->capture_activated == 1))
  {
    printf("Captured pieces : ");
    printf(COL_WHITE);
    for (unsigned int i = 0; i < g->captured_pieces[pid(WHITE)].size; i++)
    {
      if (g->captured_pieces[pid(WHITE)].sorts[i] == PAWN) printf(CHAR_PAWN" ");
      else if (g->captured_pieces[pid(WHITE)].sorts[i] == TOWER) printf(CHAR_TOWER" ");
      else if (g->captured_pieces[pid(WHITE)].sorts[i] == ELEPHANT) printf(CHAR_ELEPHANT" ");
      else if (g->captured_pieces[pid(WHITE)].sorts[i] == BISHOP) printf(CHAR_BISHOP" ");
      else if (g->captured_pieces[pid(WHITE)].sorts[i] == QUEEN) printf(CHAR_QUEEN" ");
      else if (g->captured_pieces[pid(WHITE)].sorts[i] == KING) printf(CHAR_KING" ");
    }
    for (unsigned int i = 0; i < g->captured_pieces[pid(BLACK)].size; i++)
    {
      if (g->captured_pieces[pid(BLACK)].sorts[i] == PAWN) printf(BLACK_PAWN" ");
      else if (g->captured_pieces[pid(BLACK)].sorts[i] == TOWER) printf(BLACK_TOWER" ");
      else if (g->captured_pieces[pid(BLACK)].sorts[i] == ELEPHANT) printf(BLACK_ELEPHANT" ");
      else if (g->captured_pieces[pid(BLACK)].sorts[i] == BISHOP) printf(BLACK_BISHOP" ");
      else if (g->captured_pieces[pid(BLACK)].sorts[i] == QUEEN) printf(BLACK_QUEEN" ");
      else if (g->captured_pieces[pid(BLACK)].sorts[i] == KING) printf(BLACK_KING" ");
    }
    printf(COL_NORMAL);
    printf("\n\n");
  }
}

void print_relation(int print_mode, int relation)
{
  if (print_mode == 0)
  {
    switch(relation)
    {
    case 0:
      printf("////// "UP" The board is set as a classical grid //////\n\n");
      break;
    case 1:
      printf("////// "UP" The board is set as a triangular grid //////\n\n");
      break;
    case 2:
      printf("////// "UP" The board is set as a square grid //////\n\n");
      break;
    }
  }
}
