#ifndef RAY_CONF_H
#define RAY_CONF_H


#include <SDL2/SDL.h>

#include "util.h"


/**
 * @brief Path to a file containing the world specification.
 */
#define WORLD_SPEC_FILE "assets/world.txt"

/**
 * @brief Path to a file containing the texture atlas.
 */
#define TEXTURE_ATLAS_FILE "assets/textures.png"

/**
 * @brief The size of one texture in the texture atlas.
 */
#define TEXTURE_SIZE 64

/**
 * @brief The number of textures in one row of the texture atlas.
 */
#define TEXTURE_ATLAS_WIDTH 6

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
#define SCREEN_TITLE "ray-casting"

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
#define CAMERA_RESMULT (SCREEN_WIDTH / CAMERA_FOV + 1)

/**
 * @brief Rotation speed multiplier for the camera.
 */
#define CAMERA_ROTATION_SPEED 0.5F

/**
 * @brief Movement speed multiplier for the camera.
 */
#define CAMERA_MOVEMENT_SPEED 400

/**
 * @brief Movement speed multiplier for the camera while sprinting.
 */
#define CAMERA_SPRINT_MOVEMENT_SPEED (2 * CAMERA_MOVEMENT_SPEED)

/**
 * @brief Movement speed multiplier for the camera while crouching.
 */
#define CAMERA_CROUCH_MOVEMENT_SPEED (CAMERA_MOVEMENT_SPEED / 2)

/**
 * @brief The difference in height between standing and crouching.
 */
#define CAMERA_CROUCH_HEIGHT_DELTA (SCREEN_HEIGHT / 10)

/**
 * @brief Default orientation of the camera.
 */
#define CAMERA_HEADING 270 // north/up

/**
 * @brief Default multiplier for the light intensity emitted by the camera/player.
 */
#define CAMERA_LIGHTMULT 1.0F

/**
 * @brief Size of the walls in the game world.
 */
#define WALL_SIZE 1000

/**
 * @brief The default color of the ceiling/sky.
 */
#define CEIL_COLOR rgb(10, 10, 20)

/**
 * @brief The default color of the floor/ground.
 */
#define FLOOR_COLOR rgb(0, 0, 0)

/**
 * @brief If profiling is enabled, specifies the number of ticks to run
 * the game for before exiting and dumping profiling information.
 */
#define PROFILE_TICKS 10000

#define KEY_FORWARD SDLK_UP
#define KEY_BACKWARD SDLK_DOWN
#define KEY_LEFT SDLK_LEFT
#define KEY_RIGHT SDLK_RIGHT
#define KEY_CROUCH SDLK_RCTRL
#define KEY_SPRINT SDLK_RSHIFT
#define KEY_RESET SDLK_r
#define KEY_PAUSE SDLK_ESCAPE
#define KEY_FOV_INC SDLK_KP_PLUS
#define KEY_FOV_DEC SDLK_KP_MINUS
#define KEY_RESMULT_INC SDLK_INSERT
#define KEY_RESMULT_DEC SDLK_DELETE
#define KEY_VIEW_1 SDLK_F1
#define KEY_VIEW_2 SDLK_F2
#define KEY_VIEW_3 SDLK_F3
#define KEY_VIEW_4 SDLK_F4
#define KEY_LIGHT_INC SDLK_HOME
#define KEY_LIGHT_DEC SDLK_END


#include "confvalidate.h"

#endif /* RAY_CONF_H */
