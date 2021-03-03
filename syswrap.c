#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syswrap.h"


void *malloc_or_die(size_t size) {
    void *p = malloc(size);
    if(!p)
        goto die;
    return p;

die:
    perror("MemoryError");
    exit(errno);
}

FILE *fopen_or_die(const char *filename, const char *mode) {
    FILE *fp = fopen(filename, mode);
    if(!fp)
        goto die;
    return fp;

die:
    perror("OSError");
    exit(errno);
}

char *strdup_or_die(const char *s) {
    char *p = strdup(s);
    if(!p)
        goto die;
    return p;

die:
    perror("MemoryError");
    exit(errno);
}
