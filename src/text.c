#include <SDL2/SDL.h>


#include "../fonts/font8x8_basic.h"


#include "text.h"
#include "vector.h"


/**
 * The number of bytes required to represent a single character.
 */
static const size_t FONT_BYTES = 8;


void render_putchar(SDL_Renderer *const restrict renderer, const struct vec_t *const restrict pos, int chr) {
    for (size_t i = 0; i < FONT_BYTES; i++) {
        for (size_t j = 0; j < FONT_BYTES; j++) {
            if (font8x8_basic[chr][i] & 1 << j) {
                SDL_RenderDrawPointF(renderer, pos->x + (float) j, pos->y + (float) i);
            }
        }
    }
}

void render_puts(SDL_Renderer *const restrict renderer,
                 const struct vec_t *const restrict pos,
                 const char *const restrict str) {
    for (size_t i = 0; i < strlen(str); i++) {
        const struct vec_t char_pos = {pos->x + 10.0F * (float) i, pos->y};
        render_putchar(renderer, &char_pos, str[i]);
    }
}

void render_printf(SDL_Renderer *const restrict renderer,
                   const struct vec_t *const restrict pos,
                   const char *const restrict fmt, ...) {
    char buf[TEXTBUFLEN] = {0};

    va_list args;
    va_start(args, fmt);

    vsnprintf(buf, TEXTBUFLEN, fmt, args);

    va_end(args);

    render_puts(renderer, pos, buf);
}
