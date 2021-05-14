# -*- coding: utf-8 -*-
import os
import struct


class DynSymEntry(object):
    def __init__(
        self,
        name,
        value,
        size,
        info,
        other,
        shndx
    ):
        self.st_name = name
        self.sym_value = value
        self.st_size = size
        self.st_info = info
        self.st_other = other
        self.st_shndx = shndx


    def __init__(
            self, list_data
    ):

        # testlist = [1, "22"]
        # int1, str2 = struct.unpack('I3s', *tuple(testlist))
        # print("int:", int1)
        # print("str:", str2)
        #
        # name, value, size, info, other, shndx = struct.unpack('I3sf', *content)

        self.content = list_data
        self.st_name = list_data[0:4]
        self.sym_value = 0x00020616
        self.st_size = list_data[8:12]
        self.st_info = list_data[12]
        self.st_other = list_data[13]
        self.st_shndx = list_data[14:]

        print("DynSymEntry content", self.content)
        print("st_name", self.st_name)
        print("sym_value",hex(self.sym_value))
        print("st_size", self.st_size)
        print("st_info", self.st_info)
        print("st_other", self.st_other)
        print("st_shndx", self.st_shndx)

class RelEntry(object):

    def __init__(
            self, content
    ):
        self.content = content
        self.offset = 160
        self.info = content[1]

        print("RelEntry content", self.content)
        print("offset", hex(self.offset))
        print("info", hex(self.info))



