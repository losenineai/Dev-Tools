
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>

#include <android/log.h>
#include <sys/system_properties.h>
#include "../dx_libc_prefix.h"

#define PROP_NAME_MAX  32
#define PROP_VALUE_MAX 92

#define AREA_SIZE         (128 * 1024)
#define AREA_DATA_SIZE     (AREA_SIZE - sizeof(prop_area))

#define ANDROID_N   24
#define ANDROID_O   26

#define LOG_TYPE_CONSOLE	1
#define LOG_TYPE_LOGCAT 	2
#define LOG_BUFFER	1024

#define ALIGN(x, alignment)   ((x) + (sizeof(alignment) - 1) & ~(sizeof(alignment) -1))

#define LOG_TAG "properties"
#define LOGD(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#define PROPERTIES_FILE  "/dev/__properties__"

__BEGIN_DECLS
/** 属性前缀 */
typedef struct prefix_node {
    char name[PROP_NAME_MAX];
    struct context_node *context;
    struct prefix_node *next;
} prefix_node;

/** 属性对应的security context */
typedef struct context_node {
    char name[PROP_VALUE_MAX];
    void *mem;
    struct context_node *next;
} context_node;

typedef struct prop_bt {
    uint8_t namelen;
    uint8_t reserved[3];
    uint32_t prop;
    uint32_t left;
    uint32_t right;
    uint32_t children;
    char name[0];
} prop_bt;

/** 保存属性 key value */
typedef struct prop_info {
    uint32_t serial;
    char value[PROP_VALUE_MAX];
    char name[0];
} prop_info;

typedef struct prop_info_low {
    char name[PROP_NAME_MAX];
    unsigned volatile serial;
    char value[PROP_VALUE_MAX];
} prop_info_low;


typedef struct prop_area {
    uint32_t bytes_used;
    uint32_t serial;
    uint32_t magic;
    uint32_t version;
    uint32_t reserved[28];
    char data[0];
} prop_area;

typedef struct prop_area_low {
    unsigned volatile count;
    unsigned volatile serial;
    unsigned magic;
    unsigned version;
    unsigned reserved[4];
    unsigned toc[1];
} prop_area_low;

#define TOC_NAME_LEN(toc)       ((toc) >> 24)
#define TOC_TO_INFO(area, toc)  ((prop_info_low*) (((char*) area) + ((toc) & 0xFFFFFF)))


prefix_node *g_prefixs = NULL;
context_node *g_contexts = NULL;
int g_log_type = LOG_TYPE_CONSOLE + LOG_TYPE_LOGCAT; // 默认输出到logcat和console
bool g_need_security_context = false;
char *g_current_security_context = NULL;


void init_properties();
int get_property_value(const char *prop_name, char *prop_value, const char* default_value);
void cleanup_resource();

void add_prefix_node(prefix_node *node);
prefix_node *get_prefix_node(const char *prop_name);
void add_context_node(context_node *node);
context_node *get_context_node(const char *context_name);
bool initialize_contexts(const char *context_file);
prop_area *map_prop_area(const char *file_name, bool need_write);
prop_bt *get_prop_bt(prop_area *p_area, uint32_t off);
prop_bt *new_prop_bt(prop_area *p_area, const char *name, uint8_t namelen, uint32_t *off);
prop_info *get_prop_info(prop_area *p_area, uint32_t off);
prop_info *new_prop_info(prop_area *p_area, const char *prop_name, uint8_t namelen, uint32_t *off);
int get_sdk_version();
char *get_security_context(char *prop_name);
void recursive(prop_area *p_area, uint32_t off);
bool dump_properties_from_file(const char *file_name);
void dump_all();
int cmp_prop_name(const char *one, uint8_t one_len, const char *two, uint8_t two_len);
void *find_prop_info(prop_area *area, const char *prop_name, bool need_add);


__END_DECLS