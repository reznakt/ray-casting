#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>


#include "logger.h"
#include "math.h"
#include "ray.h"
#include "util.h"
#include "vector.h"


#include "game.h"


/**
 * @brief Render objects with a certain color. The color is restored after rendering.
 * @param game A pointer to the game_t struct representing the game.
 * @param color The color to use for rendering.
 * @param ... The code to execute for rendering.
 * @example render_colored(game, COLOR_RED, { SDL_RenderDrawLineF(...); });
 */
#define render_colored(game, color, ...)                                                                    \
    do {                                                                                                    \
        SDL_Color _old_color;                                                                               \
        SDL_GetRenderDrawColor(game->renderer, &_old_color.r, &_old_color.g, &_old_color.b, &_old_color.a); \
        SDL_SetRenderDrawColor(game->renderer, color.r, color.g, color.b, color.a);                         \
        __VA_ARGS__                                                                                         \
        SDL_SetRenderDrawColor(game->renderer, _old_color.r, _old_color.g, _old_color.b, _old_color.a);     \
    } while (0)


/**
 * @brief Renders a texture from the texture atlas to the screen.
 *
 * @param game A pointer to the game_t struct representing the game.
 * @param texno The number of the texture to render (0-indexed, left-to-right, top-to-bottom).
 * @param src The source rectangle of the texture to render.
 * @param dst The destination rectangle at which to render the texture.
 */
unused static void render_texture(struct game_t *const restrict game,
                                  const uint8_t texno,
                                  const SDL_Rect *const restrict src,
                                  const SDL_FRect *const restrict dst) {
    static const int rowsize = TEXTURE_ATLAS_WIDTH * TEXTURE_SIZE;

    const SDL_Rect adjusted_src = {
            .x = texno % rowsize + src->x,
            .y = texno / rowsize + src->y,
            .w = src->w,
            .h = src->h
    };

    SDL_RenderCopyF(game->renderer, game->texture, &adjusted_src, dst);
}


/**
 * Calculates the speed coefficient for the game based on the game's FPS and a given coefficient.
 *
 * @param game A pointer to the game_t struct representing the current game.
 * @param coeff The coefficient to use in the speed calculation.
 * @return The speed coefficient for the game.
 */
static inline float speed_coeff(const struct game_t *const game, const float coeff) {
    return coeff / (float) game->fps;
}

static void render_walls(const struct game_t *const game) {
    for (size_t i = 0; i < game->nobjects; i++) {
        if (game->objects[i]->type != WALL) {
            continue;
        }

        const struct wall_t *const wall = &game->objects[i]->data.wall;

        render_colored(game, wall->color, {
            SDL_RenderDrawLineF(game->renderer, wall->a.x, wall->a.y, wall->b.x, wall->b.y);
        });
    }
}

static void render_hud(const struct game_t *const game, const SDL_Color color) {
    static const struct vec_t pos = {.x = 10.0F, .y = 10.0F};
    static const char *const fmt = "fps: %lu | ticks: %lu | frames: %lu | pos: [%.2f, %.2f] | angle: %.0f | fov: %zu "
                                   "| resmult: %zu | rays: %zu | px/ray: %.4f | threads: %zu | light: %.1f";

    render_colored(game, color, {
        render_printf(game->renderer, &pos, fmt,
                      game->fps,
                      game->ticks,
                      game->frames,
                      game->camera->pos.x,
                      game->camera->pos.y,
                      game->camera->angle,
                      game->camera->fov,
                      game->camera->resmult,
                      game->camera->nrays,
                      (float) SCREEN_WIDTH / (float) game->camera->nrays,
                      game->nthreads,
                      game->camera->lightmult);
    });
}

static void render_rays(const struct game_t *const game, const SDL_Color color) {
    render_colored(game, color, {
        for (size_t i = 0; i < game->camera->nrays; i++) {
            const struct ray_t *const ray = &game->camera->rays[i];
            const struct intersection_t *const intersection = &ray->intersection;

            if (intersection->wall != NULL) {
                SDL_RenderDrawLineF(game->renderer, ray->pos.x, ray->pos.y,
                                    ray->intersection.pos.x, ray->intersection.pos.y);
            }
        }
    });
}

static void render_3d(struct game_t *const game) {
    const float width = SCREEN_WIDTH / (float) (game->camera->nrays);

    for (size_t i = 0; i < game->camera->nrays; i++) {
        const struct ray_t *const ray = &game->camera->rays[i];

        if (ray->intersection.wall == NULL) {
            continue;
        }

        const float height = map(1.0F / ray->intersection.dist, 0.0F, 0.005F, 0.0F, (float) WALL_SIZE);

        const SDL_FRect stripe = {
                .x = width * (float) i,
                .y = game->center.y - height / 2.0F
                     - (game->camera->movement.crouch ? CAMERA_CROUCH_HEIGHT_DELTA : 0.0F),
                .h = height,
                .w = width
        };

        SDL_Color color;
        memcpy(&color, &ray->intersection.wall->color, sizeof color);

        if (game->render_mode != RENDER_MODE_WIREFRAME) {
            const float brightness = map(1.0F / powf(ray->intersection.dist, 2.0F), 0.0F, 0.00001F, 0.0F, 1.0F);
            change_brightness(&color, brightness * game->camera->lightmult);
        }

        if (game->render_mode != RENDER_MODE_WIREFRAME) {
            render_colored(game, color, {
                SDL_RenderFillRectF(game->renderer, &stripe);
            });
            continue;
        }

        render_colored(game, color, {
            const float x = stripe.x + stripe.w;
            const float y = stripe.y + stripe.h;

            // top horizontal line
            SDL_RenderDrawLineF(game->renderer,
                                stripe.x,
                                stripe.y,
                                x,
                                stripe.y);

            // bottom horizontal line
            SDL_RenderDrawLineF(game->renderer,
                                stripe.x,
                                y,
                                x,
                                y);

            const float dist_a2 = vdist2(&ray->intersection.pos, &ray->intersection.wall->a);
            const float dist_b2 = vdist2(&ray->intersection.pos, &ray->intersection.wall->b);

            // vertical line; only at the adge of a wall
            // this is a bad approximation, which works poorly in lower resolutions
            // TODO: find a better threshold than stripe.w
            if (fminf(dist_a2, dist_b2) <= stripe.w) {
                SDL_RenderDrawLineF(game->renderer, stripe.x, stripe.y, stripe.x, y);
            }
        });
    }
}

static void render_camera(const struct game_t *const game, const SDL_Color camera, const SDL_Color direction) {
    filledCircleColor(game->renderer,
                      (int16_t) game->camera->pos.x,
                      (int16_t) game->camera->pos.y,
                      5,
                      color_to_int(&camera));

    render_colored(game, direction, {
        SDL_RenderDrawLineF(game->renderer,
                            game->camera->pos.x,
                            game->camera->pos.y,
                            game->camera->pos.x + 100.0F * game->camera->dir.x,
                            game->camera->pos.y + 100.0F * game->camera->dir.y);
    });
}

static void render_visual_fps(struct game_t *const game, const SDL_Color fg, const SDL_Color bg) {
    const float size = (float) game->fps / 5.0F;

    const SDL_FRect rect = {
            .x = (float) SCREEN_WIDTH - 10.0F,
            .w = 10.0F,
            .y = (float) SCREEN_HEIGHT - size,
            .h = size
    };

    render_colored(game, fg, {
        SDL_RenderFillRectF(game->renderer, &rect);
    });

    render_colored(game, bg, {
        SDL_RenderDrawRectF(game->renderer, &rect);
    });
}

static void render_floor_and_ceiling(struct game_t *const game) {
    render_colored(game, game->ceil_color, {
        SDL_RenderClear(game->renderer);
    });

    const float height_diff = game->camera->movement.crouch ? CAMERA_CROUCH_HEIGHT_DELTA : 0.0F;

    const SDL_FRect floor = {
            .x = 0,
            .y = game->center.y - height_diff,
            .w = SCREEN_WIDTH,
            .h = game->center.y + height_diff
    };

    render_colored(game, game->floor_color, {
        SDL_RenderFillRectF(game->renderer, &floor);
    });
}

static void update_player_position(const struct game_t *const game) {
    struct vec_t *const dirvect = vmul(vcopy(&game->camera->dir), speed_coeff(game, game->camera->speed));

    if (game->camera->movement.forward ^ game->camera->movement.backward) {
        if (game->camera->movement.forward) { /* forward */
            vadd(&game->camera->pos, dirvect);
        } else { /* backward */
            vsub(&game->camera->pos, dirvect);
        }
    }

    if (game->camera->movement.left ^ game->camera->movement.right) {
        vrotate(dirvect, radians(90));

        if (game->camera->movement.left) { /* left */
            vsub(&game->camera->pos, dirvect);
        } else { /* right */
            vadd(&game->camera->pos, dirvect);
        }
    }
}

static float get_ray_angle(const struct game_t *const game, const size_t rayno) {
    const float resmult = (float) game->camera->resmult;
    const float fov = (float) game->camera->fov;
    const float angle = (float) game->camera->angle;

    return radians((float) rayno / resmult - fov / 2.0F + angle);
}

static void update_ray_intersections(const struct game_t *const game) {
#pragma omp parallel for default(none) shared(game) num_threads(game->nthreads)
    for (size_t i = 0; i < game->camera->nrays; i++) {
        struct ray_t ray;
        struct intersection_t ray_int = {0};
        float min_dist = INFINITY;

        vmove(&ray.pos, &game->camera->pos);
        vfromangle(&ray.dir, get_ray_angle(game, i));

        for (size_t j = 0; j < game->nobjects; j++) {
            if (game->objects[j]->type != WALL) {
                continue;
            }

            const struct wall_t *const wall = &game->objects[j]->data.wall;
            const struct vec_t *const intersection = ray_intersection(&ray, wall, vector());

            if (intersection == NULL) {
                continue;
            }

            const float dist = vdist(&ray.pos, intersection);

            if (dist < min_dist) {
                ray_int.pos = *intersection;
                ray_int.wall = wall;
                min_dist = ray_int.dist = dist;
            }
        }

        ray.intersection = ray_int;
        game->camera->rays[i] = ray;
    }
}

static void tick(struct game_t *const game) {
    const uint64_t ticks = SDL_GetTicks64();
    const uint64_t tick_delta = ticks - game->ticks;

    if (tick_delta > POLLINTERVAL) {
        game->fps = 1000 * game->newframes / tick_delta;
        game->ticks = ticks;
        game->frames += game->newframes;
        game->newframes = 0;
    }

    game->newframes++;
}

void camera_update_angle(struct game_t *const game, const float angle) {
    game->camera->angle = fmodf(angle, 360.0F);
    vfromangle(&game->camera->dir, radians(game->camera->angle));
}

void render(struct game_t *const game) {
    switch (game->render_mode) {
        case RENDER_MODE_FLAT:
            render_colored(game, COLOR_BLACK, {
                SDL_RenderClear(game->renderer);
            });
            render_walls(game);
            render_rays(game, COLOR_WHITE);
            render_camera(game, COLOR_RED, COLOR_GREEN);
            break;

        case RENDER_MODE_WIREFRAME:
            render_colored(game, COLOR_BLACK, {
                SDL_RenderClear(game->renderer);
            });
            render_3d(game);
            break;

        case RENDER_MODE_UNTEXTURED:
        case RENDER_MODE_TEXTURED:
            render_floor_and_ceiling(game);
            render_3d(game);
            break;
    }

    render_visual_fps(game, COLOR_WHITE, COLOR_BLACK);
    render_hud(game, COLOR_WHITE);

    SDL_RenderPresent(game->renderer);
}

void update(struct game_t *const game) {
    update_player_position(game);
    update_ray_intersections(game);
    tick(game);
}

struct game_t *game_create(void) {
    static struct game_t game = {0};
    static struct ray_t rays[FOV_MAX * RESMULT_MAX] = {0};
    static struct camera_t camera = {0};
    static struct wobject_t *objects[WORLD_NOBJECTS_MAX] = {0};
    static struct wobject_t objects_data[WORLD_NOBJECTS_MAX] = {0};

    for (size_t i = 0; i < WORLD_NOBJECTS_MAX; i++) {
        objects[i] = &objects_data[i];
    }

    game.center = (struct vec_t) {(float) SCREEN_WIDTH / 2.0F, (float) SCREEN_HEIGHT / 2.0F};

    game.camera = &camera;
    game.camera->fov = CAMERA_FOV;
    game.camera->resmult = CAMERA_RESMULT;
    game.camera->nrays = game.camera->fov * game.camera->resmult;
    game.camera->speed = CAMERA_MOVEMENT_SPEED;
    game.camera->rays = rays;
    vmove(&game.camera->pos, &game.center);
    game.camera->lightmult = CAMERA_LIGHTMULT;

    game.render_mode = RENDER_MODE_TEXTURED;
    game.ceil_color = CEIL_COLOR;
    game.floor_color = FLOOR_COLOR;
    game.nthreads = THREADS;
    game.objects = objects;

    assert(load_world(WORLD_SPEC_FILE, game.objects, &game.nobjects) == 0);
    camera_update_angle(&game, CAMERA_HEADING);

    return &game;
}

int game_init(struct game_t *const game) {
    game->window = SDL_CreateWindow(SCREEN_TITLE,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SCREEN_WIDTH,
                                    SCREEN_HEIGHT,
                                    SCREEN_FLAGS);

    if (game->window == NULL) {
        return -1;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);

    if (game->renderer == NULL) {
        return -1;
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);

    SDL_Surface *const surface = IMG_Load(TEXTURE_ATLAS_FILE);

    if (surface == NULL) {
        logger_printf(LOG_LEVEL_FATAL, "unable to load texture atlas from '%s'\n", TEXTURE_ATLAS_FILE);
        return -1;
    }

    game->texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    if (game->texture == NULL) {
        logger_print(LOG_LEVEL_FATAL, "unable to create texture from surface");
        return -1;
    }

    logger_printf(LOG_LEVEL_INFO, "loaded texture atlas from '%s' (%dx%d)\n",
                  TEXTURE_ATLAS_FILE, surface->w, surface->h);

    return 0;
}

void game_destroy(struct game_t *const game) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
}
