#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "logger.h"
#include "vector.h"

#include "text.h"


size_t text_width(const char *const text) {
    const size_t len = strlen(text);
    return len * CHAR_WIDTH + (len - 1) * CHAR_HORIZONTAL_SPACING;
}

void render_putchar(SDL_Renderer *const renderer, const struct vec_t pos, const char chr) {
    uint8_t r, g, b, a;

    if (SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a) != 0) {
        logger_printf(LOG_LEVEL_ERROR, "SDL_GetRenderDrawColor: %s", SDL_GetError());
        return;
    }

    characterRGBA(renderer, (int16_t) pos.x, (int16_t) pos.y, chr, r, g, b, a);
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
                break;
            case '\n': // line feed (newline)
                x_off = 0;
                y_off += CHAR_VERTICAL_SPACING + CHAR_HEIGHT;
                break;
            case '\r': // carriage return
                x_off = 0;
                break;
            case '\b': // backspace
                x_off -= CHAR_HORIZONTAL_SPACING + CHAR_WIDTH;
                break;
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
