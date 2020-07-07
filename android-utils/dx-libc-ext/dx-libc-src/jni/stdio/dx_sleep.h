#include "dx_stdio_utils.h"
#include "../dx_libc_prefix.h"

__BEGIN_DECLS

int dx_nanosleep(const struct timespec* __request, struct timespec* __remainder);

int dx_usleep(useconds_t useconds);

unsigned int dx_sleep(unsigned int seconds);

__END_DECLS