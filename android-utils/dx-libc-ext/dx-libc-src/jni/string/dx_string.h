#include<stdio.h>
#include "../dx_libc_prefix.h"

__BEGIN_DECLS
void* dx_mempcpy(void* dst, const void* src, size_t n);

size_t dx_strlen(const char* str);

char *dx_strcat(char *s, const char *append);

int dx_strcmp(const char *s1, const char *s2);

char *dx_strcpy(char *to, const char *from);

char *dx_strncat(char *dst, const char *src, size_t n);

int dx_strncmp(const char *s1, const char *s2, size_t n);

char *dx_strstr(const char *s, const char *find);

char* dx_strrchr(const char* p, int ch);

size_t  dx_strnlen(const char*  str, size_t  maxlen);

char* dx_strncpy(char *dst, const char *src, size_t n);

__END_DECLS