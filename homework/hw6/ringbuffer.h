#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct ringbuffer_t ringbuffer_t;

#include "threadpool.h"

struct ringbuffer_t {
    size_t size;
    size_t head;
    size_t tail;
    size_t count;
    threadpool_task_t *buffer;
};

ringbuffer_t *ringbuffer_create(size_t size);

void ringbuffer_destroy(ringbuffer_t *ringbuffer);

bool ringbuffer_is_empty(ringbuffer_t *ringbuffer);

bool ringbuffer_is_full(ringbuffer_t *ringbuffer);

bool ringbuffer_push(ringbuffer_t *ringbuffer, threadpool_task_t value);

bool ringbuffer_pop(ringbuffer_t *ringbuffer, threadpool_task_t *value);

#endif
