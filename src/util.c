#include "math.h"


#include "util.h"


SDL_Color *change_brightness(SDL_Color *const color, const float alpha) {
    color->r = (uint8_t) constrain(alpha * (float) color->r, 0.0F, 255.0F);
    color->g = (uint8_t) constrain(alpha * (float) color->g, 0.0F, 255.0F);
    color->b = (uint8_t) constrain(alpha * (float) color->b, 0.0F, 255.0F);

    return color;
}

bool is_whitespace(const char *const buf) {
    if (buf == NULL) {
        return false;
    }

    for (const char *p = buf; *p; p++) {
        if (!isspace(*p)) {
            return false;
        }
    }

    return true;
}

bool is_decimal(const char *const buf) {
    if (buf == NULL || *buf == '\0') {
        return false;
    }

    for (const char *p = buf; *p; p++) {
        if (!isdigit(*p)) {
            return false;
        }
    }

    return true;
}

int hex_to_dec(char hexnum) {
    hexnum = (char) tolower(hexnum);

    if (hexnum >= '0' && hexnum <= '9') {
        return hexnum - '0';
    }

    if (hexnum >= 'a' && hexnum <= 'f') {
        return hexnum - 'a' + 10;
    }

    return -1;
}

uint32_t color_to_int(const SDL_Color *const color) {
    return (uint32_t) color->r << 24 | (uint32_t) color->g << 16 | (uint32_t) color->b << 8 | (uint32_t) color->a;
}
