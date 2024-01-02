#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "../src/color.h"


#include "test.h"


int run_tests(struct test_t *const tests, const size_t ntests) {
    timer_start();
    size_t passed = 0;
    size_t failed = 0;
    size_t total = 0;

    for (size_t i = 0; i < ntests; i++) {
        struct test_t *const test = &tests[i];
        const size_t nrepeats = strstr(test->name, "rand") ? (size_t) NTESTS : 1;

        printf("[%3zu] ", i + 1);
        print_justified(test->name, 60, '.');

        for (size_t j = 0; j < nrepeats; j++) {
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

        if (nrepeats > 1) {
            printf(" (%.0e runs)", (double) nrepeats);
        }

        putchar('\n');
        total++;
    }

    const intmax_t elapsed = timer_stop();
    putchar('\n');

    for (size_t i = 0; i < ntests; i++) {
        struct test_t *const test = &tests[i];

        if (test->failed) {
            printf("\n%s\n", test->output);
        }
    }

    printf("\n\npassed: %s%zu%s, failed: %s%zu%s, total: %zu (%" PRIdMAX " ms)\n",
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

void test_fail(struct test_t *const restrict test, const char *const restrict condstr) {
    test->failed = true;
    snprintf(test->output,
             OUTPUT_MAXLEN,
             "%s%s:%d:\n\t%sassert(%s)%s ",
             HBLU, __func__, __LINE__, HYEL, condstr, CRESET);
}
