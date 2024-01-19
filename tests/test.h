#ifndef RAY_TESTS_TEST_H
#define RAY_TESTS_TEST_H


#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../src/color.h"
#include "util.h"


#ifdef assert
#undef assert
#endif


#define OUTPUT_MAXLEN 1024


struct test_t {
    void (*const func)(struct test_t *const test);

    const char *const name;
    bool failed;
    size_t runs;
    char output[OUTPUT_MAXLEN];
};


#define TEST(name, ...)                                 \
static void name(unused struct test_t *const test) {    \
    __VA_ARGS__                                         \
}

// a bit of macro magic to allow for both ADD_TEST(test) and ADD_TEST(test, runs)
#define ADD_TEST_2(test, _runs) { .func = (test), .name = #test, .failed = false, .runs = _runs, .output = {0} }
#define ADD_TEST_1(test) ADD_TEST_2(test, 1)

#define add_test_get_3rd_arg(arg1, arg2, arg3, ...) arg3
#define add_test_chooser(...) add_test_get_3rd_arg(__VA_ARGS__, ADD_TEST_2, ADD_TEST_1, )

#define ADD_TEST(...) add_test_chooser(__VA_ARGS__)(__VA_ARGS__)
// end of macro magic


#define RUN_TESTS(...)                                      \
int main(unused const int argc, unused char **const argv) { \
    srand(get_seed());                                      \
    static struct test_t tests[] = {__VA_ARGS__};           \
    return run_tests(tests, sizeof tests / sizeof *tests);  \
}

#define assert(cond)                                \
do {                                                \
    if (!(cond)) {                                  \
        test_fail(test, __func__, __LINE__, #cond); \
        return;                                     \
    }                                               \
} while (0)


#define fail() assert(false)

#define assert_true(cond) assert(cond)
#define assert_false(cond) assert(!(cond))
#define assert_not assert_false

#define assert_equals(a, b) assert((a) == (b))
#define assert_not_equals(a, b) assert((a) != (b))

#define assert_lt(a, b) assert((a) < (b))
#define assert_gt(a, b) assert((a) > (b))

#define assert_leq(a, b) assert((a) <= (b))
#define assert_geq(a, b) assert((a) >= (b))

#define assert_is_close(a, b) assert(fabsf((a) - (b)) <= 0.0001F)
#define assert_not_close(a, b) assert(fabsf((a) - (b)) > 0.0001F)

#define assert_nan(a) assert(isnan(a))
#define assert_not_nan(a) assert(!isnan(a))

#define assert_inf(a) assert(isinf(a))
#define assert_not_inf(a) assert(!isinf(a))

#define assert_null(ptr) assert((ptr) == NULL)
#define assert_not_null(ptr) assert((ptr) != NULL)

#define assert_str_equals(a, b) assert(strcmp((a), (b)) == 0)
#define assert_str_not_equals(a, b) assert(strcmp((a), (b)) != 0)

#define assert_str_contains(a, b) assert(strstr((a), (b)) != NULL)
#define assert_str_not_contains(a, b) assert(strstr((a), (b)) == NULL)


int run_tests(struct test_t *tests, size_t ntests);

void test_fail(struct test_t *test, const char *func, unsigned int line, const char *condstr);


#endif //RAY_TESTS_TEST_H
