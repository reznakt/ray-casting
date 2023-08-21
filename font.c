#include <SDL2/SDL.h>


#include "fonts/font8x8_basic.h"


#include "font.h"


void render_char(SDL_Renderer *const renderer, const int x, const int y, const int chr) {
    if (chr < 0 || chr >= FONT_CHARS) {
        return;
    }

    for (int i = 0; i < FONT_BYTES; i++) {
        for (int j = 0; j < FONT_BYTES; j++) {
            if (font8x8_basic[chr][i] & 1 << j) {
                SDL_RenderDrawPoint(renderer, x + j, y + i);
            }
        }
    }
}

void render_string(SDL_Renderer *const restrict renderer, const int x, const int y, const char *const restrict str) {
    for (size_t i = 0; i < strlen(str); i++) {
        render_char(renderer, x + 10 * (int) i, y, str[i]);
    }
}
