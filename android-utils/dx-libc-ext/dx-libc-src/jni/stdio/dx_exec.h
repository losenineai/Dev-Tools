
#include "../dx_libc_prefix.h"

__BEGIN_DECLS

int dx_execl(const char* name, const char* arg, ...);

int dx_execvpe(const char* name, char* const* argv, char* const* envp);

int dx_execvp(const char* name, char* const* argv);

int dx_fexecve(int fd, char* const* argv, char* const* envp);

pid_t  dx_vfork(void);

__END_DECLS