#ifndef RAY_RAY_H
#define RAY_RAY_H


#include <stdbool.h>


#include "vector.h"


typedef struct {
    Vector pos;
    Vector dir;
    Vector intersection;
    float intersection_dist;
    bool has_intersection;
} Ray;


Vector *ray_intersection(const Ray *ray, const Line *wall, Vector *dst);


#endif // RAY_RAY_H

