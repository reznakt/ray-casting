#ifndef RAY_WORLD_H
#define RAY_WORLD_H


#include <stdbool.h>


#include "vector.h"


#define WORLD_NWALLS 100


#define WALL_TYPE_SOLID (1 << 0)
#define WALL_TYPE_NONSOLID (1 << 1)
#define WALL_TYPE_MIRROR (1 << 2)
#define WALL_TYPE_PORTAL (1 << 3)


#define WALLL_TYPE_DEFAULT WALL_TYPE_SOLID


struct wall_t {
    struct vector_t a;
    struct vector_t b;
    SDL_Color color;
    unsigned int type;
};


extern const struct wall_t world_walls[WORLD_NWALLS];


bool check_walls(void);


#endif // RAY_WORLD_H

