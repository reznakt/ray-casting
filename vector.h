/**
 * @file vector.h
 * @brief Contains functions for working with 2D vectors and lines.
 *
 * This file defines the `struct vector_t` and `struct ivector_t` structs, which represent 2D vectors
 * with `float` and `long` components, respectively. It also defines the `struct line_t` and `struct iline_t`
 * structs, which represent lines defined by two points.
 *
 * In addition to the struct definitions, this file provides functions for performing operations on 2D vectors,
 * including addition, multiplication, division, dot product, length calculation, normalization, distance calculation,
 * angle calculation, copying, and rotation.
 *
 * It also includes constant definitions for the zero vector.
 */


#ifndef RAY_VECTOR_H
#define RAY_VECTOR_H


/**
 * A 2D vector with `x` and `y` coordinates.
 */
struct vector_t {
    float x; /**< The x-coordinate of the vector. */
    float y; /**< The y-coordinate of the vector. */
};

/**
 * A 2D vector with `x` and `y` coordinates, but with integer types.
 */
struct ivector_t {
    long x; /**< The x-coordinate of the vector. */
    long y; /**< The y-coordinate of the vector. */
};

/**
 * A line segment with `a` and `b` as its endpoints, both of type `struct vector_t`.
 */
struct line_t {
    struct vector_t a; /**< The first endpoint of the line segment. */
    struct vector_t b; /**< The second endpoint of the line segment. */
};

/**
 * A line segment with `a` and `b` as its endpoints, both of type `struct ivector_t`.
 */
struct iline_t {
    struct ivector_t a; /**< The first endpoint of the line segment. */
    struct ivector_t b; /**< The second endpoint of the line segment. */
};


/**
 * Constant zero vector.
 */
extern const struct vector_t vector_zero;


/**
 * @brief Adds the vector @p src to the vector @p dst.
 *
 * This function adds the components of the vector @p src to the corresponding components
 * of the vector @p dst and stores the result in @p dst.
 *
 * @param dst Pointer to the destination vector.
 * @param src Pointer to the source vector.
 *
 * @return Pointer to the resulting vector @p dst.
 *
 * @note This function modifies the vector pointed to by @p dst.
 */
struct vector_t *vector_add(struct vector_t *dst, const struct vector_t *src);

/**
 * @brief Multiplies the vector @p vec by the scalar @p x.
 *
 * This function multiplies each component of the vector @p vec by the scalar @p x and
 * stores the result in the same vector. The original vector is modified.
 *
 * @param vec Pointer to the vector to be multiplied.
 * @param x The scalar value to multiply the vector by.
 *
 * @return Pointer to the resulting vector @p vec.
 */
struct vector_t *vector_mul(struct vector_t *vec, float x);

/**
 * @brief Divides the vector @p vec by the scalar @p x.
 *
 * This function divides each component of the vector @p vec by the scalar @p x and
 * stores the result in the same vector. The original vector is modified.
 *
 * @param vec Pointer to the vector to be divided.
 * @param x The scalar value to divide the vector by.
 *
 * @return Pointer to the resulting vector @p vec.
 *
 * @note Division by zero is not checked in this function. Ensure that @p x is not zero
 *       to avoid unexpected results.
 */
struct vector_t *vector_div(struct vector_t *vec, float x);

/**
 * @brief Computes the dot product of the vectors @p vec1 and @p vec2.
 *
 * This function computes the dot product of the two input vectors @p vec1 and @p vec2,
 * which is equal to the product of their magnitudes and the cosine of the angle between
 * them. The dot product is a scalar value that represents the similarity of the two vectors.
 *
 * @param vec1 Pointer to the first input vector.
 * @param vec2 Pointer to the second input vector.
 *
 * @return The dot product of the vectors @p vec1 and @p vec2.
 */
float vector_product(const struct vector_t *vec1, const struct vector_t *vec2);

/**
 * @brief Computes the magnitude of the vector @p vec.
 *
 * This function computes the magnitude (also known as length) of the vector @p vec,
 * which is equal to the square root of the sum of the squares of its components.
 *
 * @param vec Pointer to the input vector.
 *
 * @return The magnitude of the vector @p vec.
 */
float vector_length(const struct vector_t *vec);

/**
 * @brief Prints the components of the vector @p vec to the standard output.
 *
 * This function prints the components of the vector @p vec to the standard output
 * in the format "(x, y)". The output is followed by a newline character.
 *
 * @param vec Pointer to the input vector.
 */
void print_vector(const struct vector_t *vec);

/**
 * @brief Computes the vector with magnitude 1 and angle @p angle, and stores the result in @p dst.
 *
 * This function computes the vector with magnitude 1 and angle @p angle, and stores the result in
 * the vector pointed to by @p dst. The angle is specified in radians, and the resulting vector
 * will have components (cos(angle), sin(angle)).
 *
 * @param dst Pointer to the output vector. The result will be stored here.
 * @param angle The angle (in radians) of the desired vector.
 *
 * @return A pointer to the resulting vector, which is also stored in @p dst.
 */
struct vector_t *vector_from_angle(struct vector_t *dst, float angle);

/**
 * @brief Computes the unit vector in the direction of the vector @p vec, and stores the result in @p vec.
 *
 * This function computes the unit vector (i.e., a vector with magnitude 1) in the direction of the
 * vector @p vec, and stores the result in the vector pointed to by @p vec.
 *
 * @param vec Pointer to the input/output vector. The result will be stored here.
 *
 * @return A pointer to the resulting vector, which is also stored in @p vec.
 *
 * @note The input vector @p vec may not have zero length.
 */
struct vector_t *vector_normalize(struct vector_t *vec);

/**
 * @brief Computes the unit vector in the direction of the vector @p vec, and stores the result in @p vec.
 *
 * This function computes the unit vector (i.e., a vector with magnitude 1) in the direction of the
 * vector @p vec, and stores the result in the vector pointed to by @p vec. If the vector @p vec has
 * zero length, the resulting vector will have components (0, 0).
 *
 * @param vec Pointer to the input/output vector. The result will be stored here.
 *
 * @return A pointer to the resulting vector, which is also stored in @p vec.
 */
struct vector_t *vector_normalize_weak(struct vector_t *vec);

/**
 * @brief Computes the Euclidean distance between two vectors.
 *
 * This function computes the Euclidean distance between the vectors @p vec1 and @p vec2 in two-dimensional
 * space using the formula sqrt((x2 - x1)^2 + (y2 - y1)^2), where (x1, y1) and (x2, y2) are the components
 * of @p vec1 and @p vec2, respectively.
 *
 * @param vec1 Pointer to the first vector.
 * @param vec2 Pointer to the second vector.
 *
 * @return The Euclidean distance between the vectors @p vec1 and @p vec2.
 */
float vector_distance(const struct vector_t *vec1, const struct vector_t *vec2);

/**
 * @brief Computes the angle between two vectors.
 *
 * This function computes the angle between the vectors @p vec1 and @p vec2 in two-dimensional
 * space using the formula acos(dot_product(vec1, vec2) / (length(vec1) * length(vec2))), where
 * dot_product(vec1, vec2) is the dot product of @p vec1 and @p vec2, and length(vec1) and length(vec2)
 * are the magnitudes (i.e., lengths) of the vectors @p vec1 and @p vec2, respectively. The angle is
 * returned in radians, and is always in the range [0, pi].
 *
 * @param vec1 Pointer to the first vector.
 * @param vec2 Pointer to the second vector.
 *
 * @return The angle between the vectors @p vec1 and @p vec2, in radians.
 *
 * @note The input vector @p vec may not have zero length.
 */
float vector_angle_to(const struct vector_t *vec1, const struct vector_t *vec2);

/**
 * @brief Copies a vector.
 *
 * This function copies the contents of the vector pointed to by @p src into the vector pointed to
 * by @p dst.
 *
 * @param dst Pointer to the destination vector.
 * @param src Pointer to the source vector.
 *
 * @return Pointer to the destination vector @p dst.
 */
struct vector_t *vector_copy(struct vector_t *dst, const struct vector_t *src);

/**
 * @brief Rotates a vector by a given angle.
 *
 * This function rotates the vector pointed to by @p vec by the given angle @p angle.
 *
 * @param vec Pointer to the vector to rotate.
 * @param angle The angle in radians to rotate the vector by.
 *
 * @return Pointer to the rotated vector @p vec.
 */
struct vector_t *vector_rotate(struct vector_t *vec, float angle);

#endif // RAY_VECTOR_H

