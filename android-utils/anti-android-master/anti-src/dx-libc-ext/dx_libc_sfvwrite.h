//
// Created by amlloc on 18-1-31.
//

#ifndef SYSCALLDEMO2_DXFVWRITE_H
#define SYSCALLDEMO2_DXFVWRITE_H

#include <cstdio>

#include "dx_libc_config.h"

struct __dxsiov {
    const void *iov_base;
    size_t iov_len;
};
struct __dxsuio {
    struct __dxsiov *uio_iov;
    int uio_iovcnt;
    int uio_resid;
};



#endif //SYSCALLDEMO2_DXFVWRITE_H
