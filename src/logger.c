#include <libgen.h>
#include <stdarg.h>

#include "color.h"

#include "logger.h"


struct log_target_t {
    const char *const prefix;
    const enum unused {
        STDOUT,
        STDERR
    } stream;
};

struct log_line_t {
    enum {
        LINE_NOT_REQUESTED,
        LINE_REQUESTED,
        LINE_STARTED
    } status;
    FILE *stream;
    const char *separator;
};


static const struct log_target_t LOG_TARGETS[] = {
        [LOG_LEVEL_FATAL] = {BRED "fatal" CRESET, STDERR},
        [LOG_LEVEL_ERROR] = {RED "error" CRESET, STDERR},
        [LOG_LEVEL_WARN]  = {HYEL "warn" CRESET, STDOUT},
        [LOG_LEVEL_INFO]  = {HBLU "info" CRESET, STDOUT},
        [LOG_LEVEL_DEBUG] = {HBLK "debug" CRESET, STDOUT}
};

static struct log_line_t lineinfo = {LINE_NOT_REQUESTED, NULL, NULL};


void logger_log(const enum log_level_t level,
                const char *const restrict file,
                const unsigned int line,
                const char *const restrict func,
                const char *const restrict fmt,
                const char *const restrict suffix, ...) {
    const struct log_target_t *const target = &LOG_TARGETS[level];
    FILE *const stream = target->stream == STDOUT ? stdout : stderr;

    if (lineinfo.status != LINE_STARTED) {
        char *const path = strdup(file);

        if (path == NULL) {
            perror("strdup");
            return;
        }

        fprintf(stream, HGRN "%s:%u" CRESET " [%s] " BHMAG "%s" CRESET ": ", basename(path), line, target->prefix,
                func);
        free(path);

        if (lineinfo.status == LINE_REQUESTED) {
            lineinfo.status = LINE_STARTED;
            lineinfo.stream = stream;
        }
    } else if (lineinfo.separator != NULL) {
        fputs(lineinfo.separator, stream);
    }

    va_list args;
    va_start(args, suffix);

    vfprintf(stream, fmt, args);

    va_end(args);

    if (suffix != NULL && lineinfo.status == LINE_NOT_REQUESTED) {
        fputs(suffix, stream);
    }
}

void logger_start_line(const char *const separator) {
    lineinfo.status = LINE_REQUESTED;
    lineinfo.separator = separator;
}

void logger_end_line(void) {
    if (lineinfo.status == LINE_STARTED) {
        fputc('\n', lineinfo.stream);
    }

    lineinfo.status = LINE_NOT_REQUESTED;
}
