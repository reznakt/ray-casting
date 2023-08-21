#include "math.h"


#include "util.h"


SDL_Color *change_brightness(SDL_Color *const color, const float alpha) {
    color->r = (uint8_t) constrain(alpha * (float) color->r, 0.0F, 255.0F);
    color->g = (uint8_t) constrain(alpha * (float) color->g, 0.0F, 255.0F);
    color->b = (uint8_t) constrain(alpha * (float) color->b, 0.0F, 255.0F);

    return color;
}


char *color_to_str(const SDL_Color color, char *const dst) {
    if (dst == NULL) {
        return NULL;
    }

    char buf[] = {
            '#',
            (char) dec_to_hex(color.r / 16),
            (char) dec_to_hex(color.r % 16),
            (char) dec_to_hex(color.g / 16),
            (char) dec_to_hex(color.g % 16),
            (char) dec_to_hex(color.b / 16),
            (char) dec_to_hex(color.b % 16),
            '\0'
    };

    return memcpy(dst, buf, sizeof buf);
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
    if (buf == NULL) {
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

int dec_to_hex(const int decnum) {
    if (decnum < 0 || decnum > 16) {
        return -1;
    }

    return decnum < 10 ? '0' + decnum : 'a' + decnum - 10;
}
