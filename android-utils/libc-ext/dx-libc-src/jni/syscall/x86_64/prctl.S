/* Generated by gensyscalls.py. Do not edit. */

#include "../private/bionic_asm.h"

ENTRY(dx_prctl)
    movq    %rcx, %r10
    movl    $__NR_prctl, %eax
    syscall
    cmpq    $-MAX_ERRNO, %rax
    jb      1f
    negl    %eax
    movl    %eax, %edi
    call    __set_errno_internal
1:
    ret
END(dx_prctl)
