#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "mapobj.h"

typedef struct {
    Object *varmap;
} Interpreter;

Interpreter *interpreter_new(void);
void interpreter_free(Interpreter *this);
Object *interpreter_parse_line(Interpreter *this, FILE *stream);

#endif
