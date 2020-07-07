#ifndef TestFunc1_h
#define TestFunc1_h

/*
#include <iom128.h>
//#include <ioavr.h>
//#include <intrinsics.h>
#define CPU_F           7372800UL
#define delay_us(x)     __delay_cycles(x*CPU_F/1000000)
#define delay_ms(x)     __delay_cycles(x*CPU_F/1000)
*/

#ifdef __cplusplus
extern "C" {
#endif

extern int TestSwitch(int id);
extern void Test2();
extern void Test3(int a);

#ifdef __cplusplus
}
#endif

#endif // TestFunc1_h