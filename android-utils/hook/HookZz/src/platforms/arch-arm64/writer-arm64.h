/**
 *    Copyright 2017 jmpews
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef platforms_arch_arm64_writer_h
#define platforms_arch_arm64_writer_h

#include "zkit.h"

#include "memhelper.h"
#include "writer.h"

#include "instructions.h"
#include "regs-arm64.h"
#include "writer-arm64.h"

#define MAX_INSN_SIZE 256

typedef struct _ARM64AssemblerWriter
{
    ARM64InstructionCTX *insnCTXs[MAX_INSN_SIZE];
    zz_size_t insnCTXs_count;
    zz_addr_t start_pc;
    zz_addr_t insns_buffer;
    zz_size_t insns_size;
} ARM64AssemblyrWriter;

ARM64AssemblyrWriter *arm64_writer_new(zz_ptr_t insns_buffer);

void arm64_writer_init(ARM64AssemblyrWriter *self, zz_addr_t insns_buffer, zz_addr_t target_ptr);

void arm64_writer_reset(ARM64AssemblyrWriter *self, zz_addr_t insns_buffer, zz_addr_t target_ptr);

void arm64_writer_free(ARM64AssemblyrWriter *self);

zz_size_t arm64_writer_near_jump_range_size();

// ======= user custom =======

void arm64_writer_put_ldr_br_reg_address(ARM64AssemblyrWriter *self, ARM64Reg reg, zz_addr_t address);

void arm64_writer_put_ldr_blr_b_reg_address(ARM64AssemblyrWriter *self, ARM64Reg reg, zz_addr_t address);

void arm64_writer_put_ldr_b_reg_address(ARM64AssemblyrWriter *self, ARM64Reg reg, zz_addr_t address);

void arm64_writer_put_ldr_br_b_reg_address(ARM64AssemblyrWriter *self, ARM64Reg reg, zz_addr_t address);

// ======= default =======

void arm64_writer_put_ldr_reg_imm(ARM64AssemblyrWriter *self, ARM64Reg reg, uint32_t offset);

void arm64_writer_put_str_reg_reg_offset(ARM64AssemblyrWriter *self, ARM64Reg src_reg, ARM64Reg dst_reg,
                                         uint64_t offset);

void arm64_writer_put_ldr_reg_reg_offset(ARM64AssemblyrWriter *self, ARM64Reg dst_reg, ARM64Reg src_reg,
                                         uint64_t offset);

void arm64_writer_put_br_reg(ARM64AssemblyrWriter *self, ARM64Reg reg);

void arm64_writer_put_blr_reg(ARM64AssemblyrWriter *self, ARM64Reg reg);

void arm64_writer_put_b_imm(ARM64AssemblyrWriter *self, uint64_t offset);

void arm64_writer_put_b_cond_imm(ARM64AssemblyrWriter *self, uint32_t condition, uint64_t imm);

void arm64_writer_put_add_reg_reg_imm(ARM64AssemblyrWriter *self, ARM64Reg dst_reg, ARM64Reg left_reg,
                                      uint64_t imm);

void arm64_writer_put_sub_reg_reg_imm(ARM64AssemblyrWriter *self, ARM64Reg dst_reg, ARM64Reg left_reg,
                                      uint64_t imm);

void arm64_writer_put_bytes(ARM64AssemblyrWriter *self, char *data, zz_size_t size);

void arm64_writer_put_instruction(ARM64AssemblyrWriter *self, uint32_t insn);

#endif