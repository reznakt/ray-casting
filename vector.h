#ifndef RAY_VECTOR_H
#define RAY_VECTOR_H


struct vector_t {
    float x;
    float y;
};

struct ivector_t {
    long x;
    long y;
};

struct line_t {
    struct vector_t a;
    struct vector_t b;
};

struct iline_t {
    struct ivector_t a;
    struct ivector_t b;
};


extern const struct vector_t vector_unit;
extern const struct vector_t vector_zero;


struct vector_t *vector_add(struct vector_t *dst, const struct vector_t *src);

struct vector_t *vector_mul(struct vector_t *vec, float x);

struct vector_t *vector_div(struct vector_t *vec, float x);

float vector_product(const struct vector_t *vec1, const struct vector_t *vec2);

float vector_length(const struct vector_t *vec);

void print_vector(const struct vector_t *vec);

struct vector_t *vector_from_angle(struct vector_t *dst, float angle);

struct vector_t *vector_normalize(struct vector_t *vec);

struct vector_t *vector_normalize_weak(struct vector_t *vec);

float vector_distance(const struct vector_t *vec1, const struct vector_t *vec2);

float vector_angle_to(const struct vector_t *vec1, const struct vector_t *vec2);

struct vector_t *vector_copy(struct vector_t *dst, const struct vector_t *src);

struct vector_t *vector_rotate(struct vector_t *vec, float angle);

#endif // RAY_VECTOR_H

