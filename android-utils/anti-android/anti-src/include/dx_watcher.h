/*************************************
 * @Author  tao.ma @ dingxiang-inc
 * @E-mail  373075025@qq.com
 *************************************/
#ifndef dx_watcher_h
#define dx_watcher_h

#include <list>
#include <string>
#include <cstring>

#define INVALID_WD  -1
#define INVALID_FD  -1

extern bool watcher_init();
extern void watcher_uninit();
extern bool watcher_add(const char* filepath);
extern void watcher_auto_remove(std::list<std::string> &paths);
extern void watcher_dispose(void);

#endif // dx_watcher_h