# -*- coding: utf-8 -*-
#!/usr/bin/python

import os, sys, re, StringIO
import fcntl, termios, struct

reload(sys)
sys.setdefaultencoding("utf-8")

class COLOR:
    WHITE = '\033[37m'
    GRAY = '\033[30m'
    BLUE = '\033[34m'
    GREEN = '\033[92m'
    YELLOW = '\033[33m'
    RED = '\033[91m'
    PURPLE = '\033[35m'
    ENDC = '\033[1;m'


adb_args = ' '.join(sys.argv[1:])

if os.isatty(sys.stdin.fileno()):
    input = os.popen("adb logcat -v time %s" % adb_args)
else:
    input = sys.stdin

while True:
    try:
        line = input.readline()
    except KeyboardInterrupt:
        break
    
    if len(line) == 0: break;
    else:
        outcolor = COLOR.WHITE

        strArr = line.split()

        # 是否纯字母
        if (line[0].isalpha()):
            level = line[0]
        elif len(strArr) >= 5:
            level = strArr[2][0]
        else:
            print outcolor + line + COLOR.ENDC
            continue;

        if level == 'E':
            outcolor = COLOR.RED
        elif level == 'D':
            outcolor = COLOR.BLUE
        elif level == 'V':
            outcolor = COLOR.WHITE
        elif level == 'W':
            outcolor = COLOR.YELLOW
        elif level == 'I':
            outcolor = COLOR.GREEN
        elif level == 'F':
            outcolor = COLOR.PURPLE
        line = line.strip()        
        print outcolor + line + COLOR.ENDC


            
