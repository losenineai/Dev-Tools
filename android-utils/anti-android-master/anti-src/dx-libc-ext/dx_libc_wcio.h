//
// Created by amlloc on 18-1-30.
//

#ifndef SYSCALLDEMO2_DXWCIO_H
#define SYSCALLDEMO2_DXWCIO_H

#include "dx_libc_config.h"

/* minimal requirement of SUSv2 */
#define WCIO_UNGETWC_BUFSIZE 1
struct dxwchar_io_data {
    int dummy;
};

/* BIONIC: disable wchar support */
#define dxWCIO_GET(fp) \
	((struct dxwchar_io_data*) 0)

#define dx_SET_ORIENTATION(fp, mode) ((void)0)

/*
 * WCIO_FREE should be called by fclose
 */
#define dxWCIO_FREE(fp) ((void)(0))

#define dxWCIO_FREEUB(fp) ((void)0)

#define dxWCIO_INIT(fp)  ((void)0)


#endif //SYSCALLDEMO2_DXWCIO_H
