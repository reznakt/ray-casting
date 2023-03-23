#include "math.h"


#include "util.h"


SDL_Color *change_brightness(SDL_Color *const color, const float alpha) {
    color->r = (uint8_t) constrain(alpha * (float) color->r, 0.0f, 255.0f);
    color->g = (uint8_t) constrain(alpha * (float) color->g, 0.0f, 255.0f);
    color->b = (uint8_t) constrain(alpha * (float) color->b, 0.0f, 255.0f);

    return color;
}
