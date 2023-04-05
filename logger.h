#ifndef RAY_LOGGER_H
#define RAY_LOGGER_H


#include <stdlib.h>
#include <string.h>


#include "util.h"


#define LOG_LEVEL_NOLOG 0u
#define LOG_LEVEL_FATAL 1u
#define LOG_LEVEL_ERROR 2u
#define LOG_LEVEL_WARN  3u
#define LOG_LEVEL_INFO  4u
#define LOG_LEVEL_DEBUG 5u


#define LOG_LEVEL LOG_LEVEL_INFO


#if (LOG_LEVEL < LOG_LEVEL_ERROR) || (LOG_LEVEL > LOG_LEVEL_DEBUG)
#error LOG_LEVEL: Invalid log level
#endif


/* note: out-of-order assignment is a GNU extension */
PRIVATE const char *const LOG_PREFIXV[] = {
        [LOG_LEVEL_NOLOG] = NULL,
        [LOG_LEVEL_FATAL] = "FATAL",
        [LOG_LEVEL_ERROR] = "ERROR",
        [LOG_LEVEL_WARN] = "WARN",
        [LOG_LEVEL_INFO] = "INFO",
        [LOG_LEVEL_DEBUG] = "DEBUG"
};


#define logger_printf(level, fmt, ...)                                                          \
    do {                                                                                        \
        if(!(LOG_LEVEL >= LOG_LEVEL_NOLOG && LOG_LEVEL <= LOG_LEVEL_DEBUG)) {abort();}          \
        if (LOG_LEVEL < (unsigned int) (level)) {break;};                                       \
        fprintf(stderr, "%s:%d [%s] %s: ",                                                      \
               basename(__FILE__), __LINE__, *(LOG_PREFIXV + (level)), __func__);                \
        fprintf(stderr, (fmt), __VA_ARGS__);                                                    \
    } while (0)

#define logger_print(level, msg) logger_printf((level), "%s\n", (msg))

#define logger_perror(msg)                                                  \
    do {                                                                    \
        logger_printf(LOG_LEVEL_ERROR, "%s: %s\n", (msg), strerror(errno)); \
        print_stacktrace();                                                 \
    } while (0)


void print_stacktrace(void);


#endif //RAY_LOGGER_H
