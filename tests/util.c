#include <time.h>

#include "../src/util.h"

#include "util.h"


#define TIMER_CLOCK CLOCK_MONOTONIC_RAW


static struct timespec start;


void timer_start(void) {
    clock_gettime(TIMER_CLOCK, &start);
}

intmax_t timer_stop(void) {
    struct timespec end;
    clock_gettime(TIMER_CLOCK, &end);
    return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
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

void print_justified(const char *const str, const unsigned int width, const char pad) {
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
