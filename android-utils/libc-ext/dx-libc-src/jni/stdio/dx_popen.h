#include "dx_stdio_utils.h"
#include "../dx_libc_prefix.h"

__BEGIN_DECLS

int dx_pclose(dxFILE *iop);

dxFILE *dx_popen(const char *command, const char *type);

int dx_fcntl(int fd, int cmd, ...);

int dx_pipe(int pipefd[2]);

__END_DECLS