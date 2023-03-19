#ifndef RAY_UTIL_H
#define RAY_UTIL_H


/**
 * Allocate a new object of type `type` on the stack.
 */
#define STACK_ALLOC(type) ((type [1]) {0})


#endif //RAY_UTIL_H
