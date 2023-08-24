#include "logger.h"


enum log_target_t {
    LOG_TARGET_STDERR,
    LOG_TARGET_STDOUT,
    LOG_TARGET_NONE
};


private const struct {
    const char *const prefix;
    const enum log_target_t target;
} LOG_LEVELS[] = {
        [LOG_LEVEL_NOLOG] = {NULL, LOG_TARGET_NONE},
        [LOG_LEVEL_FATAL] = {"FATAL", LOG_TARGET_STDERR},
        [LOG_LEVEL_ERROR] = {"ERROR", LOG_TARGET_STDERR},
        [LOG_LEVEL_WARN]  = {"WARN", LOG_TARGET_STDOUT},
        [LOG_LEVEL_INFO]  = {"INFO", LOG_TARGET_STDOUT},
        [LOG_LEVEL_DEBUG] = {"DEBUG", LOG_TARGET_STDOUT}
};


void logger_log(const enum log_level_t level,
                const char *const restrict file,
                const unsigned int line,
                const char *const func,
                const char *const restrict fmt, ...) {
    const char *const prefix = LOG_LEVELS[level].prefix;
    FILE *const stream = LOG_LEVELS[level].target == LOG_TARGET_STDERR ? stderr : stdout;

    if (level == LOG_LEVEL_NOLOG) {
        return;
    }

    va_list args;
    va_start(args, fmt);

    fprintf(stream, "%s:%d [%s] %s: ", basename(file), line, prefix, func);
    vfprintf(stream, fmt, args);

    va_end(args);
}
