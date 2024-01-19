#include <libgen.h>

#include "color.h"

#include "logger.h"


struct log_target_t {
    const char *const prefix;
    const enum unused {
        STDOUT,
        STDERR
    } stream;
};


static const struct log_target_t LOG_TARGETS[] = {
        [LOG_LEVEL_FATAL] = {BRED "fatal" CRESET, STDERR},
        [LOG_LEVEL_ERROR] = {RED "error" CRESET, STDERR},
        [LOG_LEVEL_WARN]  = {HYEL "warn" CRESET, STDOUT},
        [LOG_LEVEL_INFO]  = {HBLU "info" CRESET, STDOUT},
        [LOG_LEVEL_DEBUG] = {HBLK "debug" CRESET, STDOUT}
};


void logger_log(const enum log_level_t level,
                const char *const restrict file,
                const unsigned int line,
                const char *const restrict func,
                const char *const restrict fmt, ...) {
    const struct log_target_t *const target = &LOG_TARGETS[level];
    FILE *const stream = target->stream == STDOUT ? stdout : stderr;

    va_list args;
    va_start(args, fmt);

    char *const path = strdup(file);

    if (path == NULL) {
        perror("strdup");
        return;
    }

    fprintf(stream, HGRN "%s:%u" CRESET " [%s] " BHMAG "%s" CRESET ": ", basename(path), line, target->prefix, func);
    free(path);
    vfprintf(stream, fmt, args);

    va_end(args);
}
