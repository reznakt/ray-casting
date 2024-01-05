#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#ifdef __linux__

#include <linux/limits.h>

#else

#include <limits.h>

#endif


#ifndef PATH_MAX
#define PATH_MAX 4096
#endif


#include "fs.h"
#include "logger.h"


/**
 * @brief Converts a mode string to the corresponding flags for open(2).
 * @param mode The mode string to convert.
 * @return The corresponding flags for open(2) or -1 if the mode string is invalid.
 */
static int mode_to_flags(const char *const mode) {
    if (mode == NULL) {
        return -1;
    }

    if (strcmp(mode, "r") == 0) {
        return O_RDONLY;
    }

    if (strcmp(mode, "w") == 0) {
        return O_WRONLY | O_CREAT | O_TRUNC;
    }

    if (strcmp(mode, "a") == 0) {
        return O_WRONLY | O_CREAT | O_APPEND;
    }

    if (strcmp(mode, "r+") == 0) {
        return O_RDWR;
    }

    if (strcmp(mode, "w+") == 0) {
        return O_RDWR | O_CREAT | O_TRUNC;
    }

    if (strcmp(mode, "a+") == 0) {
        return O_RDWR | O_CREAT | O_APPEND;
    }

    return -1;
}

/**
 * @brief Returns the path to the application executable.
 * @return The path to the executable or NULL if an error occurred.
 */
static char *executable_path(void) {
    char *const buf = calloc(PATH_MAX + 1, sizeof *buf);

    if (buf == NULL) {
        logger_perror("calloc");
        return NULL;
    }

    switch (readlink("/proc/self/exe", buf, PATH_MAX)) {
        case -1:
            logger_perror("readlink");
            goto bailout;
        case PATH_MAX:
            logger_printf(LOG_LEVEL_ERROR, "readlink: path too long (PATH_MAX = %u)\n", PATH_MAX);
            goto bailout;
        default:
            break;
    }

    return buf;

    bailout:
    free(buf);
    return NULL;
}

/**
 * @brief Returns the directory containing the application executable.
 * @return The directory containing the executable or NULL if an error occurred.
 */
static char *executable_dir(void) {
    char *const path = executable_path();

    if (path == NULL) {
        return NULL;
    }

    char *const dir = dirname(path);

    if (dir == NULL) {
        logger_perror("dirname");
        free(path);
        return NULL;
    }

    return dir;
}

FILE *open_file(const char *const restrict path, const char *const restrict mode) {
    if (path == NULL || mode == NULL) {
        return NULL;
    }

    const int flags = mode_to_flags(mode);

    if (flags == -1) {
        logger_print(LOG_LEVEL_ERROR, "invalid mode");
        return NULL;
    }

    char *const dirpath = executable_dir();

    if (dirpath == NULL) {
        return NULL;
    }

    const int dir_fd = open(dirpath, O_DIRECTORY | O_PATH);

    if (dir_fd == -1) {
        logger_perror("open");
        free(dirpath);
        return NULL;
    }

    free(dirpath);
    const int fd = openat(dir_fd, path, flags);

    if (fd == -1) {
        logger_perror("openat");
        close(dir_fd);
        return NULL;
    }

    close(dir_fd);
    FILE *const stream = fdopen(fd, mode);

    if (stream == NULL) {
        logger_perror("fdopen");
        close(fd);
        return NULL;
    }

    return stream;
}
