#ifndef RAY_MENU_H
#define RAY_MENU_H


#include <SDL2/SDL.h>


#include "vector.h"


#define MENU_MAX_ELEMENTS 16

#define BUTTON_PADDING 8
#define BUTTON_HEIGHT (CHAR_HEIGHT + 2 * BUTTON_PADDING)


enum unused menu_alignment_t {
    MENU_ALIGN_LEFT,
    MENU_ALIGN_CENTER,
    MENU_ALIGN_RIGHT
};

struct menu_button_t {
    void (*on_click)(void *arg);

    void *on_click_arg;
    struct vec_t pos;
    bool hover;
    const char *name;
};

struct menu_text_t {
    struct vec_t pos;
    const char *value;
};

struct menu_line_t {
    struct vec_t start;
    struct vec_t end;
};

struct menu_t {
    void (*on_event)(const SDL_Event *event, void *arg);

    void *on_event_arg;
    struct vec_t pos;
    struct vec_t size;
    size_t num_buttons;
    size_t num_texts;
    size_t num_lines;
    struct menu_button_t buttons[MENU_MAX_ELEMENTS];
    struct menu_text_t texts[MENU_MAX_ELEMENTS];
    struct menu_line_t lines[MENU_MAX_ELEMENTS];
};


int menu_add_button(struct menu_t *menu, struct vec_t pos, const char *title, enum menu_alignment_t alignment,
                    void (*on_click)(void *arg), void *on_click_arg);

int menu_add_text(struct menu_t *menu, struct vec_t pos, const char *value);

int menu_add_line(struct menu_t *menu, struct vec_t start, struct vec_t end);

void menu_render(SDL_Renderer *renderer, const struct menu_t *menu);

size_t menu_button_width(const char *title);

void menu_handle_event(struct menu_t *menu, const SDL_Event *event);

void menu_create(struct menu_t *menu,
                 struct vec_t pos,
                 struct vec_t size,
                 const char *title,
                 void (*on_close)(void *arg),
                 void *on_close_arg,
                 void (*on_event)(const SDL_Event *event, void *arg),
                 void *on_event_arg);


#endif //RAY_MENU_H
