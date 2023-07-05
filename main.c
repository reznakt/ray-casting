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
#include "event.h"
#include "ringbuffer.h"


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
    struct ringbuffer_t *const buffer = ringbuffer(10);

    for (size_t i = 0; i < 10; i++) {
        ringbuffer_push(buffer, (void *) i);
    }

    printf("size: %zu, capacity: %zu\n", ringbuffer_size(buffer), ringbuffer_capacity(buffer));
    // test peek at
    printf("peek at 0: %p\n", ringbuffer_peek_at(buffer, 0));
    printf("peek at 1: %p\n", ringbuffer_peek_at(buffer, 1));
    printf("peek at 2: %p\n", ringbuffer_peek_at(buffer, 2));
    printf("peek at 3: %p\n", ringbuffer_peek_at(buffer, 3));
    printf("peek at 4: %p\n", ringbuffer_peek_at(buffer, 4));


    exit(0);
    bool profile = false;

    if (argc == 2 && (strcmp(argv[1], "--profile") == 0 || strcmp(argv[1], "-p") == 0)) {
        profile = true;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        sdl_error("SDL_Init");
        return EXIT_FAILURE;
    }

    struct game_t game = {0};
    game_init(game);

    logger_printf(LOG_LEVEL_INFO, "loaded %zu objects from %s: \n", game.nobjects, WORLD_SPEC_FILE);

    for (size_t i = 0; i < game.nobjects; i++) {
        const struct wobject_t object = game.objects[i];

        if (object.type == WALL) {
            const struct wall_t wall = object.data.wall;

            logger_printf(LOG_LEVEL_INFO, "\twall %.0f %.0f %.0f %.0f %s %u\n",
                          wall.a.x, wall.a.y, wall.b.x, wall.b.y, strcolor(wall.color), wall.type);
        }
    }


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

