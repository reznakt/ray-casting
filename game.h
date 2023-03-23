#ifndef RAY_GAME_H
#define RAY_GAME_H


#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>


#include <SDL2/SDL.h>


#include "vector.h"


#define game_init(game)                                                                                 \
    do {                                                                                                \
        game.center = (struct vector_t) {(float) SCREEN_WIDTH / 2.0f, (float) SCREEN_HEIGHT / 2.0f};    \
        game.walls = world_walls;                                                                       \
        game.nwalls = WORLD_NWALLS;                                                                     \
        game.render_mode = RENDER_MODE_NORMAL;                                                          \
        struct camera_t camera;                                                                         \
        game.camera = &camera;                                                                          \
        game.camera->fov = CAMERA_FOV;                                                                  \
        game.camera->resmult = CAMERA_RESMULT;                                                          \
        game.camera->nrays = game.camera->fov * game.camera->resmult;                                   \
        game.camera->pos = game.center;                                                                 \
        game.camera->movement.up = false;                                                               \
        game.camera->movement.down = false;                                                             \
        game.camera->movement.left = false;                                                             \
        game.camera->movement.right = false;                                                            \
        camera_update_angle(&game, -90);                                                                \
        struct ray_t rays[FOV_MAX * RESMULT_MAX];                                                       \
        game.camera->rays = rays;                                                                       \
        game.textbuf = (char[TEXTBUFLEN]) {0};                                                          \
} while (0)


/**
 * @brief Structure representing a camera in the game.
 */
struct camera_t {
    struct vector_t pos; /**< Position of the camera. */
    struct vector_t dir; /**< Direction the camera is facing. */
    struct ray_t *rays; /**< Array of rays cast from the camera. */
    struct {
        bool up, down, left, right; /**< Boolean flags indicating which movement keys are pressed. */
    } movement;
    float angle; /**< The angle (in degrees) that the camera is facing. */
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
    struct camera_t *camera; /**< The camera used for rendering the game. */
    const struct line_t *walls; /**< The walls in the game world. */
    struct ivector_t mouse; /**< The current position of the mouse. */
    struct vector_t center; /**< The center of the game window. */
    char *textbuf; /**< The buffer used for rendering text to the game window. */
    size_t nwalls; /**< The number of walls in the game world. */
    uint64_t fps; /**< The current frames per second (FPS) of the game. */
    uint64_t frames; /**< The total number of frames rendered by the game. */
    uint64_t ticks; /**< The total number of ticks elapsed since the start of the game. */
    enum {
        RENDER_MODE_NORMAL, RENDER_MODE_FLAT
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
 * @brief Handles a SDL event.
 *
 * @param game The game instance to handle the event for.
 * @param event The SDL event to handle.
 * @return If the event signals to exit the game, return true. Otherwise, return false.
 */
bool on_event(struct game_t *game, const SDL_Event *event);


#endif //RAY_GAME_H
