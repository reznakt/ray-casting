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

static void log_system_info(void) {
    logger_printf(LOG_LEVEL_INFO, "using SDL version %d.%d.%d, SDL2_gfx version %d.%d.%d\n",
                  SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL,
                  SDL2_GFXPRIMITIVES_MAJOR, SDL2_GFXPRIMITIVES_MINOR, SDL2_GFXPRIMITIVES_MICRO);

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

    update(game);
    render(game);
    tick(game);
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
    logger_print(LOG_LEVEL_INFO, "creating and initializing game objects...");

    struct game_t *const game = game_create();

    if (game_init(game) != 0) {
        logger_print(LOG_LEVEL_FATAL, "unable to initialize game");
        return EXIT_FAILURE;
    }

    if (profile) {
        logger_printf(LOG_LEVEL_WARN, "profiling enabled, will quit after %d ticks\n", PROFILE_TICKS);
    }

    if (menu_initialize() != 0) {
        logger_print(LOG_LEVEL_FATAL, "unable to initialize menu");
        return EXIT_FAILURE;
    }

    struct menu_elem_t *const foo = menu_create_element("foo", MENU_ELEM_CONTAINER);

    if (foo == NULL) {
        logger_print(LOG_LEVEL_FATAL, "unable to create menu element");
        return EXIT_FAILURE;
    }

    if (menu_append_child(NULL, foo) != 0) {
        logger_print(LOG_LEVEL_FATAL, "unable to append child to menu element");
        return EXIT_FAILURE;
    }

    foo->margin.left = menu_percent(50.0F);
    foo->margin.top = menu_percent(50.0F);

    menu_append_child(menu_get_element_by_id("foo"), menu_create_element("bar", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("bar"), menu_create_element("baz", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("baz"), menu_create_element("qux", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("qux"), menu_create_element("quux", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("quux"), menu_create_element("corge", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("corge"), menu_create_element("grault", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("grault"), menu_create_element("garply", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("garply"), menu_create_element("waldo", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("waldo"), menu_create_element("fred", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("fred"), menu_create_element("plugh", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("plugh"), menu_create_element("xyzzy", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("xyzzy"), menu_create_element("thud", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("thud"), menu_create_element("wibble", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("wibble"), menu_create_element("wobble", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("wobble"), menu_create_element("wubble", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("wubble"), menu_create_element("flob", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("flob"), menu_create_element("blop", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("flob"), menu_create_element("blop1", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("flob"), menu_create_element("blop2", MENU_ELEM_CONTAINER));
    menu_append_child(menu_get_element_by_id("blop1"), menu_create_element("blop3", MENU_ELEM_TEXT));

    menu_get_element_by_id("blop3")->specifics.text.value = "hello, world!";

    struct menu_elem_t *const foo_found = menu_get_element_by_id("foo");

    if (foo_found == NULL) {
        logger_print(LOG_LEVEL_FATAL, "unable to find menu element by id");
        return EXIT_FAILURE;
    }

    menu_print_tree();

    logger_print(LOG_LEVEL_INFO, "starting main loop...");
    start_main_loop((void (*)(void *)) main_loop, game);
}
