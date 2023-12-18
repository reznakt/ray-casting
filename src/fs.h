#ifndef RAY_FS_H
#define RAY_FS_H


#include <stdio.h>

/**
 * @brief Open a file given by a path relative to the `/src` directory as a `FILE` stream with the given mode.
 * @param path The path to the file relative to the project root.
 * @param mode The mode to open the file in. One of "r", "w", "a", "r+", "w+", "a+".
 * @return A FILE stream on success, NULL on error.
 */
FILE *load_file(const char *path, const char *mode);

#endif //RAY_FS_H
