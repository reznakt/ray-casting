#include <math.h>

#include "math.h"

#include "vector.h"


const struct vector_t vector_zero = {0, 0};


struct vector_t *vector_clear(struct vector_t *vec) {
    return vector_copy(vec, &vector_zero);
}

struct vector_t *vector_add(struct vector_t *const restrict dst, const struct vector_t *const restrict src) {
    dst->x += src->x;
    dst->y += src->y;
    return dst;
}

struct vector_t *vector_sub(struct vector_t *const restrict dst, const struct vector_t *const restrict src) {
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

float vector_product(const struct vector_t *const restrict vec1, const struct vector_t *const restrict vec2) {
    return vec1->x * vec2->x + vec1->y * vec2->y;
}

float vector_length(const struct vector_t *const vec) {
    return sqrtf(vector_length2(vec));
}

float vector_length2(const struct vector_t *vec) {
    return vector_product(vec, vec);
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

    if (isclose(length, 0)) {
        vec->x = vec->y = 0;
        return vec;
    }

    return vector_div(vec, length);
}

float vector_distance(const struct vector_t *const restrict vec1, const struct vector_t *const restrict vec2) {
    return sqrtf(vector_distance2(vec1, vec2));
}

float vector_distance2(const struct vector_t *const restrict vec1, const struct vector_t *const restrict vec2) {
    return powf(vec2->x - vec1->x, 2) + powf(vec2->y - vec1->y, 2);
}

float vector_angle_to(const struct vector_t *const restrict vec1, const struct vector_t *const restrict vec2) {
    const float length1 = vector_length(vec1);
    const float length2 = vector_length(vec2);
    const float product = vector_product(vec1, vec2);
    return acosf(product / (length1 * length2));
}

struct vector_t *vector_copy(struct vector_t *const restrict dst, const struct vector_t *const restrict src) {
    return memcpy(dst, src, sizeof *dst);
}

struct vector_t *vector_rotate(struct vector_t *const vec, const float angle) {
    const float sin_a = sinf(angle), cos_a = cosf(angle);
    const float x = vec->x, y = vec->y;

    vec->x = x * cos_a - y * sin_a;
    vec->y = x * sin_a + y * cos_a;

    return vec;
}

struct vector_t *vector_lerp(struct vector_t *const restrict dst,
                             const struct vector_t *const restrict src,
                             const float t) {
    dst->x = lerp(dst->x, src->x, t);
    dst->y = lerp(dst->y, src->y, t);

    return dst;
}

struct vector_t *vector_scale(struct vector_t *const vec, const float length) {
    vector_normalize_weak(vec);
    vector_mul(vec, length);

    return vec;
}

struct vector_t *vector_reflect(struct vector_t *const restrict vec, const struct vector_t *const restrict normal) {
    struct vector_t *const ncopy = vector_copy(vector(), normal);
    return vector_sub(vec, vector_scale(ncopy, 2.0F * vector_product(vec, normal)));
}

struct vector_t *vector_project(struct vector_t *const restrict normal, const struct vector_t *const restrict vec) {
    return vector_scale(normal, vector_product(vec, normal) / vector_product(normal, normal));
}
