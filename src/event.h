#ifndef RAY_EVENT_H
#define RAY_EVENT_H


#include <SDL2/SDL.h>


#include "game.h"


/**
 * @brief Handles an SDL event.
 *
 * @param game The game instance to handle the event for.
 * @param event The SDL event to handle.
 */
void on_event(struct game_t *game, const SDL_Event *event);

#endif //RAY_EVENT_H
