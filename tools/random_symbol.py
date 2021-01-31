import random as rd
from datetime import time

ns = 'O0o1lLiIg9'
rr = rd.Random()

def name_gen(n):
    g = lambda: ns[rr.randint(1, len(ns)) - 1]
    r = ''

    for i in range(1, n + 1, 1):
        if len(r) == 0:
            while True:
                gg = g()

                if ord(gg) - ord('0') > 9:
                    r += gg
                    break
        else:
            r += g()

    return r

if __name__ == '__main__' :
    for i in range(0, 150):
        print name_gen(20)

