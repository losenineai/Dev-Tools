// MIT License

// Copyright (c) 2018 finixbit

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "elf_parser.h"
#include "opcode_util.h"

using namespace elf_parser;

std::vector<section_t> Elf_parser::get_sections(){
    std::vector<section_t> sections;
    if(this->mode == 64){
        Elf64_Ehdr *ehdr = (Elf64_Ehdr*)m_mmap_program;
        Elf64_Shdr *shdr = (Elf64_Shdr*)(m_mmap_program + ehdr->e_shoff);
        int shnum = ehdr->e_shnum;

        Elf64_Shdr *sh_strtab = &shdr[ehdr->e_shstrndx];
        const char *const sh_strtab_p = (char*)m_mmap_program + sh_strtab->sh_offset; 
        for (int i = 0; i < shnum; ++i) {
            section_t section;
            section.section_index= i;
            section.section_name = std::string(sh_strtab_p + shdr[i].sh_name);
            section.section_type = get_section_type(shdr[i].sh_type);
            section.section_addr = shdr[i].sh_addr;
            section.section_offset = shdr[i].sh_offset;
            section.section_size = shdr[i].sh_size;
            section.section_ent_size = shdr[i].sh_entsize;
            section.section_addr_align = shdr[i].sh_addralign; 
        
            sections.push_back(section);
        }
    }else if(this->mode == 32){
        Elf32_Ehdr *ehdr = (Elf32_Ehdr*)m_mmap_program;
        Elf32_Shdr *shdr = (Elf32_Shdr*)(m_mmap_program + ehdr->e_shoff);
        int shnum = ehdr->e_shnum;

        Elf32_Shdr *sh_strtab = &shdr[ehdr->e_shstrndx];
        const char *const sh_strtab_p = (char*)m_mmap_program + sh_strtab->sh_offset; 
        for (int i = 0; i < shnum; ++i) {
            section_t section;
            section.section_index= i;
            section.section_name = std::string(sh_strtab_p + shdr[i].sh_name);
            section.section_type = get_section_type(shdr[i].sh_type);
            section.section_addr = shdr[i].sh_addr;
            section.section_offset = shdr[i].sh_offset;
            section.section_size = shdr[i].sh_size;
            section.section_ent_size = shdr[i].sh_entsize;
            section.section_addr_align = shdr[i].sh_addralign; 
        
            sections.push_back(section);
        }
    }    
    return sections;

}

std::vector<segment_t> Elf_parser::get_segments() {
    std::vector<segment_t> segments;
    if(this->mode == 64){
        Elf64_Ehdr *ehdr = (Elf64_Ehdr*)m_mmap_program;
        Elf64_Phdr *phdr = (Elf64_Phdr*)(m_mmap_program + ehdr->e_phoff);
        int phnum = ehdr->e_phnum;

        Elf64_Shdr *shdr = (Elf64_Shdr*)(m_mmap_program + ehdr->e_shoff);
        Elf64_Shdr *sh_strtab = &shdr[ehdr->e_shstrndx];
        const char *const sh_strtab_p = (char*)m_mmap_program + sh_strtab->sh_offset;

    
        for (int i = 0; i < phnum; ++i) {
            segment_t segment;
            segment.segment_type     = get_segment_type(phdr[i].p_type);
            segment.segment_offset   = phdr[i].p_offset;
            segment.segment_virtaddr = phdr[i].p_vaddr;
            segment.segment_physaddr = phdr[i].p_paddr;
            segment.segment_filesize = phdr[i].p_filesz;
            segment.segment_memsize  = phdr[i].p_memsz;
            segment.segment_flags    = get_segment_flags(phdr[i].p_flags);
            segment.segment_align    = phdr[i].p_align;
        
            segments.push_back(segment);
        }
    }else if(this->mode == 32){
        Elf32_Ehdr *ehdr = (Elf32_Ehdr*)m_mmap_program;
        Elf32_Phdr *phdr = (Elf32_Phdr*)(m_mmap_program + ehdr->e_phoff);
        int phnum = ehdr->e_phnum;

        Elf32_Shdr *shdr = (Elf32_Shdr*)(m_mmap_program + ehdr->e_shoff);
        Elf32_Shdr *sh_strtab = &shdr[ehdr->e_shstrndx];
        const char *const sh_strtab_p = (char*)m_mmap_program + sh_strtab->sh_offset;

    
        for (int i = 0; i < phnum; ++i) {
            segment_t segment;
            segment.segment_type     = get_segment_type(phdr[i].p_type);
            segment.segment_offset   = phdr[i].p_offset;
            segment.segment_virtaddr = phdr[i].p_vaddr;
            segment.segment_physaddr = phdr[i].p_paddr;
            segment.segment_filesize = phdr[i].p_filesz;
            segment.segment_memsize  = phdr[i].p_memsz;
            segment.segment_flags    = get_segment_flags(phdr[i].p_flags);
            segment.segment_align    = phdr[i].p_align;
        
            segments.push_back(segment);
        }
    }
    
    return segments;
}


std::vector<symbol_t> Elf_parser::get_symbols() {
    std::vector<symbol_t> symbols;
    
    if(this->mode == 64){
        std::vector<section_t> secs = get_sections();
        // get headers for offsets
        Elf64_Ehdr *ehdr = (Elf64_Ehdr*)m_mmap_program;
        Elf64_Shdr *shdr = (Elf64_Shdr*)(m_mmap_program + ehdr->e_shoff);

        // get strtab
        char *sh_strtab_p = nullptr;
        for(auto &sec: secs) {
            if((sec.section_type == "SHT_STRTAB") && (sec.section_name == ".strtab")){
                sh_strtab_p = (char*)m_mmap_program + sec.section_offset;
                break;
            }
        }

        // get dynstr
        char *sh_dynstr_p = nullptr;
        for(auto &sec: secs) {
            if((sec.section_type == "SHT_STRTAB") && (sec.section_name == ".dynstr")){
                sh_dynstr_p = (char*)m_mmap_program + sec.section_offset;
                break;
            }
        }

        for(auto &sec: secs) {
            if((sec.section_type != "SHT_SYMTAB") && (sec.section_type != "SHT_DYNSYM"))
                continue;

            auto total_syms = sec.section_size / sizeof(Elf64_Sym);
            auto syms_data = (Elf64_Sym*)(m_mmap_program + sec.section_offset);

            for (int i = 0; i < total_syms; ++i) {
                symbol_t symbol;
                symbol.symbol_num       = i;
                symbol.symbol_value     = syms_data[i].st_value;
                symbol.symbol_size      = syms_data[i].st_size;
                symbol.symbol_type      = get_symbol_type(syms_data[i].st_info);
                symbol.symbol_bind      = get_symbol_bind(syms_data[i].st_info);
                symbol.symbol_visibility= get_symbol_visibility(syms_data[i].st_other);
                symbol.symbol_index     = get_symbol_index(syms_data[i].st_shndx);
                symbol.symbol_section   = sec.section_name;
                
                if(sec.section_type == "SHT_SYMTAB")
                    symbol.symbol_name = std::string(sh_strtab_p + syms_data[i].st_name);
                
                if(sec.section_type == "SHT_DYNSYM")
                    symbol.symbol_name = std::string(sh_dynstr_p + syms_data[i].st_name);
                
                symbols.push_back(symbol);
            }
        }
    }else if(this->mode == 32){
        std::vector<section_t> secs = get_sections();
        // get headers for offsets
        Elf32_Ehdr *ehdr = (Elf32_Ehdr*)m_mmap_program;
        Elf32_Shdr *shdr = (Elf32_Shdr*)(m_mmap_program + ehdr->e_shoff);

        // get strtab
        char *sh_strtab_p = nullptr;
        for(auto &sec: secs) {
            if((sec.section_type == "SHT_STRTAB") && (sec.section_name == ".strtab")){
                sh_strtab_p = (char*)m_mmap_program + sec.section_offset;
                break;
            }
        }

        // get dynstr
        char *sh_dynstr_p = nullptr;
        for(auto &sec: secs) {
            if((sec.section_type == "SHT_STRTAB") && (sec.section_name == ".dynstr")){
                sh_dynstr_p = (char*)m_mmap_program + sec.section_offset;
                break;
            }
        }

        for(auto &sec: secs) {
            if((sec.section_type != "SHT_SYMTAB") && (sec.section_type != "SHT_DYNSYM"))
                continue;

            auto total_syms = sec.section_size / sizeof(Elf32_Sym);
            auto syms_data = (Elf32_Sym*)(m_mmap_program + sec.section_offset);

            for (int i = 0; i < total_syms; ++i) {
                symbol_t symbol;
                symbol.symbol_num       = i;
                symbol.symbol_value     = syms_data[i].st_value;
                symbol.symbol_size      = syms_data[i].st_size;
                symbol.symbol_type      = get_symbol_type(syms_data[i].st_info);
                symbol.symbol_bind      = get_symbol_bind(syms_data[i].st_info);
                symbol.symbol_visibility= get_symbol_visibility(syms_data[i].st_other);
                symbol.symbol_index     = get_symbol_index(syms_data[i].st_shndx);
                symbol.symbol_section   = sec.section_name;
                
                if(sec.section_type == "SHT_SYMTAB")
                    symbol.symbol_name = std::string(sh_strtab_p + syms_data[i].st_name);
                
                if(sec.section_type == "SHT_DYNSYM")
                    symbol.symbol_name = std::string(sh_dynstr_p + syms_data[i].st_name);
                
                symbols.push_back(symbol);
            }
        }
    }
    return symbols;
}


uint64_t Elf_parser::getSymbolOffset(std::string symbolname){
    uint64_t off = 0;
    std::vector<symbol_t> symbols = this->get_symbols();
    for (auto &symbol : symbols) {
        if(symbol.symbol_name == symbolname ){
            off = symbol.symbol_value - (symbol.symbol_value%2);
            break;
        }
    }
    return off;
}

unsigned char* Elf_parser::get_shellcode(uint64_t startoff, uint64_t endoff){
    unsigned char* shellcode = (unsigned char*)malloc((endoff - startoff + 1));
    for(int i = startoff; i<endoff; i++){
        shellcode[i-startoff] = (unsigned char)(this->m_mmap_program[i]);  
    }
    shellcode[endoff] = 0x00;
    return shellcode;
};

bool Elf_parser::get_signature_shellcode(std::string filename, uint64_t startoff, uint64_t endoff){
    if(this->type == X86){  //move function '__x86_get_pc_thunk_bx' after endoff, then reset the call address.
        opcodeutil oputil(this->m_mmap_program);
        uint8_t* callee = NULL;
        int len = 0;
        off_t addr1 = this->getSymbolOffset("JNI_OnLoad");
        off_t addr2 = this->getSymbolOffset("checkSignature");
        off_t addr3 = this->getSymbolOffset("dx_libc_call");
        off_t addr4 = this->getSymbolOffset("get_function");

        off_t callee1 = oputil.getCalleeAddr(addr1);
        off_t callee2 = oputil.getCalleeAddr(addr2);
        off_t callee3 = oputil.getCalleeAddr(addr3);
        off_t callee4 = oputil.getCalleeAddr(addr4);

        if(callee1 != callee2){
            LOGE("callee1:%lld != callee2:%lld", callee1, callee2);
            return false;
        }
        if(oputil.getCallee(callee1, callee, len)){
            oputil.setCalleeAddr(addr1, endoff);
            oputil.setCalleeAddr(addr2, endoff);
            oputil.setCalleeAddr(addr3, endoff);
            oputil.setCalleeAddr(addr4, endoff);
        }

        FILE* file = fopen(filename.c_str(), "w");
        int eachlinenum = 16;
        int count = 0;
        for(int i=startoff; i<endoff; i++){
            count ++;
            fprintf(file, " 0x%02x,", (unsigned char)(this->m_mmap_program[i]));
            if(count == eachlinenum){
                fprintf(file, " \n ");
                count = 0;
            }
        }
        for(int i=0; i<len; i++){
            fprintf(file, " 0x%02x, ", callee[i]);
        }
        fprintf(file, " 0x00, 0x00, 0x00, 0x00");
        fclose(file);
        return true;
    }else{

        FILE* file = fopen(filename.c_str(), "w");
        int eachlinenum = 16;
        int count = 0;
        for(int i=startoff; i<endoff; i++){
            count ++;
            fprintf(file, " 0x%02x,", (unsigned char)(this->m_mmap_program[i]));
            if(count == eachlinenum){
                fprintf(file, " \n ");
                count = 0;
            }
        }
        fprintf(file, " 0x00, 0x00, 0x00, 0x00");
        fclose(file);
        return true;

    }
    return true;
}
bool Elf_parser::get_integrity_shellcode(std::string filename, uint64_t startoff, uint64_t endoff){
    if(this->type == X86){  //move function '__x86_get_pc_thunk_bx' after endoff, then reset the call address.
        opcodeutil oputil(this->m_mmap_program);
        uint8_t* callee = NULL;
        int len = 0;
        off_t addr1 = this->getSymbolOffset("dx_init");
        off_t addr2 = this->getSymbolOffset("dx_libc_call");
        off_t addr3 = this->getSymbolOffset("checkIntegrity");
        off_t addr4 = this->getSymbolOffset("find_uchar");
        off_t addr5 = this->getSymbolOffset("get_function");

        off_t callee1 = oputil.getCalleeAddr(addr1);
        off_t callee2 = oputil.getCalleeAddr(addr2);
        off_t callee3 = oputil.getCalleeAddr(addr3);
        off_t callee4 = oputil.getCalleeAddr(addr4);
        off_t callee5 = oputil.getCalleeAddr(addr5);

        if(callee1 != callee2){
            LOGE("callee1:%lld != callee2:%lld", callee1, callee2);
            return false;
        }
        if(oputil.getCallee(callee1, callee, len)){
            oputil.setCalleeAddr(addr1, endoff);
            oputil.setCalleeAddr(addr2, endoff);
            oputil.setCalleeAddr(addr3, endoff);
            oputil.setCalleeAddr(addr4, endoff);
            oputil.setCalleeAddr(addr5, endoff);
        }

        FILE* file = fopen(filename.c_str(), "w");
        int eachlinenum = 16;
        int count = 0;
        for(int i=startoff; i<endoff; i++){
            count ++;
            fprintf(file, " 0x%02x,", (unsigned char)(this->m_mmap_program[i]));
            if(count == eachlinenum){
                fprintf(file, " \n ");
                count = 0;
            }
        }
        for(int i=0; i<len; i++){
            fprintf(file, " 0x%02x, ", callee[i]);
        }
        fprintf(file, " 0x00, 0x00, 0x00, 0x00");
        fclose(file);
        return true;
    }else{

        FILE* file = fopen(filename.c_str(), "w");
        int eachlinenum = 16;
        int count = 0;
        for(int i=startoff; i<endoff; i++){
            count ++;
            fprintf(file, " 0x%02x,", (unsigned char)(this->m_mmap_program[i]));
            if(count == eachlinenum){
                fprintf(file, " \n ");
                count = 0;
            }
        }
        fprintf(file, " 0x00, 0x00, 0x00, 0x00");
        fclose(file);
        return true;

    }
    return true;
}


bool Elf_parser::get_erase_shellcode(std::string filename, uint64_t startoff, uint64_t endoff){
        if(this->type == X86){  //move function '__x86_get_pc_thunk_bx' after endoff, then reset the call address.
        opcodeutil oputil(this->m_mmap_program);
        uint8_t* callee = NULL;
        int len = 0;
        off_t addr1 = this->getSymbolOffset("erase_phdrs");
        off_t addr2 = this->getSymbolOffset("dx_liblog_call");
        off_t callee2 = 0;
        if(addr2 != 0){
            callee2 = oputil.getCalleeAddr(addr2);
        }
        off_t callee1 = oputil.getCalleeAddr(addr1);
        
        if(callee1 <= 0){
            LOGE("callee1:%lld error...", callee1);
            return false;
        }
        if(oputil.getCallee(callee1, callee, len)){
            oputil.setCalleeAddr(addr1, endoff);
            if(callee2 != 0){
                oputil.setCalleeAddr(addr2, endoff);
            }
        }

        FILE* file = fopen(filename.c_str(), "w");
        int eachlinenum = 16;
        int count = 0;
        for(int i=startoff; i<endoff; i++){
            count ++;
            fprintf(file, " 0x%02x,", (unsigned char)(this->m_mmap_program[i]));
            if(count == eachlinenum){
                fprintf(file, " \n ");
                count = 0;
            }
        }
        for(int i=0; i<len; i++){
            fprintf(file, " 0x%02x, ", callee[i]);
        }
        fprintf(file, " 0x00, 0x00, 0x00, 0x00");
        fclose(file);
        return true;
    }else{

        FILE* file = fopen(filename.c_str(), "w");
        int eachlinenum = 16;
        int count = 0;
        for(int i=startoff; i<endoff; i++){
            count ++;
            fprintf(file, " 0x%02x,", (unsigned char)(this->m_mmap_program[i]));
            if(count == eachlinenum){
                fprintf(file, " \n ");
                count = 0;
            }
        }
        fprintf(file, " 0x00, 0x00, 0x00, 0x00");
        fclose(file);
        return true;

    }
    return true;
}

bool Elf_parser::get_antidebug_shellcode(std::string filename, uint64_t startoff, uint64_t endoff){
        if(this->type == X86){  //move function '__x86_get_pc_thunk_bx' after endoff, then reset the call address.
        opcodeutil oputil(this->m_mmap_program);
        uint8_t* callee = NULL;
        int len = 0;
        off_t addr1 = this->getSymbolOffset("dx_init");
        off_t addr2 = this->getSymbolOffset("dx_libc_call");
        off_t addr3 = this->getSymbolOffset("antidebug");
        off_t addr4 = this->getSymbolOffset("get_function");

        off_t callee1 = oputil.getCalleeAddr(addr1+8);
        off_t callee2 = oputil.getCalleeAddr(addr2);
        off_t callee3 = oputil.getCalleeAddr(addr3);
        off_t callee4 = oputil.getCalleeAddr(addr4);
        if(callee1 != callee2){
            LOGE("callee1:%lld != callee2:%lld", callee1, callee2);
            return false;
        }
        if(oputil.getCallee(callee1, callee, len)){
            
            oputil.setCalleeAddr(addr1+8, endoff);
            oputil.setCalleeAddr(addr2, endoff);
            oputil.setCalleeAddr(addr3, endoff);
            oputil.setCalleeAddr(addr4, endoff);
        }

        FILE* file = fopen(filename.c_str(), "w");
        int eachlinenum = 16;
        int count = 0;
        for(int i=startoff; i<endoff; i++){
            count ++;
            fprintf(file, " 0x%02x,", (unsigned char)(this->m_mmap_program[i]));
            if(count == eachlinenum){
                fprintf(file, " \n ");
                count = 0;
            }
        }
        for(int i=0; i<len; i++){
            fprintf(file, " 0x%02x, ", callee[i]);
        }
        fprintf(file, " 0x00, 0x00, 0x00, 0x00");
        fclose(file);
        return true;
    }else{

        FILE* file = fopen(filename.c_str(), "w");
        int eachlinenum = 16;
        int count = 0;
        for(int i=startoff; i<endoff; i++){
            count ++;
            fprintf(file, " 0x%02x,", (unsigned char)(this->m_mmap_program[i]));
            if(count == eachlinenum){
                fprintf(file, " \n ");
                count = 0;
            }
        }
        fprintf(file, " 0x00, 0x00, 0x00, 0x00");
        fclose(file);
        return true;

    }
    return true;
}

bool Elf_parser::get_shellcode(std::string filename, uint64_t startoff, uint64_t endoff){

    if(this->type == X86){  //move function '__x86_get_pc_thunk_bx' after endoff, then reset the call address.
        opcodeutil oputil(this->m_mmap_program);
        uint8_t* callee = NULL;
        int len = 0;
        off_t addr1 = this->getSymbolOffset("dx_init");
        off_t addr2 = this->getSymbolOffset("dx_libc_call");
        off_t addr3 = this->getSymbolOffset("checkIntegrity");
        off_t addr4 = this->getSymbolOffset("find_uchar");

        off_t callee1 = oputil.getCalleeAddr(addr1);
        off_t callee2 = oputil.getCalleeAddr(addr2);
        off_t callee3 = oputil.getCalleeAddr(addr3);
        off_t callee4 = oputil.getCalleeAddr(addr4);
        if(callee1 != callee2){
            LOGE("callee1:%lld != callee2:%lld", callee1, callee2);
            return false;
        }
        if(oputil.getCallee(callee1, callee, len)){
            
            oputil.setCalleeAddr(addr1, endoff);
            oputil.setCalleeAddr(addr2, endoff);
            oputil.setCalleeAddr(addr3, endoff);
            oputil.setCalleeAddr(addr4, endoff);
        }

        FILE* file = fopen(filename.c_str(), "w");
        int eachlinenum = 16;
        int count = 0;
        for(int i=startoff; i<endoff; i++){
            count ++;
            fprintf(file, " 0x%02x,", (unsigned char)(this->m_mmap_program[i]));
            if(count == eachlinenum){
                fprintf(file, " \n ");
                count = 0;
            }
        }
        for(int i=0; i<len; i++){
            fprintf(file, " 0x%02x, ", callee[i]);
        }
        fprintf(file, " 0x00, 0x00, 0x00, 0x00");
        fclose(file);
        return true;
    }else{

        FILE* file = fopen(filename.c_str(), "w");
        int eachlinenum = 16;
        int count = 0;
        for(int i=startoff; i<endoff; i++){
            count ++;
            fprintf(file, " 0x%02x,", (unsigned char)(this->m_mmap_program[i]));
            if(count == eachlinenum){
                fprintf(file, " \n ");
                count = 0;
            }
        }
        fprintf(file, " 0x00, 0x00, 0x00, 0x00");
        fclose(file);
        return true;

    }


}

std::vector<relocation_t> Elf_parser::get_relocations() {
    std::vector<relocation_t> relocations;
    if(this->mode == 64){
        auto secs = get_sections();
        auto syms = get_symbols();
    
        int  plt_entry_size = 0;
        long plt_vma_address = 0;
        for (auto &sec : secs) {
            if(sec.section_name == ".plt") {
            plt_entry_size = sec.section_ent_size;
            plt_vma_address = sec.section_addr;
            break;
            }
        }

        for (auto &sec : secs) {

            if(sec.section_type != "SHT_RELA") 
                continue;

            auto total_relas = sec.section_size / sizeof(Elf64_Rela);
            auto relas_data  = (Elf64_Rela*)(m_mmap_program + sec.section_offset);

            for (int i = 0; i < total_relas; ++i) {
                relocation_t rel;
                rel.relocation_offset = static_cast<std::intptr_t>(relas_data[i].r_offset);
                rel.relocation_info   = static_cast<std::intptr_t>(relas_data[i].r_info);
                rel.relocation_type   = \
                    get_relocation_type(relas_data[i].r_info);
                
                rel.relocation_symbol_value = \
                    get_rel_symbol_value(relas_data[i].r_info, syms);
                
                rel.relocation_symbol_name  = \
                    get_rel_symbol_name(relas_data[i].r_info, syms);
                
                rel.relocation_plt_address = plt_vma_address + (i + 1) * plt_entry_size;
                rel.relocation_section_name = sec.section_name;
                
                relocations.push_back(rel);
            }
        }
    }else if(this->mode == 32){
        auto secs = get_sections();
        auto syms = get_symbols();
    
        int  plt_entry_size = 0;
        long plt_vma_address = 0;
        for (auto &sec : secs) {
            if(sec.section_name == ".plt") {
            plt_entry_size = sec.section_ent_size;
            plt_vma_address = sec.section_addr;
            break;
            }
        }

        for (auto &sec : secs) {

            if(sec.section_type != "SHT_RELA") 
                continue;

            auto total_relas = sec.section_size / sizeof(Elf64_Rela);
            auto relas_data  = (Elf64_Rela*)(m_mmap_program + sec.section_offset);

            for (int i = 0; i < total_relas; ++i) {
                relocation_t rel;
                rel.relocation_offset = static_cast<std::intptr_t>(relas_data[i].r_offset);
                rel.relocation_info   = static_cast<std::intptr_t>(relas_data[i].r_info);
                rel.relocation_type   = \
                    get_relocation_type(relas_data[i].r_info);
                
                rel.relocation_symbol_value = \
                    get_rel_symbol_value(relas_data[i].r_info, syms);
                
                rel.relocation_symbol_name  = \
                    get_rel_symbol_name(relas_data[i].r_info, syms);
                
                rel.relocation_plt_address = plt_vma_address + (i + 1) * plt_entry_size;
                rel.relocation_section_name = sec.section_name;
                
                relocations.push_back(rel);
            }
        }
    }
    
    return relocations;
}

uint8_t *Elf_parser::get_memory_map() {
    return m_mmap_program;
}

void Elf_parser::load_memory_map() {
    int fd, i;
    struct stat st;

    if ((fd = open(m_program_path.c_str(), O_RDONLY)) < 0) {
        LOGE("Err: open");
        exit(-1);
    }
    if (fstat(fd, &st) < 0) {
        LOGE("Err: fstat");
        exit(-1);
    }
    m_mmap_program = (uint8_t*)(mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0));
    if (m_mmap_program == MAP_FAILED) {
        LOGE("Err: mmap");
        exit(-1);
    }

    auto header64 = (Elf64_Ehdr*)m_mmap_program;
    auto header32 = (Elf32_Ehdr*)m_mmap_program;
    
    if (header64->e_ident[EI_CLASS] == ELFCLASS64) {
        LOGD("64bit");
        this->mode = 64;
        if (header64->e_machine == EM_AARCH64){
            LOGD("AARCH64");
            this->type = ARM_64;
        }else if(header64->e_machine == EM_X86_64){
            LOGD("X86_64");
            this->type = X86_64;
        }else{
            LOGD("no support platform");
            this->type = 0;
        }
        
    }else if(header32->e_ident[EI_CLASS] == ELFCLASS32){
        LOGD("32bit");
        this->mode = 32;
        if(header32->e_machine == EM_ARM){
            LOGD("ARM");
            this->type = ARM;
        }else if(header32->e_machine == EM_386){
            LOGD("X8086");
            this->type = X86;
        }else{
            LOGD("no support platform");
            this->type = 0;
        }
    }else{
        LOGD("Only 64-bit / 32bit files supported");
        exit(1);
    }
}

std::string Elf_parser::get_section_type(int tt) {
    if(tt < 0)
        return "UNKNOWN";

    switch(tt) {
        case 0: return "SHT_NULL";      /* Section header table entry unused */
        case 1: return "SHT_PROGBITS";  /* Program data */
        case 2: return "SHT_SYMTAB";    /* Symbol table */
        case 3: return "SHT_STRTAB";    /* String table */
        case 4: return "SHT_RELA";      /* Relocation entries with addends */
        case 5: return "SHT_HASH";      /* Symbol hash table */
        case 6: return "SHT_DYNAMIC";   /* Dynamic linking information */
        case 7: return "SHT_NOTE";      /* Notes */
        case 8: return "SHT_NOBITS";    /* Program space with no data (bss) */
        case 9: return "SHT_REL";       /* Relocation entries, no addends */
        case 11: return "SHT_DYNSYM";   /* Dynamic linker symbol table */
        default: return "UNKNOWN";
    }
    return "UNKNOWN";
}

std::string Elf_parser::get_segment_type(uint32_t &seg_type) {
    switch(seg_type) {
        case PT_NULL:   return "NULL";                  /* Program header table entry unused */ 
        case PT_LOAD: return "LOAD";                    /* Loadable program segment */
        case PT_DYNAMIC: return "DYNAMIC";              /* Dynamic linking information */
        case PT_INTERP: return "INTERP";                /* Program interpreter */
        case PT_NOTE: return "NOTE";                    /* Auxiliary information */
        case PT_SHLIB: return "SHLIB";                  /* Reserved */
        case PT_PHDR: return "PHDR";                    /* Entry for header table itself */
        case PT_TLS: return "TLS";                      /* Thread-local storage segment */
        case PT_NUM: return "NUM";                      /* Number of defined types */
        case PT_LOOS: return "LOOS";                    /* Start of OS-specific */
        case PT_GNU_EH_FRAME: return "GNU_EH_FRAME";    /* GCC .eh_frame_hdr segment */
        case PT_GNU_STACK: return "GNU_STACK";          /* Indicates stack executability */
        case PT_GNU_RELRO: return "GNU_RELRO";          /* Read-only after relocation */
        //case PT_LOSUNW: return "LOSUNW";
        case PT_SUNWBSS: return "SUNWBSS";              /* Sun Specific segment */
        case PT_SUNWSTACK: return "SUNWSTACK";          /* Stack segment */
        //case PT_HISUNW: return "HISUNW";
        case PT_HIOS: return "HIOS";                    /* End of OS-specific */
        case PT_LOPROC: return "LOPROC";                /* Start of processor-specific */
        case PT_HIPROC: return "HIPROC";                /* End of processor-specific */
        default: return "UNKNOWN";
    }
}

std::string Elf_parser::get_segment_flags(uint32_t &seg_flags) {
    std::string flags;

    if(seg_flags & PF_R)
        flags.append("R");

    if(seg_flags & PF_W)
        flags.append("W");

    if(seg_flags & PF_X)
        flags.append("E");

    return flags;
}

std::string Elf_parser::get_symbol_type(uint8_t &sym_type) {
    switch(ELF32_ST_TYPE(sym_type)) {
        case 0: return "NOTYPE";
        case 1: return "OBJECT";
        case 2: return "FUNC";
        case 3: return "SECTION";
        case 4: return "FILE";
        case 6: return "TLS";
        case 7: return "NUM";
        case 10: return "LOOS";
        case 12: return "HIOS";
        default: return "UNKNOWN";
    }
}

std::string Elf_parser::get_symbol_bind(uint8_t &sym_bind) {
    switch(ELF32_ST_BIND(sym_bind)) {
        case 0: return "LOCAL";
        case 1: return "GLOBAL";
        case 2: return "WEAK";
        case 3: return "NUM";
        case 10: return "UNIQUE";
        case 12: return "HIOS";
        case 13: return "LOPROC";
        default: return "UNKNOWN";
    }
}

std::string Elf_parser::get_symbol_visibility(uint8_t &sym_vis) {
    switch(ELF32_ST_VISIBILITY(sym_vis)) {
        case 0: return "DEFAULT";
        case 1: return "INTERNAL";
        case 2: return "HIDDEN";
        case 3: return "PROTECTED";
        default: return "UNKNOWN";
    }
}

std::string Elf_parser::get_symbol_index(uint16_t &sym_idx) {
    switch(sym_idx) {
        case SHN_ABS: return "ABS";
        case SHN_COMMON: return "COM";
        case SHN_UNDEF: return "UND";
        case SHN_XINDEX: return "COM";
        default: return std::to_string(sym_idx);
    }
}

std::string Elf_parser::get_relocation_type(uint64_t &rela_type) {
    switch(ELF64_R_TYPE(rela_type)) {
        case 1: return "R_X86_64_32";
        case 2: return "R_X86_64_PC32";
        case 5: return "R_X86_64_COPY";
        case 6: return "R_X86_64_GLOB_DAT";
        case 7:  return "R_X86_64_JUMP_SLOT";
        default: return "OTHERS";
    }
}


std::intptr_t Elf_parser::get_rel_symbol_value(
                uint64_t &sym_idx, std::vector<symbol_t> &syms) {
    std::intptr_t sym_val = 0;
    if(this->mode == 64){
        for(auto &sym: syms) {
            if(sym.symbol_num == ELF64_R_SYM(sym_idx)) {
                sym_val = sym.symbol_value;
                break;
            }
        }
    }else if(this->mode == 32){  
        for(auto &sym: syms) {
            if(sym.symbol_num == ELF32_R_SYM(sym_idx)) {
                sym_val = sym.symbol_value;
                break;
            }
        }
    }
    return sym_val;
}

std::string Elf_parser::get_rel_symbol_name(uint64_t &sym_idx, std::vector<symbol_t> &syms) {

    std::string sym_name;
    if(this->mode == 64){
        for(auto &sym: syms) {
            if(sym.symbol_num == ELF64_R_SYM(sym_idx)) {
                sym_name = sym.symbol_name;
                break;
            }
        }
    }else if(this->mode == 32){
        for(auto &sym: syms) {
            if(sym.symbol_num == ELF32_R_SYM(sym_idx)) {
                sym_name = sym.symbol_name;
                break;
            }
        }

    }
    return sym_name;
}

