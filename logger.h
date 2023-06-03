#ifndef RAY_LOGGER_H
#define RAY_LOGGER_H


#include <stdlib.h>
#include <string.h>
#include <errno.h>


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
private const char *const LOG_PREFIXV[] = {
        [LOG_LEVEL_NOLOG] = NULL,
        [LOG_LEVEL_FATAL] = "FATAL",
        [LOG_LEVEL_ERROR] = "ERROR",
        [LOG_LEVEL_WARN] = "WARN",
        [LOG_LEVEL_INFO] = "INFO",
        [LOG_LEVEL_DEBUG] = "DEBUG"
};

/**
 * @brief Print a formatted message to the standard error stream.
 * @param level The log level of the message.
 * @param fmt The format string.
 * @param ... The arguments to the format string.
 */
#define logger_printf(level, fmt, ...)                                                          \
    do {                                                                                        \
        if(!(LOG_LEVEL >= LOG_LEVEL_NOLOG && LOG_LEVEL <= LOG_LEVEL_DEBUG)) {abort();}          \
        if (LOG_LEVEL < (unsigned int) (level)) {break;};                                       \
        fprintf(stderr, "%s:%d [%s] %s: ",                                                      \
               basename(__FILE__), __LINE__, *(LOG_PREFIXV + (level)), __func__);                \
        fprintf(stderr, (fmt), __VA_ARGS__);                                                    \
    } while (0)

/**
 * @brief Print a message to the standard error stream.
 * @param level The log level of the message.
 * @param msg The message to print.
 *
 * @note This macro is equivalent to calling logger_printf() with a format string of "%s\n".
 * @note A newline character is appended to the message.
 *
 * @see logger_printf()
 * @see logger_perror()
 */
#define logger_print(level, msg) logger_printf((level), "%s\n", (msg))

/**
 * @brief Print a message based on the value of errno to the standard error stream.
 * @param msg Additional message to print.
 */
#define logger_perror(msg)                                                  \
    do {                                                                    \
        logger_printf(LOG_LEVEL_ERROR, "%s: %s\n", (msg), strerror(errno)); \
        print_stacktrace();                                                 \
    } while (0)

/**
 * @brief Print the stack trace to the standard error stream.
 */
void print_stacktrace(void);


#endif //RAY_LOGGER_H
