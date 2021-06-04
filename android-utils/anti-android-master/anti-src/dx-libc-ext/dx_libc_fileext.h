//
// Created by amlloc on 18-1-30.
//

#ifndef SYSCALLDEMO2_DXFILEEXT_H
#define SYSCALLDEMO2_DXFILEEXT_H

#include "dx_libc_wcio.h"
#include <pthread.h>
#include "dx_libc_stdio.h"
#include "dx_libc_config.h"

#define _FILEEXT_INITIALIZER  {{NULL,0},{0}}


struct __dxsfileext {
    struct	__dxsbuf _ub; /* ungetc buffer */
    struct dxwchar_io_data _wcio;	/* wide char io status *//* file lock */
};

//struct __dxsfileext __dxsFext[3] = {
//        _FILEEXT_INITIALIZER,
//        _FILEEXT_INITIALIZER,
//        _FILEEXT_INITIALIZER,
//};




//#define _FILEEXT_INITIALIZER  {{NULL,0},{0},PTHREAD_RECURSIVE_MUTEX_INITIALIZER}


#endif //SYSCALLDEMO2_DXFILEEXT_H
