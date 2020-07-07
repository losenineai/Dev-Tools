#ifndef DXERRNO_RESTORER_H
#define DXERRNO_RESTORER_H

#include <errno.h>

#if !defined(DISALLOW_COPY_AND_ASSIGN)
// DISALLOW_COPY_AND_ASSIGN disallows the copy and operator= functions.
// It goes in the private: declarations in a class.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete
#endif  // !defined(DISALLOW_COPY_AND_ASSIGN)


class ErrnoRestorer {
 public:
  explicit ErrnoRestorer() : saved_errno_(errno) {
  }

  ~ErrnoRestorer() {
    errno = saved_errno_;
  }

  void override(int new_errno) {
    saved_errno_ = new_errno;
  }

 private:
  int saved_errno_;

  DISALLOW_COPY_AND_ASSIGN(ErrnoRestorer);
};

#endif 