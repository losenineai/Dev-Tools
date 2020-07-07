#include <stdio.h>
#include <stdlib.h>
//#include <stdint.h>
#include <string.h>
#include "TestFunc1.h"


int TestSwitch(int id)
{
    switch (id)
    {
        case 1:
            // printf("id-1\n");
            return 1;
            break;
        case 2:
            // printf("id-2\n");
            return 2;
            break;
        case 3:
            // printf("id-3\n");
            return 3;
            break;
        case 4:
            // printf("id-4\n");
            return 4;
            break;
        case 5:
            // printf("id-5\n");
            return 5;
            break;
        case 6:
            // printf("id-6\n");
            return 6;
            break;
        case 7:
            // printf("id-7\n");
            return 7;
            break;
        case 8:
            // printf("id-8\n");
            return 8;
            break;
        case 9:
            // printf("id-9\n");
            return 9;
            break;
        default:
            // printf("id-unknown\n");
            return 100;
            break;
    }
    return 0;
}

//static int l_staticVar = 100;

void Test2()
{
//  void (*func_TestSwitch)(int id) = TestSwitch;
//  func_TestSwitch(2);
//  l_staticVar = 3;
//  func_TestSwitch(l_staticVar);
}

int TestParam1(int a1)
{
    return a1;
}

int TestParam2(int a1, int a2)
{
    return a1 + a2;
}

int TestParam3(int a1, int a2, int a3)
{
    return a1 + a2 + a3;
}

int TestParam4(int a1, int a2, int a3, int a4)
{
    return a1 + a2 + a3 + a4;
}

int TestParam5(int a1, int a2, int a3, int a4, int a5)
{
    return a1 + a2 + a3 + a4 + a5;
}

int TestParam6(int a1, int a2, int a3, int a4, int a5, int a6)
{
    return a1 + a2 + a3 + a4 + a5 + a6;
}

int TestParam7(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    return a1 + a2 + a3 + a4 + a5 + a6 + a7;
}

int TestParam8(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
    return a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8;
}

int TestParam9(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
    return a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9;
}

int TestParam10(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
    return a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10;
}


void Test3(int a)
{
    int ret = 6;
    ret = TestSwitch(3);
    
    //delay_ms(1000);
    
    int b = 0;
    b += TestParam1(1);
    b += TestParam2(1, 2);
    b += TestParam3(1, 2, 3);
    b += TestParam4(1, 2, 3, 4);
    b += TestParam5(1, 2, 3, 4, 5);
    b += TestParam6(1, 2, 3, 4, 5, 6);
    b += TestParam7(1, 2, 3, 4, 5, 6, 7);
    b += TestParam8(1, 2, 3, 4, 5, 6, 7, 8);
    b += TestParam9(1, 2, 3, 4, 5, 6, 7, 8, 9);
    b += TestParam10(1, 2, 3, 4, 5, 6, 7, 8, 9, 12);
    memset(&b, 0, sizeof(b));
    char sz[5] = {0};
    char* x = "aa";
    sprintf(sz, "%s, %d", x, b);
    char* y = "bb";
}


