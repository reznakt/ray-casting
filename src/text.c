#include <SDL2/SDL.h>


#include "../fonts/font8x8_basic.h"


#include "text.h"
#include "vector.h"


/**
 * The number of bytes required to represent a single character.
 */
static const size_t FONT_BYTES = 8;

/**
 * The number of pixels between two characters on the x-axis.
 */
static const size_t CHAR_HORIZONTAL_SPACING = 10;

/**
 * The number of pixels between two characters on the y-axis.
 */
static const size_t CHAR_VERTICAL_SPACING = 15;

/**
 * The size of the tab character in spaces.
 */
static const size_t TAB_WIDTH = 4;


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
    size_t x_off = 0;
    size_t y_off = 0;

    for (size_t i = 0; i < strlen(str); i++) {
        const char ch = str[i];

        switch (ch) {
            case '\t': // horizontal tab
                x_off += CHAR_HORIZONTAL_SPACING * TAB_WIDTH;
                continue;
            case '\n': // line feed (newline)
                x_off = 0;
                y_off += CHAR_VERTICAL_SPACING;
                continue;
            case '\r': // carriage return
                x_off = 0;
                continue;
            case '\b': // backspace
                x_off -= CHAR_HORIZONTAL_SPACING;
                continue;
            default:
                render_putchar(renderer, &(struct vec_t) {pos->x + (float) x_off, pos->y + (float) y_off}, ch);
                x_off += CHAR_HORIZONTAL_SPACING;
                break;
        }
    }
}

__attribute__((__format__(__printf__, 3, 4)))
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
