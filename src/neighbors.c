#include "neighbors.h"
#include <limits.h>
#include <stdio.h>

static int relation[8] = {};

static int convert(enum dir_t c)
{
    switch (c)
    {
    case SEAST:
        return 0;
        break;
    case SOUTH:
        return 1;
        break;
    case SWEST:
        return 2;
        break;
    case WEST:
        return 3;
        break;
    case NWEST:
        return 4;
        break;
    case NORTH:
        return 5;
        break;
    case NEAST:
        return 6;
        break;
    case EAST:
        return 7;
        break;
    default:
        printf("Erreur, NO_DIR et MAX_DIR n'ont pas de place dans relation");
        return -1;
        break;
    }
}

void init_neighbors(unsigned int seed)
{
    for (int i = 0; i < 8; i++) relation[i] = 0;
    switch(seed)
    {
    case 0: // Classical grid (8 neighbors)
        for (int i = 0; i < 8; i++) relation[i] = 1;
        break;
    case 1: // Triangular grid (SEAST and NWEST closed)
        for (int i = 0; i < 8; i++) if ((i != 0) && (i != 4)) relation[i] = 1;
        break;
    case 2: // Square grid (Cardianl direction opened)
        for (int i = 1; i < 8; i += 2) relation[i] = 1;
        break;
    }
}

unsigned int get_neighbor(unsigned int idx, enum dir_t d)
{
    if (idx > WORLD_SIZE - 1) return UINT_MAX;
    switch (d)
        {
        case EAST:
            if ((idx % WIDTH == WIDTH - 1) || (relation[convert(EAST)] == 0)) return UINT_MAX;
            return idx + 1;
            break;
        case NEAST:
            if ((idx % WIDTH == WIDTH - 1) || (idx / WIDTH == 0) || (relation[convert(NEAST)] == 0)) return UINT_MAX;
            return idx - WIDTH + 1;
            break;
        case NORTH:
            if ((idx / WIDTH == 0) || (relation[convert(NORTH)] == 0)) return UINT_MAX;
            return idx - WIDTH;
            break;
        case NWEST:
            if ((idx % WIDTH == 0) || (idx / WIDTH == 0) || (relation[convert(NWEST)] == 0)) return UINT_MAX;
            return idx - WIDTH -1;
            break;
        case WEST:
            if (idx % WIDTH == 0) return UINT_MAX;
            return idx - 1;
            break;
        case SWEST:
            if ((idx % WIDTH == 0) || (idx / WIDTH == HEIGHT - 1) || (relation[convert(SWEST)] == 0)) return UINT_MAX;
            return idx + WIDTH - 1;
            break;
        case SOUTH:
            if ((idx / WIDTH == HEIGHT - 1) || (relation[convert(SOUTH)] == 0)) return UINT_MAX;
            return idx + WIDTH;
            break;
        case SEAST:
            if ((idx % WIDTH == WIDTH - 1) || (idx / WIDTH == HEIGHT - 1) || (relation[convert(SEAST)] == 0)) return UINT_MAX;
            return idx + WIDTH + 1;
            break;
        default:
            return UINT_MAX;
            break;
        }
}

struct neighbors_t get_neighbors(unsigned int idx)
{
    struct neighbors_t neighbors = {};
    int index = 0;

    // Add every neighbor
    for (int j = -4; j < 5; j++)
    {
        unsigned int neighbor = get_neighbor(idx, j);
        if (neighbor != UINT_MAX)
        {
            neighbors.n[index].i = neighbor;
            neighbors.n[index].d = j;
            index++;
        }
    }

    // add finish sequence
    neighbors.n[index].i = UINT_MAX;
    neighbors.n[index].d = NO_DIR;
    return neighbors;
}
