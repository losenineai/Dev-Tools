#include <stddef.h>
#include <unistd.h>
#include "dx_string.h"
#include "../log/bionic_fortify.h"
#define __BIONIC_FORTIFY_UNKNOWN_SIZE ((size_t) -1)

char* __dxstrrchr_chk(const char* p, int ch, size_t s_len) {
  for (const char* save = NULL;; ++p, s_len--) {
    if (s_len == 0) {
      __fortify_fatal("strrchr: prevented read past end of buffer");
    }
    if (*p == static_cast<char>(ch)) {
      save = p;
    }
    if (!*p) {
      return const_cast<char*>(save);
    }
  }
}

char* dx_strrchr(const char* p, int ch) {
  return __dxstrrchr_chk(p, ch, __BIONIC_FORTIFY_UNKNOWN_SIZE);
}
