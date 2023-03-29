#include <SDL2/SDL2_gfxPrimitives.h>


#include "math.h"
#include "vector.h"
#include "ray.h"
#include "util.h"


#include "game.h"


/**
 * Calculates the speed coefficient for the game based on the game's FPS and a given coefficient.
 *
 * @param game A pointer to the game_t struct representing the current game.
 * @param coeff The coefficient to use in the speed calculation.
 * @return The speed coefficient for the game.
 */
PRIVATE float speed_coeff(const struct game_t *const game, const float coeff) {
    return coeff / (float) game->fps;
}

PRIVATE void set_color(const struct game_t *const game, const SDL_Color color) {
    SDL_SetRenderDrawColor(game->renderer, color.r, color.g, color.b, color.a);
}

PRIVATE void render_walls(const struct game_t *const game) {
    for (size_t i = 0; i < game->nwalls; i++) {
        const struct wall_t *const wall = game->walls + i;
        SDL_RenderDrawLineF(game->renderer, wall->a.x, wall->a.y, wall->b.x, wall->b.y);
    }
}

PRIVATE void render_hud(const struct game_t *const game) {
    render_printf(game, 10, 10,
                  "fps: %lu | ticks: %lu | frames: %lu | pos: [%.2f, %.2f] | angle: %.0f | fov: %zu | resmult: %zu | rays: %zu | ppr: %.4f",
                  game->fps, game->ticks, game->frames, game->camera->pos.x, game->camera->pos.y, game->camera->angle,
                  game->camera->fov, game->camera->resmult, game->camera->nrays,
                  (float) SCREEN_WIDTH / (float) game->camera->nrays);
}

PRIVATE void render_rays(const struct game_t *const game) {
    for (size_t i = 0; i < game->camera->nrays; i++) {
        const struct ray_t *const ray = game->camera->rays + i;
        const struct intersection_t *const intersection = &ray->intersection;

        if (intersection->wall != NULL) {
            SDL_RenderDrawLineF(game->renderer, ray->pos.x, ray->pos.y,
                                ray->intersection.pos.x, ray->intersection.pos.y);
        }
    }
}

PRIVATE void render_3d(const struct game_t *const game) {
    const float width = SCREEN_WIDTH / (float) (game->camera->nrays);

    for (size_t i = 0; i < game->camera->nrays; i++) {
        const struct ray_t *const ray = game->camera->rays + i;

        if (ray->intersection.wall == NULL) {
            continue;
        }

        const float height = map(1.0f / ray->intersection.dist, 0.0f, 0.005f, 0.0f, (float) WALL_SIZE);

        const SDL_FRect stripe = {
                .x = width * (float) i,
                .y = game->center.y - height / 2.0f,
                .h = height,
                .w = width
        };

        SDL_Color color;
        memcpy(&color, &ray->intersection.wall->color, sizeof color);
        change_brightness(&color, map(1.0f / powf(ray->intersection.dist, 2.0f), 0.0f, 0.00001f, 0.0f, 1.0f));

        SDL_SetRenderDrawColor(game->renderer, color.r, color.g, color.b, 255);
        SDL_RenderFillRectF(game->renderer, &stripe);
    }
}

PRIVATE void render_camera(const struct game_t *const game) {
    filledCircleColor(game->renderer,
                      (int16_t) game->camera->pos.x,
                      (int16_t) game->camera->pos.y,
                      5,
                      0xFF0000FF);

    set_color(game, GREEN);

    const struct vector_t *const endpoint = vector_add(
            vector_copy(vector(), &game->camera->pos),
            vector_mul(vector_copy(vector(), &game->camera->dir), 100)
    );

    SDL_RenderDrawLineF(game->renderer, game->camera->pos.x, game->camera->pos.y, endpoint->x, endpoint->y);
}


void camera_update_angle(struct game_t *const game, const float angle) {
    game->camera->angle = fmodf(angle, 360.0f);
    vector_from_angle(&game->camera->dir, radians(game->camera->angle));
}

void render(struct game_t *const game) {
    switch (game->render_mode) {
        case RENDER_MODE_NORMAL: {
            set_color(game, game->ceil_color);
            SDL_RenderClear(game->renderer);

            set_color(game, game->floor_color);

            SDL_FRect floor = {
                    .x = 0,
                    .y = game->center.y,
                    .w = SCREEN_WIDTH,
                    .h = game->center.y
            };

            SDL_RenderFillRectF(game->renderer, &floor);

            render_3d(game);
            break;
        }

        case RENDER_MODE_FLAT: {
            set_color(game, BLACK);
            SDL_RenderClear(game->renderer);
            set_color(game, GREEN);
            render_walls(game);

            set_color(game, WHITE);
            render_rays(game);

            render_camera(game);
            break;
        }
    }

    set_color(game, WHITE);
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


    for (size_t i = 0; i < game->camera->nrays; i++) {
        struct ray_t ray;
        struct intersection_t ray_int = {.wall = NULL};

        ray.pos = (struct vector_t) {.x = (float) game->camera->pos.x, .y = (float) game->camera->pos.y};
        ray.dir = *vector_from_angle(vector(), radians(
                (float) i / (float) game->camera->resmult + game->camera->angle - (float) game->camera->fov / 2.0f)
        );

        float min_dist = INFINITY;

        for (size_t j = 0; j < game->nwalls; j++) {
            const struct wall_t *const wall = game->walls + j;
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

bool on_event(struct game_t *const game, const SDL_Event *const event) {
    switch (event->type) {
        case SDL_QUIT:
            return false;

        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
                case KEY_FORWARD:
                    game->camera->movement.forward = true;
                    break;
                case KEY_BACKWARD:
                    game->camera->movement.backward = true;
                    break;
                case KEY_LEFT:
                    game->camera->movement.left = true;
                    break;
                case KEY_RIGHT:
                    game->camera->movement.right = true;
                    break;
                case KEY_SPRINT:
                    game->camera->speed = CAMERA_SPRINT_MOVEMENT_SPEED;
                    break;
                case KEY_FOV_INC:
                    game->camera->fov = (size_t) constrain((float) game->camera->fov + 1, FOV_MIN, FOV_MAX);
                    game->camera->nrays = game->camera->fov * game->camera->resmult;
                    break;
                case KEY_FOV_DEC:
                    game->camera->fov = (size_t) constrain((float) game->camera->fov - 1, FOV_MIN, FOV_MAX);
                    game->camera->nrays = game->camera->fov * game->camera->resmult;
                    break;
                case KEY_RESMULT_INC:
                    game->camera->resmult = (size_t) constrain((float) game->camera->resmult + 1, RESMULT_MIN,
                                                               RESMULT_MAX);
                    game->camera->nrays = game->camera->fov * game->camera->resmult;
                    break;
                case KEY_RESMULT_DEC:
                    game->camera->resmult = (size_t) constrain((float) game->camera->resmult - 1, RESMULT_MIN,
                                                               RESMULT_MAX);
                    game->camera->nrays = game->camera->fov * game->camera->resmult;
                    break;
                case KEY_RESET:
                    game->camera->pos = game->center;
                    camera_update_angle(game, CAMERA_HEADING);
                    break;
                case KEY_QUIT_1:
                case KEY_QUIT_2:
                    return false;
                case KEY_SWITCH_MODE:
                    game->render_mode = game->render_mode == RENDER_MODE_NORMAL ? RENDER_MODE_FLAT : RENDER_MODE_NORMAL;
                    break;

                default:
                    break;
            }
            break;

        case SDL_KEYUP:
            switch (event->key.keysym.sym) {
                case KEY_FORWARD:
                    game->camera->movement.forward = false;
                    break;
                case KEY_BACKWARD:
                    game->camera->movement.backward = false;
                    break;
                case KEY_LEFT:
                    game->camera->movement.left = false;
                    break;
                case KEY_RIGHT:
                    game->camera->movement.right = false;
                    break;
                case KEY_SPRINT:
                    game->camera->speed = CAMERA_MOVEMENT_SPEED;
                    break;
                default:
                    break;
            }
            break;

        case SDL_MOUSEMOTION:
            camera_update_angle(game,
                                game->camera->angle + (float) event->motion.xrel * (float) CAMERA_ROTATION_SPEED);
            break;

        default:
            break;
    }

    return true;
}
