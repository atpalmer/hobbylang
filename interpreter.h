#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include "varmap.h"

typedef struct {
    VarEntry *varmap;
} Interpreter;

Interpreter *interpreter_new(void);
void interpreter_free(Interpreter *this);
int interpreter_parse_line(Interpreter *this, FILE *stream, double *result);

#endif
