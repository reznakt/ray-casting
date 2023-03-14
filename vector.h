#ifndef RAY_VECTOR_H
#define RAY_VECTOR_H


typedef struct {
    float x;
    float y;
} Vector;

typedef struct {
    long x;
    long y;
} IVector;

typedef struct {
    Vector a;
    Vector b;
} Line;

typedef struct {
    IVector a;
    IVector b;
} ILine;


extern const Vector vector_unit;
extern const Vector vector_zero;


Vector *vector_add(Vector *dst, const Vector *src);

Vector *vector_mul(Vector *vec, float x);

Vector *vector_div(Vector *vec, float x);

float vector_product(const Vector *vec1, const Vector *vec2);

float vector_length(const Vector *vec);

void print_vector(const Vector *vec);

Vector *vector_from_angle(Vector *dst, float angle);

Vector *vector_normalize(Vector *vec);

Vector *vector_normalize_weak(Vector *vec);

float vector_distance(const Vector *vec1, const Vector *vec2);

float vector_angle_to(const Vector *vec1, const Vector *vec2);

Vector *vector_copy(Vector *dst, const Vector *src);

Vector *vector_rotate(Vector *vec, float angle);

#endif // RAY_VECTOR_H

