/**
 * @file ray.h
 * @brief Contains structures and functions for working with rays in 2D space.
 *
 * This header file contains definitions for the `ray_t` and `line_t` structures,
 * as well as functions for performing common operations with them, such as finding
 * the intersection point between a ray and a line.
 */


#ifndef RAY_RAY_H
#define RAY_RAY_H


#include <stdbool.h>


#include "ray.h"
#include "vector.h"
#include "world.h"


/**
 * @struct intersection_t
 * @bried A struct representing an intersection between a ray and a wall.
 *
 * The fields `wall` and `ray` are NULL if there is no intersection.
 */
struct intersection_t {
    struct vec_t pos;
    float dist;
    const struct wall_t *wall;
};

/**
 * @struct ray_t
 * @brief A struct representing a ray in 2D space.
 *
 * The `ray_t` struct represents a ray in 2D space. It contains a `pos` field representing the
 * origin of the ray, a `dir` field representing the direction of the ray, and a `has_intersection`
 * flag indicating whether the ray intersects with an object in the scene.
 *
 * If the ray intersects with an object, the `intersection` field will contain the coordinates of the
 * intersection point and the `intersection_dist` field will contain the distance between the origin of
 * the ray and the intersection point. If the ray does not intersect with any object in the scene, these
 * fields will be undefined.
 */
struct ray_t {
    struct vec_t pos;
    struct vec_t dir;
    struct intersection_t intersection;
};


/**
 * Calculates the intersection point between a ray and a line segment, if there is one.
 *
 * @param ray The ray to test for intersection.
 * @param wall The line segment to test for intersection with the ray.
 * @param dst A pointer to a vector where the resulting intersection point will be stored.
 *
 * @return A pointer to the resulting intersection point, or NULL if there is no intersection.
 */
struct vec_t *ray_intersection(const struct ray_t *ray, const struct wall_t *wall, struct vec_t *dst);


#endif // RAY_RAY_H
