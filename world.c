#include "vector.h"
#include "conf.h"


#include "world.h"


const Line world_walls[WORLD_NWALLS] = {
        {
                {0,   0},
                {SCREEN_WIDTH - 1, 0}
        },
        {
                {SCREEN_WIDTH - 1, 0},
                {SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1}
        },
        {
                {SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1},
                {0,                SCREEN_HEIGHT - 1}
        },
        {
                {0,   0},
                {0,                SCREEN_HEIGHT - 1}
        },
        {
                {100, 100},
                {200, 100}
        },
        {
                {100, 100},
                {100, 200}
        },
        {
                {200, 100},
                {200, 200}
        },
        {
                {200, 200},
                {100, 200}
        }
};

