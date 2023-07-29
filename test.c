#include <float.h>
#include <stdlib.h>


#include "math.h"
#include "vector.h"


#include "test.h"


unused private float rand_float(void) {
    return (float) rand() / (float) RAND_MAX;
}


TEST(test_isclose, {
    assert(isclose(0, 0));
    assert(!isclose(0, 1));
    assert(isclose(0.0F, 0.0F));
    assert(!isclose(0.0F, 0.1F));
    assert(!isclose(1.0F, 1.0F + FLT_EPSILON));
    assert(!isclose(1.0F, 1.0F - FLT_EPSILON));
    assert(!isclose(1.0F, 1.0F + FLT_EPSILON - 0.00001F));
    assert(!isclose(1.0F, 1.0F - FLT_EPSILON + 0.00001F));
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
    assert_is_close(map(-1.0F, -1.0F, 1.0F, 0.0F, 1.0F), 0.0F);
    assert_is_close(map(0.0F, -1.0F, 1.0F, 0.0F, 1.0F), 0.5F);
    assert_is_close(map(1.0F, -1.0F, 1.0F, 0.0F, 1.0F), 1.0F);
    assert_is_close(map(-1.0F, -1.0F, 1.0F, 0.0F, 2.0F), 0.0F);
    assert_is_close(map(0.0F, -1.0F, 1.0F, 0.0F, 2.0F), 1.0F);
    assert_is_close(map(1.0F, -1.0F, 1.0F, 0.0F, 2.0F), 2.0F);
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
    assert_equals(vector_zero.x, 0.0F);
    assert_equals(vector_zero.y, 0.0F);
})

TEST(test_vector_clear_rand, {
    struct vector_t vec;
    vec.x = rand_float();
    vec.y = rand_float();

    assert_equals(vector_clear(&vec), &vec);
    assert_equals(vec.x, 0.0F);
    assert_equals(vec.y, 0.0F);
})

TEST(test_vector_add_rand, {
    struct vector_t vec1;
    struct vector_t vec2;

    const float x1 = rand_float();
    const float y1 = rand_float();
    const float x2 = rand_float();
    const float y2 = rand_float();

    vec1.x = x1;
    vec1.y = y1;
    vec2.x = x2;
    vec2.y = y2;

    assert_equals(vector_add(&vec1, &vec2), &vec1);

    assert_is_close(vec2.x, x2);
    assert_is_close(vec2.y, y2);

    assert_is_close(vec1.x, x1 + x2);
    assert_is_close(vec1.y, y1 + y2);
})

TEST(test_vector_sub_rand, {
    struct vector_t vec1;
    struct vector_t vec2;

    const float x1 = rand_float();
    const float y1 = rand_float();
    const float x2 = rand_float();
    const float y2 = rand_float();

    vec1.x = x1;
    vec1.y = y1;
    vec2.x = x2;
    vec2.y = y2;

    assert_equals(vector_sub(&vec1, &vec2), &vec1);

    assert_is_close(vec2.x, x2);
    assert_is_close(vec2.y, y2);

    assert_is_close(vec1.x, x1 - x2);
    assert_is_close(vec1.y, y1 - y2);
})

TEST(test_vector_mul_rand, {
    struct vector_t vec;

    const float x = rand_float();
    const float y = rand_float();
    const float m = rand_float();

    vec.x = x;
    vec.y = y;

    assert_equals(vector_mul(&vec, m), &vec);

    assert_is_close(vec.x, x * m);
    assert_is_close(vec.y, y * m);
})

TEST(test_vector_div_rand, {
    struct vector_t vec;

    const float x = rand_float();
    const float y = rand_float();
    const float d = rand_float();

    vec.x = x;
    vec.y = y;

    assert_equals(vector_div(&vec, d), &vec);

    assert_is_close(vec.x, x / d);
    assert_is_close(vec.y, y / d);
})

TEST(test_vector_product_rand, {
    struct vector_t vec1;
    struct vector_t vec2;

    const float x1 = rand_float();
    const float y1 = rand_float();
    const float x2 = rand_float();
    const float y2 = rand_float();

    vec1.x = x1;
    vec1.y = y1;
    vec2.x = x2;
    vec2.y = y2;

    assert_is_close(vector_product(&vec1, &vec2), x1 * x2 + y1 * y2);

    assert_is_close(vec1.x, x1);
    assert_is_close(vec1.y, y1);
    assert_is_close(vec2.x, x2);
    assert_is_close(vec2.y, y2);
})

TEST(test_vector_length_rand, {
    struct vector_t vec;

    const float x = rand_float();
    const float y = rand_float();

    vec.x = x;
    vec.y = y;
    assert_is_close(vector_length(&vec), sqrtf(x * x + y * y));

    vec.x = 0.0F;
    vec.y = 0.0F;
    assert_is_close(vector_length(&vec), 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;
    assert_is_close(vector_length(&vec), 1.0F);

    vec.x = 0.0F;
    vec.y = 1.0F;
    assert_is_close(vector_length(&vec), 1.0F);

    vec.x = 1.0F;
    vec.y = 1.0F;
    assert_is_close(vector_length(&vec), sqrtf(2.0F));
})

TEST(test_vector_length2_rand, {
    struct vector_t vec;

    const float x = rand_float();
    const float y = rand_float();

    vec.x = x;
    vec.y = y;
    assert_is_close(vector_length2(&vec), x * x + y * y);

    vec.x = 0.0F;
    vec.y = 0.0F;
    assert_is_close(vector_length2(&vec), 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;
    assert_is_close(vector_length2(&vec), 1.0F);

    vec.x = 0.0F;
    vec.y = 1.0F;
    assert_is_close(vector_length2(&vec), 1.0F);

    vec.x = 1.0F;
    vec.y = 1.0F;
    assert_is_close(vector_length2(&vec), 2.0F);
})

TEST(test_vector_from_angle, {
    struct vector_t vec;

    assert_equals(vector_from_angle(&vec, 0.0F), &vec);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);

    assert_equals(vector_from_angle(&vec, (float) M_PI / 2.0F), &vec);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, 1.0F);

    assert_equals(vector_from_angle(&vec, (float) M_PI), &vec);
    assert_is_close(vec.x, -1.0F);
    assert_is_close(vec.y, 0.0F);

    assert_equals(vector_from_angle(&vec, 3.0F * (float) M_PI / 2.0F), &vec);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, -1.0F);

    assert_equals(vector_from_angle(&vec, 2.0F * (float) M_PI), &vec);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);
})

TEST(test_vector_normalize_rand, {
    struct vector_t vec;

    const float x = rand_float();
    const float y = rand_float();

    vec.x = x == 0.0F ? 1.0F : x; // avoid division by zero
    vec.y = y == 0.0F ? 1.0F : y;

    const float length_before = vector_length(&vec);

    assert_equals(vector_normalize(&vec), &vec);

    const float length_after = vector_length(&vec);

    assert_is_close(length_after, 1.0F);
    assert_is_close(vec.x, x / length_before);
    assert_is_close(vec.y, y / length_before);
})

TEST(test_vector_normalize_weak_rand, {
    struct vector_t vec;

    vec.x = 0.0F;
    vec.y = 0.0F;
    assert_equals(vector_normalize_weak(&vec), &vec);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, 0.0F);
    assert_is_close(vector_length(&vec), 0.0F);

    const float x = rand_float();
    const float y = rand_float();

    vec.x = x;
    vec.y = y;

    const float length_before = vector_length(&vec);

    assert_equals(vector_normalize_weak(&vec), &vec);

    const float length_after = vector_length(&vec);

    assert_is_close(length_after, 1.0F);
    assert_is_close(vec.x, x / length_before);
    assert_is_close(vec.y, y / length_before);
})

TEST(test_vector_distance, {
    struct vector_t vec1;
    struct vector_t vec2;

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 0.0F;

    assert_is_close(vector_distance(&vec1, &vec2), 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    assert_is_close(vector_distance(&vec1, &vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 1.0F;

    assert_is_close(vector_distance(&vec1, &vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 1.0F;

    assert_is_close(vector_distance(&vec1, &vec2), sqrtf(2.0F));

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = -1.0F;
    vec2.y = 0.0F;

    assert_is_close(vector_distance(&vec1, &vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = -1.0F;

    assert_is_close(vector_distance(&vec1, &vec2), 1.0F);
})

TEST(test_vector_distance2, {
    struct vector_t vec1;
    struct vector_t vec2;

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 0.0F;

    assert_is_close(vector_distance2(&vec1, &vec2), 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    assert_is_close(vector_distance2(&vec1, &vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 1.0F;

    assert_is_close(vector_distance2(&vec1, &vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 1.0F;

    assert_is_close(vector_distance2(&vec1, &vec2), 2.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = -1.0F;
    vec2.y = 0.0F;

    assert_is_close(vector_distance2(&vec1, &vec2), 1.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = -1.0F;

    assert_is_close(vector_distance2(&vec1, &vec2), 1.0F);
})

TEST(test_vector_angle_to, {
    struct vector_t vec1;
    struct vector_t vec2;

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 0.0F;

    assert_is_close(vector_angle_to(&vec1, &vec2), 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    assert_is_close(vector_angle_to(&vec1, &vec2), 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 1.0F;

    assert_is_close(vector_angle_to(&vec1, &vec2), (float) M_PI / 2.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = -1.0F;
    vec2.y = 0.0F;

    assert_is_close(vector_angle_to(&vec1, &vec2), (float) M_PI);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = -1.0F;

    assert_is_close(vector_angle_to(&vec1, &vec2), 3.0F * (float) M_PI / 2.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 1.0F;

    assert_is_close(vector_angle_to(&vec1, &vec2), (float) M_PI / 4.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = -1.0F;
    vec2.y = 1.0F;

    assert_is_close(vector_angle_to(&vec1, &vec2), 3.0F * (float) M_PI / 4.0F);
})

TEST(test_vector_copy_rand, {
    struct vector_t vec1;
    struct vector_t vec2;

    vec1.x = rand_float();
    vec1.y = rand_float();

    assert_equals(vector_copy(&vec2, &vec1), &vec2);

    assert_is_close(vec1.x, vec2.x);
    assert_is_close(vec1.y, vec2.y);
})

TEST(test_vector_rotate, {
    struct vector_t vec;

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vector_rotate(&vec, 0.0F), &vec);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vector_rotate(&vec, (float) M_PI / 2.0F), &vec);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, 1.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vector_rotate(&vec, (float) M_PI), &vec);
    assert_is_close(vec.x, -1.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vector_rotate(&vec, 3.0F * (float) M_PI / 2.0F), &vec);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, -1.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vector_rotate(&vec, 2.0F * (float) M_PI), &vec);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);
})

TEST(test_vector_lerp, {
    struct vector_t vec1;
    struct vector_t vec2;

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    assert_equals(vector_lerp(&vec1, &vec2, 0.0F), &vec1);
    assert_is_close(vec1.x, 0.0F);
    assert_is_close(vec1.y, 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    assert_equals(vector_lerp(&vec1, &vec2, 0.5F), &vec1);
    assert_is_close(vec1.x, 0.5F);
    assert_is_close(vec1.y, 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    assert_equals(vector_lerp(&vec1, &vec2, 1.0F), &vec1);
    assert_is_close(vec1.x, 1.0F);
    assert_is_close(vec1.y, 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 1.0F;
    vec2.y = 0.0F;

    assert_equals(vector_lerp(&vec1, &vec2, 0.0F), &vec1);
    assert_is_close(vec1.x, 0.0F);
    assert_is_close(vec1.y, 0.0F);

    vec1.x = 0.0F;
    vec1.y = 0.0F;
    vec2.x = 0.0F;
    vec2.y = 1.0F;

    assert_equals(vector_lerp(&vec1, &vec2, 0.0F), &vec1);
    assert_is_close(vec1.x, 0.0F);
    assert_is_close(vec1.y, 0.0F);
})

TEST(test_vector_scale, {
    struct vector_t vec;

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vector_scale(&vec, 0.0F), &vec);
    assert_is_close(vec.x, 0.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vector_scale(&vec, 0.5F), &vec);
    assert_is_close(vec.x, 0.5F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vector_scale(&vec, 1.0F), &vec);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);

    vec.x = 1.0F;
    vec.y = 0.0F;

    assert_equals(vector_scale(&vec, 2.0F), &vec);
    assert_is_close(vec.x, 2.0F);
    assert_is_close(vec.y, 0.0F);
})

TEST(test_vector_reflect, {
    struct vector_t vec;
    struct vector_t normal;

    vec.x = 1.0F;
    vec.y = 0.0F;

    normal.x = 0.0F;
    normal.y = 1.0F;

    assert_equals(vector_reflect(&vec, &normal), &vec);
    assert_is_close(vec.x, 1.0F);
    assert_is_close(vec.y, 0.0F);

    // TODO: add more tests
})

TEST(test_vector_project, {
    struct vector_t vec;
    struct vector_t normal;

    vec.x = 1.0F;
    vec.y = 0.0F;

    normal.x = 0.0F;
    normal.y = 1.0F;

    assert_equals(vector_project(&vec, &normal), &vec);
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
