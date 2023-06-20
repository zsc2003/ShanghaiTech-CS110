#include "ringbuffer.h"

/* create a new ring buffer*/
ringbuffer_t *ringbuffer_create(size_t size) {
    ringbuffer_t *ringbuffer = malloc(sizeof(ringbuffer_t));
    /* malloc failed */
    if (ringbuffer == NULL)
	{
        return NULL;
    }
    ringbuffer->size = size;
    ringbuffer->head = 0;
    ringbuffer->tail = 0;
    ringbuffer->count = 0;
    ringbuffer->buffer = malloc(size * sizeof(threadpool_task_t));

    /* malloc failed */
    if (ringbuffer->buffer == NULL) {
        free(ringbuffer);
        return NULL;
    }
    /* return the newwly created ring buffer*/
    return ringbuffer;
}

void ringbuffer_destroy(ringbuffer_t *ringbuffer) {
    free(ringbuffer->buffer);
    free(ringbuffer);
}

bool ringbuffer_is_empty(ringbuffer_t *ringbuffer) {
    return ringbuffer->count == 0;
}

bool ringbuffer_is_full(ringbuffer_t *ringbuffer) {
    return ringbuffer->count == ringbuffer->size;
}

bool ringbuffer_push(ringbuffer_t *ringbuffer, threadpool_task_t value) {
    // TODO: implement
    if (ringbuffer->count == ringbuffer->size) {
        return false;
    }
    ringbuffer->count++;
    ringbuffer->buffer[ringbuffer->tail] = value;
    ringbuffer->tail = (ringbuffer->tail + 1) % ringbuffer->size;
    return true;
    // return false;
}

bool ringbuffer_pop(ringbuffer_t *ringbuffer, threadpool_task_t *value) {
    // TODO: implement
   
    if (ringbuffer->count == 0) {
        return false;
    }
    ringbuffer->count--;
    *value = ringbuffer->buffer[ringbuffer->head];
    ringbuffer->head = (ringbuffer->head + 1) % ringbuffer->size;
    return true;
    // return false;
}
