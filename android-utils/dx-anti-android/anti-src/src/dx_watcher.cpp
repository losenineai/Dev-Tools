/*************************************
 * @Author  tao.ma @ dingxiang-inc
 * @E-mail  373075025@qq.com
 *************************************/
#include "dx_watcher.h"
#include <pthread.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <stdlib.h>
#include "anti_utils.h"
#include "dx_anti_stee.h"

struct path_node_t
{
    int wd;
    // std::string path;
    char path[256];
};

static pthread_mutex_t l_mtx;
static int l_infd = INVALID_FD;

static char l_ie_buf[1000] = {0};
static int l_cur_len = 0;
static std::list<path_node_t*> l_paths(0);

STEE
void watcher_clear() {
    l_cur_len = 0;
    memset(l_ie_buf, 0, sizeof(l_ie_buf));

    pthread_mutex_lock(&l_mtx);
    for (std::list<path_node_t*>::iterator it = l_paths.begin(); it != l_paths.end(); ++it)
    {
        if (*it != NULL) {
            inotify_rm_watch(l_infd, (*it)->wd);
            free(*it);
        }
    }
    l_paths.clear();
    pthread_mutex_unlock(&l_mtx);
}

STEE
bool watcher_read(int infd, inotify_event** ie) {
    *ie = NULL;
    
    bool flag = dx_select(infd, 500, true);
    if (!flag) {
        return true;
    }

    int ret = read(infd, &l_ie_buf[l_cur_len], sizeof(l_ie_buf) - l_cur_len);
    if (ret < 0) {
        return false;
    } else if (ret == 0) {
        return true;
    }

    l_cur_len += ret;

    if (l_cur_len >= sizeof(inotify_event)) {
        inotify_event* event = (inotify_event* )l_ie_buf;

        int valid_len = sizeof(inotify_event) + event->len;
        if (valid_len > l_cur_len) {
            return true;
        }

        *ie = (inotify_event* )malloc(valid_len + 1);
        if (*ie == NULL) {
            return true;
        }

        // extract inotify_event
        memset(*ie, 0, valid_len + 1);
        memcpy(*ie, l_ie_buf, valid_len);

        // repair l_ie_buf
        l_cur_len = l_cur_len - valid_len;
        memcpy(l_ie_buf, &l_ie_buf[valid_len], l_cur_len);
    }
    return true;
}

/*************** extern ****************/
STEE
bool watcher_init() {
    l_infd = inotify_init();
    if (l_infd == INVALID_FD) {
        return false;
    }
    pthread_mutex_init(&l_mtx, NULL);
    watcher_clear();
    return true;
}

STEE
void watcher_uninit() {
    if (l_infd != INVALID_FD) {
        close(l_infd);
        l_infd = INVALID_FD;
    }
    watcher_clear();
    pthread_mutex_destroy(&l_mtx);
}

STEE
bool watcher_add(const char* filepath) {
    bool flag = false;
    bool finded = false;
    int wd = INVALID_WD;

    // LOGE("%s(%d) filepath: %s", __FUNCTION__, __LINE__, filepath);
    if (l_infd == INVALID_FD) {
        return false;
    }
    path_node_t* node = (path_node_t* )malloc(sizeof(path_node_t));
    if (node == NULL) {
        return false;
    }
    memset(node, 0, sizeof(path_node_t));

    pthread_mutex_lock(&l_mtx);
    for (std::list<path_node_t*>::iterator it = l_paths.begin(); it != l_paths.end(); ++it)
    {
        if (*it != NULL && strcmp(filepath, (*it)->path) == 0) {
            finded = true;
            break;
        }
    }

    if (!finded) {
        wd = inotify_add_watch(l_infd, filepath, IN_ACCESS | IN_OPEN | IN_ISDIR);
    }

    if (wd != INVALID_WD) {
        node->wd = wd;
        memcpy(node->path, filepath, strlen(filepath) + 1);
        l_paths.push_back(node);
        flag = true;
    } else {
        free(node);
    }
    pthread_mutex_unlock(&l_mtx);

    return flag;
}

STEE
void watcher_auto_remove(std::list<std::string> &paths) {
    pthread_mutex_lock(&l_mtx);
    for (std::list<path_node_t*>::iterator it = l_paths.begin(); it != l_paths.end(); ) {
        bool finded = false;
        for (std::list<std::string>::iterator v = paths.begin(); v != paths.end(); ++v) {
            if (*v == (*it)->path) {
                finded = true;
                break;
            }
        }
        if (!finded) {
            inotify_rm_watch(l_infd, (*it)->wd);
            free(*it);
            l_paths.erase(it++);
        } else {
            it++;
        }
    }
    pthread_mutex_unlock(&l_mtx);
}

STEE
void watcher_dispose(void) {

    inotify_event* ie = NULL;
    std::list<path_node_t*>::iterator it;
    bool dumping = false;

    if (l_infd == INVALID_FD) {
        return;
    }

    while (true)
    {
        if (!watcher_read(l_infd, &ie)) {
            break;
        }

        if (NULL != ie) {

            if (ie->wd != INVALID_WD && (IN_ACCESS || (ie->mask & (IN_OPEN | IN_ISDIR))))
            {
                pthread_mutex_lock(&l_mtx);
                for (it = l_paths.begin(); it != l_paths.end(); ++it) {
                    if (*it != NULL && (*it)->wd == ie->wd) {
                        dumping = true;
                        break;
                    }
                }
                pthread_mutex_unlock(&l_mtx);

                if (dumping) {
                    return;
                }
            }
        }

        sleep(1);
    }
}
