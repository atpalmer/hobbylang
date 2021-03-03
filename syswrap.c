#include <stdlib.h>
#include "error.h"
#include "syswrap.h"

void *malloc_or_die(size_t size) {
    void *p = malloc(size);
    error_ensure_errno_ok();
    return p;
}

FILE *fopen_or_die(const char *filename, const char *mode) {
    FILE *fp = fopen(filename, mode);
    error_ensure_errno_ok();
    return fp;
}
