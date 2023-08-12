#include <float.h>
#include <stdlib.h>


#include "math.h"
#include "vector.h"


#include "test.h"


private float randf(void) {
    return (float) rand() / (float) RAND_MAX;
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

TEST(test_min, {
    assert_equals(min(0, 0), 0);
    assert_equals(min(0, 1), 0);
    assert_equals(min(1, 0), 0);
    assert_equals(min(1, 1), 1);
    assert_equals(min(0, -1), -1);
    assert_equals(min(-1, 0), -1);
    assert_equals(min(-1, -1), -1);

    assert_is_close(min(0.0F, 0.0F), 0.0F);
    assert_is_close(min(0.0F, 1.0F), 0.0F);
    assert_is_close(min(1.0F, 0.0F), 0.0F);
    assert_is_close(min(1.0F, 1.0F), 1.0F);
    assert_is_close(min(0.0F, -1.0F), -1.0F);
    assert_is_close(min(-1.0F, 0.0F), -1.0F);

})

TEST(test_min_rand, {
    const int a = rand();
    const int b = rand();

    if (a == b) {
        assert(min(a, b) == a || min(a, b) == b);
        assert(min(b, a) == a || min(b, a) == b);
    } else if (a < b) {
        assert_equals(min(a, b), a);
        assert_equals(min(b, a), a);
    } else {
        assert_equals(min(a, b), b);
        assert_equals(min(b, a), b);
    }
})

TEST(test_max, {
    assert_equals(max(0, 0), 0);
    assert_equals(max(0, 1), 1);
    assert_equals(max(1, 0), 1);
    assert_equals(max(1, 1), 1);
    assert_equals(max(0, -1), 0);
    assert_equals(max(-1, 0), 0);
    assert_equals(max(-1, -1), -1);

    assert_is_close(max(0.0F, 0.0F), 0.0F);
    assert_is_close(max(0.0F, 1.0F), 1.0F);
    assert_is_close(max(1.0F, 0.0F), 1.0F);
    assert_is_close(max(1.0F, 1.0F), 1.0F);
    assert_is_close(max(0.0F, -1.0F), 0.0F);
    assert_is_close(max(-1.0F, 0.0F), 0.0F);
    assert_is_close(max(-1.0F, -1.0F), -1.0F);
})

TEST(test_max_rand, {
    const int a = rand();
    const int b = rand();

    if (a == b) {
        assert(max(a, b) == a || max(a, b) == b);
        assert(max(b, a) == a || max(b, a) == b);
    } else if (a < b) {
        assert_equals(max(a, b), b);
        assert_equals(max(b, a), b);
    } else {
        assert_equals(max(a, b), a);
        assert_equals(max(b, a), a);
    }
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
    assert_equals(constrain(500.0F, 0.0F, 1.0F), 1.0F);
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
    assert_is_close(degrees((float) M_PI / 2.0F), 90.0F);
    assert_is_close(degrees((float) M_PI / 3.0F), 60.0F);
    assert_is_close(degrees((float) M_PI / 4.0F), 45.0F);
    assert_is_close(degrees(0.0F), 0.0F);
    assert_is_close(degrees((float) M_PI), 180.0F);
    assert_is_close(degrees(2.0F * (float) M_PI), 360.0F);

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
    assert_is_close(radians(90.0F), (float) M_PI / 2.0F);
    assert_is_close(radians(60.0F), (float) M_PI / 3.0F);
    assert_is_close(radians(45.0F), (float) M_PI / 4.0F);
    assert_is_close(radians(0.0F), 0.0F);
    assert_is_close(radians(180.0F), (float) M_PI);
    assert_is_close(radians(360.0F), 2.0F * (float) M_PI);

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

TEST(test_vector_zero, {
    assert_equals(vzero.x, 0.0F);
    assert_equals(vzero.y, 0.0F);
})

TEST(test_vector_clear_rand, {
    struct vec_t vec;
    vec.x = randf();
    vec.y = randf();

    assert_equals(vclear(&vec), &vec);
    assert_equals(vec.x, 0.0F);
    assert_equals(vec.y, 0.0F);
})

TEST(test_vector_add_rand, {
    struct vec_t vec1;
    struct vec_t vec2;

    const float x1 = randf();
    const float y1 = randf();
    const float x2 = randf();
    const float y2 = randf();

    vec1.x = x1;
    vec1.y = y1;
    vec2.x = x2;
    vec2.y = y2;

    assert_equals(vadd(&vec1, &vec2), &vec1);

    assert_is_close(vec2.x, x2);
    assert_is_close(vec2.y, y2);

    assert_is_close(vec1.x, x1 + x2);
    assert_is_close(vec1.y, y1 + y2);
})

TEST(test_vector_sub_rand, {
    struct vec_t vec1;
    struct vec_t vec2;

    const float x1 = randf();
    const float y1 = randf();
    const float x2 = randf();
    const float y2 = randf();

    vec1.x = x1;
    vec1.y = y1;
    vec2.x = x2;
    vec2.y = y2;

    assert_equals(vsub(&vec1, &vec2), &vec1);

    assert_is_close(vec2.x, x2);
    assert_is_close(vec2.y, y2);

    assert_is_close(vec1.x, x1 - x2);
    assert_is_close(vec1.y, y1 - y2);
})

TEST(test_vector_mul_rand, {
    struct vec_t vec;

    const float x = randf();
    const float y = randf();
    const float m = randf();

    vec.x = x;
    vec.y = y;

    assert_equals(vmul(&vec, m), &vec);

    assert_is_close(vec.x, x * m);
    assert_is_close(vec.y, y * m);
})

TEST(test_vector_div_rand, {
    struct vec_t vec;

    const float x = randf();
    const float y = randf();
    float d;

    do {
        d = randf();
    } while (d == 0.0F);

    vec.x = x;
    vec.y = y;

    assert_equals(vdiv(&vec, d), &vec);

    assert_is_close(vec.x, x / d);
    assert_is_close(vec.y, y / d);
})

TEST(test_vector_product_rand, {
    struct vec_t vec1;
    struct vec_t vec2;

    const float x1 = randf();
    const float y1 = randf();
    const float x2 = randf();
    const float y2 = randf();

    vec1.x = x1;
    vec1.y = y1;
    vec2.x = x2;
    vec2.y = y2;

    assert_is_close(vprod(&vec1, &vec2), x1 * x2 + y1 * y2);

    assert_is_close(vec1.x, x1);
    assert_is_close(vec1.y, y1);
    assert_is_close(vec2.x, x2);
    assert_is_close(vec2.y, y2);
})

TEST(test_vector_length_rand, {
    struct vec_t vec;

    const float x = randf();
    const float y = randf();

    vec.x = x;
    vec.y = y;
    assert_is_close(vlen(&vec), sqrtf(x * x + y * y));

    vec.x = 0.0F;
    vec.y = 0.0F;
    assert_is_close(vlen(&vec), 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;
    assert_is_close(vlen(&vec), 1.0F);

    vec.x = 0.0F;
    vec.y = 1.0F;
    assert_is_close(vlen(&vec), 1.0F);

    vec.x = 1.0F;
    vec.y = 1.0F;
    assert_is_close(vlen(&vec), sqrtf(2.0F));
})

TEST(test_vector_length2_rand, {
    struct vec_t vec;

    const float x = randf();
    const float y = randf();

    vec.x = x;
    vec.y = y;
    assert_is_close(vlen2(&vec), x * x + y * y);

    vec.x = 0.0F;
    vec.y = 0.0F;
    assert_is_close(vlen2(&vec), 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;
    assert_is_close(vlen2(&vec), 1.0F);

    vec.x = 0.0F;
    vec.y = 1.0F;
    assert_is_close(vlen2(&vec), 1.0F);

    vec.x = 1.0F;
    vec.y = 1.0F;
    assert_is_close(vlen2(&vec), 2.0F);
})

TEST(test_vector_from_angle, {
    struct vec_t vec;

    assert_equals(vfromangle(&vec, 0.0F), &vec);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);

    assert_equals(vfromangle(&vec, (float) M_PI / 2.0F), &vec);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, 1.0F);

    assert_equals(vfromangle(&vec, (float) M_PI), &vec);
    assert_is_close(vec.x, -1.0F);
    assert_is_close(vec.y, 0.0F);

    assert_equals(vfromangle(&vec, 3.0F * (float) M_PI / 2.0F), &vec);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, -1.0F);

    assert_equals(vfromangle(&vec, 2.0F * (float) M_PI), &vec);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);
})

TEST(test_vector_normalize_rand, {
    struct vec_t vec;

    float x;
    float y;

    do {
        x = randf();
        y = randf();
    } while (x == 0.0F || y == 0.0F);

    vec.x = x;
    vec.y = y;

    const float length_before = vlen(&vec);

    assert_equals(vnorm(&vec), &vec);

    const float length_after = vlen(&vec);

    assert_is_close(length_after, 1.0F);
    assert_is_close(vec.x, x / length_before);
    assert_is_close(vec.y, y / length_before);
})

TEST(test_vector_normalize_weak_rand, {
    struct vec_t vec;

    vec.x = 0.0F;
    vec.y = 0.0F;
    assert_equals(vnorm_weak(&vec), &vec);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, 0.0F);
    assert_is_close(vlen(&vec), 0.0F);

    const float x = randf();
    const float y = randf();

    vec.x = x;
    vec.y = y;

    const float length_before = vlen(&vec);

    assert_equals(vnorm_weak(&vec), &vec);

    const float length_after = vlen(&vec);

    assert_is_close(length_after, 1.0F);
    assert_is_close(vec.x, x / length_before);
    assert_is_close(vec.y, y / length_before);
})

TEST(test_vector_distance, {
    struct vec_t vec1;
    struct vec_t vec2;

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 0.0F;

    assert_is_close(vdist(&vec1, &vec2), 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    assert_is_close(vdist(&vec1, &vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 1.0F;

    assert_is_close(vdist(&vec1, &vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 1.0F;

    assert_is_close(vdist(&vec1, &vec2), sqrtf(2.0F));

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = -1.0F;
    vec2.y = 0.0F;

    assert_is_close(vdist(&vec1, &vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = -1.0F;

    assert_is_close(vdist(&vec1, &vec2), 1.0F);
})

TEST(test_vector_distance2, {
    struct vec_t vec1;
    struct vec_t vec2;

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 0.0F;

    assert_is_close(vdist2(&vec1, &vec2), 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    assert_is_close(vdist2(&vec1, &vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 1.0F;

    assert_is_close(vdist2(&vec1, &vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 1.0F;

    assert_is_close(vdist2(&vec1, &vec2), 2.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = -1.0F;
    vec2.y = 0.0F;

    assert_is_close(vdist2(&vec1, &vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = -1.0F;

    assert_is_close(vdist2(&vec1, &vec2), 1.0F);
})

TEST(test_vector_angle_to, {
    struct vec_t vec1;
    struct vec_t vec2;

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 0.0F;

    assert_nan(vangle(&vec1, &vec2));

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    assert_nan(vangle(&vec1, &vec2));

    vec1.x = 1.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 1.0F;

    assert_is_close(vangle(&vec1, &vec2), (float) M_PI / 2.0F);

    vec1.x = 1.0F;
    vec1.y = 0.0F;
    vec2.x = -1.0F;
    vec2.y = 0.0F;

    assert_is_close(vangle(&vec1, &vec2), (float) M_PI);
})

TEST(test_vector_copy_rand, {
    struct vec_t vec1;
    struct vec_t vec2;

    vec1.x = randf();
    vec1.y = randf();

    assert_equals(vcopy(&vec2, &vec1), &vec2);

    assert_is_close(vec1.x, vec2.x);
    assert_is_close(vec1.y, vec2.y);
})

TEST(test_vector_rotate, {
    struct vec_t vec;

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vrotate(&vec, 0.0F), &vec);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vrotate(&vec, (float) M_PI / 2.0F), &vec);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, 1.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vrotate(&vec, (float) M_PI), &vec);
    assert_is_close(vec.x, -1.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vrotate(&vec, 3.0F * (float) M_PI / 2.0F), &vec);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, -1.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vrotate(&vec, 2.0F * (float) M_PI), &vec);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);
})

TEST(test_vector_lerp, {
    struct vec_t vec1;
    struct vec_t vec2;

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    assert_equals(vlerp(&vec1, &vec2, 0.0F), &vec1);
    assert_is_close(vec1.x, 0.0F);
    assert_is_close(vec1.y, 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    assert_equals(vlerp(&vec1, &vec2, 0.5F), &vec1);
    assert_is_close(vec1.x, 0.5F);
    assert_is_close(vec1.y, 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    assert_equals(vlerp(&vec1, &vec2, 1.0F), &vec1);
    assert_is_close(vec1.x, 1.0F);
    assert_is_close(vec1.y, 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    assert_equals(vlerp(&vec1, &vec2, 0.0F), &vec1);
    assert_is_close(vec1.x, 0.0F);
    assert_is_close(vec1.y, 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 1.0F;

    assert_equals(vlerp(&vec1, &vec2, 0.0F), &vec1);
    assert_is_close(vec1.x, 0.0F);
    assert_is_close(vec1.y, 0.0F);
})

TEST(test_vector_scale, {
    struct vec_t vec;

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vscale(&vec, 0.0F), &vec);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vscale(&vec, 0.5F), &vec);
    assert_is_close(vec.x, 0.5F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vscale(&vec, 1.0F), &vec);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vscale(&vec, 2.0F), &vec);
    assert_is_close(vec.x, 2.0F);
    assert_is_close(vec.y, 0.0F);
})

TEST(test_vector_reflect, {
    struct vec_t vec;
    struct vec_t normal;

    vec.x = 1.0F;
    vec.y = 0.0F;

    normal.x = 0.0F;
    normal.y = 1.0F;

    assert_equals(vreflect(&vec, &normal), &vec);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    normal.x = 1.0F;
    normal.y = 0.0F;

    assert_equals(vreflect(&vec, &normal), &vec);
    assert_is_close(vec.x, -1.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 0.0F;
    vec.y = 0.0F;

    normal.x = 1.0F;
    normal.y = 0.0F;

    assert_equals(vreflect(&vec, &normal), &vec);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 1.0F;

    normal.x = 1.0F;
    normal.y = 0.0F;

    assert_equals(vreflect(&vec, &normal), &vec);
    assert_is_close(vec.x, -1.0F);
    assert_is_close(vec.y, 1.0F);
})

TEST(test_vector_project, {
    struct vec_t vec;
    struct vec_t normal;

    vec.x = 1.0F;
    vec.y = 0.0F;

    normal.x = 0.0F;
    normal.y = 1.0F;

    assert_equals(vproject(&vec, &normal), &vec);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, 0.0F);
})


RUN_TESTS(
        ADD_TEST(test_isclose),
        ADD_TEST(test_min),
        ADD_TEST(test_min_rand),
        ADD_TEST(test_max),
        ADD_TEST(test_max_rand),
        ADD_TEST(test_map),
        ADD_TEST(test_constrain),
        ADD_TEST(test_lerp),
        ADD_TEST(test_degrees),
        ADD_TEST(test_radians),
        ADD_TEST(test_vector_zero),
        ADD_TEST(test_vector_clear_rand),
        ADD_TEST(test_vector_add_rand),
        ADD_TEST(test_vector_sub_rand),
        ADD_TEST(test_vector_mul_rand),
        ADD_TEST(test_vector_div_rand),
        ADD_TEST(test_vector_product_rand),
        ADD_TEST(test_vector_length_rand),
        ADD_TEST(test_vector_length2_rand),
        ADD_TEST(test_vector_from_angle),
        ADD_TEST(test_vector_normalize_rand),
        ADD_TEST(test_vector_normalize_weak_rand),
        ADD_TEST(test_vector_distance),
        ADD_TEST(test_vector_distance2),
        ADD_TEST(test_vector_angle_to),
        ADD_TEST(test_vector_copy_rand),
        ADD_TEST(test_vector_rotate),
        ADD_TEST(test_vector_lerp),
        ADD_TEST(test_vector_scale),
        ADD_TEST(test_vector_reflect),
        ADD_TEST(test_vector_project)
)
