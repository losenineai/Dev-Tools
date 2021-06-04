#include <stdbool.h>
#include <stddef.h>
#include <elf.h>
#include <time.h>
#include <pthread.h>

#include "dx_libc_prefix.h"

#if defined(__cplusplus)
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif

__BEGIN_DECLS

struct __dxsFILE;
typedef struct __dxsFILE dxFILE;

int extern dx_errno;
//===========fopen===========
dxFILE* dx_fopen(const char* __path, const char* __mode);

int dx_fclose(dxFILE* __fp);

char* dx_fgets(char* __buf, int __size, dxFILE* __fp);

size_t dx_fread(void* __buf, size_t __size, size_t __count, dxFILE* __fp);

size_t dx_fwrite(const void* __buf, size_t __size, size_t __count, dxFILE* __fp);

int dx_fseek(dxFILE* __fp, long __offset, int __whence);

long dx_ftell(dxFILE* __fp);

int dx_feof(dxFILE* __fp);

int dx_ferror(dxFILE* __fp);

dxFILE* dx_fdopen(int fd, const char* mode);

int dx_fileno(dxFILE* fp);

void dx_rewind(dxFILE* fp);

int dx_fflush(dxFILE* fp);

//===========open===========
int dx_open(const char* pathname, int flags, ...);

int dx_close(int fd);

ssize_t dx_write(int __fd, const void* __buf, size_t __count);;

ssize_t dx_read(int fd, void* buf, size_t count);

off_t dx_lseek(int __fd, off_t __offset, int __whence);


//===========popen===========
dxFILE *dx_popen(const char *command, const char *type);

int dx_pclose(dxFILE *iop);

int dx_execl(const char* name, const char* arg, ...);

pid_t  dx_vfork(void);

int dx_fcntl(int fd, int cmd, ...);

int dx_pipe(int pipefd[2]);


//===========dlopen===========
void *dx_dlopen(const char *libpath, int flags);

int dx_dlclose(void *handle);

void *dx_dlsym(void *handle, const char *name);

bool dx_get_symbols(const char* filepath, char*** symbols, int* p_count);

void *get_module_addr(const char *libpath);

bool isArmOnX86();

#if defined(__arm__) || defined(__i386__)
#define Elf_Ehdr Elf32_Ehdr
#else
#define Elf_Ehdr Elf64_Ehdr
#endif
// extern void get_sym_in_dynamic(Elf_Ehdr *elf, size_t &dynsym, size_t &dynstr, size_t &dynstrsz, size_t &dynsym_fake_end, size_t &loadBias);


//===========string===========
char *dx_strcat(char *s, const char *append);

char *dx_strncat(char *dst, const char *src, size_t n);

int dx_strcmp(const char *s1, const char *s2);

int dx_strncmp(const char *s1, const char *s2, size_t n);

char *dx_strcpy(char *to, const char *from);

char *dx_strncpy(char *dst, const char *src, size_t n);

size_t dx_strlen(const char* str);

char *dx_strstr(const char *s, const char *find);

void* dx_mempcpy(void* dst, const void* src, size_t n);

char* dx_strrchr(const char* p, int ch);


//=============sleep============
int dx_nanosleep(const struct timespec* __request, struct timespec* __remainder);

int dx_usleep(useconds_t useconds);

unsigned int dx_sleep(unsigned int seconds);

//==============sscanf&sprintf==============
int dx_sprintf(char* s, const char* fmt, ...);

int dx_sscanf(const char* s, const char* fmt, ...);


//==============pthread====================
int dx_pthread_create(pthread_t* thread_out, pthread_attr_t const* attr,
                   void* (*start_routine)(void*), void* arg);

int dx_clone(int (*fn)(void*), void* child_stack, int flags, void* arg, ...);

int dx_pthread_kill(pthread_t t, int sig);

int dx_pthread_detach(pthread_t t);

int dx_pthread_join(pthread_t t, void** return_value);


//===============get_properties==============
void init_properties();

int get_property_value(const char *prop_name, char *prop_value, const char* default_value);

void cleanup_resource();

void dump_all();

__END_DECLS