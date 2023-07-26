#ifndef RAY_TEST_H
#define RAY_TEST_H


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <inttypes.h>


#include "ansi-color.h"
#include "../util.h"


#define NTESTS (1000 * 1000)
#define VERBOSE true


typedef bool (*const testfunc_t)(void);

struct test_t {
    const testfunc_t func;
    const char *const name;
};


#define ADD_TEST(func) { (func), #func }

#define TEST(name, stmts)           \
unused private bool name(void) {    \
    stmts                           \
    return true;                    \
}

#define RUN_TESTS(...)                                      \
int main(void) {                                            \
    srand(get_seed());                                      \
    static const struct test_t tests[] = {__VA_ARGS__};     \
    return run_tests(tests, sizeof tests / sizeof *tests);  \
}

#define assert(cond)                                                                \
do {                                                                                \
    if (!(cond)) {                                                                  \
        if (VERBOSE) {                                                              \
            printf("%sL%d: %sassert(%s)%s ", HBLU, __LINE__, HYEL, #cond, CRESET);  \
        }                                                                           \
        return false;                                                               \
    }                                                                               \
} while (0)

#define assert_equals(a, b) assert((a) == (b))
#define assert_not_equals(a, b) assert((a) != (b))

#define TIMER_START 0
#define TIMER_STOP 1

private intmax_t timer(const int op) {
    static struct timespec start;
    struct timespec end;

    switch (op) {
        case TIMER_START:
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            return 0;
        case TIMER_STOP:
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
        default:
            return -1;
    }
}

unused private unsigned int get_seed(void) {
    FILE *const stream = fopen("/dev/urandom", "rb");

    if (stream == NULL) {
        abort();
    }

    unsigned int seed;

    if (fread(&seed, sizeof seed, 1, stream) != 1) {
        abort();
    }

    return seed;
}

unused private int run_tests(const struct test_t *const tests, const size_t ntests) {
    timer(TIMER_START);
    size_t passed = 0, failed = 0, total = 0;

    for (size_t i = 0; i < ntests; i++) {
        const struct test_t *const test = &tests[i];
        bool success = true;

        printf("%s... ", test->name);

        for (size_t j = 0; j < NTESTS; j++) {
            if (!test->func()) {
                success = false;
                break;
            }
        }

        if (success) {
            puts(HGRN "PASS" CRESET);
            passed++;
        } else {
            puts(RED "FAIL" CRESET);
            failed++;
        }

        total++;
    }

    const intmax_t elapsed = timer(TIMER_STOP);

    printf("\npassed: %s%zu%s, failed: %s%zu%s, total: %zu (%" PRIdMAX " ms)\n",
           HGRN, passed, CRESET,
           RED, failed, CRESET,
           total, elapsed);

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}


#endif //RAY_TEST_H
