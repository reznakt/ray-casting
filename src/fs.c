#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#include "fs.h"
#include "logger.h"


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

    char filepath[] = __FILE__;
    const char *const dirpath = dirname(filepath);

    if (dirpath == NULL) {
        logger_perror("dirname");
        return NULL;
    }

    const int dir_fd = open(dirpath, O_DIRECTORY | O_PATH);

    if (dir_fd == -1) {
        logger_perror("open");
        return NULL;
    }

    const int fd = openat(dir_fd, path, flags);
    close(dir_fd);

    if (fd == -1) {
        logger_perror("openat");
        return NULL;
    }

    FILE *const stream = fdopen(fd, mode);

    if (stream == NULL) {
        logger_perror("fdopen");
        close(fd);
        return NULL;
    }

    return stream;
}
