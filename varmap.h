#ifndef VARMAP_H
#define VARMAP_H

typedef struct VarEntry {
    char *key;
    double value;
    struct VarEntry *next;
} VarEntry;

void varmap_setval(VarEntry **this, const char *key, double value);
double varmap_getval(VarEntry *this, const char *key);

#endif
