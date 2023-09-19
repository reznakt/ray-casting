#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include "conf.h"
#include "event.h"
#include "game.h"
#include "logger.h"
#include "ray.h"
#include "util.h"
#include "vector.h"
#include "world.h"


private bool get_flag(const int argc,
                      char *const *const restrict argv,
                      const char *const restrict shortopt,
                      const char *const restrict longopt) {
    for (int i = 1; i < argc; i++) {
        if (shortopt != NULL && strcmp(argv[i], shortopt) == 0) {
            return true;
        }

        if (longopt != NULL && strcmp(argv[i], longopt) == 0) {
            return true;
        }
    }

    return false;
}

private void display_help(const char *const argv0) {
    const char *const fmt = "usage: %s [-h|--help] [-p|--profile]\n"
                            "\t-h, --help\t\tprint this help message and exit\n"
                            "\t-p, --profile\t\tprint profiling information and exit\n";

    printf(fmt, argv0);
}

int main(const int argc, char **const argv) {
    const bool help = get_flag(argc, argv, "-h", "--help");

    if (help) {
        display_help(argv[0]);
        return EXIT_SUCCESS;
    }

    const bool profile = get_flag(argc, argv, "-p", "--profile");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return EXIT_FAILURE;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        logger_print(LOG_LEVEL_FATAL, "SDL_Image: unable to initialize IMG_INIT_PNG");
        return EXIT_FAILURE;
    }

    struct game_t *const game = game_create();

    logger_printf(LOG_LEVEL_INFO, "loaded %zu objects from %s: \n", game->nobjects, WORLD_SPEC_FILE);

    for (size_t i = 0; i < game->nobjects; i++) {
        const struct wobject_t *const object = game->objects[i];

        if (object->type == WALL) {
            const struct wall_t wall = object->data.wall;

            logger_printf(LOG_LEVEL_INFO, "\twall %.0f %.0f %.0f %.0f %s %u\n",
                          wall.a.x, wall.a.y, wall.b.x, wall.b.y, strcolor(wall.color), wall.type);
        }
    }

    if (game_init(game) != 0) {
        return EXIT_FAILURE;
    }

    SDL_Surface *const surface = IMG_Load(TEXTURE_ATLAS_FILE);

    if (surface == NULL) {
        logger_printf(LOG_LEVEL_FATAL, "unable to load texture atlas from '%s'\n", TEXTURE_ATLAS_FILE);
        return EXIT_FAILURE;
    }

    game->texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    if (game->texture == NULL) {
        logger_print(LOG_LEVEL_FATAL, "unable to create texture from surface");
        return EXIT_FAILURE;
    }

    logger_printf(LOG_LEVEL_INFO, "loaded texture atlas from '%s' (%dx%d)\n",
                  TEXTURE_ATLAS_FILE, surface->w, surface->h);

    while (!game->quit) {
        if (profile && game->ticks >= 10000) {
            break;
        }

        update(game);
        render(game);

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            on_event(game, &event);
        }
    }

    if (profile) {
        printf("ticks: %zu, frames: %zu, avg fps: %f\n",
               game->ticks, game->frames, (float) game->frames / (float) game->ticks * 1000.0F);
    }

    game_destroy(game);
    SDL_Quit();

    return EXIT_SUCCESS;
}
