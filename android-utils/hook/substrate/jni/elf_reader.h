#ifndef _ELF_READER_H
#define _ELF_READER_H

#include <unistd.h>

class IElfReader {
public:
    virtual bool valid() = 0;
    virtual uintptr_t get_symbol_size(uintptr_t rva) = 0;
};

IElfReader *IElfReader_new(const char *filepath);
void IElfReader_delete(IElfReader *&p);

#endif // _ELF_READER_H
