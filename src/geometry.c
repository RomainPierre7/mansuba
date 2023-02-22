#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "geometry.h"

const char* place_to_string(enum color_t c, enum sort_t s)
{
    char color[20];
    char sort[20];
    size_t len = 0;

    // color
    switch (c)
    {
    case BLACK:
        strcpy(color,"is black.");
        break;
    case WHITE:
        strcpy(color, "is white.");
        break;
    default :
        strcpy(color, "has no color.");
    }

    // sort
    switch (s)
    {
    case 1:
        strcpy(sort,"The pawn ");
        break;
    default :
        strcpy(sort, "No sort ");
    }

    len = strlen(sort) + strlen(color) + 1;
    char *str = (char *)malloc(len * sizeof(char));
    strcpy(str, sort);
    strcat(str, color);
    return str;
}

/** Return a string describing the direction `d` */
const char* dir_to_string(enum dir_t d)
{
    switch (d)
    {
    case EAST:
        return "The direction is east.";
        break;
    case NEAST:
        return "The direction is northeast.";
        break;
    case NORTH:
        return "The direction is north.";
        break;
    case NWEST:
        return "The direction is northwest.";
        break;
    case WEST:
        return "The direction is west.";
        break;
    case SWEST:
        return "The direction is southwest.";
        break;
    case SOUTH:
        return "The direction is south.";
        break;
    case SEAST:
        return "The direction is southeast.";
        break;
    default :
        return "There is no direction.";
        break;
    }
}
