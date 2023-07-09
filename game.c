#include <SDL2/SDL2_gfxPrimitives.h>


#include "math.h"
#include "vector.h"
#include "ray.h"
#include "util.h"


#include "game.h"


unused private void render_texture(struct game_t *const restrict game,
                                   const uint8_t texno,
                                   const SDL_Rect *restrict src,
                                   const SDL_FRect *const restrict dst) {
    if (src->x < 0 || src->x > TEXTURE_ATLAS_WIDTH
        || src->y < 0 || src->y > TEXTURE_ATLAS_WIDTH
        || src->w < 0 || src->w > TEXTURE_ATLAS_WIDTH
        || src->h < 0 || src->h > TEXTURE_ATLAS_WIDTH) {
        return;
    }

    static const int rowsize = TEXTURE_ATLAS_WIDTH * TEXTURE_SIZE;

    src = &(SDL_Rect) {
            .x = texno % rowsize + src->x,
            .y = texno / rowsize + src->y,
            .w = src->w,
            .h = src->h
    };

    SDL_RenderCopyF(game->renderer, game->texture, src, dst);
}


/**
 * Calculates the speed coefficient for the game based on the game's FPS and a given coefficient.
 *
 * @param game A pointer to the game_t struct representing the current game.
 * @param coeff The coefficient to use in the speed calculation.
 * @return The speed coefficient for the game.
 */
private float speed_coeff(const struct game_t *const game, const float coeff) {
    return coeff / (float) game->fps;
}

private void set_color(const struct game_t *const game, const SDL_Color color) {
    SDL_SetRenderDrawColor(game->renderer, color.r, color.g, color.b, color.a);
}

private void render_walls(const struct game_t *const game) {
    for (size_t i = 0; i < game->nobjects; i++) {
        if (game->objects[i].type != WALL) {
            continue;
        }

        const struct wall_t *const wall = &game->objects[i].data.wall;
        SDL_SetRenderDrawColor(game->renderer, wall->color.r, wall->color.g, wall->color.b, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLineF(game->renderer, wall->a.x, wall->a.y, wall->b.x, wall->b.y);
    }
}

private void render_hud(const struct game_t *const game) {
    render_printf(game, 10, 10,
                  "fps: %lu | ticks: %lu | frames: %lu | pos: [%.2f, %.2f] | angle: %.0f | fov: %zu | resmult: %zu | rays: %zu | px/ray: %.4f | threads: %zu",
                  game->fps, game->ticks, game->frames, game->camera->pos.x, game->camera->pos.y, game->camera->angle,
                  game->camera->fov, game->camera->resmult, game->camera->nrays,
                  (float) SCREEN_WIDTH / (float) game->camera->nrays, game->nthreads);
}

private void render_rays(const struct game_t *const game) {
    for (size_t i = 0; i < game->camera->nrays; i++) {
        const struct ray_t *const ray = game->camera->rays + i;
        const struct intersection_t *const intersection = &ray->intersection;

        if (intersection->wall != NULL) {
            SDL_RenderDrawLineF(game->renderer, ray->pos.x, ray->pos.y,
                                ray->intersection.pos.x, ray->intersection.pos.y);
        }
    }
}

private void render_3d(struct game_t *const game) {
    const float width = SCREEN_WIDTH / (float) (game->camera->nrays);

    for (size_t i = 0; i < game->camera->nrays; i++) {
        const struct ray_t *const ray = &game->camera->rays[i];

        if (ray->intersection.wall == NULL) {
            continue;
        }

        const float height = map(1.0f / ray->intersection.dist, 0.0f, 0.005f, 0.0f, (float) WALL_SIZE);

        const SDL_FRect stripe = {
                .x = width * (float) i,
                .y = game->center.y - height / 2.0f
                     - (game->camera->movement.crouch ? CAMERA_CROUCH_HEIGHT_DELTA : 0.0f),
                .h = height,
                .w = width
        };

        SDL_Color color;
        memcpy(&color, &ray->intersection.wall->color, sizeof color);

        if (game->render_mode == RENDER_MODE_NORMAL) {
            change_brightness(&color, map(1.0f / powf(ray->intersection.dist, 2.0f), 0.0f, 0.00001f, 0.0f, 1.0f));
        }

        SDL_SetRenderDrawColor(game->renderer, color.r, color.g, color.b, 255);

        if (game->render_mode == RENDER_MODE_NORMAL) {
            SDL_RenderFillRectF(game->renderer, &stripe);
        } else {
            const float x = stripe.x + stripe.w, y = stripe.y + stripe.h;

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

            const float dist_a2 = vector_distance2(&ray->intersection.pos, &ray->intersection.wall->a);
            const float dist_b2 = vector_distance2(&ray->intersection.pos, &ray->intersection.wall->b);
            const float min_dist2 = min(dist_a2, dist_b2);

            // vertical line; only at the adge of a wall
            // this is a bad approximation, which works poorly in lower resolutions
            // TODO: find a better threshold than strip.w
            if (min_dist2 <= stripe.w) {
                SDL_RenderDrawLineF(game->renderer, stripe.x, stripe.y, stripe.x, y);
            }
        }
    }
}

private void render_camera(const struct game_t *const game) {
    filledCircleColor(game->renderer,
                      (int16_t) game->camera->pos.x,
                      (int16_t) game->camera->pos.y,
                      5,
                      0xFF0000FF);

    set_color(game, COLOR_GREEN);

    const struct vector_t *const endpoint = vector_add(
            vector_copy(vector(), &game->camera->pos),
            vector_mul(vector_copy(vector(), &game->camera->dir), 100)
    );

    SDL_RenderDrawLineF(game->renderer, game->camera->pos.x, game->camera->pos.y, endpoint->x, endpoint->y);
}

private void render_visual_fps(struct game_t *const game) {
    const float size = (float) game->fps / 5.0f;

    const SDL_FRect rect = {
            .x = (float) SCREEN_WIDTH - 10.0f,
            .w = 10.0f,
            .y = (float) SCREEN_HEIGHT - size,
            .h = size
    };

    set_color(game, COLOR_WHITE);
    SDL_RenderFillRectF(game->renderer, &rect);

    set_color(game, COLOR_BLACK);
    SDL_RenderDrawRectF(game->renderer, &rect);
}

void camera_update_angle(struct game_t *const game, const float angle) {
    game->camera->angle = fmodf(angle, 360.0f);
    vector_from_angle(&game->camera->dir, radians(game->camera->angle));
}

void render(struct game_t *const game) {
    switch (game->render_mode) {
        case RENDER_MODE_WIREFRAME:
            set_color(game, game->floor_color);
            SDL_RenderClear(game->renderer);
            render_3d(game);
            break;

        case RENDER_MODE_NORMAL:
            set_color(game, game->ceil_color);
            SDL_RenderClear(game->renderer);

            set_color(game, game->floor_color);

            const SDL_FRect floor = {
                    .x = 0,
                    .y = game->center.y - (game->camera->movement.crouch ? CAMERA_CROUCH_HEIGHT_DELTA : 0.0f),
                    .w = SCREEN_WIDTH,
                    .h = game->center.y + (game->camera->movement.crouch ? CAMERA_CROUCH_HEIGHT_DELTA : 0.0f)
            };

            SDL_RenderFillRectF(game->renderer, &floor);

            render_3d(game);
            break;

        case RENDER_MODE_FLAT:
            set_color(game, COLOR_BLACK);
            SDL_RenderClear(game->renderer);
            set_color(game, COLOR_GREEN);
            render_walls(game);
            set_color(game, COLOR_WHITE);
            render_rays(game);
            render_camera(game);
            break;

    }

    render_visual_fps(game);

    set_color(game, COLOR_WHITE);
    render_hud(game);

    SDL_RenderPresent(game->renderer);
}

void update(struct game_t *const game) {
    const uint64_t ticks = SDL_GetTicks64();

    game->newframes++;

    if (ticks - game->ticks > POLLINTERVAL) {
        game->fps = (uint64_t) ((float) game->newframes / (float) (ticks - game->ticks) * 1000.0);
        game->ticks = ticks;
        game->frames += game->newframes;
        game->newframes = 0;
    }

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    game->mouse.x = (long) mouseX;
    game->mouse.y = (long) mouseY;

    struct vector_t *const dirvect = vector_mul(vector_copy(vector(), &game->camera->dir),
                                                speed_coeff(game, game->camera->speed));

    if (game->camera->movement.forward ^ game->camera->movement.backward) {
        if (game->camera->movement.forward) { /* forward */
            vector_add(&game->camera->pos, dirvect);
        } else { /* backward */
            vector_sub(&game->camera->pos, dirvect);
        }
    }

    if (game->camera->movement.left ^ game->camera->movement.right) {
        vector_rotate(dirvect, radians(90));

        if (game->camera->movement.left) { /* left */
            vector_sub(&game->camera->pos, dirvect);
        } else { /* right */
            vector_add(&game->camera->pos, dirvect);
        }
    }

#pragma omp parallel for default(none) shared(game) num_threads(game->nthreads)
    for (size_t i = 0; i < game->camera->nrays; i++) {
        struct ray_t ray;
        struct intersection_t ray_int = {.wall = NULL};

        ray.pos = (struct vector_t) {.x = (float) game->camera->pos.x, .y = (float) game->camera->pos.y};
        ray.dir = *vector_from_angle(vector(), radians(
                (float) i / (float) game->camera->resmult + game->camera->angle - (float) game->camera->fov / 2.0f)
        );

        float min_dist = INFINITY;

        for (size_t j = 0; j < game->nobjects; j++) {
            if (game->objects[j].type != WALL) {
                continue;
            }

            const struct wall_t *const wall = &game->objects[j].data.wall;
            const struct vector_t *const intersection = ray_intersection(&ray, wall, vector());

            if (intersection == NULL) {
                continue;
            }

            const float dist = vector_distance(&ray.pos, intersection);

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
