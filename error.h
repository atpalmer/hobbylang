#ifndef ERROR_H
#define ERROR_H

#include <errno.h>

static inline void error_ensure_errno_ok(void) {
    if(!errno)
        return;
    perror("System Error");
    exit(errno);
}

#endif
