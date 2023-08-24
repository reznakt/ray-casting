#include <errno.h>
#include <SDL2/SDL.h>


#include "logger.h"
#include "util.h"
#include "vector.h"


#include "world.h"


/**
 * Maximum number of columns in a record (line).
 */
#define PARSER_MAX_COLS 256


#define parse_coord(dst, token)                                                     \
do {                                                                                \
    if (parse_coordinate((dst), (token)) != 0) {                                    \
        logger_printf(LOG_LEVEL_ERROR, "unable to parse coordinate '%s'\n", token); \
        return -1;                                                                  \
    }                                                                               \
} while (0)


private int parse_coordinate(float *const restrict dst, const char *const restrict token) {
    if (token == NULL) {
        return -1;
    }

    float result;

    if (strcmp(token, "WIDTH") == 0) {
        result = SCREEN_WIDTH;
    } else if (strcmp(token, "HEIGHT") == 0) {
        result = SCREEN_HEIGHT;
    } else if (is_decimal(token)) {
        errno = 0;
        result = (float) strtof(token, NULL);

        if (errno != 0) {
            logger_perror("strtof");
            return -1;
        }
    } else {
        logger_printf(LOG_LEVEL_ERROR, "unexpected non-decimal token: '%s'\n", token);
        return -1;
    }

    if (dst != NULL) {
        *dst = result;
    }

    return 0;
}


/**
 * @brief Parses a color string.
 * @param colorstr a string containing a color in hexadecimal format (e.g. #ff0000 for red).
 * @param dst pointer to a SDL_Color struct to store the parsed color, or NULL, if the color should not be stored.
 * @return 0 on success, -1 on error (dst is not modified in this case).
 */
private int parse_color(const char *const colorstr, SDL_Color *const dst) {
    if (strlen(colorstr) != 7 || *colorstr != '#') {
        return -1;
    }

    const int r1 = hex_to_dec(colorstr[1]), r2 = hex_to_dec(colorstr[2]);
    const int g1 = hex_to_dec(colorstr[3]), g2 = hex_to_dec(colorstr[4]);
    const int b1 = hex_to_dec(colorstr[5]), b2 = hex_to_dec(colorstr[6]);

    if (r1 == -1 || r2 == -1 || g1 == -1 || g2 == -1 || b1 == -1 || b2 == -1) {
        return -1;
    }

    if (dst) {
        dst->r = (Uint8) (r1 * 16 + r2);
        dst->g = (Uint8) (g1 * 16 + g2);
        dst->b = (Uint8) (b1 * 16 + b2);
        dst->a = 255;
    }

    return 0;
}


/**
 * @brief Parses a record (line) in the world specification.
 * @param record the record to parse. Must be a null-terminated string with at most PARSER_MAX_COLS characters.
 * @param dst pointer to a wobject_t struct to store the parsed object, or NULL, if the object should not be stored.
 * @return 0 on success, -1 on error (dst is not modified in this case).
 */
private int parse_record(char *const restrict record, struct wobject_t *const dst) {
    if (record == NULL) {
        return -1;
    }

    char *const line = strncpy(stack_alloc(char, PARSER_MAX_COLS), record, PARSER_MAX_COLS);
    const char *token;
    size_t tokenno = 0;
    struct wobject_t object = {0};

    for (token = strtok(line, " "); token != NULL; token = strtok(NULL, " "), tokenno++) {
        if (tokenno == 0) { /* type */
            if (strcmp(token, "wall") == 0) {
                object.type = WALL;
                memset(&object.data.wall, 0, sizeof object.data.wall);
                continue;
            }

            logger_printf(LOG_LEVEL_ERROR, "invalid object type: %s\n", token);
            return -1;
        }

        switch (object.type) {
            case WALL: {
                switch (tokenno) {
                    /* 1-4: wall coordinates */
                    case 1: /* a.x */
                        parse_coord(&object.data.wall.a.x, token);
                        break;
                    case 2:  /* a.y */
                        parse_coord(&object.data.wall.a.y, token);
                        break;
                    case 3:  /* b.x */
                        parse_coord(&object.data.wall.b.x, token);
                        break;
                    case 4:  /* b.y */
                        parse_coord(&object.data.wall.b.y, token);
                        break;
                    case 5:  /* wall color */
                        if (parse_color(token, &object.data.wall.color) != 0) {
                            logger_printf(LOG_LEVEL_ERROR, "invalid color: %s\n", token);
                            return -1;
                        }
                        break;
                    case 6: /* wall type/material */
                        if (strcmp(token, "solid") == 0) {
                            object.data.wall.type = WALL_TYPE_SOLID;
                            break;
                        }
                        if (strcmp(token, "nonsolid") == 0) {
                            object.data.wall.type = WALL_TYPE_NONSOLID;
                            break;
                        }
                        // TODO: add more wall types
                        break;
                    default:
                        logger_printf(LOG_LEVEL_ERROR, "unexpected token '%s' starting at position %zu\n",
                                      token, token - line + 1);
                        return -1;
                }
                break;
            }
        }
    }

    if (dst != NULL) {
        memcpy(dst, &object, sizeof object);
    }

    return 0;
}


int load_world(const char *const restrict path,
               struct wobject_t *const restrict objects,
               size_t *const restrict nobjects) {
    if (path == NULL) {
        return -1;
    }

    FILE *const stream = fopen(path, "r");

    if (stream == NULL) {
        logger_perror("fopen");
        return -1;
    }

    if (nobjects != NULL) {
        *nobjects = 0;
    }

    char line[PARSER_MAX_COLS] = {0};

    for (int i = 0, ch = fgetc(stream); ch != EOF; ch = fgetc(stream), i++) {
        if (i == sizeof line) {
            logger_print(LOG_LEVEL_ERROR, "line too long");
            fclose(stream);
            return -1;
        }

        if (ch != '\n') {
            line[i] = (char) ch;
            continue;
        }

        if (is_whitespace(line)) {
            goto next;
        }

        struct wobject_t object = {0};

        if (parse_record(line, &object) != 0) {
            fclose(stream);
            return -1;
        }

        if (objects != NULL && nobjects != NULL) {
            memcpy(&objects[(*nobjects)++], &object, sizeof object);
        }

        next:
        i = -1;
        memset(line, 0, sizeof line);
    }

    fclose(stream);
    return 0;
}
