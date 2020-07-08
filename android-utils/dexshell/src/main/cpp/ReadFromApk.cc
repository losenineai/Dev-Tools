//
// Created by 冯永健 on 2019/2/26.
//

#include "ReadFromApk.h"

static size_t readFromUnzhandle(unzFile uf, unz_file_info64 *file_info, uint8_t **ppdata) {
    if (ppdata == NULL || file_info == NULL) {
        return 0;
    }
    *ppdata = NULL;

    size_t uncompressed_size = file_info->uncompressed_size;

    errno = 0;
    if (UNZ_OK != unzOpenCurrentFilePassword(uf, NULL)) {
        LOGE("open current error! errno = %d %s\n", errno, strerror(errno));
        return 0;
    }

    if (uncompressed_size <= 0) {
        return 0;
    }

    uint8_t *content = (uint8_t *) calloc(sizeof(uint8_t), uncompressed_size);
    if (content == NULL) {
        LOGE("Not enough memory.");
        return 0;
    }

    if (uncompressed_size != unzReadCurrentFile(uf, content, uncompressed_size)) {
        LOGE("read content error!\n");
        free(content);
        content = NULL;
        return 0;
    }

    *ppdata = content;
    return uncompressed_size;
}


size_t readFromApk(const char *apkFilepath, const char *filename, uint8_t **ppdata) {
    unzFile uf = NULL;
    unz_file_info64 file_info = {0};
    char filename_inzip[256] = {0};
    int err = UNZ_OK;
    unz_global_info64 gi = {0};

    if (ppdata == NULL) {
        return 0;
    }
    *ppdata = NULL;

    uf = unzOpen64(apkFilepath);
    if (uf == NULL) {
        LOGE("open apk file failed! err = %d\n", errno);
        return 0;
    }

    err = unzGetGlobalInfo64(uf, &gi);
    if (err != UNZ_OK) {
        LOGE("error %d with zipfile in unzGetGlobalInfo \n", err);
        return 0;
    }

    err = unzGoToFirstFile(uf);

    uint8_t *content = NULL;
    size_t filesize = 0;
    for (int i = 0; i < gi.number_entry; i++) {
        if (err != UNZ_OK) {
            LOGE("get file error!\n");
            return 0;
        }

        if (unzGetCurrentFileInfo64(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0,
                                    NULL, 0)) {
            LOGE("get file infomation error!\n");
            return 0;
        }

        int name_len = strlen(filename_inzip);
        if (name_len != file_info.size_filename) {
            LOGE("file name length is not right!\n");
            return 0;
        }

        if (name_len >= 11) {
            // LOGD("filename: %s", filename_inzip);
            if (0 == strcmp(filename_inzip, filename)) {
                filesize = readFromUnzhandle(uf, &file_info, &content);
                if (filesize == 0) {
                    LOGE("Cache ontent is null.");
                }
                break;
            }
        }

        err = unzGoToNextFile(uf);
    }

    if (filesize == 0) {
        LOGE("cannot find the file!\n");
        return false;
    }

    unzCloseCurrentFile(uf);
    unzClose(uf);

    *ppdata = content;
    return filesize;
}