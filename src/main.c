#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include "event.h"
#include "game.h"
#include "logger.h"
#include "menu.h"
#include "ray.h"


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

static inline void usage(const char *const argv0) {
    static const char *const fmt = "usage: %s [-h|--help] [-p|--profile]\n"
                                   "\t-h, --help\t\tprint this help message and exit\n"
                                   "\t-p, --profile\t\tprint profiling information and exit\n";

    printf(fmt, argv0);
}

static void menu_close(void *const arg) {
    struct game_t *const game = arg;
    game->paused = false;
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

static void menu_quit(void *const arg) {
    struct game_t *const game = arg;
    game->quit = true;
}

static void menu_add_centered_button(struct menu_t *const restrict menu,
                                     struct vec_t pos,
                                     const char *const restrict name,
                                     void (*const on_click)(void *arg),
                                     void *const restrict on_click_arg) {
    const size_t width = menu_button_width(name);
    pos.x -= (float) width / 2.0F;
    menu_add_button(menu, pos, name, on_click, on_click_arg);
}

static void menu_add_centered_text(struct menu_t *const restrict menu,
                                   struct vec_t pos,
                                   const char *const restrict value) {
    const size_t width = menu_button_width(value) - 2 * BUTTON_PADDING;
    pos.x -= (float) width / 2.0F;
    menu_add_text(menu, pos, value);
}

static void menu_on_event(const SDL_Event *const event, void *const arg) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case KEY_PAUSE:
                menu_close(arg);
                break;
        }
    }
}

static void set_main_menu(struct game_t *const game) {
    static const size_t width = SCREEN_WIDTH / 4;
    static const size_t height = SCREEN_HEIGHT / 4;

    const struct vec_t pos = {
            .x = (float) SCREEN_WIDTH / 2.0F - (float) width / 2.0F,
            .y = (float) SCREEN_HEIGHT / 2.0F - (float) height / 2.0F
    };
    const struct vec_t size = {.x = (float) width, .y = (float) height};

    struct menu_t menu;
    menu_create(&menu, pos, size, "Menu", menu_close, game, menu_on_event, game);

    struct vec_t position = {.x = size.x / 2.0F, .y = size.y / 5.0F};

    menu_add_centered_text(&menu, position, "*** GAME PAUSED ***");
    position = (struct vec_t) {.x = size.x / 2.0F, .y = size.y / 3.0F};

    menu_add_centered_button(&menu, position, "Resume", menu_close, game);
    position.y += BUTTON_HEIGHT * 2;

    menu_add_centered_button(&menu, position, "Options", NULL, NULL);
    position.y += BUTTON_HEIGHT * 2;

    menu_add_centered_button(&menu, position, "Quit", menu_quit, game);

    game->menu = menu;
}

int main(const int argc, char **const argv) {
    if (get_flag(argc, argv, "-h", "--help")) {
        usage(argv[0]);
        return EXIT_SUCCESS;
    }

    for (int i = 0; i < argc; i++) {
        logger_printf(LOG_LEVEL_DEBUG, "argv[%d]: %s\n", i, argv[i]);
    }

    logger_printf(LOG_LEVEL_INFO, "using SDL version %d.%d.%d, SDL_Image version %d.%d.%d\n",
                  SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL,
                  SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_PATCHLEVEL);

    logger_printf(LOG_LEVEL_DEBUG, "platform: %s\n", SDL_GetPlatform());
    logger_printf(LOG_LEVEL_DEBUG, "CPUs: %d, memory: %d MB\n", SDL_GetCPUCount(), SDL_GetSystemRAM());
    logger_print(LOG_LEVEL_INFO, "initializing SDL...");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        logger_print(LOG_LEVEL_FATAL, "SDL: unable to initialize SDL_INIT_VIDEO");
        return EXIT_FAILURE;
    }

    logger_printf(LOG_LEVEL_DEBUG, "initialized video driver: %s\n", SDL_GetCurrentVideoDriver());
    const int displays = SDL_GetNumVideoDisplays();

    if (displays < 1) {
        logger_printf(LOG_LEVEL_ERROR, "SDL: unable to get number of video displays (reason: '%s')", SDL_GetError());
    } else {
        for (int i = 0; i < displays; i++) {
            const char *const name = SDL_GetDisplayName(i);

            if (name == NULL) {
                logger_printf(LOG_LEVEL_ERROR, "SDL: unable to get display name for display %d (reason: '%s')",
                              i, SDL_GetError());
                continue;
            }

            SDL_DisplayMode mode;

            if (SDL_GetDesktopDisplayMode(i, &mode) != 0) {
                logger_printf(LOG_LEVEL_ERROR, "SDL: unable to get desktop display mode for display %d (reason: '%s')",
                              i, SDL_GetError());
                continue;
            }

            logger_printf(LOG_LEVEL_DEBUG, "display %d: %s (%dx%d px @ %dHz)\n",
                          i, name, mode.w, mode.h, mode.refresh_rate);

        }
    }

    logger_print(LOG_LEVEL_INFO, "initializing SDL_Image...");

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        logger_print(LOG_LEVEL_FATAL, "SDL_Image: unable to initialize IMG_INIT_PNG");
        return EXIT_FAILURE;
    }

    logger_print(LOG_LEVEL_INFO, "creating and initializing game objects...");
    struct game_t *const game = game_create();

    if (game_init(game) != 0) {
        logger_print(LOG_LEVEL_FATAL, "unable to initialize game");
        return EXIT_FAILURE;
    }

    set_main_menu(game);

    const bool profile = get_flag(argc, argv, "-p", "--profile");

    if (profile) {
        logger_printf(LOG_LEVEL_WARN, "profiling enabled, will quit after %zu ticks\n", PROFILE_TICKS);
    }

    logger_print(LOG_LEVEL_INFO, "starting main loop...");

    while (!game->quit) {
        if (profile && game->ticks >= PROFILE_TICKS) {
            break;
        }

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            on_event(game, &event);
        }

        update(game);
        render(game);
    }

    logger_print(LOG_LEVEL_INFO, "quitting...");

    if (profile) {
        logger_printf(LOG_LEVEL_DEBUG, "ticks: %zu, frames: %zu, avg fps: %f\n",
                      game->ticks, game->frames, (float) game->frames / (float) game->ticks * 1000.0F);
    }

    game_destroy(game);
    SDL_Quit();

    return EXIT_SUCCESS;
}
