#ifndef RAY_VECTOR_H
#define RAY_VECTOR_H


#include "util.h"


/**
 * A 2D vector with `x` and `y` coordinates.
 */
struct vec_t {
    float x; /**< The x-coordinate of the vector. */
    float y; /**< The y-coordinate of the vector. */
};


/**
 * Constant zero vector.
 */
extern const struct vec_t vzero;


/**
 * @brief Computes the sum of vectors @p a and @p b.
 *
 * @param a The first vector.
 * @param b The second vector.
 *
 * @return The sum of vectors @p a and @p b.
 */
struct vec_t vadd(struct vec_t a, struct vec_t b);

/**
 * @brief Subtracts the vector @p b from the vector @p a.
 *
 * @param a The first vector.
 * @param b The second vector.
 *
 * @return The difference between vectors @p a and @p b.
 */
struct vec_t vsub(struct vec_t a, struct vec_t b);

/**
 * @brief Multiplies the vector @p vec by the scalar @p x.
 *
 * This function multiplies each component of the vector @p vec by the scalar @p x.
 *
 * @param vec The vector to be multiplied.
 * @param x The scalar value to multiply the vector by.
 *
 * @return The product of the vector @p vec and the scalar @p x.
 */
struct vec_t vmul(struct vec_t vec, float x);

/**
 * @brief Divides the vector @p vec by the scalar @p d.
 *
 * This function divides each component of the vector @p vec by the scalar @p d.
 *
 * @param vec The vector to be divided.
 * @param d The scalar value to divide the vector by.
 *
 * @return The quotient of the vector @p vec and the scalar @p d.
 *
 * @note Division by zero is not checked in this function. Ensure that @p x is not zero. As per the C standard,
 *       the result of dividing by zero is undefined.
 */
struct vec_t vdiv(struct vec_t vec, float d);

/**
 * @brief Computes the dot product of vectors @p a and @p b.
 *
 * This function computes the dot product of two input vectors @p a and @p b,
 * which is equal to the product of their magnitudes and the cosine of the angle between
 * them. The dot product is a scalar value that represents the similarity of the two vectors.
 *
 * @param a The first input vector.
 * @param b The second input vector.
 *
 * @return The dot product of vectors @p a and @p b.
 */
float vprod(struct vec_t a, struct vec_t b);

/**
 * @brief Computes the magnitude of the vector @p vec.
 *
 * This function computes the magnitude (also known as length) of the vector @p vec,
 * which is equal to the square root of the sum of the squares of its components.
 *
 * @param vec The input vector.
 *
 * @return The magnitude of the vector @p vec.
 */
float vlen(struct vec_t vec);

/**
 * @brief Computes the squared magnitude of the vector @p vec.
 *
 * This function computes the squared magnitude of the vector @p vec,
 * which is equal to the sum of the squares of its components.
 *
 * @param vec The input vector.
 *
 * @return The squared magnitude of the vector @p vec.
 */
float vlen2(struct vec_t vec);

/**
 * @brief Computes the vector with magnitude 1 and angle @p angle.
 *
 * This function computes the vector with magnitude 1 and angle @p angle.
 * The angle is specified in radians, and the resulting vector
 * will have components (cos(angle), sin(angle)).
 *
 * @param angle The angle (in radians) of the desired vector.
 *
 * @return The resulting vector.
 */
struct vec_t vfromangle(float angle);

/**
 * @brief Computes the unit vector in the direction of the vector @p vec.
 *
 * This function computes the unit vector (i.e., a vector with magnitude 1) in the direction of the
 * vector @p vec.
 *
 * @param vec The input vector to normalize.
 *
 * @return The normalized vector.
 *
 * @note The input vector @p vec may not have zero length. If it does, the result is undefined.
 */
struct vec_t vnorm(struct vec_t vec);

/**
 * @brief Computes the unit vector in the direction of the vector @p vec.
 *
 * This function computes the unit vector (i.e., a vector with magnitude 1) in the direction of the
 * vector @p vec. If the vector @p vec has zero length, the resulting vector will have components (0, 0).
 *
 * @param vec The input vector to normalize.
 *
 * @return The normalized vector.
 */
struct vec_t vnorm_weak(struct vec_t vec);

/**
 * @brief Computes the Euclidean distance between two vectors @p a and @p b.
 *
 * This function computes the Euclidean distance between the vectors @p a and @p b in two-dimensional
 * space using the formula sqrt((x2 - x1)^2 + (y2 - y1)^2), where (x1, y1) and (x2, y2) are the components
 * of @p a and @p b, respectively.
 *
 * @param a The first vector.
 * @param b The second vector.
 *
 * @return The Euclidean distance between the vectors @p a and @p b.
 */
float vdist(struct vec_t a, struct vec_t b);

/**
 * @brief Computes the square of the Euclidean distance between two vectors @p a and @p b.
 *
 * This function computes the squared Euclidean distance between the vectors @p a and @p b in two-dimensional
 * space using the formula (x2 - x1)^2 + (y2 - y1)^2, where (x1, y1) and (x2, y2) are the components
 * of @p a and @p b, respectively.
 *
 * @param a The first vector.
 * @param b The second vector.
 *
 * @return The squared Euclidean distance between the vectors @p a and @p b.
 */
float vdist2(struct vec_t a, struct vec_t b);

/**
 * @brief Computes the angle between two vectors @p a and @p b.
 *
 * This function computes the angle between the vectors @p a and @p b in two-dimensional
 * space using the formula acos(dot_product(a, b) / (length(a) * length(b))), where
 * dot_product(a, b) is the dot product of @p a and @p b, and length(a) and length(b)
 * are the magnitudes (i.e., lengths) of the vectors @p a and @p b, respectively. The angle is
 * returned in radians, and is always in the range [0, pi].
 *
 * @param a The first vector.
 * @param b The second vector.
 *
 * @return The angle between the vectors @p a and @p b, in radians.
 *
 * @note If the length of either vector is zero, the result will be NaN.
 */
float vangle(struct vec_t a, struct vec_t b);

/**
 * @brief Rotates a vector by a given angle.
 *
 * This function rotates the vector @p vec by the given angle @p angle.
 *
 * @param vec The vector to rotate.
 * @param angle The angle in radians to rotate the vector by.
 *
 * @return The rotated vector @p vec.
 */
struct vec_t vrotate(struct vec_t vec, float angle);

/**
 * @brief Linearly interpolate between two vectors.
 *
 * This function calculates the linear interpolation between two vectors @p a dnd @p b based on a given parameter @p t.
 *
 * @param a The first input vector.
 * @param b The second input vector.
 * @param t The interpolation parameter. Must be in the range [0, 1].
 *
 * @return The interpolated vector.
 *
 * @note If t is outside the range [0, 1], the result will be extrapolated beyond the range of @p a and @p b.
 */
struct vec_t vlerp(struct vec_t a, struct vec_t b, float t);

/**
 * @brief Scale a vector to a specified length while preserving its direction.
 *
 * This function first normalizes the input vector by calling `vnorm_weak()`,
 * and then multiplies each component of the vector by the specified length using `vmul()`.
 * The resulting vector has the same direction as the input vector but with a magnitude equal to `length`.
 *
 * @param vec The vector to be scaled.
 * @param length The desired length of the scaled vector.
 *
 * @return The scaled vector.
 *
 * @see vnorm_weak()
 * @see vmul()
 */
struct vec_t vscale(struct vec_t vec, float length);

/**
 * Reflects a vector off a plane defined by a normal vector.
 *
 * @param vec The vector to reflect.
 * @param normal The normal vector defining the plane to reflect off.
 * @return The reflected vector.
 */
struct vec_t vreflect(struct vec_t vec, struct vec_t normal);

/**
 * Projects a vector onto a plane defined by a normal vector.
 *
 * @param normal The normal vector defining the plane to project onto.
 * @param vec The vector to project.
 * @return The projected vector.
 */
struct vec_t vproject(struct vec_t normal, struct vec_t vec);


#endif // RAY_VECTOR_H
