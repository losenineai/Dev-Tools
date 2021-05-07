#include <string.h>
#include <iostream>
#include "elf.h"
#include "elf_utils.h"

#ifndef EXPAND_ELF 
#define EXPAND_ELF 1
class expand_elf{
public:    
    expand_elf(const std::string& input_filename, const std::string& output_filename);
    bool expand();
    bool expand(long expand_size);
    bool remove_so();
    bool replace_so(std::string dst_filename);

    unsigned long get_xct_vaddr();
    unsigned long get_xct_offset();
private:
    unsigned elf32_get_offset_from_address(Elf32_Phdr* phdr, int phnum, unsigned addr);
    unsigned long elf64_get_offset_from_address(Elf64_Phdr* phdr, int phnum, unsigned addr);
    
    void set_uint32(uint8_t* src, uint32_t dst);
    uint32_t get_uint32(uint8_t* src);

    void set_uint64(uint8_t* src, uint64_t dst);
    uint64_t get_uint64(uint8_t* src);

    elf_utils elf;
    std::string i_filename;
    std::string o_filename;
    unsigned long xct_vaddr;
    unsigned long xct_offset;
};


#endif
