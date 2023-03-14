#ifndef RAY_CONF_H
#define RAY_CONF_H


#include <SDL2/SDL.h>


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCREEN_FLAGS (SDL_WINDOW_SHOWN)
#define SCREEN_TITLE "RayCastingSDL"

#define POLLINTERVAL 50
#define TEXTBUFLEN 1000

#define FOV_MAX 360
#define FOV_MIN 1

#define RESMULT_MAX 100
#define RESMULT_MIN 1

#define CAMERA_FOV 90
#define CAMERA_RESMULT 1
#define CAMERA_NRAYS ((CAMERA_FOV) * (CAMERA_RESMULT))

#define WALL_SIZE (2 << 16)


#if CAMERA_FOV < FOV_MIN || CAMERA_FOV > FOV_MAX
#error CAMERA_FOV: illegal value
#endif

#if CAMERA_RESMULT < RESMULT_MIN || CAMERA_RESMULT > RESMULT_MAX
#error CAMERA_RESMULT: illegal value
#endif

#endif // RAY_CONF_H

