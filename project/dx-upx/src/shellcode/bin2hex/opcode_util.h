#include <stdio.h>
#include <stdint.h>
#ifndef OPCODEUTIL
#define OPCODEUTIL

class opcodeutil{
public:
    opcodeutil(uint8_t* buf){
        this->buf = buf;
    }

    off_t getFirstCallAddrFrom(off_t addr);
    off_t getCalleeAddr(off_t addr);
    bool setCalleeAddr(off_t calleraddr, off_t addr);
    bool getCallee(off_t addr, uint8_t* &callee, int& len);
private:
    uint8_t* buf;
};



#endif


