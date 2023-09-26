#ifndef RAY_FONT_H
#define RAY_FONT_H


#include <SDL2/SDL.h>


struct vec_t; // Forward declaration because of circular dependencies.


#include "vector.h"


/**
 * @brief Renders a single character at the specified coordinates using the given renderer.
 *
 * @param renderer Pointer to the SDL_Renderer to use for rendering.
 * @param x The x-coordinate at which to render the character.
 * @param y The y-coordinate at which to render the character.
 * @param chr The ASCII code for the character to render.
 */
void render_putchar(SDL_Renderer *renderer, const struct vec_t *pos, int chr);

/**
 * @brief Renders a null-terminated string at the specified coordinates using the given renderer.
 *
 * @param renderer Pointer to the SDL_Renderer to use for rendering.
 * @param x The x-coordinate at which to start rendering the string.
 * @param y The y-coordinate at which to start rendering the string.
 * @param str A pointer to the null-terminated string to render.
 */
void render_puts(SDL_Renderer *renderer, const struct vec_t *pos, const char *str);


/**
 * @brief This function renders the formatted text using the specified game renderer and position.
 *
 * @param game The game object.
 * @param x The x-coordinate of the text position.
 * @param y The y-coordinate of the text position.
 * @param fmt The format string.
 * @param ... The variadic arguments for the format string.
 */
void render_printf(SDL_Renderer *renderer, const struct vec_t *pos, const char *fmt, ...);


#endif // RAY_FONT_H
