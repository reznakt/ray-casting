#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <SDL2/SDL.h>

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

FILE *open_file(const char *const restrict path, const char *const restrict mode) {
    if (path == NULL || mode == NULL) {
        return NULL;
    }

    const int flags = mode_to_flags(mode);

    if (flags == -1) {
        logger_print(LOG_LEVEL_ERROR, "invalid mode");
        return NULL;
    }

    char *const dirpath = SDL_GetBasePath();

    if (dirpath == NULL) {
        return NULL;
    }

    const int dir_fd = open(dirpath, O_DIRECTORY);

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
