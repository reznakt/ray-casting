#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include <SDL2/SDL.h>


#include "vector.h"
#include "conf.h"
#include "world.h"
#include "ray.h"
#include "game.h"


int init(struct game_t *const game) {
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

void cleanup(const struct game_t *const game) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
}

int main(UNUSED int argc, UNUSED char **argv) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        sdl_error("SDL_Init");
        return EXIT_FAILURE;
    }

    struct game_t game;
    game_init(game);


    if (init(&game) != 0) {
        return EXIT_FAILURE;
    }

    while (true) {
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
    cleanup(&game);
    SDL_Quit();

    return EXIT_SUCCESS;
}

