#include <string.h>
#include "dx_string.h"

size_t dx_strlen(const char* str){
    const char* s;
    for(s=str; *s; ++s)
        ;
    return (s-str);
}   