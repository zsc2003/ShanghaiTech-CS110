#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "threadpool.h"
#include "ringbuffer.h"

static bool lock_acquire(threadpool_t *pool) { return pthread_mutex_lock(&(pool->pool_lock)) == 0; }

static bool lock_release(threadpool_t *pool) { return pthread_mutex_unlock(&(pool->pool_lock)) == 0; }

static void deallocate_all(threadpool_t *pool) {
    /* 1. Invalid address */
    if (pool == NULL) {
        return;
    }

    pthread_cond_destroy(&(pool->notify));
    pthread_mutex_destroy(&(pool->pool_lock));

    free(pool->thread_list);
    ringbuffer_destroy(pool->task_list);

    free(pool);
}

static void *threadpool_thread(void *threadpool) {
    /* 0. Reinterpret the pointer */
    threadpool_t *pool = (threadpool_t *) threadpool;

    /* 1. Make sure the pool is valid */
    assert(pool != NULL);

    while (true) {
        /* 2. Lock */
        assert(lock_acquire(pool));

        /* 3. Keep waiting for condition */
        while (ringbuffer_is_empty(pool->task_list) && (!pool->shutdown)) {
            assert(pthread_cond_wait(&(pool->notify), &(pool->pool_lock)) == 0);
        }

        /* 4. If shutdown */
        if (pool->shutdown) {
            // TODO: sync?
            /*
                If sync == true when the threadpool was created, wait until all tasks are finished.
                Else, abort all waiting tasks immediately and return as soon as the threadpool finish its running tasks.
            */

            threadpool_task_t *task = (threadpool_task_t *)malloc(sizeof(threadpool_task_t));
            if(pool->sync == true)
            {
                while(!ringbuffer_is_empty(pool->task_list))
                {
                    assert(ringbuffer_pop(pool->task_list, task));
                    task->func(task->args);
                }
            }
            else
            {
                while(!ringbuffer_is_empty(pool->task_list))
                {
                    assert(ringbuffer_pop(pool->task_list, task));
                }
            }
            free(task);

            lock_release(pool);
            return NULL;
        }

        /* 5. Fetch next task */
        // TODO: fetch next task
        threadpool_task_t task;
        assert(ringbuffer_pop(pool->task_list, &task));

        /* 6. Unlock */
        assert(lock_release(pool));

        /* 7. Get to work */
        // TODO: run next task
        task.func(task.args);
    }
}

// TODO: modify this function, make sure it is memory safe
threadpool_t *threadpool_create(size_t thread_count, size_t queue_size, bool sync) {
    /* 0. Parameters should be reasonable */
    if (thread_count == 0 || queue_size == 0 || queue_size < thread_count) {
        return NULL;
    }

    /* 1. Allocate memory for pool */
    threadpool_t *pool = (threadpool_t *) malloc(sizeof(threadpool_t));

    /* 2. If fail */
    if (!pool) { return NULL; }

    /* 3. Allocate memory for workers and tasks */
    pool->thread_list = (pthread_t *) malloc(sizeof(pthread_t) * thread_count);
    pool->task_list = ringbuffer_create(queue_size);
    pool->sync = sync;

    /* 4. If fail */
    if (!pool->thread_list || !pool->task_list) {
        return NULL;
    }

    /* 5. Initialize lock */
    pthread_mutexattr_t safeLock;
    pthread_mutexattr_init(&safeLock);
    pthread_mutexattr_settype(&safeLock, PTHREAD_MUTEX_ERRORCHECK);
    /* Note: this kind of mutex provides error checking.
     * Error would occur when:
     * 1. A thread attempt to re-lock when holding the lock.
     * 2. A thread attempt to release the lock when not holding it.
     * 3. A thread attempt to release an unlocked lock.
     * */
    if (pthread_mutex_init(&pool->pool_lock, &safeLock) != 0) {
        return NULL;
    }

    /* 6. Initialize conditional variable */
    if (pthread_cond_init(&(pool->notify), NULL) != 0) {
        return NULL;
    }

    /* 7. Initialize other stuff */
    pool->thread_count = 0;
    pool->shutdown = false;

    /* 8. Start worker threads */
    for (size_t i = 0; i < thread_count; i++, pool->thread_count = i) {
        // TODO: pthread_create may fail ... but when?
        // RTFM!

        // pthread_create failed
        if(pthread_create(&(pool->thread_list[i]), NULL, threadpool_thread, (void *)pool) != 0)
        {
            deallocate_all(pool);
            return NULL;
        }

        // pthread_create(&(pool->thread_list[i]), NULL, threadpool_thread, (void *) pool);
    }

    return pool;
}

bool threadpool_add_task(threadpool_t *pool, void (*func)(void *), void *args) {
    /* 0. Parameters should be reasonable */
    if (!pool || !func) {
        return false;
    }

    /* 1. Acquire lock */
    if (!lock_acquire(pool)) { return false; }

    /* 2. If the pool is not available for newcomers */
    if (ringbuffer_is_full(pool->task_list) || pool->shutdown) {
        /* Note: don't forget to release locks here */
        lock_release(pool);
        return false;
    }

    /* 3. Add a task to ringbuffer */
    // TODO: implement

    // The function should return false if the threadpool is full. 
    if(ringbuffer_is_full(pool->task_list))
    {
        lock_release(pool);
        return false;
    }
    // Add a task with function func and argument args into the threadpool pool.
    if(!ringbuffer_push(pool->task_list, (threadpool_task_t){func, args}))
    {
        lock_release(pool);
        return false;
    }


    /* 4. Wake up a random available worker */
    if (pthread_cond_signal(&(pool->notify)) != 0) {
        lock_release(pool);
        return false;
    }

    /* 5. Success */
    lock_release(pool);
    return true;
}

bool threadpool_destroy(threadpool_t *pool) {
    /* 0. Parameter should be reasonable */
    if (!pool) {
        return false;
    }

    /* 1. Acquire lock */
    if (!lock_acquire(pool)) { return false; }

    /* 2. If the pool is not available already shutting down */
    if (pool->shutdown) {
        lock_release(pool);
        return false;
    }

    /* 3. Start the shutting down process */
    pool->shutdown = true;

    /* 4. Wake up all workers to exit */
    if (pthread_cond_broadcast(&(pool->notify)) != 0) {
        lock_release(pool);
        return false;
    }

    /* 5. Release lock */
    lock_release(pool);

    /* 6. Wait for all threads terminate */
    // TODO: join all worker threads
    for (size_t i = 0; i != pool->thread_count; ++i)
    {
        if(pthread_join(pool->thread_list[i], NULL) != 0)
        {
            return false;
        }
    }


    /* 7. Deallocate all resources given */
    // TODO: implement
    deallocate_all(pool);


    return true;
    // return false;
}
