#include <stdlib.h>

#include <SDL2/SDL.h>

#include "logger.h"
#include "text.h"
#include "util.h"
#include "vector.h"

#include "menu.h"


static const SDL_Color TEXT_COLOR = COLOR_WHITE;
static const SDL_Color LINE_COLOR = COLOR_WHITE;

static const SDL_Color BUTTON_COLOR_BACKGROUND = COLOR_BLACK;
static const SDL_Color BUTTON_COLOR_HOVER = rgb(100, 100, 100);
static const SDL_Color BUTTON_COLOR_BORDER = COLOR_WHITE;

static const SDL_Color MENU_COLOR_BACKGROUND = COLOR_BLACK;
static const SDL_Color MENU_COLOR_BORDER = COLOR_WHITE;


int menu_add_button(
        struct menu_t *const restrict menu,
        const struct vec_t pos,
        const char *const restrict title,
        const enum menu_alignment_t alignment,
        void (*const on_click)(void *arg),
        void *const restrict on_click_arg
) {
    if (menu->num_buttons >= MENU_MAX_ELEMENTS) {
        return -1;
    }

    const size_t width = menu_button_width(title);
    struct vec_t button_pos = pos;

    if (alignment == MENU_ALIGN_CENTER) {
        button_pos.x -= (float) width / 2.0F;
    } else if (alignment == MENU_ALIGN_RIGHT) {
        button_pos.x -= (float) width;
    }

    const struct menu_button_t button = {
            .on_click = on_click,
            .pos = button_pos,
            .name = title,
            .on_click_arg = on_click_arg
    };

    menu->buttons[menu->num_buttons++] = button;
    return 0;
}

int menu_add_text(struct menu_t *const restrict menu, struct vec_t pos,
                  const char *const restrict value, const enum menu_alignment_t alignment) {
    if (menu->num_texts >= MENU_MAX_ELEMENTS) {
        return -1;
    }

    struct vec_t text_pos = pos;
    const size_t width = menu_button_width(value) - 2 * BUTTON_PADDING;

    if (alignment == MENU_ALIGN_CENTER) {
        text_pos.x -= (float) width / 2.0F;
    } else if (alignment == MENU_ALIGN_RIGHT) {
        text_pos.x -= (float) width;
    }

    const struct menu_text_t text = {.pos = text_pos, .value = value};
    menu->texts[menu->num_texts++] = text;

    return 0;
}

static void render_button(const struct menu_t *const restrict menu, SDL_Renderer *const restrict renderer,
                          const struct menu_button_t *const restrict button) {
    const struct vec_t pos = vadd(menu->pos, button->pos);

    const SDL_FRect box_pos = {
            .x = pos.x,
            .y = pos.y,
            .w = (float) menu_button_width(button->name),
            .h = CHAR_HEIGHT + 2 * BUTTON_PADDING
    };

    render_colored(renderer, button->hover ? BUTTON_COLOR_HOVER : BUTTON_COLOR_BACKGROUND, {
        SDL_RenderFillRectF(renderer, &box_pos);
    });

    render_colored(renderer, BUTTON_COLOR_BORDER, {
        SDL_RenderDrawRectF(renderer, &box_pos);
    });

    const struct vec_t text_pos = {box_pos.x + BUTTON_PADDING, box_pos.y + BUTTON_PADDING};

    render_colored(renderer, TEXT_COLOR, {
        render_puts(renderer, text_pos, button->name);
    });
}

static void render_line(const struct menu_t *const restrict menu, SDL_Renderer *const restrict renderer,
                        const struct menu_line_t *const restrict line) {
    const struct vec_t start = vadd(menu->pos, line->start);
    const struct vec_t end = vadd(menu->pos, line->end);

    render_colored(renderer, LINE_COLOR, {
        SDL_RenderDrawLineF(renderer, start.x, start.y, end.x, end.y);
    });
}

static void render_text(const struct menu_t *const restrict menu, SDL_Renderer *const restrict renderer,
                        const struct menu_text_t *const restrict text) {
    render_colored(renderer, TEXT_COLOR, {
        render_puts(renderer, vadd(menu->pos, text->pos), text->value);
    });
}

void menu_render(SDL_Renderer *const restrict renderer, const struct menu_t *const restrict menu) {
    const SDL_FRect box_pos = {
            .x = menu->pos.x,
            .y = menu->pos.y,
            .w = menu->size.x,
            .h = menu->size.y
    };

    // background
    render_colored(renderer, MENU_COLOR_BACKGROUND, {
        SDL_RenderFillRectF(renderer, &box_pos);
    });

    // border
    render_colored(renderer, MENU_COLOR_BORDER, {
        SDL_RenderDrawRectF(renderer, &box_pos);
    });

    // buttons
    for (size_t i = 0; i < menu->num_buttons; i++) {
        render_button(menu, renderer, &menu->buttons[i]);
    }

    // lines
    for (size_t i = 0; i < menu->num_lines; i++) {
        render_line(menu, renderer, &menu->lines[i]);
    }

    // text
    for (size_t i = 0; i < menu->num_texts; i++) {
        render_text(menu, renderer, &menu->texts[i]);
    }
}

size_t menu_button_width(const char *const title) {
    return text_width(title) + 2 * BUTTON_PADDING;
}

static bool is_within(const struct SDL_FRect *const restrict rect, const struct vec_t *const restrict pos) {
    return pos->x >= rect->x && pos->x <= rect->x + rect->w && pos->y >= rect->y && pos->y <= rect->y + rect->h;
}

void menu_handle_event(struct menu_t *menu, const SDL_Event *event) {
    switch (event->type) {
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN: {
            int x, y;
            SDL_GetMouseState(&x, &y);

            for (size_t i = 0; i < menu->num_buttons; i++) {
                struct menu_button_t *const button = &menu->buttons[i];

                const SDL_FRect button_rect = {
                        .x = button->pos.x + menu->pos.x,
                        .y = button->pos.y + menu->pos.y,
                        .w = (float) menu_button_width(button->name),
                        .h = BUTTON_HEIGHT
                };

                const struct vec_t mouse_pos = {.x = (float) x, .y = (float) y};

                if (!is_within(&button_rect, &mouse_pos)) {
                    button->hover = false;
                    continue;
                }

                if (event->type == SDL_MOUSEBUTTONDOWN && button->on_click != NULL) {
                    logger_printf(LOG_LEVEL_DEBUG, "button clicked: %s\n", button->name);
                    button->on_click(button->on_click_arg);
                } else if (event->type == SDL_MOUSEMOTION) {
                    button->hover = true;
                }
            }

            break;
        }
    }

    if (menu->on_event != NULL) {
        menu->on_event(event, menu->on_event_arg);
    }
}

int menu_add_line(struct menu_t *const menu, struct vec_t start, struct vec_t end) {
    if (menu->num_lines >= MENU_MAX_ELEMENTS) {
        return -1;
    }

    const struct menu_line_t line = {.start = start, .end = end};
    menu->lines[menu->num_lines++] = line;

    return 0;
}

void menu_create(struct menu_t *const restrict menu,
                 const struct vec_t pos,
                 const struct vec_t size,
                 const char *const restrict title,
                 void (*const on_close)(void *arg),
                 void *const restrict on_close_arg,
                 void (*on_event)(const SDL_Event *event, void *arg),
                 void *on_event_arg) {
    menu->pos = pos;
    menu->size = size;
    menu->num_buttons = 0;
    menu->num_texts = 0;
    menu->num_lines = 0;
    menu->on_event = on_event;
    menu->on_event_arg = on_event_arg;

    const struct vec_t close_button_pos = {.x = size.x, .y = 0.0F};
    menu_add_button(menu, close_button_pos, "X", MENU_ALIGN_RIGHT, on_close, on_close_arg);

    const struct vec_t header_line_start = {.x = 0.0F, .y = BUTTON_HEIGHT};
    const struct vec_t header_line_end = {.x = size.x, .y = BUTTON_HEIGHT};
    menu_add_line(menu, header_line_start, header_line_end);

    const struct vec_t title_pos = {.x = size.x / 2.0F, .y = BUTTON_PADDING};
    menu_add_text(menu, title_pos, title, MENU_ALIGN_CENTER);
}
