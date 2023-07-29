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
    assert(isclose(0.0f, 0.0f));
    assert(!isclose(0.0f, 0.1f));
    assert(!isclose(1.0f, 1.0f + FLT_EPSILON));
    assert(!isclose(1.0f, 1.0f - FLT_EPSILON));
    assert(!isclose(1.0f, 1.0f + FLT_EPSILON - 0.00001f));
    assert(!isclose(1.0f, 1.0f - FLT_EPSILON + 0.00001f));
})

TEST(test_min, {
    assert_equals(min(0, 0), 0);
    assert_equals(min(0, 1), 0);
    assert_equals(min(1, 0), 0);
    assert_equals(min(1, 1), 1);
    assert_equals(min(0, -1), -1);
    assert_equals(min(-1, 0), -1);
    assert_equals(min(-1, -1), -1);

    assert_is_close(min(0.0f, 0.0f), 0.0f);
    assert_is_close(min(0.0f, 1.0f), 0.0f);
    assert_is_close(min(1.0f, 0.0f), 0.0f);
    assert_is_close(min(1.0f, 1.0f), 1.0f);
    assert_is_close(min(0.0f, -1.0f), -1.0f);
    assert_is_close(min(-1.0f, 0.0f), -1.0f);

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

    assert_is_close(max(0.0f, 0.0f), 0.0f);
    assert_is_close(max(0.0f, 1.0f), 1.0f);
    assert_is_close(max(1.0f, 0.0f), 1.0f);
    assert_is_close(max(1.0f, 1.0f), 1.0f);
    assert_is_close(max(0.0f, -1.0f), 0.0f);
    assert_is_close(max(-1.0f, 0.0f), 0.0f);
    assert_is_close(max(-1.0f, -1.0f), -1.0f);
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
    assert_is_close(map(0.0f, 0.0f, 1.0f, 0.0f, 1.0f), 0.0f);
    assert_is_close(map(0.5f, 0.0f, 1.0f, 0.0f, 1.0f), 0.5f);
    assert_is_close(map(1.0f, 0.0f, 1.0f, 0.0f, 1.0f), 1.0f);
    assert_is_close(map(0.0f, 0.0f, 1.0f, 0.0f, 2.0f), 0.0f);
    assert_is_close(map(0.5f, 0.0f, 1.0f, 0.0f, 2.0f), 1.0f);
    assert_is_close(map(1.0f, 0.0f, 1.0f, 0.0f, 2.0f), 2.0f);
    assert_is_close(map(0.0f, 0.0f, 2.0f, 0.0f, 1.0f), 0.0f);
    assert_is_close(map(1.0f, 0.0f, 2.0f, 0.0f, 1.0f), 0.5f);
    assert_is_close(map(2.0f, 0.0f, 2.0f, 0.0f, 1.0f), 1.0f);
    assert_is_close(map(0.0f, 0.0f, 2.0f, 0.0f, 3.0f), 0.0f);
    assert_is_close(map(1.0f, 0.0f, 2.0f, 0.0f, 3.0f), 1.5f);
    assert_is_close(map(2.0f, 0.0f, 2.0f, 0.0f, 3.0f), 3.0f);
    assert_is_close(map(0.0f, 0.0f, 3.0f, 0.0f, 1.0f), 0.0f);
    assert_is_close(map(1.5f, 0.0f, 3.0f, 0.0f, 1.0f), 0.5f);
    assert_is_close(map(3.0f, 0.0f, 3.0f, 0.0f, 1.0f), 1.0f);
    assert_is_close(map(0.0f, 0.0f, 3.0f, 0.0f, 2.0f), 0.0f);
    assert_is_close(map(1.5f, 0.0f, 3.0f, 0.0f, 2.0f), 1.0f);
    assert_is_close(map(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f), 0.0f);
    assert_is_close(map(0.0f, -1.0f, 1.0f, 0.0f, 1.0f), 0.5f);
    assert_is_close(map(1.0f, -1.0f, 1.0f, 0.0f, 1.0f), 1.0f);
    assert_is_close(map(-1.0f, -1.0f, 1.0f, 0.0f, 2.0f), 0.0f);
    assert_is_close(map(0.0f, -1.0f, 1.0f, 0.0f, 2.0f), 1.0f);
    assert_is_close(map(1.0f, -1.0f, 1.0f, 0.0f, 2.0f), 2.0f);
})

TEST(test_constrain, {
    assert_equals(constrain(500.0f, 0.0f, 1.0f), 1.0f);
    assert_is_close(constrain(0.5f, 0.0f, 1.0f), 0.5f);
    assert_is_close(constrain(0.0f, 0.0f, 1.0f), 0.0f);
    assert_is_close(constrain(500.0f, 0.0f, 2.0f), 2.0f);
    assert_is_close(constrain(1.0f, 0.0f, 2.0f), 1.0f);
    assert_is_close(constrain(0.0f, 0.0f, 2.0f), 0.0f);
    assert_is_close(constrain(500.0f, 0.0f, 3.0f), 3.0f);
    assert_is_close(constrain(1.5f, 0.0f, 3.0f), 1.5f);
    assert_is_close(constrain(0.0f, 0.0f, 3.0f), 0.0f);
    assert_is_close(constrain(500.0f, 0.0f, 1.0f), 1.0f);
    assert_is_close(constrain(0.5f, 0.0f, 1.0f), 0.5f);
    assert_is_close(constrain(-1.0f, -1.0f, 1.0f), -1.0f);
    assert_is_close(constrain(0.0f, -1.0f, 1.0f), 0.0f);
    assert_is_close(constrain(1.0f, -1.0f, 1.0f), 1.0f);
    assert_is_close(constrain(-1.0f, -1.0f, 1.0f), -1.0f);
    assert_is_close(constrain(0.0f, -1.0f, 1.0f), 0.0f);
    assert_is_close(constrain(1.0f, -1.0f, 1.0f), 1.0f);
    assert_is_close(constrain(-1.0f, -1.0f, 2.0f), -1.0f);
})

TEST(test_lerp, {
    assert_is_close(lerp(0.0f, 1.0f, 0.0f), 0.0f);
    assert_is_close(lerp(0.0f, 1.0f, 0.5f), 0.5f);
    assert_is_close(lerp(0.0f, 1.0f, 1.0f), 1.0f);
    assert_is_close(lerp(0.0f, 2.0f, 0.0f), 0.0f);
    assert_is_close(lerp(0.0f, 2.0f, 0.5f), 1.0f);
    assert_is_close(lerp(0.0f, 2.0f, 1.0f), 2.0f);
    assert_is_close(lerp(0.0f, 3.0f, 0.0f), 0.0f);
    assert_is_close(lerp(0.0f, 3.0f, 0.5f), 1.5f);
    assert_is_close(lerp(0.0f, 3.0f, 1.0f), 3.0f);
    assert_is_close(lerp(0.0f, 1.0f, 0.0f), 0.0f);
    assert_is_close(lerp(0.0f, 1.0f, 0.5f), 0.5f);
    assert_is_close(lerp(-1.0f, 1.0f, 0.0f), -1.0f);
    assert_is_close(lerp(-1.0f, 1.0f, 0.5f), 0.0f);
    assert_is_close(lerp(-1.0f, 1.0f, 1.0f), 1.0f);
    assert_is_close(lerp(-1.0f, 1.0f, 0.0f), -1.0f);
})

TEST(test_degrees, {
    assert_is_close(degrees((float) M_PI / 2.0f), 90.0f);
    assert_is_close(degrees((float) M_PI / 3.0f), 60.0f);
    assert_is_close(degrees((float) M_PI / 4.0f), 45.0f);
    assert_is_close(degrees(0.0f), 0.0f);
    assert_is_close(degrees((float) M_PI), 180.0f);
    assert_is_close(degrees(2.0f * (float) M_PI), 360.0f);

    assert_is_close(degrees(0.0f), 0.0f);
    assert_is_close(degrees(1.0f), 57.295776f);
    assert_is_close(degrees(2.0f), 114.591553f);
    assert_is_close(degrees(3.0f), 171.887329f);
    assert_is_close(degrees(4.0f), 229.183105f);
    assert_is_close(degrees(5.0f), 286.478882f);
    assert_is_close(degrees(6.0f), 343.774658f);
    assert_is_close(degrees(7.0f), 401.070435f);
    assert_is_close(degrees(8.0f), 458.366211f);
    assert_is_close(degrees(9.0f), 515.661987f);
    assert_is_close(degrees(10.0f), 572.957764f);
    assert_is_close(degrees(11.0f), 630.253540f);
    assert_is_close(degrees(12.0f), 687.549316f);
    assert_is_close(degrees(13.0f), 744.845093f);
    assert_is_close(degrees(14.0f), 802.140869f);
    assert_is_close(degrees(15.0f), 859.436646f);
    assert_is_close(degrees(16.0f), 916.732422f);
    assert_is_close(degrees(17.0f), 974.028198f);
    assert_is_close(degrees(18.0f), 1031.323975f);
    assert_is_close(degrees(19.0f), 1088.619751f);
    assert_is_close(degrees(20.0f), 1145.915527f);
    assert_is_close(degrees(21.0f), 1203.211304f);
    assert_is_close(degrees(22.0f), 1260.507080f);
    assert_is_close(degrees(23.0f), 1317.802856f);
})

TEST(test_radians, {
    assert_is_close(radians(90.0f), (float) M_PI / 2.0f);
    assert_is_close(radians(60.0f), (float) M_PI / 3.0f);
    assert_is_close(radians(45.0f), (float) M_PI / 4.0f);
    assert_is_close(radians(0.0f), 0.0f);
    assert_is_close(radians(180.0f), (float) M_PI);
    assert_is_close(radians(360.0f), 2.0f * (float) M_PI);

    assert_is_close(radians(0.0f), 0.0f);
    assert_is_close(radians(57.295776f), 1.0f);
    assert_is_close(radians(114.591553f), 2.0f);
    assert_is_close(radians(171.887329f), 3.0f);
    assert_is_close(radians(229.183105f), 4.0f);
    assert_is_close(radians(286.478882f), 5.0f);
    assert_is_close(radians(343.774658f), 6.0f);
    assert_is_close(radians(401.070435f), 7.0f);
    assert_is_close(radians(458.366211f), 8.0f);
    assert_is_close(radians(515.661987f), 9.0f);
    assert_is_close(radians(572.957764f), 10.0f);
    assert_is_close(radians(630.253540f), 11.0f);
    assert_is_close(radians(687.549316f), 12.0f);
    assert_is_close(radians(744.845093f), 13.0f);
    assert_is_close(radians(802.140869f), 14.0f);
    assert_is_close(radians(859.436646f), 15.0f);
    assert_is_close(radians(916.732422f), 16.0f);
    assert_is_close(radians(974.028198f), 17.0f);
    assert_is_close(radians(1031.323975f), 18.0f);
    assert_is_close(radians(1088.619751f), 19.0f);
    assert_is_close(radians(1145.915527f), 20.0f);
    assert_is_close(radians(1203.211304f), 21.0f);
    assert_is_close(radians(1260.507080f), 22.0f);
    assert_is_close(radians(1317.802856f), 23.0f);
})

TEST(test_vector_zero, {
    assert_equals(vector_zero.x, 0.0f);
    assert_equals(vector_zero.y, 0.0f);
})

TEST(test_vector_clear, {
    struct vector_t vec;
    vec.x = rand_float();
    vec.y = rand_float();

    assert_equals(vector_clear(&vec), &vec);
    assert_equals(vec.x, 0.0f);
    assert_equals(vec.y, 0.0f);
})

TEST(test_vector_add, {
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

TEST(test_vector_sub, {
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

TEST(test_vector_mul, {
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

TEST(test_vector_div, {
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

TEST(test_vector_product, {
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

TEST(test_vector_length, {
    struct vector_t vec;

    const float x = rand_float();
    const float y = rand_float();

    vec.x = x;
    vec.y = y;
    assert_is_close(vector_length(&vec), sqrtf(x * x + y * y));

    vec.x = 0.0f;
    vec.y = 0.0f;
    assert_is_close(vector_length(&vec), 0.0f);

    vec.x = 1.0f;
    vec.y = 0.0f;
    assert_is_close(vector_length(&vec), 1.0f);

    vec.x = 0.0f;
    vec.y = 1.0f;
    assert_is_close(vector_length(&vec), 1.0f);

    vec.x = 1.0f;
    vec.y = 1.0f;
    assert_is_close(vector_length(&vec), sqrtf(2.0f));
})

TEST(test_vector_length2, {
    struct vector_t vec;

    const float x = rand_float();
    const float y = rand_float();

    vec.x = x;
    vec.y = y;
    assert_is_close(vector_length2(&vec), x * x + y * y);

    vec.x = 0.0f;
    vec.y = 0.0f;
    assert_is_close(vector_length2(&vec), 0.0f);

    vec.x = 1.0f;
    vec.y = 0.0f;
    assert_is_close(vector_length2(&vec), 1.0f);

    vec.x = 0.0f;
    vec.y = 1.0f;
    assert_is_close(vector_length2(&vec), 1.0f);

    vec.x = 1.0f;
    vec.y = 1.0f;
    assert_is_close(vector_length2(&vec), 2.0f);
})

TEST(test_vector_from_angle, {
    struct vector_t vec;

    assert_equals(vector_from_angle(&vec, 0.0f), &vec);
    assert_is_close(vec.x, 1.0f);
    assert_is_close(vec.y, 0.0f);

    assert_equals(vector_from_angle(&vec, (float) M_PI / 2.0f), &vec);
    assert_is_close(vec.x, 0.0f);
    assert_is_close(vec.y, 1.0f);

    assert_equals(vector_from_angle(&vec, (float) M_PI), &vec);
    assert_is_close(vec.x, -1.0f);
    assert_is_close(vec.y, 0.0f);

    assert_equals(vector_from_angle(&vec, 3.0f * (float) M_PI / 2.0f), &vec);
    assert_is_close(vec.x, 0.0f);
    assert_is_close(vec.y, -1.0f);

    assert_equals(vector_from_angle(&vec, 2.0f * (float) M_PI), &vec);
    assert_is_close(vec.x, 1.0f);
    assert_is_close(vec.y, 0.0f);
})

TEST(test_vector_normalize, {
    struct vector_t vec;

    const float x = rand_float();
    const float y = rand_float();

    vec.x = x == 0.0f ? 1.0f : x; // avoid division by zero
    vec.y = y == 0.0f ? 1.0f : y;

    const float length_before = vector_length(&vec);

    assert_equals(vector_normalize(&vec), &vec);

    const float length_after = vector_length(&vec);

    assert_is_close(length_after, 1.0f);
    assert_is_close(vec.x, x / length_before);
    assert_is_close(vec.y, y / length_before);
})

TEST(test_vector_normalize_weak, {
    struct vector_t vec;

    vec.x = 0.0f;
    vec.y = 0.0f;
    assert_equals(vector_normalize_weak(&vec), &vec);
    assert_is_close(vec.x, 0.0f);
    assert_is_close(vec.y, 0.0f);
    assert_is_close(vector_length(&vec), 0.0f);

    const float x = rand_float();
    const float y = rand_float();

    vec.x = x;
    vec.y = y;

    const float length_before = vector_length(&vec);

    assert_equals(vector_normalize_weak(&vec), &vec);

    const float length_after = vector_length(&vec);

    assert_is_close(length_after, 1.0f);
    assert_is_close(vec.x, x / length_before);
    assert_is_close(vec.y, y / length_before);
})

TEST(test_vector_distance, {
    struct vector_t vec1;
    struct vector_t vec2;

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 0.0f;
    vec2.y = 0.0f;

    assert_is_close(vector_distance(&vec1, &vec2), 0.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 1.0f;
    vec2.y = 0.0f;

    assert_is_close(vector_distance(&vec1, &vec2), 1.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 0.0f;
    vec2.y = 1.0f;

    assert_is_close(vector_distance(&vec1, &vec2), 1.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 1.0f;
    vec2.y = 1.0f;

    assert_is_close(vector_distance(&vec1, &vec2), sqrtf(2.0f));

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = -1.0f;
    vec2.y = 0.0f;

    assert_is_close(vector_distance(&vec1, &vec2), 1.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 0.0f;
    vec2.y = -1.0f;

    assert_is_close(vector_distance(&vec1, &vec2), 1.0f);
})

TEST(test_vector_distance2, {
    struct vector_t vec1;
    struct vector_t vec2;

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 0.0f;
    vec2.y = 0.0f;

    assert_is_close(vector_distance2(&vec1, &vec2), 0.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 1.0f;
    vec2.y = 0.0f;

    assert_is_close(vector_distance2(&vec1, &vec2), 1.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 0.0f;
    vec2.y = 1.0f;

    assert_is_close(vector_distance2(&vec1, &vec2), 1.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 1.0f;
    vec2.y = 1.0f;

    assert_is_close(vector_distance2(&vec1, &vec2), 2.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = -1.0f;
    vec2.y = 0.0f;

    assert_is_close(vector_distance2(&vec1, &vec2), 1.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 0.0f;
    vec2.y = -1.0f;

    assert_is_close(vector_distance2(&vec1, &vec2), 1.0f);
})

TEST(test_vector_angle_to, {
    struct vector_t vec1;
    struct vector_t vec2;

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 0.0f;
    vec2.y = 0.0f;

    assert_is_close(vector_angle_to(&vec1, &vec2), 0.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 1.0f;
    vec2.y = 0.0f;

    assert_is_close(vector_angle_to(&vec1, &vec2), 0.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 0.0f;
    vec2.y = 1.0f;

    assert_is_close(vector_angle_to(&vec1, &vec2), (float) M_PI / 2.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = -1.0f;
    vec2.y = 0.0f;

    assert_is_close(vector_angle_to(&vec1, &vec2), (float) M_PI);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 0.0f;
    vec2.y = -1.0f;

    assert_is_close(vector_angle_to(&vec1, &vec2), 3.0f * (float) M_PI / 2.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 1.0f;
    vec2.y = 1.0f;

    assert_is_close(vector_angle_to(&vec1, &vec2), (float) M_PI / 4.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = -1.0f;
    vec2.y = 1.0f;

    assert_is_close(vector_angle_to(&vec1, &vec2), 3.0f * (float) M_PI / 4.0f);
})

TEST(test_vector_copy, {
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

    vec.x = 1.0f;
    vec.y = 0.0f;

    assert_equals(vector_rotate(&vec, 0.0f), &vec);
    assert_is_close(vec.x, 1.0f);
    assert_is_close(vec.y, 0.0f);

    vec.x = 1.0f;
    vec.y = 0.0f;

    assert_equals(vector_rotate(&vec, (float) M_PI / 2.0f), &vec);
    assert_is_close(vec.x, 0.0f);
    assert_is_close(vec.y, 1.0f);

    vec.x = 1.0f;
    vec.y = 0.0f;

    assert_equals(vector_rotate(&vec, (float) M_PI), &vec);
    assert_is_close(vec.x, -1.0f);
    assert_is_close(vec.y, 0.0f);

    vec.x = 1.0f;
    vec.y = 0.0f;

    assert_equals(vector_rotate(&vec, 3.0f * (float) M_PI / 2.0f), &vec);
    assert_is_close(vec.x, 0.0f);
    assert_is_close(vec.y, -1.0f);

    vec.x = 1.0f;
    vec.y = 0.0f;

    assert_equals(vector_rotate(&vec, 2.0f * (float) M_PI), &vec);
    assert_is_close(vec.x, 1.0f);
    assert_is_close(vec.y, 0.0f);
})

TEST(test_vector_lerp, {
    struct vector_t vec1;
    struct vector_t vec2;

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 1.0f;
    vec2.y = 0.0f;

    assert_equals(vector_lerp(&vec1, &vec2, 0.0f), &vec1);
    assert_is_close(vec1.x, 0.0f);
    assert_is_close(vec1.y, 0.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 1.0f;
    vec2.y = 0.0f;

    assert_equals(vector_lerp(&vec1, &vec2, 0.5f), &vec1);
    assert_is_close(vec1.x, 0.5f);
    assert_is_close(vec1.y, 0.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 1.0f;
    vec2.y = 0.0f;

    assert_equals(vector_lerp(&vec1, &vec2, 1.0f), &vec1);
    assert_is_close(vec1.x, 1.0f);
    assert_is_close(vec1.y, 0.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 1.0f;
    vec2.y = 0.0f;

    assert_equals(vector_lerp(&vec1, &vec2, 0.0f), &vec1);
    assert_is_close(vec1.x, 0.0f);
    assert_is_close(vec1.y, 0.0f);

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec2.x = 0.0f;
    vec2.y = 1.0f;

    assert_equals(vector_lerp(&vec1, &vec2, 0.0f), &vec1);
    assert_is_close(vec1.x, 0.0f);
    assert_is_close(vec1.y, 0.0f);
})

TEST(test_vector_scale, {
    struct vector_t vec;

    vec.x = 1.0f;
    vec.y = 0.0f;

    assert_equals(vector_scale(&vec, 0.0f), &vec);
    assert_is_close(vec.x, 0.0f);
    assert_is_close(vec.y, 0.0f);

    vec.x = 1.0f;
    vec.y = 0.0f;

    assert_equals(vector_scale(&vec, 0.5f), &vec);
    assert_is_close(vec.x, 0.5f);
    assert_is_close(vec.y, 0.0f);

    vec.x = 1.0f;
    vec.y = 0.0f;

    assert_equals(vector_scale(&vec, 1.0f), &vec);
    assert_is_close(vec.x, 1.0f);
    assert_is_close(vec.y, 0.0f);

    vec.x = 1.0f;
    vec.y = 0.0f;

    assert_equals(vector_scale(&vec, 2.0f), &vec);
    assert_is_close(vec.x, 2.0f);
    assert_is_close(vec.y, 0.0f);
})

TEST(test_vector_reflect, {
    struct vector_t vec;
    struct vector_t normal;

    vec.x = 1.0f;
    vec.y = 0.0f;

    normal.x = 0.0f;
    normal.y = 1.0f;

    assert_equals(vector_reflect(&vec, &normal), &vec);
    assert_is_close(vec.x, 1.0f);
    assert_is_close(vec.y, 0.0f);

    // TODO: add more tests
})

TEST(test_vector_project, {
    struct vector_t vec;
    struct vector_t normal;

    vec.x = 1.0f;
    vec.y = 0.0f;

    normal.x = 0.0f;
    normal.y = 1.0f;

    assert_equals(vector_project(&vec, &normal), &vec);
    assert_is_close(vec.x, 0.0f);
    assert_is_close(vec.y, 0.0f);
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
        ADD_TEST(test_vector_clear),
        ADD_TEST(test_vector_add),
        ADD_TEST(test_vector_sub),
        ADD_TEST(test_vector_mul),
        ADD_TEST(test_vector_div),
        ADD_TEST(test_vector_product),
        ADD_TEST(test_vector_length),
        ADD_TEST(test_vector_length2),
        ADD_TEST(test_vector_from_angle),
        ADD_TEST(test_vector_normalize),
        ADD_TEST(test_vector_normalize_weak),
        ADD_TEST(test_vector_distance),
        ADD_TEST(test_vector_distance2),
        ADD_TEST(test_vector_angle_to),
        ADD_TEST(test_vector_copy),
        ADD_TEST(test_vector_rotate),
        ADD_TEST(test_vector_lerp),
        ADD_TEST(test_vector_scale),
        ADD_TEST(test_vector_reflect),
        ADD_TEST(test_vector_project)
)
