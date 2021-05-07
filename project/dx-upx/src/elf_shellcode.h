#ifndef UPX_ELF_SHELLCODE
#define UPX_ELF_SHELLCODE

#include "elf_utils.h"
#include <iostream>
#include "expand_elf.h"

class elf_shellcode {
public:

    explicit elf_shellcode(const std::string &filename, size_t shellcode_vaddr, size_t shellcode_offset):elf(filename) {
        sign_verify_addr = 0;
        integrity_check_addr = 0;
        anti_dump_addr = 0;
        anti_debug_addr = 0;
        xct_vaddr = 0;
        cur_offset = 0;
        // expand_elf expand(filename, filename);
        // expand.expand();
        xct_vaddr = shellcode_vaddr; //expand.get_xct_vaddr();
        xct_offset = shellcode_offset; //expand.get_xct_offset();
        cur_offset = xct_vaddr;
        ifilename = filename;

        isfirstsec  = true;
        offset_addr = 0;
        base_addr   = this->elf.calc_shellcode_address(0x1000);
        page_size = 0x1000;
    }

    bool add_sign_verify_shellcode(int relhashcode, int reldebug_hashcode);
    bool add_integerity_check_shellcode();
    bool add_anti_dump_shellcode();
    bool add_anti_debugging_shellcode();
    
    bool encrypt_shellcode();
    bool update();

    static void enc_dec_data(void* data, int length);
private:
    // bool add(const std::string &secname, size_t &shellcode_offset, size_t &shellcode_size, unsigned char* sc, size_t len);
    long hook_DT_INIT(size_t new_addr);

    size_t sign_verify_addr;
    size_t integrity_check_addr;
    size_t anti_dump_addr;
    size_t anti_debug_addr;
    size_t xct_vaddr;
    size_t xct_offset;
    size_t cur_offset;

    std::string ifilename;
    bool isfirstsec;
    elf_utils elf;
    size_t offset_addr;
    size_t base_addr;
    size_t page_size;
};

#endif

