#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include "play.h"

static struct set_t starting_positions[MAX_PLAYERS];

enum color_t get_random_player()
{
    return (rand() % 2) + 1;
}

int get_random_relation()
{
    return (rand() % MAX_RELATIONS);                
}

enum color_t next_player(enum color_t current_player)
{
    if (current_player == BLACK)
        return WHITE;
    else
        return BLACK;
}

unsigned int pid(enum color_t player_color)
{
    return player_color - 1;
}

static int opposite_player_index(int i)
{
    return MAX_PLAYERS-i-1;
}

static int place_id(unsigned int x, unsigned int y)
{
    return x + y * WIDTH;
}

int is_finish_line(unsigned int p, enum color_t current_player)
{   
    return is_in(starting_positions[opposite_player_index(pid(current_player))], p);
}

int is_cardinal(enum dir_t d)
{
    return d==-3 || d==-1 || d==1 || d==3;
}

int is_start_pos(unsigned int p)
{
    for (unsigned int i = 0; i < MAX_PLAYERS; i++)
        if (is_in(starting_positions[i], p)) return 1;
    return 0;
}

// Handle multile jumps recursively
static void multiple_jumps(struct world_t *w, struct set_t *possibles, unsigned int p, enum color_t current_color, enum board_type_t board_type) 
{
    // FINISH CHECK
    if (is_in(*possibles, p)) return; // if we already visited p, we go back
    add_place(possibles, p);
    if (world_get_sort(w, p) != NO_SORT) return; // if we capture a piece we can't go further

    // EXPLORE
    struct neighbors_t neighbors = get_neighbors(p);
    int i=0;
    while (neighbors.n[i].i != UINT_MAX && neighbors.n[i].d != NO_DIR) 
    {
        unsigned int newp = neighbors.n[i].i;
        if (world_get_sort(w, newp) != NO_SORT && (world_get(w, newp) != current_color || board_type == CHINEESE))
        {
            newp = get_neighbor(newp, neighbors.n[i].d);
            if (newp != UINT_MAX && world_get(w, newp) != current_color)
            {
                // do the same fonction for every jump possible from p
                multiple_jumps(w, possibles, newp, current_color, board_type);
            }
        }
        i++;
    }
    return;
}

struct set_t list_pawn_moves(struct world_t *w, unsigned int p, enum color_t piece_color, enum board_type_t board_type)
{
    enum color_t current_color = piece_color;
    struct set_t possibles = {};

    // Add to our set every attainable position using jumps
    multiple_jumps(w, &possibles, p, current_color, board_type);
    
    // Add to our set positions directly adjacent to p
    struct neighbors_t neighbors = get_neighbors(p);
    unsigned int i=0;
    while (neighbors.n[i].i != UINT_MAX && neighbors.n[i].d != NO_DIR) 
    {
        unsigned int newp = neighbors.n[i].i;
        if (world_get(w, newp) != current_color)
        {
            add_place(&possibles, newp);
        }
        i++;
    }

    // we remove the starting position
    remove_place(&possibles, p);

    return possibles;
}

struct set_t list_tower_moves(struct world_t *w, unsigned int p, enum color_t piece_color)
{
    struct set_t possibles = {};
    struct neighbors_t neighbors = get_neighbors(p);
    enum color_t current_color = piece_color;

    unsigned int i=0;

    while (neighbors.n[i].d != NO_DIR && neighbors.n[i].i != UINT_MAX) // for every cardinal directions
    {
        enum dir_t d = neighbors.n[i].d;
        unsigned int newp = neighbors.n[i].i;
        if (is_cardinal(d))
        {
            while (newp != UINT_MAX) // for every position in this direction until we reach an obstacle
            {
                // Handle ennemy pieces
                if (world_get_sort(w, newp) != NO_SORT)
                {
                    if (world_get(w, newp) == current_color)
                        break;
                    else
                    {
                        add_place(&possibles, newp);
                        break;
                    }
                }

                // add the place
                add_place(&possibles, newp);
                newp = get_neighbor(newp, d);
            }
        }
        i++;
    }
    return possibles;
}

struct set_t list_elephant_moves(struct world_t *w, unsigned int idx, enum color_t piece_color)
{
    enum color_t current_color = piece_color;
    struct set_t possibles = {};
    unsigned int first_move[4] = {
        get_neighbor(idx, EAST), 
        get_neighbor(idx, NORTH),
        get_neighbor(idx, WEST), 
        get_neighbor(idx, SOUTH)
    };

    for (int i = 0; i < 4; i++) // first step
    {
        unsigned int second_move[4] = {
            get_neighbor(first_move[i], EAST), 
            get_neighbor(first_move[i], NORTH),
            get_neighbor(first_move[i], WEST), 
            get_neighbor(first_move[i], SOUTH)
        };

        for (int j = 0; j < 4; j++) // second step
        {
            if ((second_move[j] != UINT_MAX) && (second_move[j] != idx) && (world_get(w, second_move[j]) != current_color))
            {
                add_place(&possibles, second_move[j]);
            }
        }
    }
    return possibles;
}

struct set_t list_bishop_moves(struct world_t *w, unsigned int p, enum color_t piece_color)
{
    struct set_t possibles = {};
    struct neighbors_t neighbors = get_neighbors(p);
    enum color_t current_color = piece_color;

    unsigned int i=0;
    while (neighbors.n[i].d != NO_DIR && neighbors.n[i].i != UINT_MAX) // for each non-cardinal direction
    {
        enum dir_t d = neighbors.n[i].d;
        unsigned int newp = neighbors.n[i].i;
        if (!is_cardinal(d))
        {
            while (newp != UINT_MAX) // add every position in this direction until we reach an obstacle
            {
                // Handle ennemy piece
                if (world_get_sort(w, newp) != NO_SORT)
                {
                    if (world_get(w, newp) == current_color)
                        break;
                    else
                    {
                        add_place(&possibles, newp);
                        break;
                    }
                }

                // add the place and continue
                add_place(&possibles, newp);
                newp = get_neighbor(newp, d);
            }
        }
        i++;
    }
    return possibles;
}

struct set_t list_queen_moves(struct world_t *w, unsigned int p, enum color_t piece_color)
{
    struct set_t possibles = set_union(
        list_tower_moves(w, p, piece_color), 
        list_bishop_moves(w, p, piece_color)
    );
    return possibles;
}

struct set_t list_king_moves(struct world_t *w, unsigned int p, enum color_t piece_color)
{
    struct set_t possibles = {};
    struct neighbors_t neighbors = get_neighbors(p);
    int j = 0;
    while(neighbors.n[j].i != UINT_MAX)
    {
        if (world_get(w, neighbors.n[j].i) != piece_color)
        {
            add_place(&possibles, neighbors.n[j].i);
        }
        j++;
    }
    return possibles;
}

struct set_t list_possibles_moves(struct game_t *g, unsigned int p)
{
    enum sort_t s = world_get_sort(g->w, p);
    enum color_t c = world_get(g->w, p);

    world_set(g->w, p, NO_COLOR);
    world_set_sort(g->w, p, NO_SORT);

    // List in a set possibles moves for this sort
    struct set_t possibles;
    switch (s) {
    case PAWN:
        possibles = list_pawn_moves(g->w, p, c, g->board_type);
        break;
    case TOWER:
        possibles = list_tower_moves(g->w, p, c);
        break;
    case ELEPHANT:
        possibles = list_elephant_moves(g->w, p, c);
        break;
    case BISHOP:
        possibles = list_bishop_moves(g->w, p, c);
        break;
    case QUEEN:
        possibles = list_queen_moves(g->w, p, c);
        break;
    case KING:
        possibles = list_king_moves(g->w, p, c);
        break;
    default:
        printf("sort_t inconnue");
        possibles.size = 0;
        break;
    }

    world_set(g->w, p, c);
    world_set_sort(g->w, p, s);

    struct set_t to_remove = {};

    // HANDLE CAPTURES

    // can't capture pieces on their starting position
    if (g->capture_activated)
    {
        for (unsigned int i = 0; i < possibles.size; i++)
        {
            if (is_start_pos(possibles.places[i]) && (world_get_sort(g->w, possibles.places[i]) != NO_SORT))
            {
                add_place(&to_remove, possibles.places[i]);
            }
        }
    }
    // can't capture pieces at all
    else
    {
        for (unsigned int i = 0; i < possibles.size; i++)
        {
            if (world_get_sort(g->w, possibles.places[i]) != NO_SORT)
            {
                add_place(&to_remove, possibles.places[i]);
            }
        }
    }

    for (unsigned int i=0; i<to_remove.size; i++)
    {
        remove_place(&possibles, to_remove.places[i]);
    }

    return possibles;
}

struct set_t list_possibles_moves_from_set(struct game_t *g, struct set_t s, unsigned int origin, struct set_t origins[], int explored[])
{
    struct set_t possibles = {};
    
    enum color_t piece_color = world_get(g->w, origin);
    enum sort_t piece_sort = world_get_sort(g->w, origin);

    // list in a set attainable places from a set
    for (unsigned int i=0; i<s.size; i++) // for each place in the set
    {
        world_set(g->w, origin, NO_COLOR);
        world_set_sort(g->w, origin, NO_SORT);

        unsigned int p = s.places[i];
        struct set_t new_positions = {};
        switch (piece_sort) {
        case PAWN:
            new_positions = list_pawn_moves(g->w, p, piece_color, g->board_type);
            break;
        case TOWER:
            new_positions = list_tower_moves(g->w, p, piece_color);
            break;
        case ELEPHANT:
            new_positions = list_elephant_moves(g->w, p, piece_color);
            break;
        case BISHOP:
            new_positions = list_bishop_moves(g->w, p, piece_color);
            break;
        case QUEEN:
            new_positions = list_queen_moves(g->w, p, piece_color);
            break;
        case KING:
            new_positions = list_king_moves(g->w, p, piece_color);
            break;
        default:
            printf("sort_t inconnue");
            new_positions.size = 0;
            break;
        }

        world_set(g->w, origin, piece_color);
        world_set_sort(g->w, origin, piece_sort);

        struct set_t to_remove = {};

        // Handle captures
        if (g->capture_activated == 1)
        {
            for (unsigned int i = 0; i < new_positions.size; i++)
            {
                if (is_start_pos(new_positions.places[i]) && (world_get_sort(g->w, new_positions.places[i]) != NO_SORT))
                {
                    add_place(&to_remove, new_positions.places[i]);
                }
            }
        }
        else
        {
            for (unsigned int i = 0; i < new_positions.size; i++)
            {
                if (world_get_sort(g->w, new_positions.places[i]) != NO_SORT)
                {
                    add_place(&to_remove, new_positions.places[i]);
                }
            }
        }

        // Do not visit 2 times the same place
        for (unsigned int i=0; i<new_positions.size; i++)
        {
            unsigned int newp = new_positions.places[i];
            if (explored[newp])
                add_place(&to_remove, newp);
        }

        for (unsigned int i=0; i<to_remove.size; i++)
        {
            remove_place(&new_positions, to_remove.places[i]);
        }

        // remember where do we come from
        for (unsigned int i=0; i<new_positions.size; i++)
        {
            unsigned int newp = new_positions.places[i];
            origins[newp] = set_union(origins[newp], origins[p]);
        }
        possibles = set_union(possibles, new_positions);
    }
    return possibles;
}

// returns the positon from start that comes closest to target
static struct set_t nearest_positions(struct set_t start, struct set_t target)
{
    // INITIALIZATION
    int explored[WORLD_SIZE] = {};
    struct set_t to_explore[2];
    struct set_t nearest = {};
    unsigned int curr = 0;
    to_explore[0] = target; // here our initial set is "target" because we are doing a reverted search
    for (unsigned int i=0; i<target.size; i++)
    {
        explored[target.places[i]] = 1;
        if (is_in(start, target.places[i]))
        {
            add_place(&nearest, target.places[i]);
        }
    }

    // SEARCH BODY LOOP
    while (nearest.size == 0) 
    {
        unsigned int next = 1-curr;
        to_explore[next].size = 0;

        // get all positions we will explore next time
        for (unsigned int i=0; i<to_explore[curr].size; i++)
        {
            struct neighbors_t neighbors = get_neighbors(to_explore[curr].places[i]);
            unsigned int j = 0;
            while (neighbors.n[j].d != NO_DIR && neighbors.n[j].i != UINT_MAX)
            {
                unsigned int p = neighbors.n[j].i;
                if (!explored[p])
                {
                    add_place(&to_explore[next], p);
                    explored[p] = 1;
                }
                j++;
            }
        }

        // if we attained some of the positions we are searching, we add them and break the loop
        for (unsigned int i=0; i<to_explore[next].size; i++)
        {
            if (is_in(start, to_explore[next].places[i])) // here our target is "start" because we are doing a reverted breadth-first search
            {
                add_place(&nearest, to_explore[next].places[i]);
            }
        }
        curr = next;
    }
    return nearest;
}

static struct set_t less_moves_positions(struct set_t start, struct set_t target, struct game_t *g, unsigned int origin)
{
    // INITIALIZATION
    struct set_t closest_start[WORLD_SIZE] = {};
    int explored[WORLD_SIZE] = {};
    struct set_t to_explore[2];
    struct set_t nearest = {};
    unsigned int curr = 0;
    to_explore[0] = start;
    for (unsigned int i=0; i<start.size; i++)
    {
        add_place(&closest_start[start.places[i]], start.places[i]);
        explored[start.places[i]] = 1;
        if (is_in(target, start.places[i]))
        {
            add_place(&nearest, start.places[i]);
        }
    }
    
    // SEARCH BODY LOOP
    while (nearest.size == 0) 
    {
        unsigned int next = 1-curr;

        // get all positions we will explore next time
        to_explore[next] = list_possibles_moves_from_set(g, to_explore[curr], origin, closest_start, explored);

        if (to_explore[next].size == 0) return nearest; // end function if we have nothing left to explore

        // if we reach a target position we store it and break the loop
        for (unsigned int i=0; i<to_explore[next].size; i++)
        {
            if (is_in(target, to_explore[next].places[i]))
            {
                nearest = set_union(nearest, closest_start[to_explore[next].places[i]]);
            }
            explored[to_explore[next].places[i]] = 1; 
        }
        curr = next;
    }
    return nearest;
}

unsigned int choose_accurate_piece(struct game_t *g, enum color_t current_player_color)
{
    // create a copy of the player pieces
    struct set_t pieces = g->players_pieces[pid(current_player_color)];

    // while we don't have a satisfying piece we pick one other
    while (pieces.size > 0)
    {
        unsigned int piece = pieces.places[ rand() % pieces.size ];
        if (rand() % 5 == 0 || !is_in(starting_positions[opposite_player_index(pid(current_player_color))], piece))
            if (list_possibles_moves(g, piece).size > 0)
                return piece;
        remove_place(&pieces, piece);
    }
    return UINT_MAX;
}

struct move_t choose_random_move_for_piece(struct game_t *g, unsigned int p)
{
    // List possible moves
    struct set_t possibles = list_possibles_moves(g, p);

    // pick one
    struct move_t m;
    m.from = p;
    if (possibles.size > 0) m.to = possibles.places[ rand() % possibles.size ];
    else m.to = UINT_MAX; // no move is possible
    return m;
}

struct move_t choose_nearest_move(struct game_t *g, unsigned int p)
{
    // Handle exeptions
    struct move_t m;
    m.from = p;
    if (m.from == UINT_MAX)
    {
        m.to = UINT_MAX;
        return m;
    }

    unsigned int current_player = pid(world_get(g->w, p));
    unsigned int opposite_player = opposite_player_index(current_player);

    // list possibles moves
    struct set_t bests = list_possibles_moves(g, p);

    // set the target places
    struct set_t target = {};
    for (unsigned int i=0; i<starting_positions[opposite_player].size; i++)
    {
        unsigned int startp = starting_positions[opposite_player].places[i];
        if (world_get(g->w, startp) != world_get(g->w, p))
        {
            add_place(&target, startp);
        }
    }
    
    // keep the closest ones
    if (bests.size > 0) bests = nearest_positions(bests, target);
    if (bests.size > 0) m.to = bests.places[ rand() % bests.size ];
    else m.to = UINT_MAX;
    return m;
}

struct move_t choose_best_move(struct game_t *g, unsigned int p)
{
    struct move_t m;
    m.from = p;

    // Handle exeptions
    if (m.from == UINT_MAX)
    {
        m.to = UINT_MAX;
        return m;
    }

    // Initialize
    unsigned int current_player = pid(world_get(g->w, p));
    unsigned int opposite_player = opposite_player_index(current_player);
    struct set_t possibles = list_possibles_moves(g, p);
    if (possibles.size == 0)
    {
        m.to = UINT_MAX;
        return m;
    }

    // Set target positions
    struct set_t target = {};
    for (unsigned int i=0; i<starting_positions[opposite_player].size; i++)
    {
        unsigned int startp = starting_positions[opposite_player].places[i];
        if (world_get(g->w, startp) != world_get(g->w, p))
        {
            add_place(&target, startp);
        }
    }

    // we try by looking best positions (taking in consideration how the pieces moves)
    struct set_t bests = {};
    bests = less_moves_positions(possibles, target, g, p);
    if (bests.size > 0)
    {
        m.to = bests.places[ rand() % bests.size ];
        return m;
    }
    
    // if we reach this line, it means the piece can't reach any target position (in the current state of the game)
    // so we try looking the the positions that comes closest to target
    bests = nearest_positions(possibles, target);
    if (bests.size > 0)
    {
        m.to = bests.places[ rand() % bests.size ];
        return m;
    }

    // if we reach this line, that means there is no existing path to our targets (possible with odd relations)
    // so we choose a random possible move
    m.to = possibles.places[ rand() % possibles.size ];
    return m;
}

void move_piece(struct world_t *w, struct set_t *current_player_pieces, struct set_t *opponent_player_pieces, struct set_captured_t *opponent_capt, struct move_t m)
{
    if (m.to != UINT_MAX) 
    {
        enum color_t current_color = world_get(w, m.from);

        // Handle capture
        if (world_get(w, m.to) == next_player(current_color)) capture_piece(w, m.to, opponent_player_pieces, opponent_capt);

        // update the world
        world_set(w, m.to, world_get(w, m.from));
        world_set_sort(w, m.to, world_get_sort(w, m.from));
        world_set(w, m.from, NO_COLOR);
        world_set_sort(w, m.from, NO_SORT);

        // update player pieces
        remove_place(current_player_pieces, m.from);
        add_place(current_player_pieces, m.to);
    }
}

int end_game(struct set_t player_pieces[], char victory_mode)
{
    switch (victory_mode) 
    {
        // simple victory
        case 's':
            for (int p=0; p<MAX_PLAYERS; p++)
                for (unsigned int i=0; i<player_pieces[p].size; i++)
                    if (is_in(starting_positions[opposite_player_index(p)], player_pieces[p].places[i]))
                        return 1;
            break;

        // complex victory
        case 'c':
            for (int p=0; p<MAX_PLAYERS; p++)
                if (equal(player_pieces[p], starting_positions[opposite_player_index(p)]))
                    return 1;
            break;
    }
    return 0;
}

static enum sort_t get_random_sort()
{
    return rand() % (MAX_SORT-1) + 1;
}

void add_piece(struct world_t *w, struct set_t players_pieces[], unsigned int pos, enum color_t color, enum sort_t sort)
{
    world_set(w, pos, color);
    world_set_sort(w, pos, sort);
    add_place(&players_pieces[pid(color)], pos);
}

static void set_classic_start(struct world_t *w, struct set_t players_pieces[], enum color_t c, const enum sort_t sorts[])
{
    // Initialize
    for (unsigned int i=0; i<MAX_PLAYERS; i++)
        players_pieces[i].size = 0;

    int idpiece[MAX_PLAYERS] = {};

    // Place pieces in the right and left columns
    switch (c)
    {
    case BLACK:
        for (int i = 0; i < WORLD_SIZE; i++)
        {
            if (i % WIDTH == 0)
            {
                enum color_t curr_color = BLACK;
                add_piece(w, players_pieces, i, curr_color, sorts[idpiece[pid(curr_color)]]);
                add_place(&starting_positions[pid(curr_color)], i);
                idpiece[pid(curr_color)]++;
            }
            else if (i % WIDTH == WIDTH - 1)
            {
                enum color_t curr_color = WHITE;
                add_piece(w, players_pieces, i, curr_color, sorts[idpiece[pid(curr_color)]]);
                add_place(&starting_positions[pid(curr_color)], i);
                idpiece[pid(curr_color)]++;
            }
        }
        break;
    case WHITE:
        for (int i = 0; i < WORLD_SIZE; i++)
        {
            if (i % WIDTH == 0)
            {
                enum color_t curr_color = WHITE;
                add_piece(w, players_pieces, i, curr_color, sorts[idpiece[pid(curr_color)]]);
                add_place(&starting_positions[pid(curr_color)], i);
                idpiece[pid(curr_color)]++;
            }
            else if (i % WIDTH == WIDTH - 1)
            {
                enum color_t curr_color = BLACK;
                add_piece(w, players_pieces, i, curr_color, sorts[idpiece[pid(curr_color)]]);
                add_place(&starting_positions[pid(curr_color)], i);
                idpiece[pid(curr_color)]++;
            }
        }
        break;
    default:
        break;
    }
}

static void set_triangle_start(struct world_t *w, struct set_t players_pieces[], enum color_t c, const enum sort_t sorts[])
{
    // Initialize
    for (unsigned int i=0; i<MAX_PLAYERS; i++)
        players_pieces[i].size = 0;
    int idpiece[MAX_PLAYERS] = {};

    // calculate the number of layers we use (to have a perfect triangle)
    unsigned int num_layers = 0;
    unsigned int num_pieces = 0;
    while (num_pieces + num_layers + 1 <= MAX_PIECES)
    {
        num_pieces += num_layers + 1;
        num_layers++;
    }

    // place the pieces in the corners (top right and bottom left)
    for (unsigned int i=0; i<num_layers; i++)
    {
        for (unsigned int j=0; j<num_layers-i; j++)
        {
                int pos = place_id(i, j);
                add_piece(w, players_pieces, pos, c, sorts[idpiece[pid(c)]]);
                add_place(&starting_positions[pid(c)], pos);
                idpiece[pid(c)]++;

                pos = place_id(WIDTH-i-1, HEIGHT-j-1); 
                add_piece(w, players_pieces, pos, next_player(c), sorts[idpiece[opposite_player_index(pid(c))]]);
                add_place(&starting_positions[opposite_player_index(pid(c))], pos);
                idpiece[opposite_player_index(pid(c))]++;
        }
    }
}

static void set_square_start(struct world_t *w, struct set_t players_pieces[], enum color_t c, const enum sort_t sorts[])
{
    set_classic_start(w, players_pieces, c, sorts);
}

static void shuffle(enum sort_t sorts[], unsigned int size)
{
    for (unsigned int it=0; it<SHUFFLE_ITERATIONS; it++)
    {
        unsigned int i = rand() % size;
        unsigned int j = rand() % size;
        enum sort_t tmp;
        tmp = sorts[i];
        sorts[i] = sorts[j];
        sorts[j] = tmp;
    }
}

void set_start(struct world_t *w, struct set_t players_pieces[], enum color_t c, int rel)
{
    // pick what sorts we use
    enum sort_t sorts[MAX_PIECES];
    int i=0;
    while (i<MAX_SORT-1 && i<MAX_PIECES) // ensure that every sort is represented (when possible)
    {
        sorts[i] = i+1;
        i++;
    }
    while (i<MAX_PIECES) // fill with random sorts
    {
        sorts[i] = get_random_sort();
        i++;
    }
    shuffle(sorts, MAX_PIECES);

    switch (rel) {
        case 0:
            set_classic_start(w, players_pieces, c, sorts);
            break;
        case 1:
            set_triangle_start(w, players_pieces, c, sorts);
            break;
        case 2:
            set_square_start(w, players_pieces, c, sorts);
            break;
    }
}

void set_start_predefined(struct world_t *w, struct set_t players_pieces[], enum color_t c, int rel, enum sort_t *sorts)
{
    switch (rel) {
        case 0:
            set_classic_start(w, players_pieces, c, sorts);
            break;
        case 1:
            set_triangle_start(w, players_pieces, c, sorts);
            break;
        case 2:
            set_square_start(w, players_pieces, c, sorts);
            break;
    }
}

void free_piece(struct world_t *w, struct set_t *player_pieces, struct set_captured_t *capt, enum color_t c, int print_mode)
{
    if (capt->size > 0) //if it exists a captured piece
    {
        int piece = rand() % capt->size;//choose randomly a piece and give it 50% of chance to be free
        int chance = (rand() % 100) + 1;
        if (chance > 50)
        {
            if (world_get(w, capt->places[piece]) == NO_COLOR)
            {
                if (print_mode == 0) printf(CHAIN" Succesful escape\n");
                world_set(w, capt->places[piece], c);
                world_set_sort(w, capt->places[piece], capt->sorts[piece]);
                add_place(player_pieces, capt->places[piece]);
                capt->places[piece] = capt->places[capt->size - 1];
                capt->sorts[piece] = capt->sorts[capt->size  - 1];
                capt->size--;
            }
        }
        else if (print_mode == 0) printf(CHAIN" Failed escape (No new move)\n");
    }
}

void capture_piece(struct world_t *w, unsigned int idx, struct set_t *player_pieces, struct set_captured_t *capt)
{
    capt->places[capt->size] = idx;
    capt->sorts[capt->size] = world_get_sort(w, idx);
    capt->size++;
    world_set(w, idx, NO_COLOR);
    world_set_sort(w, idx, NO_SORT);
    
    remove_place(player_pieces, capt->places[capt->size - 1]);
}
