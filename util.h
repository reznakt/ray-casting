#ifndef RAY_UTIL_H
#define RAY_UTIL_H


#include "font.h"
#include "conf.h"


#define unused __attribute__((unused))
#define private static


#define EAST 0
#define SOUTH 90
#define WEST 180
#define NORTH 270

#define rgba(r, g, b, a) (SDL_Color) {(r), (g), (b), (a)}
#define rgb(r, g, b) rgba((r), (g), (b), 255)

#define COLOR_RED rgb(255, 0, 0)
#define COLOR_GREEN rgb(0, 255, 0)
#define COLOR_BLUE rgb(0, 0, 255)
#define COLOR_WHITE rgb(255, 255, 255)
#define COLOR_BLACK rgb(0, 0, 0)
#define COLOR_YELLOW rgb(255, 255, 0)
#define COLOR_CYAN rgb(0, 255, 255)
#define COLOR_MAGENTA rgb(255, 0, 255)


/**
 * Allocate `n` new objects of type `type` on the stack.
 */
#define stack_alloc(type, n) ((type [n]) {0})

/**
 * @brief Macro for printing SDL errors.
 *
 * This macro prints the given error message and the SDL error message to the standard error stream.
 *
 * @param msg The error message to print.
 */
#define sdl_error(msg) \
    fprintf(stderr, msg ": %s\n", SDL_GetError())

/**
 * @brief Macro for rendering formatted text to a game window.
 *
 * This macro renders the formatted text using the specified game renderer and position.
 *
 * @param game The game object.
 * @param x The x-coordinate of the text position.
 * @param y The y-coordinate of the text position.
 * @param fmt The format string.
 * @param ... The variadic arguments for the format string.
 */
#define render_printf(game, x, y, fmt, ...)                     \
    do {                                                        \
        memset(game->textbuf, 0, TEXTBUFLEN);                   \
        snprintf(game->textbuf, TEXTBUFLEN, fmt, __VA_ARGS__);  \
        render_string(game->renderer, x, y, game->textbuf);     \
    } while (0)


/**
 * @brief Changes the brightness of an SDL_Color object by a given factor.
 *
 * This function multiplies the RGB values of the given SDL_Color object by the given alpha factor
 * and constrains the resulting values to be within the valid range of 0 to 255. The original
 * SDL_Color object is modified in place.
 *
 * @param color A pointer to the SDL_Color object to modify.
 * @param alpha The brightness factor to apply to the color.
 *
 * @return A pointer to the modified SDL_Color object.
 */
SDL_Color *change_brightness(SDL_Color *color, float alpha);

#endif //RAY_UTIL_H
