#ifndef RAY_TESTS_UTIL_H
#define RAY_TESTS_UTIL_H


enum timer_op_t {
    TIMER_START,
    TIMER_STOP
};


/**
 * @brief Measures the time elapsed between calls to this function with TIMER_START and TIMER_STOP.
 * @param op TIMER_START to start the timer, TIMER_STOP to stop it.
 * @return If op is TIMER_START, returns 0. If op is TIMER_STOP, returns the elapsed time in milliseconds.
 */
intmax_t timer(enum timer_op_t op);

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
void print_justified(const char *str, size_t width, char pad);


#endif //RAY_TESTS_UTIL_H
