#include <math.h>

#include "math.h"

#include "vector.h"


const struct vec_t vzero = {0, 0};


struct vec_t *vclear(struct vec_t *const vec) {
    return vcopy(vec, &vzero);
}

struct vec_t *vadd(struct vec_t *const restrict dst, const struct vec_t *const restrict src) {
    dst->x += src->x;
    dst->y += src->y;
    return dst;
}

struct vec_t *vsub(struct vec_t *const restrict dst, const struct vec_t *const restrict src) {
    dst->x -= src->x;
    dst->y -= src->y;
    return dst;
}

struct vec_t *vmul(struct vec_t *const vec, const float x) {
    vec->x *= x;
    vec->y *= x;
    return vec;
}

struct vec_t *vdiv(struct vec_t *const vec, const float d) {
    vec->x /= d;
    vec->y /= d;
    return vec;
}

float vprod(const struct vec_t *const restrict vec1, const struct vec_t *const restrict vec2) {
    return vec1->x * vec2->x + vec1->y * vec2->y;
}

float vlen(const struct vec_t *const vec) {
    return sqrtf(vlen2(vec));
}

float vlen2(const struct vec_t *const vec) {
    return vprod(vec, vec);
}


struct vec_t *vfromangle(struct vec_t *const dst, const float angle) {
    dst->x = cosf(angle);
    dst->y = sinf(angle);
    return dst;
}

struct vec_t *vnorm(struct vec_t *const vec) {
    return vdiv(vec, vlen(vec));
}

struct vec_t *vnorm_weak(struct vec_t *const vec) {
    const float length = vlen(vec);

    if (isclose(length, 0)) {
        vec->x = vec->y = 0;
        return vec;
    }

    return vdiv(vec, length);
}

float vdist(const struct vec_t *const restrict vec1, const struct vec_t *const restrict vec2) {
    return sqrtf(vdist2(vec1, vec2));
}

float vdist2(const struct vec_t *const restrict vec1, const struct vec_t *const restrict vec2) {
    return powf(vec2->x - vec1->x, 2) + powf(vec2->y - vec1->y, 2);
}

float vangle(const struct vec_t *const restrict vec1, const struct vec_t *const restrict vec2) {
    const float length1 = vlen(vec1);
    const float length2 = vlen(vec2);
    const float product = vprod(vec1, vec2);
    return acosf(product / (length1 * length2));
}

struct vec_t *vcopy(struct vec_t *const restrict dst, const struct vec_t *const restrict src) {
    return memcpy(dst, src, sizeof *dst);
}

struct vec_t *vrotate(struct vec_t *const vec, const float angle) {
    const float sin_a = sinf(angle), cos_a = cosf(angle);
    const float x = vec->x, y = vec->y;

    vec->x = x * cos_a - y * sin_a;
    vec->y = x * sin_a + y * cos_a;

    return vec;
}

struct vec_t *vlerp(struct vec_t *const restrict dst,
                    const struct vec_t *const restrict src,
                    const float t) {
    dst->x = lerp(dst->x, src->x, t);
    dst->y = lerp(dst->y, src->y, t);

    return dst;
}

struct vec_t *vscale(struct vec_t *const vec, const float length) {
    vnorm_weak(vec);
    vmul(vec, length);

    return vec;
}

struct vec_t *vreflect(struct vec_t *const restrict vec, const struct vec_t *const restrict normal) {
    struct vec_t *const ncopy = vcopy(vector(), normal);
    return vsub(vec, vscale(ncopy, 2.0F * vprod(vec, normal)));
}

struct vec_t *vproject(struct vec_t *const restrict normal, const struct vec_t *const restrict vec) {
    return vscale(normal, vprod(vec, normal) / vprod(normal, normal));
}
