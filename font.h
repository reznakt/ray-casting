/**
 * @file font.h
 * @brief Defines a basic font set for text rendering.
 */


#ifndef RAY_FONT_H
#define RAY_FONT_H


#define FONT_BYTES 8 /**< The number of bytes required to represent a single character. */
#define FONT_BASIC 128 /**< The number of characters in the basic font set. */


/**
 * @brief The basic font set.
 *
 * This is a 128 character ASCII-compatible font set.
 */
extern const int font_basic[FONT_BASIC][FONT_BYTES];


#endif // RAY_FONT_H
