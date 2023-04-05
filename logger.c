#include "logger.h"

#include <execinfo.h>


void print_stacktrace(void) {
    logger_print(LOG_LEVEL_FATAL, "Unexpected internal error");
    logger_print(LOG_LEVEL_FATAL, "");
    logger_print(LOG_LEVEL_FATAL, "Stack trace:");

    void *symbols[50];
    const int length = backtrace(symbols, 50);
    char **stacktrace = backtrace_symbols(symbols, length);

    for (int i = length - 1; i >= 0; i--) {
        logger_printf(LOG_LEVEL_FATAL, "\t%s%s\n", stacktrace[i], i ? "" : " (latest)");
    }

    free(stacktrace);
    logger_print(LOG_LEVEL_FATAL, "");
}
