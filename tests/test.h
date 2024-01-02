#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#ifndef RAY_TESTS_TEST_H
#define RAY_TESTS_TEST_H


#include <stdbool.h>
#include <stdlib.h>


#include "../src/color.h"


#include "util.h"


#ifdef assert
#undef assert
#endif


#define NTESTS 1e6
#define OUTPUT_MAXLEN 1024


struct test_t;

typedef void (*test_func_t)(struct test_t *const test);


struct test_t {
    test_func_t func;
    const char *const name;
    bool failed;
    char output[OUTPUT_MAXLEN];
};


#define TEST(name, ...)                                 \
static void name(unused struct test_t *const test) {    \
    __VA_ARGS__                                         \
}

#define ADD_TEST(test) { .func = (test), .name = #test, .failed = false, .output = {0} }

#define RUN_TESTS(...)                                      \
int main(void) {                                            \
    srand(get_seed());                                      \
    static struct test_t tests[] = {__VA_ARGS__};           \
    return run_tests(tests, sizeof tests / sizeof *tests);  \
}

#define assert(cond)                                            \
do {                                                            \
    if (!(cond)) {                                              \
        test->failed = true;                                    \
        snprintf(test->output,                                  \
                OUTPUT_MAXLEN,                                  \
                "%s%s:%d:\n\t%sassert(%s)%s ",                  \
                HBLU, __func__, __LINE__, HYEL, #cond, CRESET); \
    }                                                           \
} while (0)

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


#endif //RAY_TESTS_TEST_H
#pragma clang diagnostic pop
