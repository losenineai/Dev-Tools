
#ifndef UPX_ELF_UTILS
#define UPX_ELF_UTILS

#include <libelf.h>
#include <iostream>
#include <vector>


enum arch_type {
    ARCH_NONE, ARCH_ARM, ARCH_THUMB, ARCH_THUMB2, ARCH_ARM64, ARCH_X86, ARCH_X86_64
};
enum platform {
    PLATFORM_NONE, PLATFORM_ARM, PLATFORM_ARM64, PLATFORM_X86, PLATFORM_X86_64
};
enum arch_bit {
    ARCH_BIT_NONE, ARCH_BIT_32, ARCH_BIT_64
};

class elf_utils{
private:
    std::string filename;
    uint8_t *file_buffer;
    Elf* elf;
    int fd;
    enum arch_bit mode;
    platform type;
    arch_type arch;
    std::vector<std::string> added_sections;
    std::vector<uint64_t> all_sections_offset;
    std::vector<uint64_t> all_sections_size;
    size_t page_size;
public:
    explicit elf_utils(const std::string& name);
    ~elf_utils();
    void release_fd();
    void release_elf();

    platform get_type(){return type;};
    arch_type get_arch(){return arch;};
    arch_bit get_mode(){return mode;};

    // ELF manipulation
    bool add_section(const std::string &name, uint8_t* content, size_t size, size_t virtual_addr, bool isloadable);
    bool add_section(const std::string &name, uint8_t* content, size_t size, void* pshdr, bool isloadable);
    bool add_section(const std::string &name, uint8_t* content, size_t size, size_t virtual_addr, int align, bool isloadable);
    bool attach_shellcode(size_t target_offset, uint8_t* content, size_t content_len);
    
    void* get_shdr(const std::string & name);
    void* get_shdr(uint8_t* buffer, const std::string & name);
    void* get_shdr(uint8_t* buffer, int index);
    void* get_shdr_by_type(uint8_t* buffer, int type);
    void* get_shdrs(uint8_t* buffer, int& shnum);

    bool update();
    bool fix_section();
    void* get_section(const std::string &name);
    void* get_section(int index);
    char* get_section_name(int nameoff);
    char* get_section_name(uint8_t* buffer, int nameoff);
    size_t add_section_name(const std::string &name);
    void* get_symbol(const std::string &name);
    bool get_needed_libraries(std::vector<std::string> &needlibrarys);
    void* get_phdr(int& phnum);
    void* get_phdr(uint8_t* buffer, int& phnum);

    void* get_pt_load(int n);

    bool update_sections(uint8_t* buffer, const std::string &name, void* pshdr);
    bool update_sections(uint8_t* buffer, int index, void* pshdr);

    // IO
    unsigned char* read(off_t offset, size_t len, size_t &readbytes);
    size_t write(uint8_t *buffer, size_t len, off_t offset);
    bool update_file();
    unsigned char* get_buffer();
    size_t get_file_size();

    // functional
    static off_t search_bytes(const char* buffer, size_t buff_len, const char *str, size_t str_len);

    // hook
    size_t calc_shellcode_address(int align);
    size_t hook(const std::string &funcname, size_t newaddr);
    long get_dt_func_val(int dt_func);   //it will be generated if not
    bool set_dt_func_val(int dt_func, size_t new_addr);
    bool set_dt_type(int src_type, int des_type);

    // to filter so
    bool is_need_library(const std::string &libName);
    bool has_function(const std::string &funcName, bool &bimported);
    
    bool make_function_local(const std::string &name);
private:
    bool update_phdr();
    bool init();
    bool store_all_section_headers();
    bool safety_check();
    bool restore_section_headers();

};

#endif