
inline void deadcall() {
#ifdef M_ARM64
    asm(
        "\tmov x0, 0x1000\n"
        "\tbr x0\n"
        "\t.word 0xde00"
    );
#elif M_ARM
    asm(
        "\tmov lr, 0x1000\n"
        "\tmov r0, 0x1000\n"
        "\tpop {pc}\n"
        //"\tmov pc, 0x1000\n"
    );
#else
    void (*invalid_call)() = (void*)0x1000;
    invalid_call();
    asm(
        "\t.WORD 0xdea0\n"
    );
#endif

}