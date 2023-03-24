#ifndef RAY_WORLD_H
#define RAY_WORLD_H


#include "vector.h"


#define WORLD_NWALLS 100


struct wall_t {
    struct vector_t a;
    struct vector_t b;
    SDL_Color color;
};


extern const struct wall_t world_walls[WORLD_NWALLS];


#endif // RAY_WORLD_H

