#!/usr/bin/env python3
import lief
import sys
if len (sys.argv) != 4 :
    print("Usage: python %s org.so need.so out.so" % sys.argv[0])
    sys.exit (1)

libnative = lief.parse(sys.argv[1])
libnative.add_library(sys.argv[2]) # Injection!
libnative.write(sys.argv[3])