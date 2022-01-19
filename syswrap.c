#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "syswrap.h"


void *malloc_or_die(size_t size) {
    void *p = malloc(size);
    if(!p)
        sysdie(MemoryError);
    return p;
}

FILE *fopen_or_die(const char *filename, const char *mode) {
    FILE *fp = fopen(filename, mode);
    if(!fp)
        sysdie(OSError);
    return fp;
}

char *strdup_or_die(const char *s) {
    char *p = strdup(s);
    if(!p)
        sysdie(MemoryError);
    return p;
}
