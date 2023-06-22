#ifndef RAY_EVENT_H
#define RAY_EVENT_H


#include <stdbool.h>


#include <SDL2/SDL.h>


#include "game.h"


/**
 * @brief Handles a SDL event.
 *
 * @param game The game instance to handle the event for.
 * @param event The SDL event to handle.
 * @return If the event signals to exit the game, return true. Otherwise, return false.
 */
bool on_event(struct game_t *game, const SDL_Event *event);

#endif //RAY_EVENT_H
