#include <math.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "logger.h"
#include "util.h"

#include "menu.h"


#define notnull(x, rv)                                  \
do {                                                    \
    if ((x) == NULL) {                                  \
        logger_print(LOG_LEVEL_ERROR, #x " is NULL");   \
        return (rv);                                    \
    }                                                   \
} while (0)


static struct menu_elem_t *menu_root = NULL;

static const float vw = (float) SCREEN_WIDTH / 100.0F;
static const float vh = (float) SCREEN_HEIGHT / 100.0F;


/* ------------------------------ */
/*    Public function headers     */
/* ------------------------------ */


static void cleanup(void);

static bool has_children(const struct menu_elem_t *elem);

static struct menu_elem_t *menu_get_element_by_id_helper(struct menu_elem_t *root, const char *id);

static inline const char *safestr(const char *str);

static struct vec_t element_size(const struct menu_elem_t *elem);

static struct vec_t element_pos(const struct menu_elem_t *elem);

static struct menu_padding_t get_padding(const struct menu_elem_t *elem);

static struct menu_padding_t get_margin(const struct menu_elem_t *elem);

static struct menu_number_t get_width(const struct menu_elem_t *elem);

static struct menu_number_t get_height(const struct menu_elem_t *elem);

static inline const char *bool_to_str(bool b);

static void print_element(const struct menu_elem_t *elem, size_t depth);

static const char *number_unit(struct menu_number_t number);


struct menu_padding_t get_padding(const struct menu_elem_t *const elem) {
    const bool left_percent = elem->padding.left.type == MENU_NUMBER_PERCENT;
    const bool right_percent = elem->padding.right.type == MENU_NUMBER_PERCENT;
    const bool top_percent = elem->padding.top.type == MENU_NUMBER_PERCENT;
    const bool bottom_percent = elem->padding.bottom.type == MENU_NUMBER_PERCENT;

    if (!left_percent && !right_percent && !top_percent && !bottom_percent) {
        return elem->padding;
    }

    const struct vec_t parent_size = elem->parent == NULL ? (struct vec_t) {vw, vh} : element_size(elem->parent);

    const float left = elem->padding.left.type == MENU_NUMBER_PX
                       ? elem->padding.left.value
                       : parent_size.x * elem->padding.left.value / 100.0F;

    const float right = elem->padding.right.type == MENU_NUMBER_PX
                        ? elem->padding.right.value
                        : parent_size.x * elem->padding.right.value / 100.0F;

    const float top = elem->padding.top.type == MENU_NUMBER_PX
                      ? elem->padding.top.value
                      : parent_size.y * elem->padding.top.value / 100.0F;

    const float bottom = elem->padding.bottom.type == MENU_NUMBER_PX
                         ? elem->padding.bottom.value
                         : parent_size.y * elem->padding.bottom.value / 100.0F;

    return (struct menu_padding_t) {
            .left = menu_px(left),
            .right = menu_px(right),
            .top = menu_px(top),
            .bottom = menu_px(bottom)
    };
}

static struct menu_padding_t get_margin(const struct menu_elem_t *const elem) {
    const bool left_percent = elem->margin.left.type == MENU_NUMBER_PERCENT;
    const bool right_percent = elem->margin.right.type == MENU_NUMBER_PERCENT;
    const bool top_percent = elem->margin.top.type == MENU_NUMBER_PERCENT;
    const bool bottom_percent = elem->margin.bottom.type == MENU_NUMBER_PERCENT;

    if (!left_percent && !right_percent && !top_percent && !bottom_percent) {
        return elem->margin;
    }

    const struct vec_t parent_size = elem->parent == NULL ? (struct vec_t) {vw, vh} : element_size(elem->parent);

    const float left = elem->margin.left.type == MENU_NUMBER_PX
                       ? elem->margin.left.value
                       : parent_size.x * elem->margin.left.value / 100.0F;

    const float right = elem->margin.right.type == MENU_NUMBER_PX
                        ? elem->margin.right.value
                        : parent_size.x * elem->margin.right.value / 100.0F;

    const float top = elem->margin.top.type == MENU_NUMBER_PX
                      ? elem->margin.top.value
                      : parent_size.y * elem->margin.top.value / 100.0F;

    const float bottom = elem->margin.bottom.type == MENU_NUMBER_PX
                         ? elem->margin.bottom.value
                         : parent_size.y * elem->margin.bottom.value / 100.0F;

    return (struct menu_padding_t) {
            .left = menu_px(left),
            .right = menu_px(right),
            .top = menu_px(top),
            .bottom = menu_px(bottom)
    };
}

struct menu_number_t menu_vw(const float vw_) {
    return (struct menu_number_t) {vw * vw_, MENU_NUMBER_PX};
}

struct menu_number_t menu_vh(const float vh_) {
    return (struct menu_number_t) {vh * vh_, MENU_NUMBER_PX};
}

struct menu_number_t menu_px(const float px) {
    return (struct menu_number_t) {px, MENU_NUMBER_PX};
}

struct menu_number_t menu_percent(const float percent) {
    return (struct menu_number_t) {percent, MENU_NUMBER_PERCENT};
}

static struct menu_number_t get_width(const struct menu_elem_t *const elem) {
    notnull(elem, menu_px(NAN));

    if (elem->width.type == MENU_NUMBER_PX) {
        return elem->width;
    }

    const struct vec_t parent_size = elem->parent == NULL ? (struct vec_t) {vw, vh} : element_size(elem->parent);
    return menu_px(parent_size.x * elem->width.value / 100.0F);
}

static struct menu_number_t get_height(const struct menu_elem_t *const elem) {
    notnull(elem, menu_px(NAN));

    if (elem->height.type == MENU_NUMBER_PX) {
        return elem->height;
    }

    const struct vec_t parent_size = elem->parent == NULL ? (struct vec_t) {vw, vh} : element_size(elem->parent);
    return menu_px(parent_size.y * elem->height.value / 100.0F);
}

static inline const char *safestr(const char *const str) {
    return str == NULL ? "(null)" : str;
}

static bool has_children(const struct menu_elem_t *const elem) {
    notnull(elem, false);

    const bool num_children = elem->num_children > 0;
    const bool children = elem->children != NULL;

    if (num_children != children) {
        logger_printf(LOG_LEVEL_WARN, "element '%s' has num_children = %d, but children = %d",
                      safestr(elem->id), num_children, children);
        return false;
    }

    return children;
}

static struct menu_elem_t *menu_get_element_by_id_helper(struct menu_elem_t *const restrict root,
                                                         const char *const restrict id) {
    notnull(id, NULL);
    notnull(root, NULL);

    if (strcmp(root->id, id) == 0) {
        return root;
    }

    if (!has_children(root)) {
        return NULL;
    }

    for (size_t i = 0; i < root->num_children; i++) {
        struct menu_elem_t *const elem = root->children[i];

        if (strcmp(elem->id, id) == 0) {
            return elem;
        }

        struct menu_elem_t *const child = menu_get_element_by_id_helper(elem, id);

        if (child != NULL) {
            return child;
        }
    }

    return NULL;
}

struct menu_elem_t *menu_get_element_by_id(const char *const id) {
    return menu_get_element_by_id_helper(menu_root, id);
}

void menu_destroy_element(struct menu_elem_t *const elem) {
    if (elem == NULL) {
        return;
    }

    if (has_children(elem)) {
        for (size_t i = 0; i < elem->num_children; i++) {
            menu_destroy_element(elem->children[i]);
        }

        free(elem->children);
    }

    const char *const id = elem->id;
    free(elem);

    logger_printf(LOG_LEVEL_DEBUG, "destroyed element '%s'\n", safestr(id));
}

static inline void cleanup(void) {
    menu_destroy_element(menu_root);
}

int menu_initialize(void) {
    menu_root = menu_create_element(":root", MENU_ELEM_CONTAINER);

    if (menu_root == NULL) {
        return -1;
    }

    menu_root->width = menu_vw(100.0F);
    menu_root->height = menu_vh(100.0F);
    menu_root->border.width = menu_px(0.0F);

    if (atexit(cleanup) != 0) {
        logger_perror("atexit");
        return -1;
    }

    return 0;
}

static void initialize_element(struct menu_elem_t *const restrict elem,
                               const char *const restrict id,
                               const enum menu_elem_type_t type) {
    elem->id = id;
    elem->type = type;

    elem->width = menu_px(NAN);
    elem->height = menu_px(NAN);

    elem->padding.top = menu_px(10.0F);
    elem->padding.right = menu_px(10.0F);
    elem->padding.bottom = menu_px(10.0F);
    elem->padding.left = menu_px(10.0F);

    elem->margin.top = menu_px(0.0F);
    elem->margin.right = menu_px(0.0F);
    elem->margin.bottom = menu_px(0.0F);
    elem->margin.left = menu_px(0.0F);

    elem->border.width = menu_px(1.0F);
    elem->border.color = COLOR_WHITE;

    elem->visible = true;

    elem->text_color = COLOR_WHITE;
    elem->background_color = COLOR_BLACK;
}

struct menu_elem_t *menu_create_element(const char *const id, const enum menu_elem_type_t type) {
    notnull(id, NULL);

    if (menu_root != NULL && menu_get_element_by_id(id) != NULL) {
        logger_printf(LOG_LEVEL_ERROR, "element with id '%s' already exists\n", id);
        return NULL;
    }

    struct menu_elem_t *const elem = calloc(1, sizeof *elem);

    if (elem == NULL) {
        logger_perror("malloc");
        return NULL;
    }

    initialize_element(elem, id, type);

    logger_printf(LOG_LEVEL_DEBUG, "created element '%s'\n", id);
    return elem;
}

int menu_append_child(struct menu_elem_t *restrict parent, struct menu_elem_t *const restrict child) {
    notnull(child, -1);

    if (child->parent != NULL) {
        logger_printf(LOG_LEVEL_ERROR, "element '%s' is already a child of '%s'\n",
                      safestr(child->id), safestr(child->parent->id));
        return -1;
    }

    if (parent == NULL) {
        notnull(menu_root, -1);
        parent = menu_root;
    }

    parent->num_children++;
    struct menu_elem_t **const children = realloc(parent->children, parent->num_children * sizeof *children);

    if (children == NULL) {
        logger_perror("realloc");
        return -1;
    }

    parent->children = children;
    parent->children[parent->num_children - 1] = child;
    child->parent = parent;

    return 0;
}

static struct vec_t padding_to_vec(const struct menu_padding_t padding) {
    return (struct vec_t) {
            padding.left.value + padding.right.value,
            padding.top.value + padding.bottom.value
    };
}

static struct vec_t element_size_helper(const struct menu_elem_t *const elem) {
    notnull(elem, ((struct vec_t) {NAN, NAN}));
    struct vec_t size = {0.0F, 0.0F};

    if (!elem->visible) {
        return size;
    }

    const float width = get_width(elem).value;
    const float height = get_height(elem).value;

    if (!isnan(width) && !isnan(height)) {
        return (struct vec_t) {width, height};
    }

    size = vadd(size, padding_to_vec(get_padding(elem)));
    size = vadd(size, padding_to_vec(get_margin(elem)));

    switch (elem->type) {
        case MENU_ELEM_TEXT:
        case MENU_ELEM_BUTTON:
            if (elem->specifics.text.value != NULL) {
                size.x += (float) text_width(elem->specifics.text.value);
                size.y += (float) CHAR_HEIGHT;
            }
            break;
        case MENU_ELEM_CONTAINER:
        case MENU_ELEM_OPTION:
            break;
    }

    struct vec_t children_size = {0.0F, 0.0F};

    if (has_children(elem)) {
        for (size_t i = 0; i < elem->num_children; i++) {
            const struct menu_elem_t *const child = elem->children[i];
            const struct vec_t child_size = element_size(child);

            children_size.x = fmaxf(child_size.x, children_size.x);
            children_size.y += child_size.y;
        }
    }

    size = vadd(size, children_size);

    if (!isnan(width)) {
        size.x = width;
    }

    if (!isnan(height)) {
        size.y = height;
    }

    return size;
}

static struct vec_t element_size(const struct menu_elem_t *const elem) {
    return vsub(element_size_helper(elem), padding_to_vec(get_margin(elem)));
}

static struct vec_t element_pos(const struct menu_elem_t *const elem) {
    notnull(elem, ((struct vec_t) {NAN, NAN}));

    if (elem->parent == NULL) {
        return (struct vec_t) {0.0F, 0.0F};
    }

    struct vec_t pos = element_pos(elem->parent);

    const struct menu_padding_t padding = get_padding(elem->parent);
    const struct menu_padding_t margin = get_margin(elem);

    pos.x += padding.left.value + margin.left.value;
    pos.y += padding.top.value + margin.top.value;

    for (size_t i = 0; i < elem->parent->num_children; i++) {
        const struct menu_elem_t *const child = elem->parent->children[i];

        if (child == elem) {
            break;
        }

        const struct vec_t child_size = vadd(element_size(child), padding_to_vec(get_margin(child)));
        pos.x = fminf(pos.x + child_size.x, pos.x);
        pos.y += child_size.y;
    }

    return pos;
}

static int render_element(SDL_Renderer *const restrict renderer, const struct menu_elem_t *const restrict elem) {
    notnull(renderer, -1);
    notnull(elem, -1);

    if (!elem->visible) {
        return 0;
    }

    const struct vec_t size = element_size(elem);
    const struct vec_t pos = element_pos(elem);

    if (elem->border.width.value > 0.0F) {
        const SDL_FRect rect = {
                pos.x,
                pos.y,
                size.x,
                size.y
        };

        render_colored(renderer, elem->border.color, {
            SDL_RenderDrawRectF(renderer, &rect);
        });
    }

    const struct vec_t pos_with_padding = vadd(pos, (struct vec_t) {elem->padding.left.value, elem->padding.top.value});

    switch (elem->type) {
        case MENU_ELEM_TEXT:
            if (elem->specifics.text.value != NULL) {
                render_colored(renderer, elem->text_color, {
                    render_puts(renderer, pos_with_padding, elem->specifics.text.value);
                });
            }
            break;
        case MENU_ELEM_OPTION:
        case MENU_ELEM_CONTAINER:
        case MENU_ELEM_BUTTON:
            break;
    }

    if (!has_children(elem)) {
        return 0;
    }

    for (size_t i = 0; i < elem->num_children; i++) {
        const struct menu_elem_t *const child = elem->children[i];

        if (render_element(renderer, child) != 0) {
            logger_printf(LOG_LEVEL_ERROR, "unable to render child %s (no. %zu) of element %s\n",
                          safestr(child->id), i, safestr(elem->id));
            return -1;
        }
    }

    return 0;
}

int menu_render(SDL_Renderer *const renderer) {
    return render_element(renderer, menu_root);
}

static const char *element_type_name(const struct menu_elem_t *const elem) {
    notnull(elem, NULL);

    switch (elem->type) {
        case MENU_ELEM_TEXT:
            return "Text";
        case MENU_ELEM_OPTION:
            return "Option";
        case MENU_ELEM_CONTAINER:
            return "Container";
        case MENU_ELEM_BUTTON:
            return "Button";
    }

    return "(unknown)";
}

static inline const char *bool_to_str(const bool b) {
    return b ? "true" : "false";
}

static inline const char *number_unit(const struct menu_number_t number) {
    switch (number.type) {
        case MENU_NUMBER_PX:
            return "px";
        case MENU_NUMBER_PERCENT:
            return "%";
    }

    return "(unknown)";
}

static void print_element(const struct menu_elem_t *const elem, const size_t depth) {
    if (elem == NULL) {
        logger_print(LOG_LEVEL_ERROR, "element is NULL");
        return;
    }

    const struct menu_number_t width = get_width(elem);
    const struct menu_number_t height = get_height(elem);
    const struct vec_t pos = element_pos(elem);
    const struct vec_t size = element_size(elem);
    const struct menu_padding_t padding = get_padding(elem);
    const struct menu_padding_t margin = get_margin(elem);

    logger_printf(
            LOG_LEVEL_DEBUG,
            "%*s<%s id='%s' width=%.1f height=%.1f pos=[%.1f, %.1f] size=%.1f x %.1f px "
            "visible=%s hover=%s focus=%s "
            "border=%.1f px #%02x%02x%02x%02x text-color=#%02x%02x%02x%02x background-color=#%02x%02x%02x%02x "
            "padding=[%.1f %s (%.1f px), %.1f %s (%.1f px), %.1f %s (%.1f px), %.1f %s (%.1f px)] "
            "margin=[%.1f %s (%.1f px), %.1f %s (%.1f px), %.1f %s (%.1f px), %.1f %s (%.1f px)]>\n",
            (int) depth, "",
            safestr(element_type_name(elem)),
            safestr(elem->id),
            width.value,
            height.value,
            pos.x, pos.y,
            size.x, size.y,
            bool_to_str(elem->visible), bool_to_str(elem->hover), bool_to_str(elem->focus), elem->border.width.value,
            elem->border.color.r, elem->border.color.g, elem->border.color.b, elem->border.color.a,
            elem->text_color.r, elem->text_color.g, elem->text_color.b, elem->text_color.a,
            elem->background_color.r, elem->background_color.g, elem->background_color.b, elem->background_color.a,
            elem->padding.top.value, number_unit(elem->padding.top), padding.top.value,
            elem->padding.right.value, number_unit(elem->padding.right), padding.right.value,
            elem->padding.bottom.value, number_unit(elem->padding.bottom), padding.bottom.value,
            elem->padding.left.value, number_unit(elem->padding.left), padding.left.value,
            elem->margin.top.value, number_unit(elem->margin.top), margin.top.value,
            elem->margin.right.value, number_unit(elem->margin.right), margin.right.value,
            elem->margin.bottom.value, number_unit(elem->margin.bottom), margin.bottom.value,
            elem->margin.left.value, number_unit(elem->margin.left), margin.left.value
    );

    if (has_children(elem)) {
        for (size_t i = 0; i < elem->num_children; i++) {
            print_element(elem->children[i], depth + 2);
        }
    }
}

void menu_print_tree(void) {
    print_element(menu_root, 0);
}
