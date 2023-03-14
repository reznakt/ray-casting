#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "math.h"

#include "vector.h"


const Vector vector_unit = {1, 1};
const Vector vector_zero = {0, 0};


Vector *vector_add(Vector *const dst, const Vector *const src) {
    dst->x += src->x;
    dst->y += src->y;
    return dst;
}

Vector *vector_mul(Vector *const vec, const float x) {
    vec->x *= x;
    vec->y *= x;
    return vec;
}

Vector *vector_div(Vector *const vec, const float x) {
    vec->x /= x;
    vec->y /= x;
    return vec;
}

float vector_product(const Vector *const vec1, const Vector *const vec2) {
    return vec1->x * vec2->x + vec1->y * vec2->y;
}

float vector_length(const Vector *const vec) {
    return sqrtf(vector_product(vec, vec));
}

void print_vector(const Vector *const vec) {
    printf("(%f, %f)\n", vec->x, vec->y);
}

Vector *vector_from_angle(Vector *const dst, const float angle) {
    dst->x = cosf(angle);
    dst->y = sinf(angle);
    return dst;
}

Vector *vector_normalize(Vector *const vec) {
    return vector_div(vec, vector_length(vec));
}

Vector *vector_normalize_weak(Vector *const vec) {
    const float length = vector_length(vec);

    if (isclose(length, 0, 8)) {
        vec->x = vec->y = 0;
        return vec;
    }

    vec->x /= length;
    vec->y /= length;
    return vec;
}

float vector_distance(const Vector *const vec1, const Vector *const vec2) {
    return sqrtf(powf(vec2->x - vec1->x, 2) + powf(vec2->y - vec1->y, 2));
}

float vector_angle_to(const Vector *const vec1, const Vector *const vec2) {
    const float length1 = vector_length(vec1);
    const float length2 = vector_length(vec2);
    const float product = vector_product(vec1, vec2);
    return acosf(product / (length1 * length2));
}

Vector *vector_copy(Vector *const dst, const Vector *const src) {
    dst->x = src->x;
    dst->y = dst->y;
    return dst;
}

Vector *vector_rotate(Vector *const vec, const float angle) {
    const float sin_a = sinf(angle);
    const float cos_a = cosf(angle);

    vec->x = cos_a * vec->x - sin_a * vec->y;
    vec->x = sin_a * vec->x + cos_a * vec->y;

    return vec;
}

