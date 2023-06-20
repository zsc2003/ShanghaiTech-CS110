#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <stdbool.h>
#include <pthread.h>

typedef struct threadpool_task_t threadpool_task_t;
typedef struct threadpool_t threadpool_t;


#include "ringbuffer.h"

typedef struct threadpool_task_t {
    void (*func)(void *);           /* Function of the task */

    void *args;                     /* Arguments of the task */
} threadpool_task_t;

typedef struct threadpool_t {
    pthread_mutex_t pool_lock;      /* A big lock to ensure safety */
    pthread_cond_t notify;          /* A condition var for waking workers up */

    /* Number of workers */
    size_t thread_count;
    pthread_t *thread_list;         /* Array of workers */

    ringbuffer_t *task_list;        /* Ringbuffer of tasks */

    bool sync;                      /* Wait for all tasks to finish before destruct */
    bool shutdown;                  /* Is this pool ready for close? */
} threadpool_t;

threadpool_t *threadpool_create(size_t thread_count, size_t queue_size, bool sync);

bool threadpool_add_task(threadpool_t *pool, void (*func)(void *), void *args);

bool threadpool_destroy(threadpool_t *pool);

#endif
