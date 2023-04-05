#include <SDL2/SDL.h>


#include "vector.h"
#include "conf.h"
#include "util.h"


#include "world.h"


const struct wall_t world_walls[WORLD_NWALLS] = {
        {
                {0,   0},
                {SCREEN_WIDTH - 1, 0},
                RED,
                WALL_TYPE_SOLID
        },
        {
                {SCREEN_WIDTH - 1, 0},
                {SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1},
                GREEN,
                WALL_TYPE_SOLID
        },
        {
                {SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1},
                {0,                SCREEN_HEIGHT - 1},
                BLUE,
                WALL_TYPE_SOLID
        },
        {
                {0,   0},
                {0,                SCREEN_HEIGHT - 1},
                CYAN,
                WALL_TYPE_NONSOLID
        },
        {
                {100, 100},
                {200,  100},
                BLUE,
                WALL_TYPE_NONSOLID
        },
        {
                {100, 100},
                {100,  200},
                BLUE,
                WALL_TYPE_NONSOLID
        },
        {
                {200, 100},
                {200,  200},
                MAGENTA,
                WALL_TYPE_NONSOLID
        },
        {
                {200, 200},
                {100,  200},
                BLUE,
                WALL_TYPE_NONSOLID
        },
        {
                {500, 1000},
                {1000, 800},
                YELLOW,
                WALL_TYPE_MIRROR
        }
};

