#ifndef RAY_MENU_H
#define RAY_MENU_H


#include <stdlib.h>

#include <SDL2/SDL.h>

#include "vector.h"


enum unused menu_elem_type_t {
    MENU_ELEM_BUTTON,
    MENU_ELEM_TEXT,
    MENU_ELEM_OPTION,
    MENU_ELEM_CONTAINER
};

enum unused menu_number_type_t {
    MENU_NUMBER_PX,
    MENU_NUMBER_PERCENT,
};

enum unused menu_event_type_t {
    MENU_EVENT_CLICK,
    MENU_EVENT_HOVER_ENTER,
    MENU_EVENT_HOVER_LEAVE,
    MENU_EVENT_FOCUS_ENTER,
    MENU_EVENT_FOCUS_LEAVE
};

struct menu_number_t {
    float value;
    enum menu_number_type_t type;
};

struct menu_padding_t {
    struct menu_number_t top;
    struct menu_number_t right;
    struct menu_number_t bottom;
    struct menu_number_t left;
};

struct menu_border_t {
    struct menu_number_t width;
    SDL_Color color;
};

struct menu_event_t {
    enum menu_event_type_t type;
    struct menu_elem_t *elem;

    union {
        struct vec_t pos;
    } data;
};

typedef void (*menu_event_handler_t)(const struct menu_event_t *event, void *arg);

struct menu_elem_t;

struct menu_elem_t {
    const char *id;
    enum menu_elem_type_t type;
    struct menu_elem_t *parent;

    struct menu_number_t width;
    struct menu_number_t height;

    struct menu_padding_t padding;
    struct menu_padding_t margin;
    struct menu_border_t border;

    bool hover;
    bool focus;
    bool visible;

    SDL_Color text_color;
    SDL_Color background_color;

    union {
        struct {
            const char *value;
        } text;

        struct {
            const char *title;
        } button;

        struct {
            const char *title;
            const char *value;

            void (*on_next)(void *arg);

            void (*on_prev)(void *arg);

            void *on_next_arg;
            void *on_prev_arg;
        } option;
    } specifics;

    menu_event_handler_t on_event;
    void *event_arg;

    size_t num_children;
    struct menu_elem_t **children;
};

struct menu_elem_t *menu_get_element_by_id(const char *id);

void menu_destroy_element(struct menu_elem_t *elem);

struct menu_elem_t *menu_create_element(const char *id, enum menu_elem_type_t type);

int menu_append_child(struct menu_elem_t *parent, struct menu_elem_t *child);

int menu_render(SDL_Renderer *renderer);

int menu_handle_event(const SDL_Event *event);

int menu_initialize(void);

struct menu_number_t menu_px(float px);

struct menu_number_t menu_percent(float percent);

struct menu_number_t menu_vw(float vw);

struct menu_number_t menu_vh(float vh);

void menu_print_tree(void);


#endif //RAY_MENU_H
