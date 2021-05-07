#include <iostream>
#include "elf_parser.h"
#include "opcode_util.h"
#include <stdio.h>
#include <unistd.h>

void print_symbols(std::vector<elf_parser::symbol_t> &symbols) {
    std::cout <<"print symbols : " <<std::endl;
    for (auto &symbol : symbols) {
        std::cout <<symbol.symbol_name << "  vaddr:0x" <<std::hex<<symbol.symbol_value <<std::endl;
    }
}

void print_sections(std::vector<elf_parser::section_t> &sections){
    for(auto &section : sections){
        std::cout << section.section_name << "  vaddr:0x" << std::hex <<section.section_offset << "  size:0x" << section.section_size << std::endl;
    }
}


int main(int argc, char* argv[]){
    int ch, destConsole = 0;
    int def = 0;
    char* argbuf;
    int kind = 0;
    opterr = 0;
    while((ch = getopt(argc, argv, "f:siea")) != -1){
        switch(ch){
            case 'f':
                argbuf = optarg;
                break;
            case 's':
                kind = 1;
                LOGD("signature shellcode extracting...");
                break;
            case 'i':
                kind = 2;
                LOGD("integrity shellcode extracting...");
                break;
            case 'e':
                kind = 3;
                LOGD("phdrs erases shellcode extracting...");
                break;
            case 'a':
                kind = 4;
                LOGD("anti debug shellcode extracting...");
                break;
            default:
                LOGD("usage : ./bin2hex -[s i e a] -f [file_path]");
                LOGD("-s : extract signature-check shellcode");
                LOGD("-i : extract integrity-check shellcode");
                LOGD("-e : extract erase-phdrs shellcode");
                LOGD("-a : extract anti-debug shellcode");
                break;
        }
    }
    
    if(kind == 1){
        //signature check shellcode extracting
        //integrity check shellcode extracting
        std::string program(argbuf);
        elf_parser::Elf_parser myelf_parser(program);

        uint8_t* buf = myelf_parser.get_memory_map();
        opcodeutil oputil(buf);

    
        int startoff = 0;
        int endoff = 0;

        if(myelf_parser.get_type() == ARM_64){
            
            startoff = myelf_parser.getSymbolOffset("JNI_OnLoad");
            endoff = myelf_parser.getSymbolOffset("DX_ShellcodeEnd");

        }else if(myelf_parser.get_type() == ARM){
            
            startoff = myelf_parser.getSymbolOffset("JNI_OnLoad");
            
            std::vector<elf_parser::section_t> sections = myelf_parser.get_sections();
            for(auto &section : sections){
                if(section.section_name == ".rodata"){
                    endoff = section.section_offset + section.section_size;
                }
            }
            LOGI("endoff:%d", endoff);
            
        }else if(myelf_parser.get_type() == X86){

            startoff = myelf_parser.getSymbolOffset("JNI_OnLoad");
            LOGD("startoff:%d", startoff);

            std::vector<elf_parser::section_t> sections = myelf_parser.get_sections();
            for(auto &section : sections){
                if(section.section_name == ".rodata"){
                    endoff = section.section_offset + section.section_size;
                }
            }
            if(endoff == 0){
                endoff = myelf_parser.getSymbolOffset("DX_ShellcodeEnd");
            }
            LOGD("endoff:%d", endoff);
        
        }

        myelf_parser.get_signature_shellcode(std::string("hexcode.dump"), startoff, endoff);

        int jniOnLoadOff = myelf_parser.getSymbolOffset("JNI_OnLoad");
        int checkSignatureOff = myelf_parser.getSymbolOffset("checkSignature");
        int dx_syscallOff = myelf_parser.getSymbolOffset("dx_syscall");

        int rel_onload2syscall = dx_syscallOff - jniOnLoadOff;
        int rel_onload2checksign = checkSignatureOff - jniOnLoadOff;
        
        std::cout<<"rel_onload2syscall:0x"<<std::hex<<rel_onload2syscall<<std::endl;
        std::cout<<"rel_onload2checksign:0x"<<std::hex<<rel_onload2checksign<<std::endl;



    }else if(kind == 2){
        //integrity check shellcode extracting
        std::string program(argbuf);
        elf_parser::Elf_parser myelf_parser(program);

        uint8_t* buf = myelf_parser.get_memory_map();
        opcodeutil oputil(buf);

    
        int startoff = 0;
        int endoff = 0;

        if(myelf_parser.get_type() == ARM_64){
            printf("arm64");

            startoff = myelf_parser.getSymbolOffset("dx_init");
            endoff = myelf_parser.getSymbolOffset("DX_ShellcodeEnd");

        }else if(myelf_parser.get_type() == ARM){
            printf("arm");
            startoff = myelf_parser.getSymbolOffset("dx_init");
            
            std::vector<elf_parser::section_t> sections = myelf_parser.get_sections();
            for(auto &section : sections){
                if(section.section_name == ".rodata"){
                    endoff = section.section_offset + section.section_size;
                }
            }
            
        }else if(myelf_parser.get_type() == X86){
            printf("x86\n");
            startoff = myelf_parser.getSymbolOffset("dx_init");
            std::vector<elf_parser::section_t> sections = myelf_parser.get_sections();
            for(auto &section : sections){
                if(section.section_name == ".rodata"){
                    endoff = section.section_offset + section.section_size;
                }
            }

            if(endoff == 0){
                endoff = myelf_parser.getSymbolOffset("DX_ShellcodeEnd");
            }
            LOGD("endoff:%d", endoff);
        
        }

        myelf_parser.get_integrity_shellcode(std::string("hexcode.dump"), startoff, endoff);

        int dxInitOff = myelf_parser.getSymbolOffset("dx_init");
        int checkIntegrityOff = myelf_parser.getSymbolOffset("checkIntegrity");
        int dx_syscallOff = myelf_parser.getSymbolOffset("dx_syscall");


        int relCurfun2Syscall = dx_syscallOff - dxInitOff;
        int relCurfun2IntegrityCheck = checkIntegrityOff - dxInitOff;
        
        std::cout<<"relCurfun2Syscall:0x"<<std::hex<<relCurfun2Syscall<<std::endl;
        std::cout<<"relCurfun2IntegrityCheck:0x"<<std::hex<<relCurfun2IntegrityCheck<<std::endl;
    }else if(kind == 3){
        //erase phdrs shellcode extracting
        std::string program(argbuf);
        elf_parser::Elf_parser myelf_parser(program);

        uint8_t* buf = myelf_parser.get_memory_map();
        opcodeutil oputil(buf);

        int startoff = 0;
        int endoff = 0;

        if(myelf_parser.get_type() == ARM_64){
            printf("arm64");
            
            startoff = myelf_parser.getSymbolOffset("erase_phdrs");
            endoff = myelf_parser.getSymbolOffset("DX_ShellcodeEnd");
            
        }else if(myelf_parser.get_type() == ARM){
            printf("arm");
            startoff = myelf_parser.getSymbolOffset("erase_phdrs");
          
            std::vector<elf_parser::section_t> sections = myelf_parser.get_sections();
            for(auto &section : sections){
                if(section.section_name == ".rodata"){
                    endoff = section.section_offset + section.section_size;
                }
            }
            if(endoff == 0){
                endoff = myelf_parser.getSymbolOffset("DX_ShellcodeEnd");
            }
        }else if(myelf_parser.get_type() == X86){
            printf("x86\n");
            startoff = myelf_parser.getSymbolOffset("erase_phdrs");
            
            std::vector<elf_parser::section_t> sections = myelf_parser.get_sections();
            for(auto &section : sections){
                if(section.section_name == ".rodata"){
                    endoff = section.section_offset + section.section_size;
                }
            }    
            if(endoff == 0){
                endoff = myelf_parser.getSymbolOffset("DX_ShellcodeEnd");
            }
        }
        LOGD("start off:%x, endoff:%x", startoff, endoff);
        myelf_parser.get_erase_shellcode(std::string("hexcode.dump"), startoff, endoff);
    }else if(kind == 4){
        //antidebug shellcode extracting
        std::string program(argbuf);
        elf_parser::Elf_parser myelf_parser(program);

        uint8_t* buf = myelf_parser.get_memory_map();
        opcodeutil oputil(buf);

    
        int startoff = 0;
        int endoff = 0;

        if(myelf_parser.get_type() == ARM_64){
            printf("arm64");

            startoff = myelf_parser.getSymbolOffset("dx_init");
            endoff = myelf_parser.getSymbolOffset("DX_ShellcodeEnd");

        }else if(myelf_parser.get_type() == ARM){
            printf("arm");
            startoff = myelf_parser.getSymbolOffset("dx_init");
            std::vector<elf_parser::section_t> sections = myelf_parser.get_sections();
            for(auto &section : sections){
                if(section.section_name == ".rodata"){
                    endoff = section.section_offset + section.section_size;
                }
            }
            
        }else if(myelf_parser.get_type() == X86){
            printf("x86\n");
            startoff = myelf_parser.getSymbolOffset("dx_init");
            std::vector<elf_parser::section_t> sections = myelf_parser.get_sections();
            for(auto &section : sections){
                if(section.section_name == ".rodata"){
                    endoff = section.section_offset + section.section_size;
                }
            } 
            if(endoff == 0){
                endoff = myelf_parser.getSymbolOffset("DX_ShellcodeEnd");
            }
        }

        int dxInitOff = myelf_parser.getSymbolOffset("dx_init");
        printf("dxinitoff : %x\n", dxInitOff);
        int antidebugOff = myelf_parser.getSymbolOffset("antidebug");
        //printf("antidebugoff : %x\n", antidebugOff);
        int relCurfun2antidebug = antidebugOff - dxInitOff;
        
        std::cout<<"relCurfun2antidebug:0x"<<std::hex<<relCurfun2antidebug<<std::endl;

        myelf_parser.get_antidebug_shellcode(std::string("hexcode.dump"), startoff, endoff);
    }else{
        LOGD("usage : ./bin2hex -[s i e a] -f [file_path]");
        LOGD("-s : extract signature-check shellcode");
        LOGD("-i : extract integrity-check shellcode");
        LOGD("-e : extract erase-phdrs shellcode");
        LOGD("-a : extract anti-debug shellcode");
    }

    return 0;
}

