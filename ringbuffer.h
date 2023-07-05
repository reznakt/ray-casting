#ifndef RAY_RINGBUFFER_H
#define RAY_RINGBUFFER_H


#include <stdlib.h>


struct ringbuffer_t {
    size_t size;
    size_t capacity;
    size_t head;
    size_t tail;
    void **data;
};

/**
 * Creates a ringbuffer with the given capacity.
 * @param capacity the capacity of the ringbuffer
 * @return the ringbuffer
 */
#define ringbuffer(capacity) ringbuffer_create(capacity, stack_alloc(struct ringbuffer_t, 1), stack_alloc(void *, capacity))


/**
 * Creates a ringbuffer with the given capacity.
 * @param capacity the capacity of the ringbuffer
 * @param dst the destination ringbuffer
 * @param data the data array
 * @return the ringbuffer
 */
struct ringbuffer_t *ringbuffer_create(size_t capacity, struct ringbuffer_t *dst, void **data);

/**
 * Gets the size of the ringbuffer.
 * @param buffer the ringbuffer
 * @return the size of the ringbuffer
 */
size_t ringbuffer_size(const struct ringbuffer_t *buffer);

/**
 * Gets the capacity of the ringbuffer.
 * @param buffer the ringbuffer
 * @return the capacity of the ringbuffer
 */
size_t ringbuffer_capacity(const struct ringbuffer_t *buffer);

/**
 * Checks if the ringbuffer is empty.
 * @param buffer the ringbuffer
 * @return true if the ringbuffer is empty, false otherwise
 */
bool ringbuffer_is_empty(const struct ringbuffer_t *buffer);

/**
 * Checks if the ringbuffer is full.
 * @param buffer the ringbuffer
 * @return true if the ringbuffer is full, false otherwise
 */
bool ringbuffer_is_full(const struct ringbuffer_t *buffer);

/**
 * Pushes an item to the ringbuffer.
 * @param buffer the ringbuffer
 * @param item the item
 * @return 0 if successful, -1 otherwise
 */
int ringbuffer_push(struct ringbuffer_t *buffer, void *item);

/**
 * Pops an item from the ringbuffer.
 * @param buffer the ringbuffer
 * @return the item if successful, NULL otherwise
 */
void *ringbuffer_pop(struct ringbuffer_t *buffer);

/**
 * Peeks at the next item in the ringbuffer.
 * @param buffer the ringbuffer
 * @return the item if successful, NULL otherwise
 */
void *ringbuffer_peek(const struct ringbuffer_t *buffer);

/**
 * Peeks at the item at the given index in the ringbuffer.
 * @param buffer the ringbuffer
 * @param index the index
 * @return the item if successful, NULL otherwise
 */
void *ringbuffer_peek_at(const struct ringbuffer_t *buffer, size_t index);

/**
 * Clears the ringbuffer.
 * @param buffer the ringbuffer
 */
void ringbuffer_clear(struct ringbuffer_t *buffer);


#endif //RAY_RINGBUFFER_H
