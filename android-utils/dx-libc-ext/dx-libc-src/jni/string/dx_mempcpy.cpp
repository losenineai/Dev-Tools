#include <string.h>
#include "dx_string.h"

__BEGIN_DECLS
void* dx_mempcpy(void* dst, const void* src, size_t n) {
  return reinterpret_cast<char*>(memcpy(dst, src, n)) + n;
}
__END_DECLS