#ifndef SYSWRAP_H
#define SYSWRAP_H

#include <stdlib.h>
#include <stdio.h>

void *malloc_or_die(size_t size);
FILE *fopen_or_die(const char *filename, const char *mode);
char *strdup_or_die(const char *s);

#endif
