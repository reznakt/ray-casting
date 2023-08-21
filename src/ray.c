#include <stdlib.h>


#include "math.h"


#include "ray.h"


struct vec_t *ray_intersection(const struct ray_t *const restrict ray,
                               const struct wall_t *const restrict wall,
                               struct vec_t *const restrict dst) {
    const float x1 = wall->a.x;
    const float y1 = wall->a.y;
    const float x2 = wall->b.x;
    const float y2 = wall->b.y;

    const float x3 = ray->pos.x;
    const float y3 = ray->pos.y;
    const float x4 = ray->pos.x + ray->dir.x;
    const float y4 = ray->pos.y + ray->dir.y;

    const float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    if (isclose(den, 0)) {
        return NULL;
    }

    const float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    const float u = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / den;

    if (t > 0 && t < 1 && u > 0) {
        dst->x = x3 + u * (x4 - x3);
        dst->y = y3 + u * (y4 - y3);
        return dst;
    }

    return NULL;
}
