#include <libgen.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#include "color.h"

#include "logger.h"


#define c(fmt) "%s" fmt "%s"


struct log_target_t {
    const char *const color;
    const char *const prefix;
    const enum unused {
        STDOUT,
        STDERR
    } stream;
};


static const struct log_target_t LOG_TARGETS[] = {
        [LOG_LEVEL_FATAL] = {BRED, "fatal", STDERR},
        [LOG_LEVEL_ERROR] = {RED, "error", STDERR},
        [LOG_LEVEL_WARN]  = {HYEL, "warn", STDOUT},
        [LOG_LEVEL_INFO]  = {HBLU, "info", STDOUT},
        [LOG_LEVEL_DEBUG] = {HBLK, "debug", STDOUT}
};

static bool env_enabled(const char *const name) {
    const char *const value = getenv(name);

    if (value == NULL) {
        return false;
    }

    return strcmp(value, "1") == 0 || strcasecmp(value, "true") == 0;
}

static inline bool supports_color(void) {
    static int cache = -1;

    if (cache == -1) {
        cache = isatty(STDOUT_FILENO) && isatty(STDERR_FILENO) && !env_enabled("NO_COLOR");
    }

    return cache;
}

static inline const char *color(const char *const code) {
    return supports_color() ? code : "";
}

void logger_log(const enum log_level_t level,
                const char *const restrict file,
                const unsigned int line,
                const char *const restrict func,
                const char *const restrict fmt, ...) {
    const struct log_target_t *const target = &LOG_TARGETS[level];
    FILE *const stream = target->stream == STDOUT ? stdout : stderr;
    char *const path = strdup(file);

    if (path == NULL) {
        perror("strdup");
        return;
    }

    fprintf(stream,
            c("%s:%u") " [" c("%s") "] " c("%s") ": ",
            color(HGRN),
            basename(path),
            line,
            color(CRESET),
            color(target->color),
            target->prefix,
            color(CRESET),
            color(BHMAG),
            func,
            color(CRESET));
    free(path);

    va_list args;
    va_start(args, fmt);

    vfprintf(stream, fmt, args);

    va_end(args);
}
