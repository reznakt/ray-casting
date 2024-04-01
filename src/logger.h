#ifndef RAY_LOGGER_H
#define RAY_LOGGER_H


#include <errno.h>
#include <stdio.h>

#include "util.h"


/**
 * @brief Log levels used by the logger_* family of functions.
 */
enum unused log_level_t {
    LOG_LEVEL_FATAL, /**< A fatal error that causes the program to exit. */
    LOG_LEVEL_ERROR, /**< An error that may or may not cause the program to exit. */
    LOG_LEVEL_WARN, /**< A suspicious condition that should be brought to the user's attention. */
    LOG_LEVEL_INFO, /**< An informational message. */
    LOG_LEVEL_DEBUG /**< A message intended for debugging purposes. */
};


/**
 * @brief Log a formatted message to one of the standard streams.
 * @param level The log level of the message.
 * @param fmt The format string.
 * @param ... The arguments to the format string.
 */
#define logger_printf(level, fmt, ...) logger_log((level), __FILE__, __LINE__, __func__, (fmt), NULL, __VA_ARGS__)

/**
 * @brief Log a formatted message to one of the standard streams.
 * @param level The log level of the message.
 * @param msg The message to print.
 *
 * @note This macro is equivalent to calling logger_printf() with a format string of "%s\n".
 * @note A newline character is appended to the message.
 *
 * @see logger_printf()
 * @see logger_perror()
 */
#define logger_print(level, msg) logger_log((level), __FILE__, __LINE__, __func__, "%s", "\n", (msg))

/**
 * @brief Log a message based on the value of errno to the standard error stream.
 * @param msg Additional message to print.
 */
#define logger_perror(msg) logger_printf(LOG_LEVEL_ERROR, "%s: %s\n", (msg), strerror(errno))


/**
 * @brief Log a formatted message to a stream.
 * @param level The log level of the message.
 * @param file The file name.
 * @param line The line number.
 * @param func The function name.
 * @param fmt The format string.
 * @param append_newline Whether to append a newline character to the message.
 * @param ... The arguments to the format string.
 */
__attribute__((__format__(__printf__, 5, 7)))
void logger_log(enum log_level_t level,
                const char *file,
                unsigned int line,
                const char *func,
                const char *fmt,
                const char *suffix, ...);


/**
 * @brief Start a new line in the log.
 * All subsequent log messages will be printed on the same line until @p logger_end_line() is called.
 * @param separator The separator to print between log messages, or NULL, if no separator should be printed.
 */
unused void logger_start_line(const char *separator);

/**
 * @brief End the current line in the log.
 * This function has no effect if no line has been started.
 */
unused void logger_end_line(void);


#endif //RAY_LOGGER_H
