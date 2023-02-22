#include <limits.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "set.h"

unsigned int choose_random_place_in_set(struct set_t pieces)
{
    if (pieces.size == 0) return UINT_MAX;
    return pieces.places[ rand() % pieces.size ];
}

struct set_t set_union(struct set_t a, struct set_t b)
{
    // optimization : to add the smallest set into the biggest
    struct set_t *big = &a, *little = &b;
    if (b.size > a.size) 
    {
        big = &b;
        little = &a;
    }

    // function body
    for (unsigned int i = 0; i < little->size; i++)
    {
        add_place(big, little->places[i]);
    }
    return *big;
}

static int index(struct set_t s, unsigned int p)
{
    int a = 0 , b = s.size-1, m = (a+b) / 2;
    while (a<=b)
    {
        if (s.places[m] == p)
            return m;
        if (s.places[m]<p)
            a = m + 1;
        else
            b = m - 1;
        m = (a+b) / 2;
    }
    return -a-1;
}

int is_sorted(struct set_t s)
{
    for (unsigned int i=1; i<s.size; i++)
        if (s.places[i-1] > s.places[i])
            return 0;
    return 1;
}

void add_place(struct set_t *s, unsigned int p)
{
    int pind = index(*s, p);
    if (pind >= 0) return; // if p is already in s

    pind = -pind-1; // pind = index where p should be in the set
    s->size++;
    for (int i = s->size-1; i>pind; i--)
    {
        s->places[i] = s->places[i-1]; // we shift all values to the right to make place for p
    }
    s->places[pind] = p;
}

void remove_place(struct set_t *s, unsigned int p)
{
    int pind = index(*s, p);
    if (pind < 0) return; // if p is not in s

    for (unsigned int i=pind; i<s->size; i++)
        s->places[i] = s->places[i+1]; // shift all places to the right
    s->size--;
    assert(is_sorted(*s));
}

int is_in(struct set_t s, unsigned int p)
{
    return index(s, p) >= 0;
}

int equal(struct set_t a, struct set_t b)
{
    if (a.size != b.size) return 0;
    for (unsigned int i=0; i<a.size; i++)
        if (a.places[i] != b.places[i])
            return 0;
    return 1;
}

