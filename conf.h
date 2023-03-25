/**
 * @file conf.h
 * @brief Configuration parameters for the RayCastingSDL program.
 */


#ifndef RAY_CONF_H
#define RAY_CONF_H


#include <SDL2/SDL.h>


#include "util.h"


/**
 * @brief Width of the window in pixels.
 */
#define SCREEN_WIDTH 1920

/**
 * @brief Height of the window in pixels.
 */
#define SCREEN_HEIGHT 1080

/**
 * @brief Flags used for creating the window.
 */
#define SCREEN_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN)

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
#define CAMERA_RESMULT 10

/**
 * @brief Rotation speed multiplier for the camera.
 */
#define CAMERA_ROTATION_SPEED 0.5

/**
 * @brief Movement speed multiplier for the camera.
 */
#define CAMERA_MOVEMENT_SPEED 400

/**
 * @brief Default orientation of the camera.
 */
#define CAMERA_HEADING NORTH

/**
 * @brief Size of the walls in the game world.
 */
#define WALL_SIZE 1000

/**
 * @brief The default color of the ceiling/sky.
 */
#define CEIL_COLOR color_opaque(10, 10, 20)

/**
 * @brief The default color of the floor/ground.
 */
#define FLOOR_COLOR color_opaque(0, 0, 0)

#define KEY_FORWARD SDLK_UP
#define KEY_BACKWARD SDLK_DOWN
#define KEY_LEFT SDLK_LEFT
#define KEY_RIGHT SDLK_RIGHT
#define KEY_RESET SDLK_r
#define KEY_SWITCH_MODE SDLK_TAB
#define KEY_QUIT_1 SDLK_q
#define KEY_QUIT_2 SDLK_ESCAPE
#define KEY_FOV_INC SDLK_KP_PLUS
#define KEY_FOV_DEC SDLK_KP_MINUS
#define KEY_RESMULT_INC SDLK_INSERT
#define KEY_RESMULT_DEC SDLK_DELETE

// Check if the camera field of view is within the allowed range
#if CAMERA_FOV < FOV_MIN || CAMERA_FOV > FOV_MAX
#error CAMERA_FOV: illegal value
#endif

// Check if the camera resolution multiplier is within the allowed range
#if CAMERA_RESMULT < RESMULT_MIN || CAMERA_RESMULT > RESMULT_MAX
#error CAMERA_RESMULT: illegal value
#endif


#endif /* RAY_CONF_H */
