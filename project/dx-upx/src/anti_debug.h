#ifndef _UPX_ANTI_DEBUG
#define _UPX_ANTI_DEBUG
#include "elf_utils.h"
#include<iostream>

class anti_debug {
private:
    std::string filename;
    elf_utils elf;
    std::vector<std::string> section_names;

public:
    explicit anti_debug(const std::string &fname):filename(fname), elf(fname){};

    bool add_empty_sections(int n);
    bool anti_ida();
    bool fix_elf();
    bool hide_symbols();

};

#endif
