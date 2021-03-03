#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "varmap.h"

void varmap_setval(VarEntry **this, const char *key, double value) {
    VarEntry **curr = this;
    while(*curr) {
        if(strcmp((*curr)->key, key) == 0) {
            (*curr)->value = value;
            return;
        }
        curr = &(*curr)->next;
    }

    VarEntry *new = malloc(sizeof *new);
    error_ensure_errno_ok();

    new->key = strdup(key);
    error_ensure_errno_ok();

    new->value = value;
    new->next = NULL;
    *curr = new;
}

double varmap_getval(VarEntry *this, const char *key) {
    VarEntry *curr = this;
    while(curr) {
        if(strcmp(curr->key, key) == 0)
            return curr->value;
        curr = curr->next;
    }

    fprintf(stderr, "Error: No value for key: %s\n", key);
    exit(-1);
}

void varmap_free(VarEntry *this) {
    if(this->next)
        free(this->next);
    free(this);
}
