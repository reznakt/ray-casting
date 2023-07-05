#include <stdbool.h>


#include "ringbuffer.h"


struct ringbuffer_t *ringbuffer_create(const size_t capacity,
                                       struct ringbuffer_t *const restrict dst,
                                       void **const restrict data) {
    dst->size = 0;
    dst->capacity = capacity;
    dst->head = 0;
    dst->tail = 0;
    dst->data = data;

    return dst;
}

size_t ringbuffer_size(const struct ringbuffer_t *const buffer) {
    return buffer->size;
}

size_t ringbuffer_capacity(const struct ringbuffer_t *const buffer) {
    return buffer->capacity;
}

bool ringbuffer_is_empty(const struct ringbuffer_t *const buffer) {
    return buffer->size == 0;
}

bool ringbuffer_is_full(const struct ringbuffer_t *const buffer) {
    return buffer->size == buffer->capacity;
}

int ringbuffer_push(struct ringbuffer_t *const restrict buffer, void *const restrict item) {
    if (buffer == NULL || ringbuffer_is_full(buffer)) {
        return -1;
    }

    buffer->data[buffer->head] = item;
    buffer->head = (buffer->head + 1) % buffer->capacity;
    buffer->size++;

    return 0;
}

void *ringbuffer_pop(struct ringbuffer_t *const buffer) {
    if (buffer == NULL || ringbuffer_is_empty(buffer)) {
        return NULL;
    }

    void *const item = buffer->data[buffer->tail];
    buffer->tail = (buffer->tail + 1) % buffer->capacity;
    buffer->size--;

    return item;
}

void *ringbuffer_peek(const struct ringbuffer_t *const buffer) {
    if (buffer == NULL || ringbuffer_is_empty(buffer)) {
        return NULL;
    }

    return buffer->data[buffer->tail];
}

void *ringbuffer_peek_at(const struct ringbuffer_t *const buffer, const size_t index) {
    if (buffer == NULL || ringbuffer_is_empty(buffer) || index >= buffer->size) {
        return NULL;
    }

    return buffer->data[(buffer->tail + index) % buffer->capacity];
}

void ringbuffer_clear(struct ringbuffer_t *const buffer) {
    buffer->size = 0;
    buffer->head = 0;
    buffer->tail = 0;
}
