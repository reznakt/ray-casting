#include <time.h>


#include "../src/util.h"


#include "util.h"


intmax_t timer(const enum timer_op_t op) {
    static struct timespec start;
    struct timespec end;

    switch (op) {
        case TIMER_START:
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            return 0;
        case TIMER_STOP:
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
    }

    return -1; // unreachable
}

unsigned int get_seed(void) {
    FILE *const stream = fopen("/dev/urandom", "rb");

    if (stream == NULL) {
        abort();
    }

    unsigned int seed;
    const size_t bytes = fread(&seed, sizeof seed, 1, stream);
    fclose(stream);

    if (bytes != 1) {
        abort();
    }

    return seed;
}

void print_justified(const char *const str, const size_t width, const char pad) {
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
