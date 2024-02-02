#ifndef RAY_TESTS_UTIL_H
#define RAY_TESTS_UTIL_H


#include <inttypes.h>


/**
 * @brief Starts a timer. Call timer_stop() to get the elapsed time.
 * Only one timer can be active at any given time.
 *
 * @return 0 on success, -1 on error.
 */
int timer_start(void);

/**
 * @brief Stops the timer started by timer_start() and returns the elapsed time in milliseconds.
 * The result is stored in @p duration.
 * @return 0 on success, -1 on error.
 */
int timer_stop(intmax_t *duration);

/**
 * @brief Returns a random seed obtained from /dev/urandom.
 * @return A random seed. If /dev/urandom could not be opened, calls abort(3).
 */
unsigned int get_seed(void);

/**
 * @brief Prints a string justified to the left.
 * @param str The string to print.
 * @param width The total desired width of the string.
 * @param pad The character used to pad the string.
 */
void print_justified(const char *str, unsigned int width, char pad);


#endif //RAY_TESTS_UTIL_H
