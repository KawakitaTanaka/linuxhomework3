#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;

    // wait before obtaining mutex
    usleep(thread_func_args->wait_to_obtain_ms * 1000);

    // obtain mutex
    if(pthread_mutex_lock(&thread_func_args->mutex) != 0) {
        ERROR_LOG("Could not lock mutex");
        return NULL;
    }

    // wait before releasing mutex
    usleep(thread_func_args->wait_to_release_ms * 1000);

    // release mutex
    if(pthread_mutex_unlock(&thread_func_args->mutex) != 0) {
        ERROR_LOG("Could not unlock mutex");
        return NULL;
    }

    return NULL;
}

bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex, int wait_to_obtain_ms, int wait_to_release_ms)
{
    struct thread_data *t_data;
    t_data = malloc(sizeof(struct thread_data));

    if (t_data == NULL) {
        ERROR_LOG("Could not allocate memory for thread data");
        return false;
    }

    // Setup the thread data structure
    t_data->mutex = *mutex;
    t_data->wait_to_obtain_ms = wait_to_obtain_ms;
    t_data->wait_to_release_ms = wait_to_release_ms;

    // Create the thread
    if(pthread_create(thread, NULL, threadfunc, (void *)t_data) != 0) {
        ERROR_LOG("Could not create thread");
        free(t_data);
        return false;
    }

    return true;
}
