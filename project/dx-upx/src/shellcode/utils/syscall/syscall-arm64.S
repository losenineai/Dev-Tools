#define MAX_ERRNO 4095

.text
.align 4

.globl dx_syscall
dx_syscall:
/* Move syscall No. from x0 to x8 */
    mov     x8, x0
    /* Move syscall parameters from x1 thru x6 to x0 thru x5 */
    mov     x0, x1
    mov     x1, x2
    mov     x2, x3
    mov     x3, x4
    mov     x4, x5
    mov     x5, x6
    svc     #0

    /* check if syscall returned successfully 
    cmn     x0, #(MAX_ERRNO + 1)
    cneg    x0, x0, hi
    /*b.hi    __set_errno_internal*/

    ret

.global DX_ShellcodeEnd
DX_ShellcodeEnd:
