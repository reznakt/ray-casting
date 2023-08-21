/**
 * @file font.h
 * @brief Defines a basic font set for text rendering.
 */


#ifndef RAY_FONT_H
#define RAY_FONT_H


#include <SDL2/SDL.h>


#define FONT_BYTES 8 /**< The number of bytes required to represent a single character. */
#define FONT_CHARS 128 /**< The number of characters in the basic font set. */


/**
 * @brief Renders a single character at the specified coordinates using the given renderer.
 *
 * @param renderer Pointer to the SDL_Renderer to use for rendering.
 * @param x The x-coordinate at which to render the character.
 * @param y The y-coordinate at which to render the character.
 * @param chr The ASCII code for the character to render.
 */
void render_char(SDL_Renderer *renderer, int x, int y, int chr);

/**
 * @brief Renders a null-terminated string at the specified coordinates using the given renderer.
 *
 * @param renderer Pointer to the SDL_Renderer to use for rendering.
 * @param x The x-coordinate at which to start rendering the string.
 * @param y The y-coordinate at which to start rendering the string.
 * @param str A pointer to the null-terminated string to render.
 */
void render_string(SDL_Renderer *renderer, int x, int y, const char *str);

#endif // RAY_FONT_H
