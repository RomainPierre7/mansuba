#ifndef __SET_H__
#define __SET_H__

#include "geometry.h"

// struct representing a set of places
struct set_t {
  unsigned int size;
  unsigned int places[WORLD_SIZE];
};

// struct representing a set of captured pieces
struct set_captured_t {
  unsigned int size;
  unsigned int places[WORLD_SIZE];
  enum sort_t sorts[WORLD_SIZE];
};

// Select randomly a place in a set
unsigned int choose_random_place_in_set(struct set_t pieces);

// return union of a and b
struct set_t set_union(struct set_t a, struct set_t b);

// returns true if there is no problem
int is_sorted(struct set_t s);

// add p to s (with doublon gestion)
void add_place(struct set_t *s, unsigned int p);

// remove place p from set s 
void remove_place(struct set_t *s, unsigned int p);

// returns 1 if p is in the set
int is_in(struct set_t s, unsigned int p);

//test equality of 2 sets
int equal(struct set_t a, struct set_t b);

#endif // __SET_H__
