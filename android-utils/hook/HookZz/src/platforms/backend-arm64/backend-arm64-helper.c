//
// Created by z on 2018/2/11.
//
#include "dx_hook.h"
#include "backend-arm64-helper.h"

CodeSlice *arm64_code_patch(ARM64AssemblyrWriter *writer, ExecuteMemoryManager *emm, zz_addr_t target_addr,
                            zz_size_t range_size) {
    CodeSlice *codeslice = NULL;
    if (range_size > 0) {
        codeslice = ExecuteMemoryManagerAllocateNearCodeSlice(emm, target_addr, range_size, writer->insns_size + 4);
    } else {
        codeslice = ExecuteMemoryManagerAllocateCodeSlice(emm, writer->insns_size + 4);
    }

    if (!codeslice)
        return NULL;

    if (!MemoryHelperPatchCode((zz_addr_t)codeslice->data, (zz_ptr_t)writer->insns_buffer, writer->insns_size)) {
        free(codeslice);
        return NULL;
    }
    return codeslice;
}

CodeSlice *arm64_relocate_code_patch(ARM64Relocator *relocator, ARM64AssemblyrWriter *writer, ExecuteMemoryManager *emm,
                                     zz_addr_t target_addr, zz_size_t range_size) {
    CodeSlice *codeslice = NULL;
    if (range_size > 0) {
        codeslice = ExecuteMemoryManagerAllocateNearCodeSlice(emm, target_addr, range_size, writer->insns_size + 4);
    } else {
        codeslice = ExecuteMemoryManagerAllocateCodeSlice(emm, writer->insns_size + 4);
    }

    if (!codeslice)
        return NULL;

#if 0
    arm64_relocator_relocate_writer(relocator, (zz_addr_t)codeslice->data);
#else
    arm64_relocator_double_write(relocator, (zz_addr_t)codeslice->data);
#endif

    if (!MemoryHelperPatchCode((zz_addr_t)codeslice->data, (zz_ptr_t)writer->insns_buffer, writer->insns_size)) {
        free(codeslice);
        return NULL;
    }
    return codeslice;
}