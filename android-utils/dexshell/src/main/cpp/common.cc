//
// Created by my on 18-12-11.
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include "common.h"

#ifdef STACK
namespace {

    struct BacktraceState {
        void **current;
        void **end;
    };

    static _Unwind_Reason_Code unwindCallback(struct _Unwind_Context *context, void *arg) {
        BacktraceState *state = static_cast<BacktraceState *>(arg);
        uintptr_t pc = _Unwind_GetIP(context);
        if (pc) {
            if (state->current == state->end) {
                return _URC_END_OF_STACK;
            } else {
                *state->current++ = reinterpret_cast<void *>(pc);
            }
        }
        return _URC_NO_REASON;
    }

}

size_t captureBacktrace(void **buffer, size_t max) {
    BacktraceState state = {buffer, buffer + max};
    _Unwind_Backtrace(unwindCallback, &state);

    return state.current - buffer;
}

void dumpBacktrace(std::ostream &os, void **buffer, size_t count) {
    for (size_t idx = 0; idx < count; ++idx) {
        const void *addr = buffer[idx];
        const char *symbol = "";

        Dl_info info;
        if (dladdr(addr, &info) && info.dli_sname) {
            symbol = info.dli_sname;
        }

        os << "  #" << std::setw(2) << idx << ": " << addr << "  " << symbol << "\n";
    }
}
#endif

const char *getHex(char *buf, size_t outputLen, void *src, size_t len){
    memset(buf, 0, outputLen);
    size_t curLen = 0;
    char *ptr = buf;
    unsigned char* in = (unsigned char*) src;
    for(int i=0; i<len; i++){
        //保证能写下
        if (curLen + 1 + 2 <= outputLen) {
            int count = sprintf(ptr, "%02x", in[i]);
            if (count > 2) {
                LOGE("print hex error, count > 2");
                abort();
            }
            ptr += count;
            curLen += count;
        }
        else {
            break;
        }
    }
    return buf;
}

int get_fname_by_fd(int fd, char *outname, size_t sz) {
    char path[0x100] = {'\0'};

    if (fd < 0) {
        return -1;
    }

    sprintf(path, "/proc/self/fd/%d", fd);
    if (readlink(path, outname, sz) < 0) {
        LOGI("[%s] readlink failed", __func__);
        return -2;
    }

    return 0;
}