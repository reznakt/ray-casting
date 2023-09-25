#ifndef RAY_UTIL_H
#define RAY_UTIL_H


#include <stdbool.h>


#include "conf.h"
#include "text.h"


#define unused __attribute__((__unused__))


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
#define render_printf(game, pos, fmt, ...)                      \
    do {                                                        \
        memset(game->textbuf, 0, TEXTBUFLEN);                   \
        snprintf(game->textbuf, TEXTBUFLEN, fmt, __VA_ARGS__);  \
        render_string(game->renderer, pos, game->textbuf);      \
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

/**
 * @brief Determines whether a string contains only whitespace.
 * @param buf the string to check.
 * @return true if the string contains only whitespace, false otherwise. Also returns false if buf is NULL.
 * @see isspace(3)
 */
bool is_whitespace(const char *buf);

/**
 * @brief Determines whether a string contains a valid non-negative decimal integer.
 * @param buf the string to check.
 * @return true if the string contains a valid decimal number, false otherwise. Also returns false if buf is NULL.
 * @see isdigit(3)
 */
bool is_decimal(const char *buf);

/**
 * @brief Converts a hexadecimal digit to a decimal number.
 * @param hexnum Hexadecimal digit to convert.
 * @return Decimal number corresponding to the hexadecimal digit, or -1 if the digit is invalid.
 */
int hex_to_dec(char hexnum);

/**
 * @brief Converts an SDL_Color to a uint32_t.
 * @param color The color to convert.
 * @return The converted color.
 */
uint32_t color_to_int(const SDL_Color *color);


#endif //RAY_UTIL_H
