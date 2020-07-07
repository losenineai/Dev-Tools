#include <pthread.h>
#include "../dx_libc_prefix.h"

__BEGIN_DECLS

int dx_pthread_create(pthread_t* thread_out, pthread_attr_t const* attr,
                   void* (*start_routine)(void*), void* arg);

int dx_clone(int (*fn)(void*), void* child_stack, int flags, void* arg, ...);

int dx_pthread_kill(pthread_t t, int sig);

int dx_pthread_detach(pthread_t t);

int dx_pthread_join(pthread_t t, void** return_value);

__END_DECLS