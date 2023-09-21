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


static bool get_flag(const int argc,
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

static void display_help(const char *const argv0) {
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

    if (game_init(game) != 0) {
        return EXIT_FAILURE;
    }

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
