#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

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

static void set_main_menu(struct game_t *game);

static void set_options_menu(struct game_t *game);

static inline void menu_close(struct game_t *const game) {
    game->paused = false;
    memset(&game->camera->movement, false, sizeof game->camera->movement);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    set_main_menu(game);
}

static inline void menu_quit(struct game_t *const game) {
    game->quit = true;
}

static inline void menu_on_event(const SDL_Event *const event, void *const arg) {
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
    menu_create(&menu, pos, size, "Menu", (void (*)(void *)) menu_close, game, menu_on_event, game);

    struct vec_t position = {.x = size.x / 2.0F, .y = size.y / 5.0F};

    menu_add_text(&menu, position, "*** GAME PAUSED ***", MENU_ALIGN_CENTER);
    position = (struct vec_t) {.x = size.x / 2.0F, .y = size.y / 3.0F};

    menu_add_button(&menu, position, "Resume", MENU_ALIGN_CENTER, (void (*)(void *)) menu_close, game);
    position.y += BUTTON_HEIGHT * 2;

    menu_add_button(&menu, position, "Options", MENU_ALIGN_CENTER, (void (*)(void *)) set_options_menu, game);
    position.y += BUTTON_HEIGHT * 2;

    menu_add_button(&menu, position, "Quit", MENU_ALIGN_CENTER, (void (*)(void *)) menu_quit, game);

    game->menu = menu;
}

static void set_options_menu(struct game_t *const game) {
    static const size_t width = SCREEN_WIDTH / 4;
    static const size_t height = SCREEN_HEIGHT / 4;

    const struct vec_t pos = {
            .x = (float) SCREEN_WIDTH / 2.0F - (float) width / 2.0F,
            .y = (float) SCREEN_HEIGHT / 2.0F - (float) height / 2.0F
    };
    const struct vec_t size = {.x = (float) width, .y = (float) height};

    struct menu_t menu;
    menu_create(&menu, pos, size, "Options", (void (*)(void *)) menu_close, game, menu_on_event, game);
    const float x = size.x / 2.0F;

    menu_add_text(&menu, (struct vec_t) {x, size.y / 3.0F}, "TBD", MENU_ALIGN_CENTER);
    menu_add_button(&menu, (struct vec_t) {x, size.y - size.y / 3.0F}, "Back", MENU_ALIGN_CENTER,
                    (void (*)(void *)) set_main_menu, game);

    game->menu = menu;
}

static void log_system_info(void) {
    logger_printf(LOG_LEVEL_INFO, "using SDL version %d.%d.%d, SDL_Image version %d.%d.%d\n",
                  SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL,
                  SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_PATCHLEVEL);

    logger_printf(LOG_LEVEL_DEBUG, "platform: %s\n", SDL_GetPlatform());
    logger_printf(LOG_LEVEL_DEBUG, "CPUs: %d, memory: %d MB\n", SDL_GetCPUCount(), SDL_GetSystemRAM());
    logger_print(LOG_LEVEL_INFO, "initializing SDL...");
    logger_printf(LOG_LEVEL_DEBUG, "initialized video driver: %s\n", SDL_GetCurrentVideoDriver());

    const int displays = SDL_GetNumVideoDisplays();

    if (displays < 1) {
        logger_printf(LOG_LEVEL_ERROR, "SDL: unable to get number of video displays (reason: '%s')", SDL_GetError());
        return;
    }

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

static void main_loop(struct game_t *const game) {
    if (game->quit) {
        logger_print(LOG_LEVEL_INFO, "quitting...");

        game_destroy(game);
        SDL_Quit();

#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#else
        exit(EXIT_SUCCESS);
#endif
    }

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        on_event(game, &event);
    }

    if (!game->paused) {
        update(game);
    }

    render(game);
    tick(game);
}

#ifndef __EMSCRIPTEN__

__attribute__((__noreturn__))
#endif
static void start_main_loop(struct game_t *const game) {
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg((void(*)(void *)) main_loop, game, 0, true);
#else
    for (;;) {
        main_loop(game);
    }
#endif
}

int main(const int argc, char **const argv) {
    const bool help = get_flag(argc, argv, "-h", "--help");
    const bool profile = get_flag(argc, argv, "-p", "--profile");

    if (help) {
        usage(argv[0]);
        return EXIT_SUCCESS;
    }

    for (int i = 0; i < argc; i++) {
        logger_printf(LOG_LEVEL_DEBUG, "argv[%d]: %s\n", i, argv[i]);
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        logger_print(LOG_LEVEL_FATAL, "SDL: unable to initialize SDL_INIT_VIDEO");
        return EXIT_FAILURE;
    }

    log_system_info();
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

    if (profile) {
        logger_printf(LOG_LEVEL_WARN, "profiling enabled, will quit after %d ticks\n", PROFILE_TICKS);
    }

    logger_print(LOG_LEVEL_INFO, "starting main loop...");
    start_main_loop(game);
}
