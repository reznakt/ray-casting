#include <stdio.h>

#include "../src/color.h"

#include "runner.h"


int run_tests(struct test_t *const tests, const size_t ntests) {
    if (timer_start() != 0) {
        return -1;
    }

    size_t passed = 0;
    size_t failed = 0;
    size_t total = 0;

    for (size_t i = 0; i < ntests; i++) {
        struct test_t *const test = &tests[i];
        size_t repeats;

        printf("[%3zu] ", i + 1);
        print_justified(test->name, 60, '.');

        for (repeats = 0; repeats < test->runs; repeats++) {
            test->func(test);

            if (test->failed) {
                break;
            }
        }

        if (test->failed) {
            fputs(RED " FAIL" CRESET, stdout);
            failed++;
        } else {
            fputs(HGRN " PASS" CRESET, stdout);
            passed++;
        }

        if (test->runs > 1) {
            if (repeats == test->runs) {
                printf(" (%.0e runs)", (double) repeats);
            } else if (test->failed) {
                printf(" (after %.0e of %.0e runs)", (double) repeats, (double) test->runs);
            }
        }

        putchar('\n');
        total++;
    }

    intmax_t elapsed;

    if (timer_stop(&elapsed) != 0) {
        return -1;
    }

    putchar('\n');

    for (size_t i = 0; i < ntests; i++) {
        const struct test_t *const test = &tests[i];

        if (test->failed) {
            printf("\n%s\n", test->output);
        }
    }

    printf("\n\npassed: " HGRN "%zu" CRESET ", failed: " RED "%zu" CRESET ", total: %zu (%" PRIdMAX " ms)\n",
           passed, failed, total, elapsed);


    if (failed > 0) {
        puts(BRED "\nTest failed!" CRESET);
    } else {
        puts(BHGRN "\nAll tests passed!" CRESET);
    }

    return failed == 0 ? 0 : -1;
}

void test_fail(struct test_t *const restrict test, const char *const restrict func,
               const unsigned int line, const char *const restrict condstr) {
    test->failed = true;
    snprintf(test->output, OUTPUT_MAXLEN, HBLU "%s:%u:\n\t" HYEL "assert(%s) " CRESET, func, line, condstr);
}
