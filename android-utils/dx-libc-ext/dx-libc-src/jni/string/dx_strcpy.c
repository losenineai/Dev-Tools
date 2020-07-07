#include <string.h>
#include "dx_string.h"

char * dx_strcpy(char *to, const char *from)
{
	char *save = to;

	for (; (*to = *from) != '\0'; ++from, ++to);
	return(save);
}
