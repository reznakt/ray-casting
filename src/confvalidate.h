#ifndef RAY_CONFVALIDATE_H
#define RAY_CONFVALIDATE_H

#include "stdint.h"

#define static_assert_inner1(cond, line) typedef char __static_assert_at_line_##line##__[2 * (!!(cond)) - 1]
#define static_assert_inner2(cond, line) static_assert_inner1(cond, line)

#define STATIC_ASSERT(cond) static_assert_inner2(cond, __LINE__)


#if THREADS < 1
#error "THREADS must be positive"
#endif

#if TEXTURE_SIZE < 1
#error "TEXTURE_SIZE must be positive"
#endif

#if TEXTURE_ATLAS_WIDTH < 1
#error "TEXTURE_ATLAS_WIDTH must be positive"
#endif

#if SCREEN_WIDTH < 1
#error "SCREEN_WIDTH must be positive"
#endif

#if SCREEN_HEIGHT < 1
#error "SCREEN_HEIGHT must be positive"
#endif

#if POLLINTERVAL < 1
#error "POLLINTERVAL must be positive"
#endif

#if TEXTBUFLEN < 1
#error "TEXTBUFLEN must be positive"
#endif

#if FOV_MAX < 1
#error "FOV_MAX must be positive"
#endif

#if FOV_MIN < 1
#error "FOV_MIN must be positive"
#endif

#if FOV_MIN > FOV_MAX
#error "FOV_MIN must be less than or equal to FOV_MAX"
#endif

#if RESMULT_MAX < 1
#error "RESMULT_MAX must be positive"
#endif

#if RESMULT_MIN < 1
#error "RESMULT_MIN must be positive"
#endif

#if RESMULT_MIN > RESMULT_MAX
#error "RESMULT_MIN must be less than or equal to RESMULT_MAX"
#endif

#if (CAMERA_FOV < FOV_MIN) || (CAMERA_FOV > FOV_MAX)
#error "CAMERA_FOV must be between FOV_MIN and FOV_MAX"
#endif

#if (CAMERA_RESMULT < RESMULT_MIN) || (CAMERA_RESMULT > RESMULT_MAX)
#error "CAMERA_RESMULT must be between RESMULT_MIN and RESMULT_MAX"
#endif

#if (CAMERA_HEADING < 0) || (CAMERA_HEADING >= 360)
#error "CAMERA_HEADING must be between 0 and 359"
#endif

#if WALL_SIZE < 1
#error "WALL_SIZE must be positive"
#endif


STATIC_ASSERT((intmax_t) CAMERA_ROTATION_SPEED >= 0); // CAMERA_ROTATION_SPEED must be non-negative
STATIC_ASSERT((intmax_t) CAMERA_MOVEMENT_SPEED >= 0); // CAMERA_MOVEMENT_SPEED must be non-negative
STATIC_ASSERT((intmax_t) CAMERA_SPRINT_MOVEMENT_SPEED >= 0); // CAMERA_SPRINT_MOVEMENT_SPEED must be non-negative
STATIC_ASSERT((intmax_t) CAMERA_CROUCH_MOVEMENT_SPEED >= 0); // CAMERA_CROUCH_MOVEMENT_SPEED must be non-negative
STATIC_ASSERT((intmax_t) CAMERA_CROUCH_HEIGHT_DELTA >= 0); // CAMERA_CROUCH_HEIGHT_DELTA must be non-negative
STATIC_ASSERT((intmax_t) CAMERA_LIGHTMULT >= 1); // CAMERA_LIGHTMULT must be positive


#undef STATIC_ASSERT
#undef static_assert_inner2
#undef static_assert_inner1

#endif //RAY_CONFVALIDATE_H
