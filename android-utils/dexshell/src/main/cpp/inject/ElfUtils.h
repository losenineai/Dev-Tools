//
// Created by my on 18-12-10.
//

#ifndef TESTDEMO_ELFUTILS_H
#define TESTDEMO_ELFUTILS_H

#endif //TESTDEMO_ELFUTILS_H
void getDynFromSo(void *load_addr, void *&dynsym, void *&dynstr, size_t &nsyms);

void *findDynSymAddr(void *load_addr, const char *symbol, void *dynsym, void *dynstr, size_t nsyms);

