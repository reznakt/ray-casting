#include "math.h"


#include "menu.h"
#include "logger.h"


#include "event.h"


static void camera_set_resmult(struct game_t *const game, const size_t resmult) {
    game->camera->resmult = (size_t) constrain((float) resmult, RESMULT_MIN, RESMULT_MAX);
    game->camera->nrays = game->camera->fov * game->camera->resmult;
}

static void camera_set_fov(struct game_t *const game, const size_t fov) {
    game->camera->fov = (size_t) constrain((float) fov, FOV_MIN, FOV_MAX);
    game->camera->nrays = game->camera->fov * game->camera->resmult;
}

static void camera_set_lightmult(struct game_t *const game, const float lightmult) {
    game->camera->lightmult = constrain(lightmult, 0.0F, INFINITY);
}


void on_event(struct game_t *const restrict game, const SDL_Event *const restrict event) {
    if (game->paused) {
        menu_handle_event(&game->menu, event);
    }

    static SDL_Keycode last_key = SDLK_UNKNOWN;
    SDL_Keycode key;

    switch (event->type) {
        case SDL_QUIT:
            game->quit = true;
            break;

        case SDL_KEYDOWN:
            key = event->key.keysym.sym;

            if (key != last_key) {
                logger_printf(LOG_LEVEL_DEBUG, "key event: %s\n", SDL_GetKeyName(key));
                last_key = key;
            }

            switch (key) {
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
                case KEY_CROUCH:
                    game->camera->movement.crouch = true;
                    game->camera->speed = CAMERA_CROUCH_MOVEMENT_SPEED;
                    break;
                case KEY_SPRINT:
                    game->camera->speed = CAMERA_SPRINT_MOVEMENT_SPEED;
                    break;
                case KEY_FOV_INC:
                    camera_set_fov(game, game->camera->fov + 1);
                    break;
                case KEY_FOV_DEC:
                    camera_set_fov(game, game->camera->fov - 1);
                    break;
                case KEY_RESMULT_INC:
                    camera_set_resmult(game, game->camera->resmult + 1);
                    break;
                case KEY_RESMULT_DEC:
                    camera_set_resmult(game, game->camera->resmult - 1);
                    break;
                case KEY_NTHREADS_INC:
                    game->nthreads++;
                    break;
                case KEY_NTHREADS_DEC:
                    if (game->nthreads > 1) {
                        game->nthreads--;
                    }
                    break;
                case KEY_RESET:
                    game->camera->pos = game->center;
                    camera_update_angle(game, CAMERA_HEADING);
                    break;
                case KEY_QUIT:
                    game->quit = true;
                    break;
                case KEY_PAUSE:
                    game->paused = !game->paused;
                    SDL_SetRelativeMouseMode(game->paused ? SDL_FALSE : SDL_TRUE);

                    if (game->paused) {
                        SDL_WarpMouseInWindow(game->window, (int) game->center.x, (int) game->center.y);
                    }
                    break;
                case KEY_VIEW_1:
                    game->render_mode = RENDER_MODE_FLAT;
                    break;
                case KEY_VIEW_2:
                    game->render_mode = RENDER_MODE_WIREFRAME;
                    break;
                case KEY_VIEW_3:
                    game->render_mode = RENDER_MODE_UNTEXTURED;
                    break;
                case KEY_VIEW_4:
                    game->render_mode = RENDER_MODE_TEXTURED;
                    break;
                case KEY_LIGHT_INC:
                    camera_set_lightmult(game, game->camera->lightmult + 0.1F);
                    break;
                case KEY_LIGHT_DEC:
                    camera_set_lightmult(game, game->camera->lightmult - 0.1F);
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
                case KEY_CROUCH:
                    game->camera->movement.crouch = false;
                    game->camera->speed = CAMERA_MOVEMENT_SPEED;
                    break;
                case KEY_SPRINT:
                    game->camera->speed = CAMERA_MOVEMENT_SPEED;
                    break;
                default:
                    break;
            }
            break;

        case SDL_MOUSEMOTION:
            camera_update_angle(game, game->camera->angle + (float) event->motion.xrel * (float) CAMERA_ROTATION_SPEED);
            break;
    }
}
