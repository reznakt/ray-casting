/**
 * @file conf.h
 * @brief Configuration parameters for the RayCastingSDL program.
 */


#ifndef RAY_CONF_H
#define RAY_CONF_H


#include <SDL2/SDL.h>


/**
 * @brief Width of the window in pixels.
 */
#define SCREEN_WIDTH 1280

/**
 * @brief Height of the window in pixels.
 */
#define SCREEN_HEIGHT 720

/**
 * @brief Flags used for creating the window.
 */
#define SCREEN_FLAGS (SDL_WINDOW_SHOWN)

/**
 * @brief Title of the window.
 */
#define SCREEN_TITLE "RayCastingSDL"

/**
 * @brief Interval between input polling events in milliseconds.
 */
#define POLLINTERVAL 50

/**
 * @brief Length of the text buffer used for rendering text.
 */
#define TEXTBUFLEN 1000

/**
 * @brief Maximum field of view for the camera.
 */
#define FOV_MAX 360

/**
 * @brief Minimum field of view for the camera.
 */
#define FOV_MIN 1

/**
 * @brief Maximum resolution multiplier for the camera.
 */
#define RESMULT_MAX 100

/**
 * @brief Minimum resolution multiplier for the camera.
 */
#define RESMULT_MIN 1

/**
 * @brief Default field of view for the camera.
 */
#define CAMERA_FOV 90

/**
 * @brief Default resolution multiplier for the camera.
 */
#define CAMERA_RESMULT 1

/**
 * @brief Size of the walls in the game world.
 */
#define WALL_SIZE (2 << 16)

// Check if the camera field of view is within the allowed range
#if CAMERA_FOV < FOV_MIN || CAMERA_FOV > FOV_MAX
#error CAMERA_FOV: illegal value
#endif

// Check if the camera resolution multiplier is within the allowed range
#if CAMERA_RESMULT < RESMULT_MIN || CAMERA_RESMULT > RESMULT_MAX
#error CAMERA_RESMULT: illegal value
#endif


#endif /* RAY_CONF_H */
