#include <pthread.h>
#include <assert.h>

#include "../threadpool.h"

#define Ntestcase1 100
pthread_spinlock_t spin;

void test_drain_buffer(void *arg) {
    int *count = (int *) arg;
    pthread_spin_lock(&spin);
    (*count)++;
    pthread_spin_unlock(&spin);
}

int main(void) {
    threadpool_t *pool;
    pthread_spin_init(&spin, 0);

    /* test case 1: wait queue drain */
    int count = 0;
    pool = threadpool_create(4, Ntestcase1, 1);
    for (int i = 0; i < Ntestcase1; i++) {
        assert(threadpool_add_task(pool, test_drain_buffer, &count));
    }
    assert(threadpool_destroy(pool));
    assert(count == Ntestcase1);
    pthread_spin_destroy(&spin);
    return 0;
}

