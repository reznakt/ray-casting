#include <SDL2/SDL.h>


#include "vector.h"
#include "conf.h"
#include "util.h"


#include "world.h"


const struct wall_t world_walls[WORLD_NWALLS] = {
        {
                {0,   0},
                {SCREEN_WIDTH - 1, 0},
                RED
        },
        {
                {SCREEN_WIDTH - 1, 0},
                {SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1},
                GREEN
        },
        {
                {SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1},
                {0,                SCREEN_HEIGHT - 1},
                BLUE
        },
        {
                {0,   0},
                {0,                SCREEN_HEIGHT - 1},
                WHITE
        },
        {
                {100, 100},
                {200, 100},
                BLUE
        },
        {
                {100, 100},
                {100, 200},
                BLUE
        },
        {
                {200, 100},
                {200, 200},
                BLUE
        },
        {
                {200, 200},
                {100, 200},
                BLUE
        }
};

