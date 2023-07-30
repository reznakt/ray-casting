#ifndef RAY_GAME_H
#define RAY_GAME_H


#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>


#include <SDL2/SDL.h>


#include "vector.h"
#include "world.h"
#include "util.h"
#include "conf.h"


/**
 * @brief Structure representing a camera in the game.
 */
struct camera_t {
    struct vec_t pos; /**< Position of the camera. */
    struct vec_t dir; /**< Direction the camera is facing. */
    struct ray_t *rays; /**< Array of rays cast from the camera. */
    struct {
        bool forward, backward, left, right, crouch; /**< Boolean flags indicating which movement keys are pressed. */
    } movement;
    float angle; /**< The angle (in degrees) that the camera is facing. */
    float speed; /**< The current movement speed of the camera. */
    size_t resmult; /**< The resolution multiplier for the camera. */
    size_t fov; /**< The field of view (in degrees) for the camera. */
    size_t nrays; /**< The number of rays cast from the camera. */
};

/**
 * @brief Structure representing the game.
 */
struct game_t {
    SDL_Renderer *renderer; /**< The SDL renderer for the game. */
    SDL_Window *window; /**< The SDL window for the game. */
    SDL_Texture *texture; /**< The texture atlas for the game. */
    struct camera_t *camera; /**< The camera used for rendering the game. */
    struct wobject_t *objects; /**< The objects in the game world. */
    struct vec_t center; /**< The center of the game window. */
    char *textbuf; /**< The buffer used for rendering text to the game window. */
    size_t nobjects; /**< The number of objects in the game world. */
    size_t nthreads; /**< The number of threads to use for parallel computation. */
    uint64_t fps; /**< The current frames per second (FPS) of the game. */
    uint64_t frames; /**< The total number of frames rendered by the game. */
    uint64_t newframes; /**< The number of frames rendered by the game since the last polling event. */
    uint64_t ticks; /**< The total number of ticks elapsed since the start of the game. */
    SDL_Color ceil_color; /**< The color of the ceiling/sky. */
    SDL_Color floor_color; /**< The color of the floor/ground. */
    unused enum {
        RENDER_MODE_FLAT, RENDER_MODE_WIREFRAME, RENDER_MODE_UNTEXTURED, RENDER_MODE_TEXTURED
    } render_mode; /**< The current render mode. */
};

/**
 * @brief Updates the camera angle with the given amount.
 *
 * @param game The game instance to update.
 * @param angle The new amount to update the camera angle with.
 */
void camera_update_angle(struct game_t *game, float angle);

/**
 * @brief Renders the game scene.
 *
 * @param game The game instance to render.
 */
void render(struct game_t *game);

/**
 * @brief Updates the game state.
 *
 * @param game The game instance to update.
 */
void update(struct game_t *game);

/**
 * @brief Initializes the game by allocating memory and setting default values.
 * @param game The game instance to initialize.
 */
struct game_t *game_create(void);


#endif //RAY_GAME_H
