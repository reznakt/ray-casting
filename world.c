#include <SDL2/SDL.h>


#include "vector.h"
#include "conf.h"
#include "util.h"


#include "world.h"


const struct wall_t world_walls[WORLD_NWALLS] = {
        {
                {0,   0},
                {SCREEN_WIDTH - 1, 0},
                COLOR_RED,
                WALL_TYPE_SOLID
        },
        {
                {SCREEN_WIDTH - 1, 0},
                {SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1},
                COLOR_GREEN,
                WALL_TYPE_SOLID
        },
        {
                {SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1},
                {0,                SCREEN_HEIGHT - 1},
                COLOR_BLUE,
                WALL_TYPE_SOLID
        },
        {
                {0,   0},
                {0,                SCREEN_HEIGHT - 1},
                COLOR_CYAN,
                WALL_TYPE_NONSOLID
        },
        {
                {100, 100},
                {200,  100},
                COLOR_BLUE,
                WALL_TYPE_NONSOLID
        },
        {
                {100, 100},
                {100,  200},
                COLOR_BLUE,
                WALL_TYPE_NONSOLID
        },
        {
                {200, 100},
                {200,  200},
                COLOR_MAGENTA,
                WALL_TYPE_NONSOLID
        },
        {
                {200, 200},
                {100,  200},
                COLOR_BLUE,
                WALL_TYPE_NONSOLID
        },
        {
                {500, 1000},
                {1000, 800},
                COLOR_YELLOW,
                WALL_TYPE_NONSOLID
        }
};

