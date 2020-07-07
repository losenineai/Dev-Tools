#include "dx_hook_prefix.h"
#include "closure-bridge-arm64.h"
#include "backend-arm64-helper.h"
#include <string.h>

#include <CommonKit/log/log_kit.h>

#include <sys/mman.h>
#include <unistd.h>

#define closure_bridge_trampoline_template_length (7 * 4)

static ClosureBridgeTrampolineTable *gClosureBridageTrampolineTable;

void common_bridge_handler(RegState *rs, ClosureBridgeData *cbd) {

    USER_CODE_CALL userCodeCall = cbd->user_code;
    // printf("CommonBridgeHandler:");
    // printf("\tTrampoline Address: %p", cbd->redirect_trampoline);
    userCodeCall(rs, cbd);
    // set return address
    rs->general.x[17] = rs->general.x[17];
    return;
}

static ClosureBridgeTrampolineTable *ClosureBridgeTrampolineTableAllocate(void) {
    void *mmap_page;
    long page_size;
    page_size = sysconf(_SC_PAGESIZE);

    mmap_page = mmap(0, 1, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    if (mmap_page == MAP_FAILED) {
        COMMON_ERROR_LOG();
        return NULL;
    }

    if (mprotect(mmap_page, (size_t)page_size, (PROT_READ | PROT_WRITE))) {
        COMMON_ERROR_LOG();
        return NULL;
    }

    int t              = page_size / closure_bridge_trampoline_template_length;
    void *copy_address = mmap_page;
    for (int i = 0; i < t; ++i) {
        copy_address = (void *)((intptr_t)mmap_page + i * closure_bridge_trampoline_template_length);
        memcpy(copy_address, closure_bridge_trampoline_template, closure_bridge_trampoline_template_length);
    }

    if (mprotect(mmap_page, (size_t)page_size, (PROT_READ | PROT_EXEC))) {
        COMMON_ERROR_LOG();
        return NULL;
    }

    ClosureBridgeTrampolineTable *table = (ClosureBridgeTrampolineTable *)malloc(sizeof(ClosureBridgeTrampolineTable));
    table->entry                        = mmap_page;
    table->trampoline_page              = mmap_page;
    table->used_count                   = 0;
    table->free_count                   = (uint16_t)t;
    return table;
}

static void ClosureBridgeTrampolineTableFree(ClosureBridgeTrampolineTable *table) { return; }

ClosureBridgeData *ClosureBridgeAllocate(void *user_data, void *user_code) {
    long page_size                      = sysconf(_SC_PAGESIZE);
    ClosureBridgeTrampolineTable *table = gClosureBridageTrampolineTable;
    if (table == NULL || table->free_count == 0) {
        table = ClosureBridgeTrampolineTableAllocate();
        if (table == NULL)
            return NULL;

        table->next = gClosureBridageTrampolineTable;
        if (table->next != NULL) {
            table->next->prev = table;
        }
        gClosureBridageTrampolineTable = table;
    }

    ClosureBridgeData *bridgeData     = (ClosureBridgeData *)malloc(sizeof(ClosureBridgeData));
    // bridgeData->common_bridge_handler = (void *)common_bridge_handler;
    bridgeData->common_bridge_handler = (void *)i1ggoiIgll1i99iIoliOl1I1I9IIO9I0;

    bridgeData->user_code           = user_code;
    bridgeData->user_data           = user_data;
    uint16_t trampoline_used_count  = gClosureBridageTrampolineTable->used_count;
    bridgeData->redirect_trampoline = (void *)((intptr_t)gClosureBridageTrampolineTable->trampoline_page +
                                               closure_bridge_trampoline_template_length * trampoline_used_count);

    if (mprotect(gClosureBridageTrampolineTable->trampoline_page, (size_t)page_size, (PROT_READ | PROT_WRITE))) {
        COMMON_ERROR_LOG();
        return NULL;
    }

    // bind data to trampline
    void *tmp = (void *)((intptr_t)bridgeData->redirect_trampoline + 4 * 3);
    memcpy(tmp, &bridgeData, sizeof(ClosureBridgeData *));

    // set trampoline to bridge
    // void *tmpX = (void *)closure_bridge_template;
    void *tmpX = (void *)g0OI1OI1iI0oi9Iog0LL0g190LOIOl90;
    tmp        = (void *)((intptr_t)bridgeData->redirect_trampoline + 4 * 5);
    memcpy(tmp, &tmpX, sizeof(void *));

    if (mprotect(gClosureBridageTrampolineTable->trampoline_page, (size_t)page_size, (PROT_READ | PROT_EXEC))) {
        COMMON_ERROR_LOG();
        return NULL;
    }

    table->used_count++;
    table->free_count--;

    return bridgeData;
}

static void ClosureBridgeFree(ClosureBridgeData *bridgeData) { return; }