#include <assert.h>
#include <inttypes.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "fs.h"
#include "logger.h"
#include "math.h"
#include "menu.h"
#include "ray.h"
#include "util.h"
#include "vector.h"

#include "game.h"


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

/**
 * Calculates the number of rays cast by the camera.
 *
 * @param game A pointer to the game_t struct representing the current game.
 * @return The number of rays cast by the camera.
 */
static inline size_t camera_nrays(const struct game_t *const game) {
    return game->camera->fov * game->camera->resmult;
}

static void render_walls(const struct game_t *const game) {
    for (size_t i = 0; i < game->nobjects; i++) {
        if (game->objects[i]->type != WALL) {
            continue;
        }

        const struct wall_t *const wall = &game->objects[i]->data.wall;

        render_colored(game->renderer, wall->color, {
            SDL_RenderDrawLineF(game->renderer, wall->a.x, wall->a.y, wall->b.x, wall->b.y);
        });
    }
}

static void render_hud(const struct game_t *const restrict game, const SDL_Color color) {
    static const struct vec_t pos = {.x = 10.0F, .y = 10.0F};
    static const char *const fmt =
            "fps: %" PRIu64 " | ticks: %" PRIu64 " | frames: %" PRIu64 " | pos: [%.2f, %.2f] | angle: %.0f | fov: %zu "
            "| resmult: %zu | rays: %zu | px/ray: %.4f | light: %.1f";

    const size_t nrays = game->camera->fov * game->camera->resmult;

    render_colored(game->renderer, color, {
        render_printf(game->renderer, pos, fmt,
                      game->fps,
                      game->ticks,
                      game->frames,
                      game->camera->pos.x,
                      game->camera->pos.y,
                      game->camera->angle,
                      game->camera->fov,
                      game->camera->resmult,
                      nrays,
                      (float) SCREEN_WIDTH / (float) nrays,
                      game->camera->lightmult);
    });
}

static void render_rays(const struct game_t *const restrict game, const SDL_Color color) {
    render_colored(game->renderer, color, {
        for (size_t i = 0; i < camera_nrays(game); i++) {
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
    const size_t nrays = camera_nrays(game);
    const float width = SCREEN_WIDTH / (float) (nrays);

    for (size_t i = 0; i < nrays; i++) {
        const struct ray_t *const ray = &game->camera->rays[i];

        if (ray->intersection.wall == NULL) {
            continue;
        }

        const float height = map(1.0F / ray->intersection.dist, 0.0F, 0.005F, 0.0F, (float) WALL_SIZE);
        const float height_diff = game->camera->movement.crouch ? (float) CAMERA_CROUCH_HEIGHT_DELTA : 0.0F;

        const SDL_FRect stripe = {
                .x = width * (float) i,
                .y = game->center.y - height / 2.0F - height_diff,
                .h = height,
                .w = width
        };

        if (game->render_mode != RENDER_MODE_WIREFRAME) {
            const float brightness = map(1.0F / powf(ray->intersection.dist, 2.0F), 0.0F, 0.00001F, 0.0F, 1.0F);
            const SDL_Color color = change_brightness(ray->intersection.wall->color,
                                                      brightness * game->camera->lightmult);

            render_colored(game->renderer, color, {
                SDL_RenderFillRectF(game->renderer, &stripe);
            });
            continue;
        }

        render_colored(game->renderer, ray->intersection.wall->color, {
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

            const float dist_a2 = vdist2(ray->intersection.pos, ray->intersection.wall->a);
            const float dist_b2 = vdist2(ray->intersection.pos, ray->intersection.wall->b);

            // vertical line; only at the adge of a wall
            // this is a bad approximation, which works poorly in lower resolutions
            // TODO: find a better threshold than stripe.w
            if (fminf(dist_a2, dist_b2) <= stripe.w) {
                SDL_RenderDrawLineF(game->renderer, stripe.x, stripe.y, stripe.x, y);
            }
        });
    }

    filledCircleColor(game->renderer,
                      (int16_t) game->center.x,
                      (int16_t) game->center.y,
                      3,
                      color_to_int(COLOR_WHITE));

    const struct ray_t *const center_ray = &game->camera->rays[camera_nrays(game) / 2];
    const struct wall_t *const center_wall = center_ray->intersection.wall;
    const struct vec_t center_pos = vadd(game->center, (struct vec_t) {10.0F, 10.0F});

    render_colored(game->renderer, COLOR_WHITE, {
        render_printf(game->renderer, center_pos, "%.2f m",
                      center_wall == NULL ? INFINITY : center_ray->intersection.dist / 100.0F);
    });
}

static void render_camera(const struct game_t *const restrict game, const SDL_Color camera, const SDL_Color direction) {
    filledCircleColor(game->renderer,
                      (int16_t) game->camera->pos.x,
                      (int16_t) game->camera->pos.y,
                      5,
                      color_to_int(camera));

    render_colored(game->renderer, direction, {
        SDL_RenderDrawLineF(game->renderer,
                            game->camera->pos.x,
                            game->camera->pos.y,
                            game->camera->pos.x + 100.0F * game->camera->dir.x,
                            game->camera->pos.y + 100.0F * game->camera->dir.y);
    });
}

static void render_visual_fps(struct game_t *const restrict game,
                              const SDL_Color fg,
                              const SDL_Color bg) {
    const float size = (float) game->fps / 5.0F;

    const SDL_FRect rect = {
            .x = (float) SCREEN_WIDTH - 10.0F,
            .w = 10.0F,
            .y = (float) SCREEN_HEIGHT - size,
            .h = size
    };

    render_colored(game->renderer, fg, {
        SDL_RenderFillRectF(game->renderer, &rect);
    });

    render_colored(game->renderer, bg, {
        SDL_RenderDrawRectF(game->renderer, &rect);
    });
}

static void render_floor_and_ceiling(struct game_t *const game) {
    render_colored(game->renderer, game->ceil_color, {
        SDL_RenderClear(game->renderer);
    });

    const float height_diff = game->camera->movement.crouch ? CAMERA_CROUCH_HEIGHT_DELTA : 0.0F;

    const SDL_FRect floor = {
            .x = 0,
            .y = game->center.y - height_diff,
            .w = SCREEN_WIDTH,
            .h = game->center.y + height_diff
    };

    render_colored(game->renderer, game->floor_color, {
        SDL_RenderFillRectF(game->renderer, &floor);
    });
}

static void update_player_position(const struct game_t *const game) {
    struct vec_t dirvect = vmul(game->camera->dir, speed_coeff(game, game->camera->speed));

    if (game->camera->movement.forward ^ game->camera->movement.backward) {
        if (game->camera->movement.forward) { /* forward */
            game->camera->pos = vadd(game->camera->pos, dirvect);
        } else { /* backward */
            game->camera->pos = vsub(game->camera->pos, dirvect);
        }
    }

    if (game->camera->movement.left ^ game->camera->movement.right) {
        dirvect = vrotate(dirvect, radians(90.0F));

        if (game->camera->movement.left) { /* left */
            game->camera->pos = vsub(game->camera->pos, dirvect);
        } else { /* right */
            game->camera->pos = vadd(game->camera->pos, dirvect);
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
    for (size_t i = 0; i < camera_nrays(game); i++) {
        struct ray_t ray;
        struct intersection_t ray_int = {0};
        float min_dist = INFINITY;

        ray.pos = game->camera->pos;
        ray.dir = vfromangle(get_ray_angle(game, i));

        for (size_t j = 0; j < game->nobjects; j++) {
            if (game->objects[j]->type != WALL) {
                continue;
            }

            const struct wall_t *const wall = &game->objects[j]->data.wall;
            struct vec_t intersection;

            if (!ray_intersection(&ray, wall, &intersection)) {
                continue;
            }

            const float dist = vdist(ray.pos, intersection);

            if (dist < min_dist) {
                ray_int.pos = intersection;
                ray_int.wall = wall;
                min_dist = ray_int.dist = dist;
            }
        }

        ray.intersection = ray_int;
        game->camera->rays[i] = ray;
    }
}

void camera_update_angle(struct game_t *const game, float angle) {
    if (angle < 0) {
        angle += 360.0F;
    } else if (angle > 360.0F) {
        angle -= 360.0F;
    }

    game->camera->angle = angle;
    game->camera->dir = vfromangle(radians(game->camera->angle));
}

void tick(struct game_t *const game) {
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

void render(struct game_t *const game) {
    switch (game->render_mode) {
        case RENDER_MODE_FLAT:
            render_colored(game->renderer, COLOR_BLACK, {
                SDL_RenderClear(game->renderer);
            });
            render_walls(game);
            render_rays(game, COLOR_WHITE);
            render_camera(game, COLOR_RED, COLOR_GREEN);
            break;

        case RENDER_MODE_WIREFRAME:
            render_colored(game->renderer, COLOR_BLACK, {
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

    if (game->paused) {
        menu_render(game->renderer, &game->menu);
    }

    SDL_RenderPresent(game->renderer);
}

void update(const struct game_t *const game) {
    update_player_position(game);
    update_ray_intersections(game);
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
    game.camera->speed = CAMERA_MOVEMENT_SPEED;
    game.camera->rays = rays;
    game.camera->pos = game.center;
    game.camera->lightmult = CAMERA_LIGHTMULT;

    game.render_mode = RENDER_MODE_TEXTURED;
    game.ceil_color = (SDL_Color) CEIL_COLOR;
    game.floor_color = (SDL_Color) FLOOR_COLOR;
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
        logger_print(LOG_LEVEL_ERROR, "unable to create SDL window");
        return -1;
    }

    logger_printf(LOG_LEVEL_INFO, "created SDL window '%s' (%dx%d px)\n", SCREEN_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);

    if (game->renderer == NULL) {
        logger_print(LOG_LEVEL_ERROR, "unable to create SDL renderer");
        return -1;
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);
    return 0;
}

void game_destroy(struct game_t *const game) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
}
