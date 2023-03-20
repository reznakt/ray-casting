#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "math.h"

#include "vector.h"


const struct vector_t vector_zero = {0, 0};


struct vector_t *vector_add(struct vector_t *const dst, const struct vector_t *const src) {
    dst->x += src->x;
    dst->y += src->y;
    return dst;
}

struct vector_t *vector_sub(struct vector_t *const dst, const struct vector_t *const src) {
    dst->x -= src->x;
    dst->y -= src->y;
    return dst;
}

struct vector_t *vector_mul(struct vector_t *const vec, const float x) {
    vec->x *= x;
    vec->y *= x;
    return vec;
}

struct vector_t *vector_div(struct vector_t *const vec, const float x) {
    vec->x /= x;
    vec->y /= x;
    return vec;
}

float vector_product(const struct vector_t *const vec1, const struct vector_t *const vec2) {
    return vec1->x * vec2->x + vec1->y * vec2->y;
}

float vector_length(const struct vector_t *const vec) {
    return sqrtf(vector_product(vec, vec));
}

void print_vector(const struct vector_t *const vec) {
    printf("(%f, %f)\n", vec->x, vec->y);
}

struct vector_t *vector_from_angle(struct vector_t *const dst, const float angle) {
    dst->x = cosf(angle);
    dst->y = sinf(angle);
    return dst;
}

struct vector_t *vector_normalize(struct vector_t *const vec) {
    return vector_div(vec, vector_length(vec));
}

struct vector_t *vector_normalize_weak(struct vector_t *const vec) {
    const float length = vector_length(vec);

    if (isclose(length, 0, 8)) {
        vec->x = vec->y = 0;
        return vec;
    }

    vec->x /= length;
    vec->y /= length;
    return vec;
}

float vector_distance(const struct vector_t *const vec1, const struct vector_t *const vec2) {
    return sqrtf(powf(vec2->x - vec1->x, 2) + powf(vec2->y - vec1->y, 2));
}

float vector_angle_to(const struct vector_t *const vec1, const struct vector_t *const vec2) {
    const float length1 = vector_length(vec1);
    const float length2 = vector_length(vec2);
    const float product = vector_product(vec1, vec2);
    return acosf(product / (length1 * length2));
}

struct vector_t *vector_copy(struct vector_t *const dst, const struct vector_t *const src) {
    dst->x = src->x;
    dst->y = dst->y;
    return dst;
}

struct vector_t *vector_rotate(struct vector_t *const vec, const float angle) {
    const float sin_a = sinf(angle);
    const float cos_a = cosf(angle);

    vec->x = cos_a * vec->x - sin_a * vec->y;
    vec->x = sin_a * vec->x + cos_a * vec->y;

    return vec;
}

struct vector_t *vector_lerp(struct vector_t *dst, const struct vector_t *src, float t) {
    dst->x = lerp(dst->x, src->x, t);
    dst->y = lerp(dst->y, src->y, t);

    return dst;
}

struct vector_t *vector_scale(struct vector_t *vec, float length) {
    vector_normalize_weak(vec);
    vector_mul(vec, length);

    return vec;
}

struct vector_t *vector_reflect(struct vector_t *const vec, const struct vector_t *const normal) {
    struct vector_t *const ncopy = vector_copy(vector(), normal);
    return vector_sub(vec, vector_scale(ncopy, 2.0f * vector_product(vec, normal)));
}

struct vector_t *vector_project(struct vector_t *const normal, const struct vector_t *const vec) {
    return vector_scale(normal, vector_product(vec, normal) / vector_product(normal, normal));
}
