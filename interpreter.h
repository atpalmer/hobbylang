#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include "varmap.h"
#include "object.h"

typedef struct {
    VarEntry *varmap;
} Interpreter;

Interpreter *interpreter_new(void);
void interpreter_free(Interpreter *this);
Object *interpreter_parse_line(Interpreter *this, FILE *stream);

#endif
