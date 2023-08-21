#ifndef RAY_WORLD_H
#define RAY_WORLD_H


#include <stdbool.h>


#include "vector.h"


#define WORLD_NOBJECTS_MAX 100


#define WALL_TYPE_SOLID (1 << 0)
#define WALL_TYPE_NONSOLID (1 << 1)
#define WALL_TYPE_MIRROR (1 << 2)
#define WALL_TYPE_PORTAL (1 << 3)


#define WALLL_TYPE_DEFAULT WALL_TYPE_SOLID


struct wall_t {
    struct vec_t a;
    struct vec_t b;
    SDL_Color color;
    unsigned int type;
};

/**
 * Represents an object in the game world.
 */
struct wobject_t {
    enum unused {
        WALL
    } type; /**< type of the object. */
    union {
        struct wall_t wall;
    } data; /**< data of the object. Depends on the type. */
};


/**
 * @brief Parses the world specification.
 * @param stream the stream to read the world specification from.
 * @param objects pointer to an array of wobject_t structs to store the parsed objects, or NULL, if the objects should not be stored.
 * @param nobjects pointer to a size_t variable to store the number of parsed objects, or NULL, if the number of objects should not be stored.
 * @return 0 on success, -1 on error (objects and nobjects are not modified in this case).
 * @see parse_record
 */
int load_world(const char *path, struct wobject_t *objects, size_t *nobjects);


#endif // RAY_WORLD_H

