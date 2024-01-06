#ifndef RAY_UTIL_H
#define RAY_UTIL_H


#include <stdbool.h>


#include "conf.h"
#include "text.h"


#define unused __attribute__((__unused__))

/**
 * Allocate `n` new objects of type `type` on the stack.
 */
#define stack_alloc(type, n) ((type [n]) {0})
#define rgba(r, g, b, a) {(r), (g), (b), (a)}
#define rgb(r, g, b) rgba((r), (g), (b), SDL_ALPHA_OPAQUE)


#define float_literal_internal(dbl) dbl##F

/**
 * @brief Converts a double literal to a float literal.
 *
 * This macro is used to convert double literals to float literals,
 * mainly for compatibility with -Wunsuffixed-float-constants.
 *
 * @param dbl The double literal to convert.
 * @return The converted float literal.
 */
#define float_literal(dbl) float_literal_internal(dbl)


unused static const SDL_Color COLOR_RED = rgb(255, 0, 0);
unused static const SDL_Color COLOR_GREEN = rgb(0, 255, 0);
unused static const SDL_Color COLOR_WHITE = rgb(255, 255, 255);
unused static const SDL_Color COLOR_BLACK = rgb(0, 0, 0);


/**
 * @brief Changes the brightness of an SDL_Color object by a given factor.
 *
 * This function multiplies the RGB values of the given SDL_Color object by the given alpha factor
 * and constrains the resulting values to be within the valid range of 0 to 255.
 *
 * @param color The source color.
 * @param alpha The brightness factor to apply to the color.
 *
 * @return The new color.
 */
SDL_Color change_brightness(SDL_Color color, float alpha);

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
uint32_t color_to_int(SDL_Color color);


#endif //RAY_UTIL_H
