# -*- coding: utf-8 -*-
import os
from lief import ELF
import lief
import struct
from elf_struct import DynSymEntry, RelEntry



def patch_file(filename, offset, patch_data):
    print("patch_file patch data type:", type(patch_data))
    patch_data = bytes(patch_data)
    patch_data_len = len(patch_data)

    with open(filename, 'rb') as fp:
        src_bin = fp.read()
        out_data = src_bin[:offset] + patch_data + src_bin[offset + patch_data_len:]

        with open(filename, 'wb') as fp:
            fp.write(out_data)
            print("patch_file finished")
            return True

    return False


def print_symbol(soname, sym_name):
    elf = lief.parse(soname)
    symbol = elf.get_symbol(sym_name)
    print("print_symbol value:", hex(symbol.value))


def print_rel(soname, sym_name):
    elf = lief.parse(soname)
    relocation = elf.get_relocation(sym_name)
    print("print_rel address:", hex(relocation.address))


def update_symbol(soname, sym_name, value):
    elf = lief.parse(soname)
    symbol = elf.get_symbol(sym_name)
    symbol.value = value
    elf.write(soname)

def update_rel(soname, sym_name, offset):
    print("update_rel offset:", hex(offset))
    elf = lief.parse(soname)
    relocation = elf.get_relocation(sym_name)
    relocation.address = offset
    elf.write(soname)




def get_offset(inaddr):
    high = inaddr // 0x0100000
    mid = (inaddr & 0xfffff) // 0x1000
    low = (inaddr & 0xfff)
    return high, mid, low



def inject_export_sym(target_so, inject_so, out_so):
    target_elf = lief.parse(target_so)
    inject_elf = lief.parse(inject_so)

    target_got = target_elf.get_section(".got")
    print("target_got name:", target_got.name)
    print("target_got size:", hex(target_got.size))
    print("target_got entry_size:", hex(target_got.entry_size))
    print("target_got link:", hex(target_got.link))
    print("target_got virtual_address:", hex(target_got.virtual_address))
    print("target_got file_offset:", hex(target_got.file_offset))
    print("target_got offset:", hex(target_got.file_offset))
    print("target_got content:", target_got.content)
    print("target_got len(content):", len(target_got.content))
    print("target_got type(content):", type(target_got.content))


    new_got = ELF.Section()
    new_got.name = ".new"
    new_got.link = 0x12
    new_got.content = [1, 2, 3, 4]
    target_elf.add(new_got, True)

    target_elf.write(out_so)
    target_elf = lief.parse(out_so)
    target_got = target_elf.get_section(".new")

    print("new target_got content:", target_got.content)
    print("new target_got.content:", len(target_got.content))


    inject_got = inject_elf.get_section(".got")
    in_got_index = inject_got.search("inject_code")
    print("in_got_index:", in_got_index)




    inject_relplt_list = inject_elf.pltgot_relocations

    # 向 .rel.plt, .dynsym, .dynstr 新增大小
    for inject_rel in inject_relplt_list:
        if not target_elf.has_symbol(inject_rel.symbol.name):
            print("inject_rel:", inject_rel)
            print("injetc_rel.symbol:", inject_rel.symbol)
            sym = target_elf.add_dynamic_symbol(inject_rel.symbol)  # FIXME 后面修复

            add_reloc = ELF.Relocation()
            add_reloc.type = inject_rel.type
            add_reloc.symbol = sym
            add_reloc.address = 0  # FIXME 后面修复
            add_reloc.purpose = ELF.RELOCATION_PURPOSES.PLTGOT
            add_reloc = target_elf.add_pltgot_relocation(add_reloc)
            print("add_reloc:", add_reloc)

    target_elf.write(out_so)

    # 修复地址
    out_elf = lief.parse(out_so)
    out_got = out_elf.get_section(".got")
    print("out_got file_offset:", hex(out_got.file_offset))
    print("out_got offset:", hex(out_got.file_offset))






def add_symbol(target_so, inject_so, out):
    print("lief version:", lief.__version__)
    target_elf = lief.parse(target_so)
    inject_elf = lief.parse(inject_so)
    add_got = inject_elf.get_section(".got")
    add_data = inject_elf.get_section(".data")
    add_plt = inject_elf.get_section(".plt")
    add_text = inject_elf.get_section(".text")

    before_add_load_num = 0
    for i in target_elf.segments:
        if i.type == ELF.SEGMENT_TYPES.LOAD:
            before_add_load_num += 1
    print("add_symbol before_add_load_num:", before_add_load_num)

    """
    添加2个load段，用于将add中内容添加进target
    """
    # 第一个可读可执行,用来映射新的.plt节 + .text节
    add_RE_seg = lief.ELF.Segment()
    add_RE_seg.alignment = 0x1000
    add_RE_seg.type = ELF.SEGMENT_TYPES.LOAD
    add_RE_seg.add(ELF.SEGMENT_FLAGS.X)
    add_RE_seg.add(ELF.SEGMENT_FLAGS.R)
    add_RE_seg.content = add_plt.content + add_text.content
    print("add_symbol add load1 len:", hex(len(add_RE_seg.content)))
    target_elf.add(add_RE_seg)

    # 第二个load段 可读可写 添加 .got .data
    add_RW_seg = lief.ELF.Segment()
    add_RW_seg.alignment = 0x1000
    add_RW_seg.type = ELF.SEGMENT_TYPES.LOAD
    add_RW_seg.add(ELF.SEGMENT_FLAGS.W)
    add_RW_seg.add(ELF.SEGMENT_FLAGS.R)
    add_RW_seg.content = add_got.content + add_data.content
    target_elf.add(add_RW_seg)
    print("add_symbol add load2 len", hex(len(add_RW_seg.content)))

    addbin_relplt = inject_elf.pltgot_relocations

    """
        添加inject_elf中的relplt，并且向dynsym添加对应的symbol。
        此操作改动了dynstr、dynsym、rel.plt。
        本来针对原节拓展即可，但lief工具新增了3个load段进行加载新的内容，所以还修改了dynamic
    """
    add_sym_value_list = list()
    for add_entry in addbin_relplt:
        if target_elf.has_symbol(add_entry.symbol.name):
            # print("add_symbol inject plt symbol name:", add_entry.symbol.name)
            sym = target_elf.get_symbol(add_entry.symbol.name)
        else:
            print("add_entry:", add_entry)
            sym = target_elf.add_dynamic_symbol(add_entry.symbol)  # 后面修复
            print("added symbol.value:", hex(sym.value))
            add_sym_value_list.append(add_entry.symbol.value - add_text.virtual_address)
            print("add_sym_value_list:", add_sym_value_list)

            # 此处lief工具又有问题，写入后value变了，坑爹货
            # 工具不出问题，此处减掉add中text的虚拟地址，加上intermediate中的新增的text虚拟地址就行了

            add_reloc = ELF.Relocation()
            add_reloc.type = add_entry.type
            add_reloc.symbol = sym
            add_reloc.address = add_entry.address - add_got.virtual_address # 后面修复
            add_reloc.purpose = ELF.RELOCATION_PURPOSES.PLTGOT
            add_reloc = target_elf.add_pltgot_relocation(add_reloc)
            print("add_reloc.address:", hex(add_reloc.address))


    target_elf.write(out)

    # return

    # 辣鸡工具，会导致偏移出问题，所以不得不进行手工修复
    inter = lief.parse(out)

    print_symbol(out, "inject_code")

    # 获取前面添加的两个load段的虚拟地址
    add_RE_seg_virtual_address = 0
    add_RW_seg_virtual_address = 0

    after_add_load_num = 0
    for i in inter.segments:
        if i.type == ELF.SEGMENT_TYPES.LOAD:
            after_add_load_num += 1
            if after_add_load_num > before_add_load_num and after_add_load_num <= before_add_load_num + 2:
                if i.has(ELF.SEGMENT_FLAGS.X):
                    add_RE_seg_virtual_address = i.virtual_address
                if i.has(ELF.SEGMENT_FLAGS.W):
                    add_RW_seg_virtual_address = i.virtual_address

    print("add_symbol add_RE_seg_virtual_address:", hex(add_RE_seg_virtual_address))
    print("add_symbol add_RW_seg_virtual_address:", hex(add_RW_seg_virtual_address))

    # 修复dynsym中新增的symbol，将Elf32_Sym->st_value 指向text段即可，0不修改是指向import func
    new_dynsym_content = inter.get_section(".dynsym").content
    print(".dynsym.file_offset:", hex(inter.get_section(".dynsym").file_offset))
    add_dynsym_start = len(new_dynsym_content) - len(add_sym_value_list) * 16
    print("add_symbol add_dynsym_start:", hex(add_dynsym_start))

    modify_dynsym_content = []
    inx = 0
    for entry_content in [new_dynsym_content[i:i + 16] for i in range(add_dynsym_start, len(new_dynsym_content), 16)]:
        print("entry_content:", entry_content)

        # entry = DynSymEntry.parse_from_content(entry_content)
        entry = DynSymEntry(entry_content)
        print("entry:", entry)
        if (entry.sym_value != 0):
            print("old entry.sym_value:", hex(entry.sym_value))
            entry.sym_value = add_sym_value_list[inx] + add_RE_seg_virtual_address + len(add_plt.content)
            print("add_sym_value_list[inx]:", add_sym_value_list[inx])
            update_symbol(out, "inject_code", entry.sym_value)
            print("new entry.sym_value:", hex(entry.sym_value))
        inx += 1
        modify_dynsym_content += entry.content
    # patch_file(out, inter.get_section(".dynsym").offset + add_dynsym_start, modify_dynsym_content)

    # 修复.rel.plt中新增的rel项，指向新增的第二个load段中加载的add.so中的got表
    modify_rel_content = []
    relplt = inter.get_section(".rel.plt")
    add_rel_start = inter.get_section(".rel.plt").size - len(add_sym_value_list) * 8
    print("add_symbol add_rel_start:", hex(add_rel_start))


    add_entry_ndx = 0
    for rel_content in [relplt.content[i:i + 8] for i in range(add_rel_start, len(relplt.content), 8)]:
        print("rel_contenn:", rel_content)
        # rel = RelEntry.parse_from_content(rel_content)
        relEntry = RelEntry(rel_content)
        if (relEntry.offset != 0):
            print("old rel.offset:", hex(relEntry.offset))
            relEntry.offset = relEntry.offset + add_RW_seg_virtual_address
            update_rel(out, "inject_code", relEntry.offset)
            print("new rel.offset:", hex(relEntry.offset))
        modify_rel_content += relEntry.content
        add_entry_ndx += 1

    print_rel(out, "inject_code")
    # patch_file(out, inter.get_section(".rel.plt").offset + add_rel_start, modify_rel_content)


    print("after update")
    print_symbol(out, "inject_code")
    print_rel(out, "inject_code")




    # 需要fix plt 表,调用外部函数时需要 通过plt表进行跳转
    # add_plt_size = add_plt.size
    # print("add_plt_size:", add_plt_size)
    # PLT_TABLE_HEAD_LEN = 0x14
    # need_fix_plt_content = add_RE_seg.content[PLT_TABLE_HEAD_LEN:add_plt_size]
    # # print(need_fix_plt_content)
    # # print(hex(add_RW_seg_virtual_address - add_RE_seg_virtual_address))
    #
    # got_address_list = []
    # add_got_size = add_got.size
    # print("add_got size:", add_got.size)
    # index = 0
    # for got_entry in [add_RW_seg.content[i:i + 4] for i in range(0, add_got_size, 4)]:
    #     print("got_entry:i", got_entry)
    #     value = int(got_entry[0]) + (int(got_entry[1]) << 8) + (int(got_entry[2]) << 16) + (int(got_entry[3]) << 24)
    #     print("value:", hex(value))
    #
    #     got_address_list.append(add_RW_seg_virtual_address + index * 4)
    #     index += 1
    # print("got_address_list len:", len(got_address_list))
    #
    #
    # inx = 0
    # modify_plt_content = []
    # for plt_entry in [add_RE_seg.content[i:i + 12] for i in range(PLT_TABLE_HEAD_LEN, add_plt_size, 12)]:
    #     print("plt_entry[{0}]: {1}".format(inx, plt_entry))
    #     inx += 1
    #
    #     # +8是因为ADR取地址是取PC的值
    #     got2plt_offset = got_address_list[inx] - (inx * 12 + add_RE_seg_virtual_address + 8 + PLT_TABLE_HEAD_LEN)
    #     print("got -> plt offset :", hex(got2plt_offset))
    #     print(plt_entry)
    #     h, m, l = get_offset(got2plt_offset)
    #     plt_entry[0] = h
    #     plt_entry[4] = m
    #     plt_entry[8] = l & 0xff
    #     plt_entry[9] = (plt_entry[9] & 0xf0) + (l >> 8)
    #     print("fix entry:", plt_entry, hex(plt_entry[9]))
    #     modify_plt_content += plt_entry
    #
    # # print("modify_plt_content:", modify_plt_content)
    # patch_file(out, add_RE_seg_virtual_address + PLT_TABLE_HEAD_LEN, modify_plt_content)



def add_init_proc(target_so, out):
    print("add_init_proc start")

    """
      添加init段，并且调用ADD_FUNC_NAME并传入一个参数（当前So的基址）
    """
    INIT_PROC_SIZE = 0x2c
    INIT_PROC_CONTENT = [0xC0, 0x46, 0xFF, 0xB5, 0x83, 0xB0, 0x6D, 0x46, 0x00, 0xA3, 0x14, 0x3B, 0x19, 0x1C, 0x1F, 0x68,
                         0xC9, 0x1B, 0x29, 0x60, 0x5E, 0x68, 0x03, 0xD0, 0x76, 0x18, 0x6E, 0x60, 0x28, 0x68, 0xB0, 0x47,
                         0x03, 0xB0, 0xFF, 0xBD]  # 腾讯代码里扣出来的嘿嘿
    # init_proc 调用的函数名
    ADD_FUNC_NAME = r"inject_code"

    target_elf = lief.parse(target_so)
    # 检测.dynamic节的空位是否足够,如果小于3个就要拓展dynamic节内容
    free_dynamic_entry = 0
    for entry in target_elf.dynamic_entries:
        if entry.tag == lief.ELF.DYNAMIC_TAGS.NULL:
            free_dynamic_entry += 1
    print("free dynamic entry num:", free_dynamic_entry)
    assert free_dynamic_entry > 3

    # 获取位于segment header 后的 offset，用于添加init_proc
    init_proc_offset = target_elf.header.program_header_offset + \
                       target_elf.header.program_header_size * (target_elf.header.numberof_segments + 2)
    print("init_proc_offset:%s" %(hex(init_proc_offset)))

    # 添加init入口
    if not target_elf.has(ELF.DYNAMIC_TAGS.INIT):
        # 先用0占位,直接写入偏移，lief工具会有点问题
        # 如果不出问题
        target_elf.add(ELF.DynamicEntry(ELF.DYNAMIC_TAGS.INIT, init_proc_offset + 8))
        # binary.add(ELF.DynamicEntry(ELF.DYNAMIC_TAGS.INIT, 0))
        print("insert dynamic init finished")

    else:
        init_entry = target_elf.get(ELF.DYNAMIC_TAGS.INIT)
        print("already insert dynamic init [%x]", hex(init_entry))
        # TODO so已存在INIT，需要追加
        exit(1)

    target_elf.write(out)

    outbin = lief.parse(out)


    # 获取 ADD_FUNC_NAME 地址
    add_func_addr = outbin.get_symbol(ADD_FUNC_NAME).value
    print("add_func_addr:%s" %(hex(add_func_addr)))

    # 前4字节为函数 init_proc，后4字节为调用函数地址，后面为 init 代码
    init_proc_all_bin = list(struct.pack("<I", init_proc_offset)) + list(
        struct.pack("<I", add_func_addr)) + INIT_PROC_CONTENT
    # patch init_proc函数包括数据
    patch_file(out, init_proc_offset, init_proc_all_bin)



    out_dynamic = outbin.get_section(".dynamic")
    num = 0
    for entry in outbin.dynamic_entries:
        if entry.tag == ELF.DYNAMIC_TAGS.INIT:
            break
        num += 1

    # dynamic init 段的偏移
    init_dyn_offset = out_dynamic.offset + (num * out_dynamic.entry_size)
    print("init_dyn_offset:%x" %(init_dyn_offset))

    # patch dynamic init 段的value值，指向 init_proc 地址，+8存放init_proc地址和add_func地址，+1 是thumb模式
    patch_file(out, init_dyn_offset + 4, struct.pack("<I", init_proc_offset + 8 + 1))


# 向 target so 添加TD_NEEDED，内容是add so
def add_library(target, add):
    elf = lief.parse(target)
    elf.add_library(add)  # Injection!
    elf.write(target)



def decrypt_tprt():
    name = "com.tencent.tmgp.sgame_363010502/lib/armeabi-v7a/libtprt.so"
    so_data = b""
    with open(name, "rb") as f:
        so_data = f.read()

    print("libtprt.so data len:%d" % (len(so_data)))

    tprt_lief = lief.parse(name)
    tptext_section = tprt_lief.get_section(".tptext").content

    print("tptext src len:%d" %(len(tptext_section)))
    offset = tprt_lief.get_section(".tptext").offset
    out = b""
    for i in range(len(tptext_section)):
        out += (tptext_section[i] ^ 0xb8).to_bytes(1, byteorder='little')

    print("tptext dest len:%d" %(len(out)))
    out_data = so_data[:offset] + out + so_data[offset + len(tptext_section):]
    with open("de_tprt.so", 'wb') as fp:
        fp.write(out_data)





if __name__ == '__main__':
    # decrypt_tprt()
    input = "target.so"
    inject = "inject.so"
    out = "libnative-lib.so"
    if os.path.isfile(out):
        os.remove(out)

    add_symbol(input, inject, out)

    add_init_proc(out, out)

    add_library(out, inject)

    # inject_export_sym(input, inject, out)

