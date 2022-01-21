#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <errno.h>

#define die_f(errstr, format, ...)\
    do{\
        fprintf(stderr,("%s in %s:\n  " format "\n"),(#errstr),(__func__),__VA_ARGS__);\
        exit(-1);\
    }while(0)

#define die(errstr, msg)            die_f(errstr, "%s", msg)

#define sysdie(errstr)              die(errstr, strerror(errno))

#define DEBUG_CRUMB__   fprintf(stderr, "DEBUG: %s: %s [line: %d]\n", __FILE__, __func__, __LINE__);
#define DEBUG_PTR__(p)  fprintf(stderr, "DEBUG PTR: %s: %s [line: %d]; \"%s\" addr: %p\n", __FILE__, __func__, __LINE__, #p, (void *)p);

#endif
