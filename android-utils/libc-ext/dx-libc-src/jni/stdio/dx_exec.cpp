#include <sys/types.h>
#include <sys/uio.h>

#include <errno.h>
#include <limits.h>
#include <paths.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include "../dx_libc.h"

#include "dx_exec.h"
enum ExecVariant { kIsExecL, kIsExecLE, kIsExecLP };

extern "C" int dx_execve(const char * __file, char * const * __argv, char * const * __envp);

static int __dxexecl(const char* name, const char* argv0, ExecVariant variant, va_list ap) {
  // Count the arguments.
  va_list count_ap;
  va_copy(count_ap, ap);
  size_t n = 1;
  while (va_arg(count_ap, char*) != nullptr) {
    ++n;
  }
  va_end(count_ap);

  // Construct the new argv.
  char* argv[n + 1];
  argv[0] = const_cast<char*>(argv0);
  n = 1;
  while ((argv[n] = va_arg(ap, char*)) != nullptr) {
    ++n;
  }

  // Collect the argp too.
  char** argp = (variant == kIsExecLE) ? va_arg(ap, char**) : environ;

  va_end(ap);

  return (variant == kIsExecLP) ? dx_execvp(name, argv) : dx_execve(name, argv, argp);
}

int dx_execl(const char* name, const char* arg, ...) {
  va_list ap;
  va_start(ap, arg);
  int result = __dxexecl(name, arg, kIsExecL, ap);
  va_end(ap);
  return result;
}

static int __exec_as_script(const char* buf, char* const* argv, char* const* envp) {
  size_t arg_count = 1;
  while (argv[arg_count] != nullptr) ++arg_count;

  const char* script_argv[arg_count + 2];
  script_argv[0] = "sh";
  script_argv[1] = buf;
  memcpy(script_argv + 2, argv + 1, arg_count * sizeof(char*));
  return dx_execve(_PATH_BSHELL, const_cast<char**>(script_argv), envp);
}

int dx_execvpe(const char* name, char* const* argv, char* const* envp) {
  // Do not allow null name.
  if (name == nullptr || *name == '\0') {
    errno = ENOENT;
    return -1;
  }

  // If it's an absolute or relative path name, it's easy.
  if (strchr(name, '/') && dx_execve(name, argv, envp) == -1) {
    if (errno == ENOEXEC) return __exec_as_script(name, argv, envp);
    return -1;
  }

  // Get the path we're searching.
  const char* path = getenv("PATH");
  if (path == nullptr) path = _PATH_DEFPATH;

  // Make a writable copy.
  size_t len = strlen(path) + 1;
  char writable_path[len];
  memcpy(writable_path, path, len);

  bool saw_EACCES = false;

  // Try each element of $PATH in turn...
  char* strsep_buf = writable_path;
  const char* dir;
  while ((dir = strsep(&strsep_buf, ":"))) {
    // It's a shell path: double, leading and trailing colons
    // mean the current directory.
    if (*dir == '\0') dir = const_cast<char*>(".");

    size_t dir_len = strlen(dir);
    size_t name_len = strlen(name);

    char buf[dir_len + 1 + name_len + 1];
    dx_mempcpy(dx_mempcpy(dx_mempcpy(buf, dir, dir_len), "/", 1), name, name_len + 1);

    dx_execve(buf, argv, envp);
    switch (errno) {
    case EISDIR:
    case ELOOP:
    case ENAMETOOLONG:
    case ENOENT:
    case ENOTDIR:
      break;
    case ENOEXEC:
      return __exec_as_script(buf, argv, envp);
    case EACCES:
      saw_EACCES = true;
      break;
    default:
      return -1;
    }
  }
  if (saw_EACCES) errno = EACCES;
  return -1;
}

int dx_execvp(const char* name, char* const* argv) {
  return dx_execvpe(name, argv, environ);
}

class FdPath {
 public:
  explicit FdPath(int fd) {
    snprintf(buf, sizeof(buf), "/proc/self/fd/%d", fd);
  }

  const char* c_str() {
    return buf;
  }

 private:
  char buf[40];
};

int dx_fexecve(int fd, char* const* argv, char* const* envp) {
  // execveat with AT_EMPTY_PATH (>= 3.19) seems to offer no advantages.
  dx_execve(FdPath(fd).c_str(), argv, envp);
  if (errno == ENOENT) errno = EBADF;
  return -1;
}
