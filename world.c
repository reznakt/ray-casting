#include <SDL2/SDL.h>


#include "vector.h"
#include "conf.h"


#include "world.h"


const struct wall_t world_walls[WORLD_NWALLS] = {
        {
                {0,   0},
                {SCREEN_WIDTH - 1, 0},
                {255, 0,   0,   SDL_ALPHA_OPAQUE}
        },
        {
                {SCREEN_WIDTH - 1, 0},
                {SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1},
                {0,   255, 0,   SDL_ALPHA_OPAQUE}
        },
        {
                {SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1},
                {0,                SCREEN_HEIGHT - 1},
                {0,   0,   255, SDL_ALPHA_OPAQUE}
        },
        {
                {0,   0},
                {0,                SCREEN_HEIGHT - 1},
                {255, 255, 255, SDL_ALPHA_OPAQUE}
        },
        {
                {100, 100},
                {200, 100},
                {0,   0,   255, SDL_ALPHA_OPAQUE}
        },
        {
                {100, 100},
                {100, 200},
                {0,   0,   255, SDL_ALPHA_OPAQUE}
        },
        {
                {200, 100},
                {200, 200},
                {0,   0,   255, SDL_ALPHA_OPAQUE}
        },
        {
                {200, 200},
                {100, 200},
                {0,   0,   255, SDL_ALPHA_OPAQUE}
        }
};

