#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __EMSCRIPTEN__
#include <limits.h>
#include <emscripten.h>
#endif /* __EMSCRIPTEN__ */

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "event.h"
#include "game.h"
#include "logger.h"
#include "menu.h"
#include "version.h"


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

    fprintf(stderr, fmt, argv0);
}

static inline void version(const char *const argv0) {
    puts(argv0);
    putchar('\n');
    printf("commit: %s\nmessage: %.50s\nbranch: %s\ncreated at: %s\n\n",
           GIT_COMMIT_HASH, GIT_COMMIT_MESSAGE, GIT_BRANCH, GIT_COMMIT_TIME);
    printf("built by: %s@%s\nbuilt at: %s\n", BUILD_USER, BUILD_HOST, BUILD_TIME);
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
    logger_printf(LOG_LEVEL_INFO, "using SDL version %d.%d.%d, SDL2_gfx version %d.%d.%d\n",
                  SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL,
                  SDL2_GFXPRIMITIVES_MAJOR, SDL2_GFXPRIMITIVES_MINOR, SDL2_GFXPRIMITIVES_MICRO);

    logger_printf(LOG_LEVEL_DEBUG, "platform: %s\n", SDL_GetPlatform());
    logger_printf(LOG_LEVEL_DEBUG, "CPUs: %d, memory: %d MiB\n", SDL_GetCPUCount(), SDL_GetSystemRAM());
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

        logger_printf(LOG_LEVEL_DEBUG, "display %d: %s (%d x %d px @ %d Hz)\n",
                      i, name, mode.w, mode.h, mode.refresh_rate);
    }
}

#ifndef __EMSCRIPTEN__

__attribute__((__noreturn__))
#endif
static void start_main_loop(void (*const func)(void *), void *const arg) {
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(func, arg, INT_MAX, true);
#else
    for (;;) {
        func(arg);
    }
#endif
}

__attribute__((__noreturn__)) static void stop_main_loop(void) {
#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();
#endif

    exit(EXIT_SUCCESS);
}

static void main_loop(struct game_t *const game) {
    if (game->quit) {
        logger_print(LOG_LEVEL_INFO, "quitting...");
        game_destroy(game);
        SDL_Quit();
        stop_main_loop();
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

int main(const int argc, char **const argv) {
    if (get_flag(argc, argv, "-h", "--help")) {
        usage(argv[0]);
        return EXIT_SUCCESS;
    }

    if (get_flag(argc, argv, "-v", "--version")) {
        version(argv[0]);
        return EXIT_SUCCESS;
    }

    logger_printf(LOG_LEVEL_INFO, "built at %s from commit %s on branch %s\n", BUILD_TIME, GIT_COMMIT_HASH, GIT_BRANCH);

    for (int i = 0; i < argc; i++) {
        logger_printf(LOG_LEVEL_DEBUG, "argv[%d]: %s\n", i, argv[i]);
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        logger_printf(LOG_LEVEL_FATAL, "SDL_Init: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    log_system_info();
    logger_print(LOG_LEVEL_INFO, "creating and initializing game objects...");

    struct game_t *const game = game_create();

    if (game_init(game) != 0) {
        logger_print(LOG_LEVEL_FATAL, "unable to initialize game");
        return EXIT_FAILURE;
    }

    set_main_menu(game);

    if (get_flag(argc, argv, "-p", "--profile")) {
        logger_printf(LOG_LEVEL_WARN, "profiling enabled, will quit after %d ticks\n", PROFILE_TICKS);
    }

    logger_print(LOG_LEVEL_INFO, "starting main loop...");
    start_main_loop((void (*)(void *)) main_loop, game);
}
