#include <math.h>

#include "math.h"

#include "vector.h"


const struct vec_t vzero = {0.0F, 0.0F};


struct vec_t vadd(const struct vec_t a, const struct vec_t b) {
    return (struct vec_t) {a.x + b.x, a.y + b.y};
}

struct vec_t vsub(const struct vec_t a, const struct vec_t b) {
    return (struct vec_t) {a.x - b.x, a.y - b.y};
}

struct vec_t vmul(const struct vec_t vec, const float x) {
    return (struct vec_t) {vec.x * x, vec.y * x};
}

struct vec_t vdiv(const struct vec_t vec, const float d) {
    return (struct vec_t) {vec.x / d, vec.y / d};
}

float vprod(const struct vec_t a, const struct vec_t b) {
    return a.x * b.x + a.y * b.y;
}

float vlen(const struct vec_t vec) {
    return sqrtf(vlen2(vec));
}

float vlen2(const struct vec_t vec) {
    return vprod(vec, vec);
}

struct vec_t vfromangle(const float angle) {
    return (struct vec_t) {cosf(angle), sinf(angle)};
}

struct vec_t vnorm(const struct vec_t vec) {
    return vdiv(vec, vlen(vec));
}

struct vec_t vnorm_weak(const struct vec_t vec) {
    const float length = vlen(vec);

    if (isclose(length, 0.0F)) {
        return vzero;
    }

    return vdiv(vec, length);
}

float vdist(const struct vec_t a, const struct vec_t b) {
    return sqrtf(vdist2(a, b));
}

float vdist2(const struct vec_t a, const struct vec_t b) {
    return powf(b.x - a.x, 2.0F) + powf(b.y - a.y, 2.0F);
}

float vangle(const struct vec_t a, const struct vec_t b) {
    const float length1 = vlen(a);

    if (isclose(length1, 0.0F)) {
        return NAN;
    }

    const float length2 = vlen(b);

    if (isclose(length2, 0.0F)) {
        return NAN;
    }

    const float product = vprod(a, b);
    return acosf(product / (length1 * length2));
}

struct vec_t vrotate(const struct vec_t vec, const float angle) {
    const float sin_a = sinf(angle);
    const float cos_a = cosf(angle);

    return (struct vec_t) {vec.x * cos_a - vec.y * sin_a, vec.x * sin_a + vec.y * cos_a};
}

struct vec_t vlerp(const struct vec_t a, const struct vec_t b, const float t) {
    return (struct vec_t) {lerp(a.x, b.x, t), lerp(a.y, b.y, t)};
}

struct vec_t vscale(const struct vec_t vec, const float length) {
    return vmul(vnorm_weak(vec), length);
}

struct vec_t vreflect(const struct vec_t vec, const struct vec_t normal) {
    return vsub(vec, vscale(normal, 2.0F * vprod(vec, normal)));
}

struct vec_t vproject(const struct vec_t normal, const struct vec_t vec) {
    return vscale(normal, vprod(vec, normal) / vprod(normal, normal));
}
