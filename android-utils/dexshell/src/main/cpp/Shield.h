#ifndef DX_SHIELD_H__
#define DX_SHIELD_H__

#include <stdint.h>
#include <sys/types.h>

//#define ELF_MAGIC 0x464C457F

typedef struct methodEntry{
    void* point1;
    uint16_t data1;
    uint16_t data2;
    uint16_t data3;
    uint16_t data4;
    uint16_t data5;
    uint16_t data6;
    void *point2;
    void *point3;
    void *nativeFunc;
    void *point4;
    uint16_t data7;
    uint16_t data8;
    void *point5;
    uint32_t data9;
}VMethodEntryStruct;

typedef struct HashTable
{
    int i1;
    int i2;
    int i3;
    void* p1;
    void* p2;
    pthread_mutex_t lock;
} HashTable;

struct JNIEnvExt {
    const struct JNINativeInterface* funcTable;     /* must be first */

    const struct JNINativeInterface* baseFuncTable;

    u4      envThreadId;
    Thread* self;

    /* if nonzero, we are in a "critical" JNI call */
    int     critical;

    struct JNIEnvExt* prev;
    struct JNIEnvExt* next;
};

struct RawDexFile {
    char*       cacheFileName;
    DvmDex*     pDvmDex;
};

struct DexOrJar {
    char*       fileName;
    bool        isDex;
    bool        okayToFree;
    RawDexFile* pRawDexFile;
    JarFile*    pJarFile;
    u1*         pDexMemory;
};

#endif
