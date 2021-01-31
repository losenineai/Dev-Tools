# coding: utf-8

from hashlib import md5, sha1
from zlib import crc32
import sys

def getMd5(filename): #计算md5
    mdfive = md5()
    with open(filename, 'rb') as f:
        mdfive.update(f.read())
    return mdfive.hexdigest()

def getMd5ByStr(src): #计算md5
    mdfive = md5()
    mdfive.update(src)
    return mdfive.hexdigest()

def getSha1(filename): #计算sha1
    sha1Obj = sha1()
    with open(filename, 'rb') as f:
        sha1Obj.update(f.read())
    return sha1Obj.hexdigest()

def getCrc32(filename): #计算crc32
    with open(filename, 'rb') as f:
        return crc32(f.read())


if __name__ == '__main__':
    # if len(sys.argv) < 2:
    #     print('You must enter the file')
    #     exit(1)
    # elif len(sys.argv) > 2:
    #     print('Only one file is permitted')
    #     exit(1)

    src_str = sys.argv[1]

    print(getMd5ByStr(src_str))