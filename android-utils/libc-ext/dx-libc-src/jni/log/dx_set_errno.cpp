#include <errno.h>
#include "../dx_libc_prefix.h"
#include "../dx_libc.h"

int dx_errno;

extern "C" __LIBC_HIDDEN__ long __set_errno_internal(int n) {
  dx_errno = n;
  return -1;
}

