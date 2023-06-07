#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include <SDL2/SDL.h>


#include "vector.h"
#include "conf.h"
#include "world.h"
#include "ray.h"
#include "game.h"
#include "logger.h"
#include "util.h"


private int init(struct game_t *const game) {
    game->window = SDL_CreateWindow(SCREEN_TITLE, SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                                    SCREEN_FLAGS);
    if (!game->window) {
        sdl_error("SDL_CreateWindow");
        return -1;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);

    if (!game->renderer) {
        sdl_error("SDL_CreateRenderer");
        return -1;
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);

    return 0;
}

private void cleanup(const struct game_t *const game) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
}

int main(int argc, char **argv) {
    bool profile = false;

    if (argc == 2 && (strcmp(argv[1], "--profile") == 0 || strcmp(argv[1], "-p") == 0)) {
        profile = true;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        sdl_error("SDL_Init");
        return EXIT_FAILURE;
    }

    if (!check_walls()) {
        logger_print(LOG_LEVEL_FATAL, "invalid wall configuration");
        return EXIT_FAILURE;
    }

    struct game_t game = {0};
    game_init(game);

    if (init(&game) != 0) {
        return EXIT_FAILURE;
    }

    while (true) {
        if (profile && game.ticks >= 10000) {
            goto __cleanup;
        }

        update(&game);
        render(&game);

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (!on_event(&game, &event)) {
                goto __cleanup;
            }
        }
    }

    __cleanup:
    if (profile) {
        printf("ticks: %zu, frames: %zu, avg fps: %f\n",
               game.ticks, game.frames, (float) game.frames / (float) game.ticks * 1000.0f);
    }
    cleanup(&game);
    SDL_Quit();

    return EXIT_SUCCESS;
}

