#include <SDL2/SDL.h>


#include "vector.h"
#include "conf.h"
#include "util.h"
#include "logger.h"


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


bool check_walls(void) {
    for (size_t i = 0; i < WORLD_NWALLS; i++) {
        if (world_walls[i].a.x < 0 || world_walls[i].a.x > SCREEN_WIDTH ||
            world_walls[i].a.y < 0 || world_walls[i].a.y > SCREEN_HEIGHT ||
            world_walls[i].b.x < 0 || world_walls[i].b.x > SCREEN_WIDTH ||
            world_walls[i].b.y < 0 || world_walls[i].b.y > SCREEN_HEIGHT) {
            logger_printf(LOG_LEVEL_ERROR, "wall %zu is out of bounds\n", i);
            return false;
        }

        if (world_walls[i].type & WALL_TYPE_SOLID && world_walls[i].type & WALL_TYPE_NONSOLID) {
            logger_printf(LOG_LEVEL_ERROR, "wall %zu is both solid and nonsolid\n", i);
            return false;
        }
    }

    return true;
}
