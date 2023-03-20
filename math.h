/**
 * @file math.h
 *
 * @brief Provides commonly used mathematical functions and constants.
 */


#ifndef RAY_MATH_H
#define RAY_MATH_H


#include <stdbool.h>


#define ISCLOSE_1 0.1f
#define ISCLOSE_2 0.01f
#define ISCLOSE_3 0.001f
#define ISCLOSE_4 0.0001f
#define ISCLOSE_5 0.00001f
#define ISCLOSE_6 0.000001f
#define ISCLOSE_7 0.0000001f
#define ISCLOSE_8 0.00000001f
#define ISCLOSE_9 0.000000001f
#define ISCLOSE_10 0.000000001f


/**
 * @brief Returns the smaller of two values.
 *
 * @param a The first value.
 * @param b The second value.
 * @return The smaller value.
 */
#define min(a, b) ((a) < (b) ? (a) : (b))

/**
 * @brief Returns the larger of two values.
 *
 * @param a The first value.
 * @param b The second value.
 * @return The larger value.
 */
#define max(a, b) ((a) > (b) ? (a) : (b))


/**
 * @brief Maps a value from one range to another.
 *
 * @param value The value to be mapped.
 * @param old_low The lower bound of the original range.
 * @param old_high The upper bound of the original range.
 * @param new_low The lower bound of the new range.
 * @param new_high The upper bound of the new range.
 * @return The mapped value.
 */
float map(float value, float old_low, float old_high, float new_low, float new_high);

/**
 * @brief Constrains a value between a lower and an upper bound.
 *
 * @param value The value to be constrained.
 * @param low The lower bound.
 * @param high The upper bound.
 * @return The constrained value.
 */
float constrain(float value, float low, float high);

/**
 * @brief Linearly interpolate between two float values.
 *
 * This function calculates the linear interpolation between two float values a and b based on a given parameter t.
 *
 * @param a     The starting value for the interpolation.
 * @param b     The ending value for the interpolation.
 * @param t     The interpolation parameter. Must be in the range [0, 1].
 *
 * @return      The interpolated float value between a and b.
 *
 * @note        If t is outside the range [0, 1], the result will be extrapolated beyond the range of a and b.
 */
float lerp(float a, float b, float t);

/**
 * @brief Compares two floats for closeness.
 *
 * @param a The first value.
 * @param b The second value.
 * @param epsilon The tolerance for closeness.
 * @return True if the values are close, false otherwise.
 */
bool isclose(float a, float b, float epsilon);

/**
 * @brief Converts an angle from degrees to radians.
 *
 * @param angle The angle in degrees.
 * @return The angle in radians.
 */
float degrees(float angle);

/**
 * @brief Converts an angle from radians to degrees.
 *
 * @param angle The angle in radians.
 * @return The angle in degrees.
 */
float radians(float angle);


#endif // RAY_MATH_H
