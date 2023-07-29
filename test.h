#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#ifndef RAY_TEST_H
#define RAY_TEST_H


#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#include "color.h"
#include "util.h"
#include "math.h"


#ifdef assert
#undef assert
#endif


#define NTESTS (1000u * 1000u)
#define OUTPUT_MAXLEN 1000

#define TIMER_START 0
#define TIMER_STOP 1


struct test_t {
    bool (*const func)(struct test_t *const test);

    const char *const name;
    bool passed;
    char output[OUTPUT_MAXLEN];
};


#define TEST(name, stmts)                               \
unused private bool name(struct test_t *const test) {   \
    stmts                                               \
    return true;                                        \
}

#define ADD_TEST(func) { (func), #func, true, {0} }

#define RUN_TESTS(...)                                      \
int main(void) {                                            \
    srand(get_seed());                                      \
    static struct test_t tests[] = {__VA_ARGS__};           \
    return run_tests(tests, sizeof tests / sizeof *tests);  \
}

#define assert(cond)                                            \
do {                                                            \
    if (!(cond)) {                                              \
        snprintf(test->output,                                  \
                OUTPUT_MAXLEN,                                  \
                "%s%s:%d:\n\t%sassert(%s)%s ",                  \
                HBLU, __func__, __LINE__, HYEL, #cond, CRESET); \
        return false;                                           \
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

#define assert_null(ptr) assert((ptr) == NULL)
#define assert_not_null(ptr) assert((ptr) != NULL)

#define assert_str_equals(a, b) assert(strcmp((a), (b)) == 0)
#define assert_str_not_equals(a, b) assert(strcmp((a), (b)) != 0)

#define assert_str_contains(a, b) assert(strstr((a), (b)) != NULL)
#define assert_str_not_contains(a, b) assert(strstr((a), (b)) == NULL)


unused private intmax_t timer(const int op) {
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

unused private void print_justified(const char *const str, const size_t width, const char pad) {
    const size_t len = strlen(str);

    if (len >= width) {
        printf("%.*s", (int) width, str);
        return;
    }

    printf("%s", str);

    for (size_t i = 0; i < width - len; i++) {
        putchar(pad);
    }
}


unused private int run_tests(struct test_t *const tests, const size_t ntests) {
    timer(TIMER_START);
    size_t passed = 0;
    size_t failed = 0;
    size_t total = 0;

    for (size_t i = 0; i < ntests; i++) {
        struct test_t *const test = &tests[i];
        const size_t nrepeats = strstr(test->name, "rand") ? NTESTS : 1;

        printf("[%3zu] ", i + 1);
        print_justified(test->name, 60, '.');

        for (size_t j = 0; j < nrepeats; j++) {
            if (!test->func(test)) {
                test->passed = false;
                break;
            }
        }

        if (test->passed) {
            fputs(HGRN " PASS" CRESET, stdout);
            passed++;
        } else {
            fputs(RED " FAIL" CRESET, stdout);
            failed++;
        }

        if (nrepeats > 1) {
            printf(" (%.0e runs)", (double) nrepeats);
        }

        putchar('\n');
        total++;
    }

    const intmax_t elapsed = timer(TIMER_STOP);

    puts("");
    for (size_t i = 0; i < ntests; i++) {
        struct test_t *const test = &tests[i];

        if (!test->passed) {
            printf("\n%s\n", test->output);
        }
    }
    puts("");

    printf("\npassed: %s%zu%s, failed: %s%zu%s, total: %zu (%" PRIdMAX " ms)\n",
           HGRN, passed, CRESET,
           RED, failed, CRESET,
           total, elapsed);


    if (failed > 0) {
        puts(BRED "\nTest failed!" CRESET);
    } else {
        puts(BHGRN "\nAll tests passed!" CRESET);
    }

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

#endif //RAY_TEST_H
#pragma clang diagnostic pop
