#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>


#include "math.h"
#include "vector.h"
#include "conf.h"
#include "world.h"
#include "ray.h"
#include "font.h"


#define sdl_error(msg) \
    fprintf(stderr, msg ": %s\n", SDL_GetError())

#define unused __attribute((unused))


#define render_printf(game, x, y, fmt, ...)                     \
    do {                                                        \
        memset(game->textbuf, 0, TEXTBUFLEN);                   \
        snprintf(game->textbuf, TEXTBUFLEN, fmt, __VA_ARGS__);  \
        render_string(game, x, y, game->textbuf);               \
    } while (0)


typedef struct {
    Vector pos;
    float angle;
    Ray *rays;
    size_t resmult;
    size_t fov;
    size_t nrays;
    struct {
        bool up, down, left, right;
    } movement;
} Camera;

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Camera *camera;
    const Line *walls;
    size_t nwalls;
    uint64_t fps;
    uint64_t frames;
    uint64_t ticks;
    char *textbuf;
    IVector mouse;
    Vector center;
    IVector icenter;
} Game;


void render_char(const Game *const game, const int x, const int y, const int chr) {
    if (chr < 0 || chr >= FONT_BASIC) {
        return;
    }

    for (int i = 0; i < FONT_BYTES; i++) {
        for (int j = 0; j < FONT_BYTES; j++) {
            if (font_basic[chr][i] & 1 << j) {
                SDL_RenderDrawPoint(game->renderer, x + j, y + i);
            }
        }
    }
}

void render_string(const Game *const game, const int x, const int y, const char *const str) {
    for (size_t i = 0; i < strlen(str); i++) {
        render_char(game, x + 10 * (int) i, y, str[i]);
    }
}

void render_walls(const Game *const game) {
    for (size_t i = 0; i < game->nwalls; i++) {
        const Line *const wall = game->walls + i;
        SDL_RenderDrawLineF(game->renderer, wall->a.x, wall->a.y, wall->b.x, wall->b.y);
    }
}

void render_hud(const Game *const game) {
    render_printf(game, 10, 10,
                  "fps: %lu | ticks: %lu | frames: %lu | pos: [%.2f, %.2f] | angle: %.0f | fov: %zu | resmult: %zu | rays: %zu",
                  game->fps, game->ticks, game->frames, game->camera->pos.x, game->camera->pos.y, game->camera->angle,
                  game->camera->fov, game->camera->resmult, game->camera->nrays);
}

void render_rays(const Game *const game) {
    for (size_t i = 0; i < game->camera->nrays; i++) {
        const Ray *const ray = game->camera->rays + i;

        if (ray->has_intersection) {
            SDL_RenderDrawLineF(game->renderer, ray->pos.x, ray->pos.y, ray->intersection.x, ray->intersection.y);
        }
    }
}

void render_3d(const Game *const game) {
    const float width = SCREEN_WIDTH / (float) (game->camera->nrays);

    for (size_t i = 0; i < game->camera->nrays; i++) {
        const Ray *const ray = game->camera->rays + i;

        if (!ray->has_intersection) {
            continue;
        }

        const float height = map(1.0f / ray->intersection_dist, 0.0f, 1.0f, 0.0f, (float) WALL_SIZE);

        const SDL_FRect stripe = {
                .x = width * (float) i,
                .y = game->center.y - height / 2.0f,
                .h = height,
                .w = width
        };

        SDL_RenderFillRectF(game->renderer, &stripe);
    }
}

void render_camera(const Game *const game) {
    filledCircleColor(game->renderer,
                      (int16_t) game->camera->pos.x,
                      (int16_t) game->camera->pos.y,
                      5,
                      0xFF0000FF);
    SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);
    /*
    SDL_RenderDrawLineF(game->renderer, 
            game->camera->pos.x, 
            game->camera->pos.y, 
            vector_add((vector_copy((Vector[1]) {0}, &game->camera->pos), game->camera->angle); 
            game->camera->pos.y + 50);
    */
}

void render(Game *const game) {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);

    SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);
    render_walls(game);

    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    render_rays(game);

    render_camera(game);

    SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255);
    render_3d(game);

    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    render_hud(game);

    SDL_RenderPresent(game->renderer);
}

void update(Game *const game) {
    static uint64_t newframes;
    const uint64_t ticks = SDL_GetTicks64();

    newframes++;

    if (ticks - game->ticks > POLLINTERVAL) {
        game->fps = (uint64_t) ((float) newframes / (float) (ticks - game->ticks) * 1000.0);
        game->ticks = ticks;
        game->frames += newframes;
        newframes = 0;
    }

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    game->mouse.x = (long) mouseX;
    game->mouse.y = (long) mouseY;

    Vector dirvect;
    vector_from_angle(&dirvect, game->camera->angle);
    vector_mul(&dirvect, 1);

    if (game->camera->movement.up) {
        vector_add(&game->camera->pos, &dirvect);
    }


    for (size_t i = 0; i < game->camera->nrays; i++) {
        Ray ray;
        ray.pos = (Vector) {.x = (float) game->camera->pos.x, .y = (float) game->camera->pos.y};
        ray.dir = *vector_from_angle((Vector[1]) {0}, radians((float) i / (float) game->camera->resmult));
        ray.has_intersection = false;

        float min_dist = INFINITY;

        for (size_t j = 0; j < game->nwalls; j++) {
            const Line *const wall = game->walls + j;
            const Vector *const intersection = ray_intersection(&ray, wall, (Vector[1]) {0});

            if (intersection == NULL) {
                continue;
            }

            const float dist = vector_distance(&ray.pos, intersection);

            if (dist < min_dist) {
                ray.intersection = *intersection;
                ray.has_intersection = true;
                min_dist = ray.intersection_dist = dist;
            }
        }

        game->camera->rays[i] = ray;
    }
}

bool on_event(Game *const game, const SDL_Event *const event) {
    switch (event->type) {
        case SDL_QUIT:
            return false;

        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
                case SDLK_UP:
                    game->camera->movement.up = true;
                    break;
                case SDLK_DOWN:
                    game->camera->movement.down = true;
                    break;
                case SDLK_LEFT:
                    game->camera->movement.left = true;
                    break;
                case SDLK_RIGHT:
                    game->camera->movement.right = true;
                    break;
                case SDLK_KP_PLUS:
                    game->camera->fov = (size_t) constrain((float) game->camera->fov + 1, FOV_MIN, FOV_MAX);
                    game->camera->nrays = game->camera->fov * game->camera->resmult;
                    break;
                case SDLK_KP_MINUS:
                    game->camera->fov = (size_t) constrain((float) game->camera->fov - 1, FOV_MIN, FOV_MAX);
                    game->camera->nrays = game->camera->fov * game->camera->resmult;
                    break;
                case SDLK_INSERT:
                    game->camera->resmult = (size_t) constrain((float) game->camera->resmult + 1, RESMULT_MIN,
                                                               RESMULT_MAX);
                    game->camera->nrays = game->camera->fov * game->camera->resmult;
                    break;
                case SDLK_DELETE:
                    game->camera->resmult = (size_t) constrain((float) game->camera->resmult - 1, RESMULT_MIN,
                                                               RESMULT_MAX);
                    game->camera->nrays = game->camera->fov * game->camera->resmult;
                    break;
                case SDLK_r:
                    game->camera->pos = game->center;
                    game->camera->angle = 0;
                    break;
                case SDLK_ESCAPE:
                case SDLK_q:
                    return false;

                default:
                    break;
            }
            break;

        case SDL_KEYUP:
            switch (event->key.keysym.sym) {
                case SDLK_UP:
                    game->camera->movement.up = false;
                    break;
                case SDLK_DOWN:
                    game->camera->movement.down = false;
                    break;
                case SDLK_LEFT:
                    game->camera->movement.left = false;
                    break;
                case SDLK_RIGHT:
                    game->camera->movement.right = false;
                    break;
                default:
                    break;
            }
            break;

        case SDL_MOUSEMOTION:
            game->camera->angle += (float) event->motion.xrel;
            break;

        default:
            break;
    }

    return true;
}

int init(Game *const game) {
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

    return 0;
}

void cleanup(const Game *const game) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
}

int main(unused int argc, unused char **argv) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        sdl_error("SDL_Init");
        return EXIT_FAILURE;
    }

    Game game;

    game.center = (Vector) {(float) SCREEN_WIDTH / 2.0f, (float) SCREEN_HEIGHT / 2.0f};
    game.icenter = (IVector) {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};


    game.walls = world_walls;
    game.nwalls = WORLD_NWALLS;

    Camera camera;
    game.camera = &camera;

    game.camera->fov = CAMERA_FOV;
    game.camera->resmult = CAMERA_RESMULT;
    game.camera->nrays = game.camera->fov * game.camera->resmult;
    game.camera->pos = game.center;
    game.camera->angle = 0;
    game.camera->movement.up = false;
    game.camera->movement.down = false;
    game.camera->movement.left = false;
    game.camera->movement.right = false;

    Ray rays[FOV_MAX * RESMULT_MAX];
    game.camera->rays = rays;

    game.textbuf = (char[TEXTBUFLEN]) {0};


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

