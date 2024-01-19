#include <SDL2/SDL.h>

#include "../fonts/font8x8_basic.h"
#include "vector.h"

#include "text.h"


size_t text_width(const char *const text) {
    const size_t len = strlen(text);
    return len * CHAR_WIDTH + (len - 1) * CHAR_HORIZONTAL_SPACING;
}

void render_putchar(SDL_Renderer *const renderer, const struct vec_t pos, int chr) {
    for (size_t i = 0; i < CHAR_HEIGHT; i++) {
        for (size_t j = 0; j < CHAR_WIDTH; j++) {
            if (font8x8_basic[chr][i] & 1 << j) {
                SDL_RenderDrawPointF(renderer, pos.x + (float) j, pos.y + (float) i);
            }
        }
    }
}

void render_puts(SDL_Renderer *const restrict renderer,
                 const struct vec_t pos,
                 const char *const restrict str) {
    size_t x_off = 0;
    size_t y_off = 0;

    for (size_t i = 0; i < strlen(str); i++) {
        const char ch = str[i];

        switch (ch) {
            case '\t': // horizontal tab
                x_off += (CHAR_HORIZONTAL_SPACING + CHAR_WIDTH) * TAB_WIDTH;
                continue;
            case '\n': // line feed (newline)
                x_off = 0;
                y_off += CHAR_VERTICAL_SPACING + CHAR_HEIGHT;
                continue;
            case '\r': // carriage return
                x_off = 0;
                continue;
            case '\b': // backspace
                x_off -= CHAR_HORIZONTAL_SPACING + CHAR_WIDTH;
                continue;
            default:
                render_putchar(renderer, (struct vec_t) {pos.x + (float) x_off, pos.y + (float) y_off}, ch);
                x_off += CHAR_HORIZONTAL_SPACING + CHAR_WIDTH;
                break;
        }
    }
}

void render_printf(SDL_Renderer *const restrict renderer,
                   const struct vec_t pos,
                   const char *const restrict fmt, ...) {
    char buf[TEXTBUFLEN] = {0};

    va_list args;
    va_start(args, fmt);

    vsnprintf(buf, TEXTBUFLEN, fmt, args);

    va_end(args);

    render_puts(renderer, pos, buf);
}
