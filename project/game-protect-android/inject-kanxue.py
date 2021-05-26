import lief,struct
from lief import ELF
from mystructs import DynSymEntry,SectionEntry,SegmentEntry,DynamicEntry,RelEntry


PAGE_SIZE = 0x1000
# init_proc的大小和内容
INIT_PROC_SIZE = 0x2c
INIT_PROC_CONTENT = [0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x46,0xFF,0xB5,0x83,0xB0,0x6D,0x46,0x00,0xA3,0x14,0x3B,0x19,0x1C,0x1F,0x68,0xC9,0x1B,0x29,0x60,0x5E,0x68,0x03,0xD0,0x76,0x18,0x6E,0x60,0x28,0x68,0xB0,0x47,0x03,0xB0,0xFF,0xBD]




# 配置参数
ADD_BIN = "inject.so"
TARGET_BIN = "target.so"
NEEDED_BIN = "libpro.so"

MAIN_FUNC_NAME = "inject_code"


def print_list(l :list):
    print(hex(len(l))," [{}]".format(",".join(hex(i) for i in l)))


def patch_file(path, offset, content):
    print("write in:", path, " offset:",hex(offset))
    print(hex(len(content)), content)
    with open(path, "r+b") as f:
        if not f.seek(offset) == -1:
            f.write(bytes(content))


def page_end(addr):
    return (addr // PAGE_SIZE + 1) * PAGE_SIZE


def page_start(addr):
    return (addr // PAGE_SIZE) * PAGE_SIZE

def align(v, al):
    return (v // al + 1) * al


print("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=")
print(
    """
    通过解析libadd.so来获取需要的基础数据
    代码 - .text .plt
    数据 - .data .got
    重定向信息 - .rel.plt
    """
)

add_bin = lief.parse(ADD_BIN)

MAIN_OFFSET = add_bin.get_symbol(MAIN_FUNC_NAME).value

add_got = add_bin.get_section(".got")
add_data = add_bin.get_section(".data")
add_plt = add_bin.get_section(".plt")
add_text = add_bin.get_section(".text")
addbin_relplt = add_bin.pltgot_relocations


# 默认的节排列 .plt-.text-.got-.data
assert add_plt.file_offset < add_text.file_offset
assert add_got.file_offset < add_data.file_offset
assert add_text.file_offset < add_data.file_offset

# 重定位项中符号名和偏移的关系
add_relplt_info_table = {}
add_relplt_info_list = []
for reloc in addbin_relplt:
    add_relplt_info_table[reloc.symbol.name] = reloc.address
    add_relplt_info_list.append(reloc.address)
    print("libadd.so reloc info:", reloc.symbol.name, ":", hex(add_relplt_info_table[reloc.symbol.name]))



print("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=")
print(
    """
    生成、获取  target  dynstr\dynsym\dynamic节基础的数据
    """
)


binary = lief.parse(TARGET_BIN)

# 检测.dynamic节的空位是否足够,如果小于3个就要拓展dynamic节内容
free_dynamic_entry = 0
for entry in binary.dynamic_entries:
    if entry.tag == lief.ELF.DYNAMIC_TAGS.NULL:
        free_dynamic_entry += 1
print("free dynamic entry num:",free_dynamic_entry)
assert free_dynamic_entry > 3

# 添加needed依赖
if not binary.has_library(NEEDED_BIN):
    binary.add_library(NEEDED_BIN)


# 添加init入口
if not binary.has(ELF.DYNAMIC_TAGS.INIT):
    # 先用0占位
    binary.add(ELF.DynamicEntry(ELF.DYNAMIC_TAGS.INIT, 0x0))
else:
    init_entry = binary.get(ELF.DYNAMIC_TAGS.INIT)
    print("[x] binary has init_proc:",init_entry)
    exit(1)



add_entry_ndx = 0
for add_entry in addbin_relplt:
    if binary.has_symbol(add_entry.symbol.name):
        sym = binary.get_symbol(add_entry.symbol.name)
    else:
        sym = binary.add_dynamic_symbol(add_entry.symbol)

    add_reloc = ELF.Relocation()
    add_reloc.type = add_entry.type
    add_reloc.symbol = sym
    add_reloc.address = add_entry_ndx
    add_entry_ndx += 1
    add_reloc.purpose = ELF.RELOCATION_PURPOSES.PLTGOT
    # 注意,这里添加了重定向信息不一定准备,以最后write了之后的数据为准
    add_reloc = binary.add_pltgot_relocation(add_reloc)
    print("add_reloc - ",add_reloc)

binary.write("OUT.so")

bin_out1 = lief.parse("OUT.so")

sym_index_table = {}

# 使用写入后的文件的relplt信息来作为最终的relplt段来参考
for entry in bin_out1.pltgot_relocations:
    if entry.address < 0xFF:
        sym_index_table[entry.info] = entry.symbol.name
        print("add relocate info :", entry)

# -=-=-=-=-=数据读取部分-=-=-=-=-=-=-=-=
new_dynstr_content  = bin_out1.get_section(".dynstr").content
new_dynsym_content = bin_out1.get_section(".dynsym").content
new_dynamic_content = bin_out1.get_section(".dynamic").content
new_relplt_content = bin_out1.get_section(".rel.plt").content

# 处理一下dynsym条目,把有内部符号的value值改成相对于段基址的偏移,方便后期的重定位
new_dynsym_entries = []


for entry_content in [new_dynsym_content[i:i+16] for i in range(0, len(new_dynsym_content), 16)]:
    entry = DynSymEntry.parse_from_content(entry_content)

    if entry.sym_value != 0 and entry.sym_shndx != 0xFFF1 : #SHN_ABS

        host_section = bin_out1.sections[entry.sym_shndx]
        # if host_section.name == ".plt":
        #     print(host_section)
        #     print(hex(entry.sym_value))
        #     print(entry.sym_shndx)
        #     continue
        entry.sym_value -= host_section.virtual_address

        print("fix dynsym entry value:",hex(entry.sym_value))
    new_dynsym_entries.append(entry)


# 过滤一下dynamic内容,删除空条目
filter_dynamic_content = []
for entry in [new_dynamic_content[i:i+8] for i in range(0, len(new_dynamic_content), 8)]:
    if not entry == (8 * [0]):
        filter_dynamic_content += entry

# end entry
new_dynamic_content = filter_dynamic_content + (8 * [0])


print("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=")
print(
    """
    修改原始文件
    1.添加segments信息
    2.拓展dynstr节\拓展dynsym节
    3.4字节对齐最后一个section,并拼接需要的数据在最后一个section之后
    """
)
binary = lief.parse(TARGET_BIN)

# 1 (通过lief来添加segment会导致前面的节大面积移位)
# 目前方案仅需要添加两个LOAD segment
# 第一个可读可执行,用来映射新的.rel.plt节 + .plt节 + .text节
# 第二个可读可写,用来映射新的.got节 + .data节
# 这里先构造出要的字节空间,后面再填入数据
assert binary.sections[0].name == ""


first_section = binary.sections[1]
print("first_section:", first_section.name,"length:", hex(first_section.size))
first_section_extend_num = 0x20 * 2 + INIT_PROC_SIZE # 2个segment头部 + 1个init_proc代码字节数
first_section_content = first_section.content
print(first_section.name," need extend",hex(first_section_extend_num),"bytes")
binary.extend(first_section, first_section_extend_num)
binary.header.numberof_segments += 2

# 2
# 根据新的dynsym长度和dynstr长度来拓展原文件的dynsym和dynstr
#
dynsym_sec = binary.get_section(".dynsym")
dynsym_extend_num = align(len(new_dynsym_content) - dynsym_sec.size, 4)
if dynsym_extend_num > 0:
    print("dynsym old length:", hex(dynsym_sec.size))
    print("dynsym section extend:", hex(dynsym_extend_num))
    binary.extend(dynsym_sec, dynsym_extend_num)

dynstr_sec = binary.get_section(".dynstr")
dynstr_extend_num = align(len(new_dynstr_content) - dynstr_sec.size, 4)
if dynstr_extend_num > 0:
    print("dynstr old length:", hex(dynstr_sec.size))
    print("dynstr section extend:", hex(dynstr_extend_num))
    binary.extend(dynstr_sec, dynstr_extend_num)



print("last offset:", hex(binary.last_offset_section))
last_section =  binary.section_from_offset(binary.last_offset_section - 1)
print("last section:", last_section)
padding_num = align(binary.last_offset_section, 4) - binary.last_offset_section
add_content_offset = binary.last_offset_section + padding_num
print("padding", hex(padding_num), "btyes, [add_content_offset]", hex(add_content_offset))
assert add_plt.file_offset > len(new_relplt_content)
add_bytes = padding_num + align(add_data.file_offset + add_data.size, 4)
print("need add",hex(add_bytes),"bytes")


# 这里选择通过添加section的方式来拓展
dummy_section = lief.ELF.Section()
dummy_section.content = add_bytes * [0]
dummy_section.alignment = 0x1
dummy_section = binary.add(dummy_section, loaded= False)
binary.remove(dummy_section)

# 清空原.rel.plt中的内容
binary.get_section(".rel.plt").clear()

out2_plt =  binary.pltgot_relocations

binary.write("libnative-lib-out.so")
bin_out2 = lief.parse("libnative-lib-out.so")
bin_out2_path = "libnative-lib-out.so"

#获取位于segment header 后的 offset，用于添加init_proc
init_proc_offset = bin_out2.header.program_header_offset + bin_out2.header.program_header_size * bin_out2.header.numberof_segments
print("init_proc offset:",hex(init_proc_offset))

# 3
# 修正最新的section header table(这里的逻辑较高耦合)
last_map_section = sorted(bin_out2.sections,key=lambda s:s.virtual_address)[-1]
new_map_page_start = page_end(last_map_section.virtual_address + last_map_section.size)

modify_section_header_table_content = []
for ndx,entry in enumerate(bin_out2.sections):
    ent_obj = SectionEntry.parse_from_lief(entry)
    if ndx == 1:
        print("fix first section header")
        ent_obj.file_offset += first_section_extend_num
        ent_obj.virtual_address += first_section_extend_num

    if ndx == 2:
        assert entry.virtual_address == (bin_out2.sections[1].virtual_address + bin_out2.sections[1].size + first_section_extend_num)
        if ent_obj.file_offset != ent_obj.virtual_address:
            print("fix second section header")
            ent_obj.file_offset = ent_obj.virtual_address

    if entry.name == ".dynsym":
        print("fix dynsym section header")
        ent_obj.size += dynsym_extend_num

    if entry.name == ".dynstr":
        print("fix dynstr section header")
        ent_obj.size += dynstr_extend_num

    if entry.name == ".rel.plt":
        ent_obj.virtual_address = new_map_page_start + (add_content_offset & 0xFFF)
        ent_obj.file_offset = add_content_offset
        ent_obj.size = len(new_relplt_content)

    modify_section_header_table_content += ent_obj.content

print("patch section header table")
patch_file(bin_out2_path, bin_out2.header.section_header_offset, modify_section_header_table_content)

# 4
# 填入数据到之前开辟的空间中
#
# 首先是segment header信息
# 第一个可读可执行,用来映射新的.rel.plt节 + .plt节 + .text节

add_RE_seg = SegmentEntry()
add_RE_seg.alignment = 0x1000
add_RE_seg.type = ELF.SEGMENT_TYPES.LOAD
add_RE_seg.flag = ELF.SEGMENT_FLAGS.X | ELF.SEGMENT_FLAGS.R
add_RE_seg.file_offset = add_content_offset
add_RE_seg.virtual_address = new_map_page_start + (add_RE_seg.file_offset & 0xFFF)
add_RE_seg.physical_address = add_RE_seg.virtual_address
add_RE_seg.virtual_size = add_text.file_offset + add_text.size + len(new_relplt_content)
add_RE_seg.physical_size = add_RE_seg.virtual_size

# 第二个可读可写,用来映射新的.got节 + .data节
add_RW_seg = SegmentEntry()
add_RW_seg.alignment = 0x1000
add_RW_seg.type = ELF.SEGMENT_TYPES.LOAD
add_RW_seg.flag = ELF.SEGMENT_FLAGS.W | ELF.SEGMENT_FLAGS.R
add_RW_seg.file_offset = add_content_offset + add_got.file_offset
add_RW_seg.virtual_address = page_end(add_RE_seg.virtual_address + add_got.file_offset) + (add_RW_seg.file_offset & 0xFFF)
add_RW_seg.physical_address = add_RW_seg.virtual_address
add_RW_seg.virtual_size = add_data.file_offset + add_data.size - add_got.file_offset
add_RW_seg.physical_size = add_RW_seg.virtual_size

# 排列segment header table
new_segments_content = []
for idx, val in enumerate(bin_out2.segments):
    if idx == bin_out2.header.numberof_segments - 2:
        break

    new_segments_content += SegmentEntry.parse_from_lief(val).content
    if val.type == ELF.SEGMENT_TYPES.LOAD and binary.segments[idx + 1].type != ELF.SEGMENT_TYPES.LOAD:
        new_segments_content += add_RE_seg.content
        new_segments_content += add_RW_seg.content

# 写入segment header table
print("patch segment header table")
patch_file(bin_out2_path,bin_out2.header.program_header_offset,new_segments_content)
# 写入第一个节内容
print("patch first section content")
patch_file(bin_out2_path,bin_out2.header.program_header_offset + len(new_segments_content) + INIT_PROC_SIZE, first_section_content)

# 写入dynsym信息
print("patch dynsym section content")
modify_dynsym_content = []
for entry in new_dynsym_entries:
    if entry.sym_value != 0 and entry.sym_shndx != 0xFFF1:
        hold_section = bin_out2.sections[entry.sym_shndx]
        entry.sym_value += hold_section.virtual_address
    modify_dynsym_content += entry.content
patch_file(bin_out2_path,bin_out2.get_section(".dynsym").offset, modify_dynsym_content)
# 写入dynstr信息
print("patch dynstr section content")
patch_file(bin_out2_path,bin_out2.get_section(".dynstr").offset, new_dynstr_content)

# 写入dynamic信息
template_bin = lief.parse(bin_out2_path)
old_bin = lief.parse(TARGET_BIN)
new_dynamic_fix_content = list()
for entry in [DynamicEntry.parse_from_content(new_dynamic_content[i:i+8]) for i in range(0, len(new_dynamic_content), 8)]:
    if entry.tag == int(ELF.DYNAMIC_TAGS.PLTGOT):
        got_ptr_offset = old_bin.get(ELF.DYNAMIC_TAGS.PLTGOT).value - old_bin.get_section(".got").virtual_address
        entry.value = template_bin.get_section(".got").virtual_address + got_ptr_offset

    elif entry.tag == int(ELF.DYNAMIC_TAGS.PLTRELSZ):
        entry.value = template_bin.get_section(".rel.plt").size

    elif entry.tag == int(ELF.DYNAMIC_TAGS.JMPREL):
        entry.value = template_bin.get_section(".rel.plt").virtual_address

    elif entry.tag == int(ELF.DYNAMIC_TAGS.PLTREL):
        entry.value = old_bin.get(ELF.DYNAMIC_TAGS.PLTREL).value

    elif entry.tag == int(ELF.DYNAMIC_TAGS.REL):
        entry.value = template_bin.get_section(".rel.dyn").virtual_address

    elif entry.tag == int(ELF.DYNAMIC_TAGS.RELSZ):
        entry.value = template_bin.get_section(".rel.dyn").size

    elif entry.tag == int(ELF.DYNAMIC_TAGS.RELENT):
        entry.value = old_bin.get(ELF.DYNAMIC_TAGS.RELENT).value

    elif entry.tag == int(ELF.DYNAMIC_TAGS.RELCOUNT):
        entry.value = old_bin.get(ELF.DYNAMIC_TAGS.RELCOUNT).value

    elif entry.tag == int(ELF.DYNAMIC_TAGS.SYMTAB):
        entry.value = template_bin.get_section(".dynsym").virtual_address

    elif entry.tag == int(ELF.DYNAMIC_TAGS.SYMENT):
        entry.value = old_bin.get(ELF.DYNAMIC_TAGS.SYMENT).value

    elif entry.tag == int(ELF.DYNAMIC_TAGS.STRTAB):
        entry.value = template_bin.get_section(".dynstr").virtual_address

    elif entry.tag == int(ELF.DYNAMIC_TAGS.STRSZ):
        entry.value = template_bin.get_section(".dynstr").size

    elif entry.tag == int(ELF.DYNAMIC_TAGS.GNU_HASH):
        entry.value = template_bin.get_section(".gnu.hash").virtual_address

    elif entry.tag == int(ELF.DYNAMIC_TAGS.HASH):
        entry.value = template_bin.get_section(".hash").virtual_address

    elif entry.tag == int(ELF.DYNAMIC_TAGS.NEEDED):
        pass

    elif entry.tag == int(ELF.DYNAMIC_TAGS.SONAME):
        pass

    elif entry.tag == int(ELF.DYNAMIC_TAGS.FINI_ARRAY):
        entry.value = template_bin.get_section(".fini_array").virtual_address

    elif entry.tag == int(ELF.DYNAMIC_TAGS.FINI_ARRAYSZ):
        entry.value = template_bin.get_section(".fini_array").size

    elif entry.tag == int(ELF.DYNAMIC_TAGS.FLAGS):
        entry.value = old_bin.get(ELF.DYNAMIC_TAGS.FLAGS).value

    elif entry.tag == int(ELF.DYNAMIC_TAGS.FLAGS_1):
        entry.value = old_bin.get(ELF.DYNAMIC_TAGS.FLAGS_1).value

    elif entry.tag == int(ELF.DYNAMIC_TAGS.VERSYM):
        # entry.value = template_bin.get_section(".gnu.version").virtual_address
        continue

    elif entry.tag == int(ELF.DYNAMIC_TAGS.VERDEF):
        # entry.value = template_bin.get_section(".gnu.version_d").virtual_address
        continue

    elif entry.tag == int(ELF.DYNAMIC_TAGS.VERDEFNUM):
        # entry.value = old_bin.get(ELF.DYNAMIC_TAGS.VERDEFNUM).value
        continue

    elif entry.tag == int(ELF.DYNAMIC_TAGS.VERNEED):
        # entry.value = template_bin.get_section(".gnu.version_r").virtual_address
        continue

    elif entry.tag == int(ELF.DYNAMIC_TAGS.VERNEEDNUM):
        # entry.value = old_bin.get(ELF.DYNAMIC_TAGS.VERNEEDNUM).value
        continue

    elif entry.tag == int(ELF.DYNAMIC_TAGS.INIT):
        entry.value = init_proc_offset + 8 + 1 # 前8个字节是数据 thumb + 1

    else:
        print("未处理的tag:", hex(entry.tag))
        entry.value = old_bin.get(ELF.DYNAMIC_TAGS(entry.tag)).value

    print("fix entry:", hex(entry.tag), hex(entry.value))
    new_dynamic_fix_content += entry.content

# 补齐删除的tag内容
new_dynamic_fix_content += (len(new_dynamic_content) - len(new_dynamic_fix_content)) * [0]
print("patch dynamic section content")
patch_file(bin_out2_path, binary.get_section(".dynamic").offset, new_dynamic_fix_content)

# 写入添加的数据
# 首先是新的.rel.plt
# 这里需要填充一些重定位项中新增项的offset
new_relplt_entries = [new_relplt_content[i: i+8] for i in range(0, len(new_relplt_content), 8)]
modify_relplt_entries = list()
orig_relplt_entrise = list()
modify_relplt_content = []

# 先把新的content封装成RelEntry
for item in new_relplt_entries:
    entry = RelEntry()
    entry.content = item
    modify_relplt_entries.append(entry)

# 把原始的条目转换成RelEntry
for item in out2_plt:
    print("orig_entry:", item)
    orig_relplt_entrise.append(RelEntry.parse_from_lief(item))

# 先把旧的条目的offset修复
# for ndx in range(len(bin_out2.pltgot_relocations)):
for ndx in range(len(out2_plt)):
    new_entry = modify_relplt_entries[ndx]
    old_entry = orig_relplt_entrise[ndx]
    if new_entry.info != old_entry.info:
        print("[warning] found a pair entries which should have same info have different info index:",ndx)
        print("[warining] new_entry:", new_entry.content)
        print("[warining] old_entry:", old_entry.content)
        print(out2_plt[ndx])
        modify_relplt_content += old_entry.content
        continue

    if old_entry != 8 * [0]:
        new_entry.offset = old_entry.offset

    print("fix old relptl entry:",new_entry.content)
    modify_relplt_content += new_entry.content

# 再把新的条目的offset修复
for ndx in range(len(bin_out2.pltgot_relocations), len(modify_relplt_entries)):
    new_entry = modify_relplt_entries[ndx]
    new_entry.offset = add_relplt_info_list[new_entry.offset] + add_RE_seg.virtual_address
    print("fix new relptl entry:", new_entry.content)
    modify_relplt_content += new_entry.content

print("write new relplt content")
patch_file(bin_out2_path, add_content_offset, modify_relplt_content)

print("write new plt content")
patch_file(bin_out2_path, add_content_offset + add_plt.file_offset, add_plt.content)

print("write new text content")
patch_file(bin_out2_path, add_content_offset + add_text.file_offset, add_text.content)

print("write new got content")
patch_file(bin_out2_path, add_content_offset + add_got.file_offset, add_got.content)

print("write new data content")
patch_file(bin_out2_path, add_content_offset + add_data.file_offset, add_data.content)

print("write init_proc content")
INIT_PROC_CONTENT = list(struct.pack("<I",init_proc_offset)) + INIT_PROC_CONTENT[4:]
# 注意thumb/arm


INIT_PROC_CONTENT = INIT_PROC_CONTENT[:4] + list(struct.pack("<I",MAIN_OFFSET + add_RE_seg.virtual_address)) + INIT_PROC_CONTENT[8:]
patch_file(bin_out2_path, init_proc_offset, INIT_PROC_CONTENT)
