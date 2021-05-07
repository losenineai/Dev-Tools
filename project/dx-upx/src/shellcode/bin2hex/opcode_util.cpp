#include "x86_asm_tool/hde64.h"
#include "opcode_util.h"
#include <stdio.h>
#include <iostream>


off_t opcodeutil::getFirstCallAddrFrom(off_t addr){
    off_t offset = 0;
    hde64s decode = {0};
    //std::cout<<std::hex<<addr<<std::endl;
    for(; offset < 30; offset++){
        hde64_disasm(this->buf + addr + offset, &decode);
        //std::cout<<std::hex<<"orig hex:"<<*(uint8_t*)(this->buf + addr + offset)<<"    0x"<<decode.opcode<<std::endl; 
        if(decode.opcode == 0xe8){
            //std::cout<<std::hex<<"callee addr:"<<decode.imm.imm32<<std::endl;
            return (uint32_t)offset + (uint32_t)addr;
        }
        
    }
    
    return 0;
}

off_t opcodeutil::getCalleeAddr(off_t addr){
    off_t offset = 0;
    hde64s decode = {0};

    uint32_t off = this->getFirstCallAddrFrom(addr);

    
    hde64_disasm(this->buf + off, &decode);
    
    if(decode.opcode == 0xe8){
        //std::cout<<std::hex<<"callee addr:"<<off + decode.imm.imm32<<std::endl;
        return off + decode.imm.imm32;
    }
    
    
    return 0;
}

bool opcodeutil::setCalleeAddr(off_t addr, off_t addroff){
    
    hde64s decode = {0};

    uint32_t off = this->getFirstCallAddrFrom(addr);
    uint32_t calleeaddr = addroff - off;
    

    hde64_disasm(this->buf + off, &decode);

    if (decode.opcode != 0xe8){  //call opcode
        return false;
    }
    
    
    *(uint32_t*)(this->buf + off + 1) = calleeaddr;
    
    return true;
}

bool opcodeutil::getCallee(off_t addr, uint8_t* &callee, int& len){
    int offset = 0;
    hde64s decode = {0};
    len = 0;
    for(offset = 0; offset < 40; offset++){
        hde64_disasm(this->buf + addr + offset, &decode);
        if(decode.opcode == 0xc3){  //ret opcode
            break;
        }    
    }
    
    if(offset<20 && offset>0){
        len = offset+1;
        callee = (uint8_t*)malloc(len);
        for(int i = 0; i<len; i++){
            callee[i] = *(uint8_t*)(this->buf+ addr + i);
        }
    }else{
        callee = NULL;
        len = 0;
        return false;
    }

    return true;
}



