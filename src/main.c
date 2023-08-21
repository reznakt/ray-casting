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

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        logger_print(LOG_LEVEL_FATAL, "SDL_Image: unable to initialize IMG_INIT_PNG");
        return EXIT_FAILURE;
    }

    struct game_t *const game = game_create();

    logger_printf(LOG_LEVEL_INFO, "loaded %zu objects from %s: \n", game->nobjects, WORLD_SPEC_FILE);

    for (size_t i = 0; i < game->nobjects; i++) {
        const struct wobject_t object = game->objects[i];

        if (object.type == WALL) {
            const struct wall_t wall = object.data.wall;

            logger_printf(LOG_LEVEL_INFO, "\twall %.0f %.0f %.0f %.0f %s %u\n",
                          wall.a.x, wall.a.y, wall.b.x, wall.b.y, strcolor(wall.color), wall.type);
        }
    }


    if (init(game) != 0) {
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

    logger_printf(LOG_LEVEL_INFO, "loaded texture atlas from '%s' (%dx%d)\n", TEXTURE_ATLAS_FILE, surface->w,
                  surface->h);

    while (true) {
        if (profile && game->ticks >= 10000) {
            goto __cleanup;
        }

        update(game);
        render(game);

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (!on_event(game, &event)) {
                goto __cleanup;
            }
        }
    }

    __cleanup:
    if (profile) {
        printf("ticks: %zu, frames: %zu, avg fps: %f\n",
               game->ticks, game->frames, (float) game->frames / (float) game->ticks * 1000.0F);
    }
    cleanup(game);
    SDL_Quit();

    return EXIT_SUCCESS;
}
