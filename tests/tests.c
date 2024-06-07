#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/math.h"
#include "runner.h"


#define REPEATS 1e6F


static float randf(void) {
    return (float) rand() / (float) RAND_MAX;
}

static bool colors_equal(const SDL_Color a, const SDL_Color b) {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}


TEST(test_isclose, {
    assert(isclose(0.0F, 0.0F));
    assert(isclose(0.0F, 0.0F));
    assert(isclose(1.0F, 1.0F + FLT_EPSILON));
    assert(isclose(1.0F, 1.0F - FLT_EPSILON));

    assert_not(isclose(0.0F, 1.0F));
    assert_not(isclose(0.0F, 0.1F));
    assert_not(isclose(1.0F, 1.0F + FLT_EPSILON - 0.00001F));
    assert_not(isclose(1.0F, 1.0F - FLT_EPSILON + 0.00001F));
})

TEST(test_map, {
    assert_is_close(map(0.0F, 0.0F, 1.0F, 0.0F, 1.0F), 0.0F);
    assert_is_close(map(0.5F, 0.0F, 1.0F, 0.0F, 1.0F), 0.5F);
    assert_is_close(map(1.0F, 0.0F, 1.0F, 0.0F, 1.0F), 1.0F);
    assert_is_close(map(0.0F, 0.0F, 1.0F, 0.0F, 2.0F), 0.0F);
    assert_is_close(map(0.5F, 0.0F, 1.0F, 0.0F, 2.0F), 1.0F);
    assert_is_close(map(1.0F, 0.0F, 1.0F, 0.0F, 2.0F), 2.0F);
    assert_is_close(map(0.0F, 0.0F, 2.0F, 0.0F, 1.0F), 0.0F);
    assert_is_close(map(1.0F, 0.0F, 2.0F, 0.0F, 1.0F), 0.5F);
    assert_is_close(map(2.0F, 0.0F, 2.0F, 0.0F, 1.0F), 1.0F);
    assert_is_close(map(0.0F, 0.0F, 2.0F, 0.0F, 3.0F), 0.0F);
    assert_is_close(map(1.0F, 0.0F, 2.0F, 0.0F, 3.0F), 1.5F);
    assert_is_close(map(2.0F, 0.0F, 2.0F, 0.0F, 3.0F), 3.0F);
    assert_is_close(map(0.0F, 0.0F, 3.0F, 0.0F, 1.0F), 0.0F);
    assert_is_close(map(1.5F, 0.0F, 3.0F, 0.0F, 1.0F), 0.5F);
    assert_is_close(map(3.0F, 0.0F, 3.0F, 0.0F, 1.0F), 1.0F);
    assert_is_close(map(0.0F, 0.0F, 3.0F, 0.0F, 2.0F), 0.0F);
    assert_is_close(map(1.5F, 0.0F, 3.0F, 0.0F, 2.0F), 1.0F);
})

TEST(test_constrain, {
    assert_is_close(constrain(500.0F, 0.0F, 1.0F), 1.0F);
    assert_is_close(constrain(0.5F, 0.0F, 1.0F), 0.5F);
    assert_is_close(constrain(0.0F, 0.0F, 1.0F), 0.0F);
    assert_is_close(constrain(500.0F, 0.0F, 2.0F), 2.0F);
    assert_is_close(constrain(1.0F, 0.0F, 2.0F), 1.0F);
    assert_is_close(constrain(0.0F, 0.0F, 2.0F), 0.0F);
    assert_is_close(constrain(500.0F, 0.0F, 3.0F), 3.0F);
    assert_is_close(constrain(1.5F, 0.0F, 3.0F), 1.5F);
    assert_is_close(constrain(0.0F, 0.0F, 3.0F), 0.0F);
    assert_is_close(constrain(500.0F, 0.0F, 1.0F), 1.0F);
    assert_is_close(constrain(0.5F, 0.0F, 1.0F), 0.5F);
    assert_is_close(constrain(-1.0F, -1.0F, 1.0F), -1.0F);
    assert_is_close(constrain(0.0F, -1.0F, 1.0F), 0.0F);
    assert_is_close(constrain(1.0F, -1.0F, 1.0F), 1.0F);
    assert_is_close(constrain(-1.0F, -1.0F, 1.0F), -1.0F);
    assert_is_close(constrain(0.0F, -1.0F, 1.0F), 0.0F);
    assert_is_close(constrain(1.0F, -1.0F, 1.0F), 1.0F);
    assert_is_close(constrain(-1.0F, -1.0F, 2.0F), -1.0F);
})

TEST(test_lerp, {
    assert_is_close(lerp(0.0F, 1.0F, 0.0F), 0.0F);
    assert_is_close(lerp(0.0F, 1.0F, 0.5F), 0.5F);
    assert_is_close(lerp(0.0F, 1.0F, 1.0F), 1.0F);
    assert_is_close(lerp(0.0F, 2.0F, 0.0F), 0.0F);
    assert_is_close(lerp(0.0F, 2.0F, 0.5F), 1.0F);
    assert_is_close(lerp(0.0F, 2.0F, 1.0F), 2.0F);
    assert_is_close(lerp(0.0F, 3.0F, 0.0F), 0.0F);
    assert_is_close(lerp(0.0F, 3.0F, 0.5F), 1.5F);
    assert_is_close(lerp(0.0F, 3.0F, 1.0F), 3.0F);
    assert_is_close(lerp(0.0F, 1.0F, 0.0F), 0.0F);
    assert_is_close(lerp(0.0F, 1.0F, 0.5F), 0.5F);
    assert_is_close(lerp(-1.0F, 1.0F, 0.0F), -1.0F);
    assert_is_close(lerp(-1.0F, 1.0F, 0.5F), 0.0F);
    assert_is_close(lerp(-1.0F, 1.0F, 1.0F), 1.0F);
    assert_is_close(lerp(-1.0F, 1.0F, 0.0F), -1.0F);
})

TEST(test_degrees, {
    assert_is_close(degrees(PI / 2.0F), 90.0F);
    assert_is_close(degrees(PI / 3.0F), 60.0F);
    assert_is_close(degrees(PI / 4.0F), 45.0F);
    assert_is_close(degrees(0.0F), 0.0F);
    assert_is_close(degrees(PI), 180.0F);
    assert_is_close(degrees(2.0F * PI), 360.0F);

    assert_is_close(degrees(0.0F), 0.0F);
    assert_is_close(degrees(1.0F), 57.295776F);
    assert_is_close(degrees(2.0F), 114.591553F);
    assert_is_close(degrees(3.0F), 171.887329F);
    assert_is_close(degrees(4.0F), 229.183105F);
    assert_is_close(degrees(5.0F), 286.478882F);
    assert_is_close(degrees(6.0F), 343.774658F);
    assert_is_close(degrees(7.0F), 401.070435F);
    assert_is_close(degrees(8.0F), 458.366211F);
    assert_is_close(degrees(9.0F), 515.661987F);
    assert_is_close(degrees(10.0F), 572.957764F);
    assert_is_close(degrees(11.0F), 630.253540F);
    assert_is_close(degrees(12.0F), 687.549316F);
    assert_is_close(degrees(13.0F), 744.845093F);
    assert_is_close(degrees(14.0F), 802.140869F);
    assert_is_close(degrees(15.0F), 859.436646F);
    assert_is_close(degrees(16.0F), 916.732422F);
    assert_is_close(degrees(17.0F), 974.028198F);
    assert_is_close(degrees(18.0F), 1031.323975F);
    assert_is_close(degrees(19.0F), 1088.619751F);
    assert_is_close(degrees(20.0F), 1145.915527F);
    assert_is_close(degrees(21.0F), 1203.211304F);
    assert_is_close(degrees(22.0F), 1260.507080F);
    assert_is_close(degrees(23.0F), 1317.802856F);
})

TEST(test_radians, {
    assert_is_close(radians(90.0F), PI / 2.0F);
    assert_is_close(radians(60.0F), PI / 3.0F);
    assert_is_close(radians(45.0F), PI / 4.0F);
    assert_is_close(radians(0.0F), 0.0F);
    assert_is_close(radians(180.0F), PI);
    assert_is_close(radians(360.0F), 2.0F * PI);

    assert_is_close(radians(0.0F), 0.0F);
    assert_is_close(radians(57.295776F), 1.0F);
    assert_is_close(radians(114.591553F), 2.0F);
    assert_is_close(radians(171.887329F), 3.0F);
    assert_is_close(radians(229.183105F), 4.0F);
    assert_is_close(radians(286.478882F), 5.0F);
    assert_is_close(radians(343.774658F), 6.0F);
    assert_is_close(radians(401.070435F), 7.0F);
    assert_is_close(radians(458.366211F), 8.0F);
    assert_is_close(radians(515.661987F), 9.0F);
    assert_is_close(radians(572.957764F), 10.0F);
    assert_is_close(radians(630.253540F), 11.0F);
    assert_is_close(radians(687.549316F), 12.0F);
    assert_is_close(radians(744.845093F), 13.0F);
    assert_is_close(radians(802.140869F), 14.0F);
    assert_is_close(radians(859.436646F), 15.0F);
    assert_is_close(radians(916.732422F), 16.0F);
    assert_is_close(radians(974.028198F), 17.0F);
    assert_is_close(radians(1031.323975F), 18.0F);
    assert_is_close(radians(1088.619751F), 19.0F);
    assert_is_close(radians(1145.915527F), 20.0F);
    assert_is_close(radians(1203.211304F), 21.0F);
    assert_is_close(radians(1260.507080F), 22.0F);
    assert_is_close(radians(1317.802856F), 23.0F);
})

TEST(test_vzero, {
    assert_is_close(vzero.x, 0.0F);
    assert_is_close(vzero.y, 0.0F);
})

TEST(test_vadd_rand, {
    const struct vec_t vec1 = {randf(), randf()};
    const struct vec_t vec2 = {randf(), randf()};

    const struct vec_t result = vadd(vec1, vec2);

    assert_is_close(result.x, vec1.x + vec2.x);
    assert_is_close(result.y, vec1.y + vec2.y);
})

TEST(test_vsub_rand, {
    const struct vec_t vec1 = {randf(), randf()};
    const struct vec_t vec2 = {randf(), randf()};

    const struct vec_t result = vsub(vec1, vec2);

    assert_is_close(result.x, vec1.x - vec2.x);
    assert_is_close(result.y, vec1.y - vec2.y);
})

TEST(test_vmul_rand, {
    const struct vec_t vec = {randf(), randf()};
    const float m = randf();

    const struct vec_t result = vmul(vec, m);

    assert_is_close(result.x, vec.x * m);
    assert_is_close(result.y, vec.y * m);
})

TEST(test_vdiv_rand, {
    const struct vec_t vec = {randf(), randf()};

    float d;

    do {
        d = randf();
    } while (isclose(d, 0.0F));

    const struct vec_t result = vdiv(vec, d);

    assert_is_close(result.x, vec.x / d);
    assert_is_close(result.y, vec.y / d);
})

TEST(test_vprod_rand, {
    const struct vec_t vec1 = {randf(), randf()};
    const struct vec_t vec2 = {randf(), randf()};

    const float result = vprod(vec1, vec2);

    assert_is_close(result, vec1.x * vec2.x + vec1.y * vec2.y);
})

TEST(test_vlen_rand, {
    struct vec_t vec = {randf(), randf()};

    assert_is_close(vlen(vec), sqrtf(vec.x * vec.x + vec.y * vec.y));

    vec.x = 0.0F;
    vec.y = 0.0F;
    assert_is_close(vlen(vec), 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;
    assert_is_close(vlen(vec), 1.0F);

    vec.x = 0.0F;
    vec.y = 1.0F;
    assert_is_close(vlen(vec), 1.0F);

    vec.x = 1.0F;
    vec.y = 1.0F;
    assert_is_close(vlen(vec), sqrtf(2.0F));
})

TEST(test_vlen2_rand, {
    struct vec_t vec = {randf(), randf()};

    assert_is_close(vlen2(vec), vec.x * vec.x + vec.y * vec.y);

    vec.x = 0.0F;
    vec.y = 0.0F;
    assert_is_close(vlen2(vec), 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;
    assert_is_close(vlen2(vec), 1.0F);

    vec.x = 0.0F;
    vec.y = 1.0F;
    assert_is_close(vlen2(vec), 1.0F);

    vec.x = 1.0F;
    vec.y = 1.0F;
    assert_is_close(vlen2(vec), 2.0F);
})

TEST(test_vfromangle, {
    struct vec_t vec;

    vec = vfromangle(0.0F);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);

    vec = vfromangle(PI / 2.0F);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, 1.0F);

    vec = vfromangle(PI);
    assert_is_close(vec.x, -1.0F);
    assert_is_close(vec.y, 0.0F);

    vec = vfromangle(3.0F * PI / 2.0F);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, -1.0F);

    vec = vfromangle(2.0F * PI);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);
})

TEST(test_vnorm_rand, {
    struct vec_t vec;

    do {
        vec.x = randf();
        vec.y = randf();
    } while (isclose(vec.x, 0.0F) || isclose(vec.y, 0.0F));

    const float length_before = vlen(vec);
    const struct vec_t result = vnorm(vec);
    const float length_after = vlen(result);

    assert_is_close(length_after, 1.0F);
    assert_is_close(result.x, vec.x / length_before);
    assert_is_close(result.y, vec.y / length_before);
})

TEST(test_vnorm_weak_rand, {
    struct vec_t vec = vzero;
    struct vec_t result;

    result = vnorm_weak(vec);
    assert_is_close(result.x, 0.0F);
    assert_is_close(result.y, 0.0F);
    assert_is_close(vlen(result), 0.0F);

    vec = (struct vec_t) {randf(), randf()};

    const float length_before = vlen(vec);
    result = vnorm_weak(vec);
    const float length_after = vlen(result);

    assert_is_close(length_after, 1.0F);
    assert_is_close(result.x, vec.x / length_before);
    assert_is_close(result.y, vec.y / length_before);
})

TEST(test_vdist, {
    struct vec_t vec1;
    struct vec_t vec2;

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 0.0F;

    assert_is_close(vdist(vec1, vec2), 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    assert_is_close(vdist(vec1, vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 1.0F;

    assert_is_close(vdist(vec1, vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 1.0F;

    assert_is_close(vdist(vec1, vec2), sqrtf(2.0F));

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = -1.0F;
    vec2.y = 0.0F;

    assert_is_close(vdist(vec1, vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = -1.0F;

    assert_is_close(vdist(vec1, vec2), 1.0F);
})

TEST(test_vdist2, {
    struct vec_t vec1;
    struct vec_t vec2;

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 0.0F;

    assert_is_close(vdist2(vec1, vec2), 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    assert_is_close(vdist2(vec1, vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 1.0F;

    assert_is_close(vdist2(vec1, vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 1.0F;

    assert_is_close(vdist2(vec1, vec2), 2.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = -1.0F;
    vec2.y = 0.0F;

    assert_is_close(vdist2(vec1, vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = -1.0F;

    assert_is_close(vdist2(vec1, vec2), 1.0F);
})

TEST(test_vangle, {
    struct vec_t vec1;
    struct vec_t vec2;

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 0.0F;

    assert_nan(vangle(vec1, vec2));

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    assert_nan(vangle(vec1, vec2));

    vec1.x = 1.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 1.0F;

    assert_is_close(vangle(vec1, vec2), PI / 2.0F);

    vec1.x = 1.0F;
    vec1.y = 0.0F;
    vec2.x = -1.0F;
    vec2.y = 0.0F;

    assert_is_close(vangle(vec1, vec2), PI);
})

TEST(test_vrotate, {
    struct vec_t vec;

    vec.x = 1.0F;
    vec.y = 0.0F;

    vec = vrotate(vec, 0.0F);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    vec = vrotate(vec, PI / 2.0F);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, 1.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    vec = vrotate(vec, PI);
    assert_is_close(vec.x, -1.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    vec = vrotate(vec, 3.0F * PI / 2.0F);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, -1.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    vec = vrotate(vec, 2.0F * PI);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);
})

TEST(test_vlerp, {
    struct vec_t vec1;
    struct vec_t vec2;

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    vec1 = vlerp(vec1, vec2, 0.0F);
    assert_is_close(vec1.x, 0.0F);
    assert_is_close(vec1.y, 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    vec1 = vlerp(vec1, vec2, 0.5F);
    assert_is_close(vec1.x, 0.5F);
    assert_is_close(vec1.y, 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    vec1 = vlerp(vec1, vec2, 1.0F);
    assert_is_close(vec1.x, 1.0F);
    assert_is_close(vec1.y, 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    vec1 = vlerp(vec1, vec2, 0.0F);
    assert_is_close(vec1.x, 0.0F);
    assert_is_close(vec1.y, 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 1.0F;

    vec1 = vlerp(vec1, vec2, 0.0F);
    assert_is_close(vec1.x, 0.0F);
    assert_is_close(vec1.y, 0.0F);
})

TEST(test_vscale, {
    struct vec_t vec;

    vec.x = 1.0F;
    vec.y = 0.0F;

    vec = vscale(vec, 0.0F);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    vec = vscale(vec, 0.5F);
    assert_is_close(vec.x, 0.5F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    vec = vscale(vec, 1.0F);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    vec = vscale(vec, 2.0F);
    assert_is_close(vec.x, 2.0F);
    assert_is_close(vec.y, 0.0F);
})

TEST(test_vreflect, {
    struct vec_t vec;
    struct vec_t normal;

    vec.x = 1.0F;
    vec.y = 0.0F;

    normal.x = 0.0F;
    normal.y = 1.0F;

    vec = vreflect(vec, normal);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    normal.x = 1.0F;
    normal.y = 0.0F;

    vec = vreflect(vec, normal);
    assert_is_close(vec.x, -1.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 0.0F;
    vec.y = 0.0F;

    normal.x = 1.0F;
    normal.y = 0.0F;

    vec = vreflect(vec, normal);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 1.0F;

    normal.x = 1.0F;
    normal.y = 0.0F;

    vec = vreflect(vec, normal);
    assert_is_close(vec.x, -1.0F);
    assert_is_close(vec.y, 1.0F);
})

TEST(test_vproject, {
    struct vec_t vec;
    struct vec_t normal;

    vec.x = 1.0F;
    vec.y = 0.0F;

    normal.x = 0.0F;
    normal.y = 1.0F;

    vec = vproject(normal, vec);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, 0.0F);
})

static const struct {
    const SDL_Color input;
    const uint32_t output;
} test_color_to_int_cases[] = {
        {.input = rgba(0, 0, 0, 0), .output=0x00000000},
        {.input = rgba(0, 0, 0, 255), .output=0x000000FF},
        {.input = rgba(255, 255, 255, 255), .output=0xFFFFFFFF},
        {.input = rgba(255, 0, 0, 255), .output=0xFF0000FF},
        {.input = rgba(0, 255, 0, 255), .output=0x00FF00FF},
        {.input = rgba(0, 0, 255, 255), .output=0x0000FFFF},
        {.input = rgba(255, 255, 0, 255), .output=0xFFFF00FF},
        {.input = rgba(0, 255, 255, 255), .output=0x00FFFFFF},
        {.input = rgba(255, 0, 255, 255), .output=0xFF00FFFF},
};

TEST(test_color_to_int, {
    const size_t ncases = sizeof test_color_to_int_cases / sizeof *test_color_to_int_cases;

    for (size_t i = 0; i < ncases; i++) {
        const SDL_Color input = test_color_to_int_cases[i].input;
        const uint32_t output = test_color_to_int_cases[i].output;

        assert_equals(color_to_int(input), output);
    }
})

TEST(test_is_decimal_valid, {
    assert_true(is_decimal("0"));
    assert_true(is_decimal("1"));
    assert_true(is_decimal("2"));
    assert_true(is_decimal("3"));
    assert_true(is_decimal("4"));
    assert_true(is_decimal("5"));
    assert_true(is_decimal("6"));
    assert_true(is_decimal("7"));
    assert_true(is_decimal("8"));
    assert_true(is_decimal("9"));
    assert_true(is_decimal("10"));
    assert_true(is_decimal("16518651238516981"));
})

TEST(test_is_decimal_valid_rand, {
    static char buffer[100] = {0};

    snprintf(buffer, 100, "%d", rand());
    assert_true(is_decimal(buffer));
})

TEST(test_is_decimal_invalid, {
    assert_false(is_decimal(""));
    assert_false(is_decimal(" "));
    assert_false(is_decimal("a"));
    assert_false(is_decimal("0a"));
    assert_false(is_decimal("a0"));
    assert_false(is_decimal("0.0.0"));
    assert_false(is_decimal("0x0"));
    assert_false(is_decimal("0b0"));
    assert_false(is_decimal("-1"));
})

TEST(test_change_brightness, {
    assert_true(colors_equal(change_brightness((SDL_Color) rgb(0, 0, 0), 0.0F), (SDL_Color) rgb(0, 0, 0)));
    assert_true(colors_equal(change_brightness((SDL_Color) rgb(255, 255, 255), 1.0F), (SDL_Color) rgb(255, 255, 255)));
    assert_true(colors_equal(change_brightness((SDL_Color) rgb(255, 255, 255), 0.5F), (SDL_Color) rgb(127, 127, 127)));
    assert_true(colors_equal(change_brightness((SDL_Color) rgb(255, 255, 255), 0.1F), (SDL_Color) rgb(25, 25, 25)));
    assert_true(colors_equal(change_brightness((SDL_Color) rgb(255, 0, 0), 0.5F), (SDL_Color) rgb(127, 0, 0)));
    assert_true(colors_equal(change_brightness((SDL_Color) rgb(0, 255, 0), 0.5F), (SDL_Color) rgb(0, 127, 0)));
    assert_true(colors_equal(change_brightness((SDL_Color) rgb(0, 0, 255), 0.5F), (SDL_Color) rgb(0, 0, 127)));
})

TEST(test_is_whitespace, {
    assert_true(is_whitespace(""));
    assert_true(is_whitespace(" "));
    assert_true(is_whitespace("\t"));
    assert_true(is_whitespace("\n"));
    assert_true(is_whitespace("\r"));
    assert_true(is_whitespace("\f"));
    assert_true(is_whitespace("\v"));
    assert_true(is_whitespace(" \t\n\r\f\v"));

    assert_false(is_whitespace(NULL));
    assert_false(is_whitespace("a"));
    assert_false(is_whitespace("0"));
    assert_false(is_whitespace("!"));
    assert_false(is_whitespace("A"));
    assert_false(is_whitespace("Z"));
    assert_false(is_whitespace("["));
    assert_false(is_whitespace("`"));
    assert_false(is_whitespace("z"));
    assert_false(is_whitespace("{"));
    assert_false(is_whitespace("~"));
    assert_false(is_whitespace("a0!AZ[`z{~"));
})

TEST(test_hex_to_dec, {
    assert_equals(hex_to_dec('0'), 0);
    assert_equals(hex_to_dec('1'), 1);
    assert_equals(hex_to_dec('2'), 2);
    assert_equals(hex_to_dec('3'), 3);
    assert_equals(hex_to_dec('4'), 4);
    assert_equals(hex_to_dec('5'), 5);
    assert_equals(hex_to_dec('6'), 6);
    assert_equals(hex_to_dec('7'), 7);
    assert_equals(hex_to_dec('8'), 8);
    assert_equals(hex_to_dec('9'), 9);

    assert_equals(hex_to_dec('a'), 10);
    assert_equals(hex_to_dec('b'), 11);
    assert_equals(hex_to_dec('c'), 12);
    assert_equals(hex_to_dec('d'), 13);
    assert_equals(hex_to_dec('e'), 14);
    assert_equals(hex_to_dec('f'), 15);

    assert_equals(hex_to_dec('A'), 10);
    assert_equals(hex_to_dec('B'), 11);
    assert_equals(hex_to_dec('C'), 12);
    assert_equals(hex_to_dec('D'), 13);
    assert_equals(hex_to_dec('E'), 14);
    assert_equals(hex_to_dec('F'), 15);
})


RUN_TESTS(
        ADD_TEST(test_is_decimal_valid_rand, REPEATS),
        ADD_TEST(test_vadd_rand, REPEATS),
        ADD_TEST(test_vdiv_rand, REPEATS),
        ADD_TEST(test_vlen2_rand, REPEATS),
        ADD_TEST(test_vlen_rand, REPEATS),
        ADD_TEST(test_vmul_rand, REPEATS),
        ADD_TEST(test_vnorm_rand, REPEATS),
        ADD_TEST(test_vnorm_weak_rand, REPEATS),
        ADD_TEST(test_vprod_rand, REPEATS),
        ADD_TEST(test_vsub_rand, REPEATS),
        ADD_TEST(test_change_brightness),
        ADD_TEST(test_color_to_int),
        ADD_TEST(test_constrain),
        ADD_TEST(test_degrees),
        ADD_TEST(test_hex_to_dec),
        ADD_TEST(test_is_decimal_invalid),
        ADD_TEST(test_is_decimal_valid),
        ADD_TEST(test_is_whitespace),
        ADD_TEST(test_isclose),
        ADD_TEST(test_lerp),
        ADD_TEST(test_map),
        ADD_TEST(test_radians),
        ADD_TEST(test_vangle),
        ADD_TEST(test_vdist),
        ADD_TEST(test_vdist2),
        ADD_TEST(test_vfromangle),
        ADD_TEST(test_vlerp),
        ADD_TEST(test_vproject),
        ADD_TEST(test_vreflect),
        ADD_TEST(test_vrotate),
        ADD_TEST(test_vscale),
        ADD_TEST(test_vzero),
)
