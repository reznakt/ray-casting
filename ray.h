#ifndef RAY_RAY_H
#define RAY_RAY_H


#include <stdbool.h>


#include "vector.h"


struct ray_t {
    struct vector_t pos;
    struct vector_t dir;
    struct vector_t intersection;
    float intersection_dist;
    bool has_intersection;
};


struct vector_t *ray_intersection(const struct ray_t *ray, const struct line_t *wall, struct vector_t *dst);


#endif // RAY_RAY_H

