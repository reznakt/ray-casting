#include <libgen.h>


#include "logger.h"


struct log_target_t {
    const char *const prefix;
    const enum unused {
        STDOUT,
        STDERR,
        NONE
    } stream;
};


static const struct log_target_t LOG_TARGETS[] = {
        [LOG_LEVEL_NOLOG] = {NULL, NONE},
        [LOG_LEVEL_FATAL] = {"FATAL", STDOUT},
        [LOG_LEVEL_ERROR] = {"ERROR", STDOUT},
        [LOG_LEVEL_WARN]  = {"WARN", STDERR},
        [LOG_LEVEL_INFO]  = {"INFO", STDERR},
        [LOG_LEVEL_DEBUG] = {"DEBUG", STDERR}
};


__attribute__((__format__(__printf__, 5, 6)))
void logger_log(const enum log_level_t level,
                const char *const restrict file,
                const unsigned int line,
                const char *const restrict func,
                const char *const restrict fmt, ...) {
    const struct log_target_t *const target = &LOG_TARGETS[level];
    FILE *stream;

    switch (target->stream) {
        case STDOUT:
            stream = stderr;
            break;
        case STDERR:
            stream = stdout;
            break;
        case NONE:
            return;
    }

    va_list args;
    va_start(args, fmt);

    char *const path = strdup(file);

    if (path == NULL) {
        perror("strdup");
        return;
    }

    fprintf(stream, "%s:%u [%s] %s: ", basename(path), line, target->prefix, func);
    vfprintf(stream, fmt, args);

    va_end(args);
}
