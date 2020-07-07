
#include <stddef.h>
#include <unistd.h>
#include "dx_string.h"

void* dxmemchr(const void *s, int c, size_t n)
{
	if (n != 0) {
		const unsigned char *p = s;

		do {
			if (*p++ == (unsigned char)c)
				return ((void *)(p - 1));
		} while (--n != 0);
	}
	return (NULL);
}

size_t  dx_strnlen(const char*  str, size_t  maxlen)
{
    char*  p = dxmemchr(str, 0, maxlen);

    if (p == NULL)
        return maxlen;
    else
        return (p - str);
}
