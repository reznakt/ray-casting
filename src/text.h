#ifndef RAY_FONT_H
#define RAY_FONT_H


#include <stdlib.h>

#include <SDL2/SDL.h>

struct vec_t; // Forward declaration because of circular dependencies.

#include "vector.h"


/**
 * The width of a single character in pixels.
 */
#define CHAR_WIDTH 8

/**
 * The height of a single character in pixels.
 */
#define CHAR_HEIGHT 8

/**
 * The number of pixels between two characters on the x-axis.
 */
#define CHAR_HORIZONTAL_SPACING 2

/**
 * The number of pixels between two characters on the y-axis.
 */
#define CHAR_VERTICAL_SPACING CHAR_HEIGHT

/**
 * The size of the tab character in spaces.
 */
#define TAB_WIDTH 4


size_t text_width(const char *text);

/**
 * @brief Renders a single character at the specified coordinates using the given renderer.
 *
 * @param renderer Pointer to the SDL_Renderer to use for rendering.
 * @param pos The position at which to render the character.
 * @param chr The ASCII code for the character to render.
 */
void render_putchar(SDL_Renderer *renderer, struct vec_t pos, char chr);

/**
 * @brief Renders a null-terminated string at the specified coordinates using the given renderer.
 *
 * @param renderer Pointer to the SDL_Renderer to use for rendering.
 * @param pos The position at which to render the string.
 * @param str A pointer to the null-terminated string to render.
 */
void render_puts(SDL_Renderer *renderer, struct vec_t pos, const char *str);

/**
 * @brief This function renders the formatted text using the specified game renderer and position.
 *
 * @param game The game object.
 * @param pos The position at which to render the text.
 * @param fmt The format string.
 * @param ... The variadic arguments for the format string.
 */
__attribute__((__format__(__printf__, 3, 4)))
void render_printf(SDL_Renderer *renderer, struct vec_t pos, const char *fmt, ...);


#endif // RAY_FONT_H
