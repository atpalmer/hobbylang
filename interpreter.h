#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include "varmap.h"

typedef struct {
    Parser *parser;
    VarEntry *varmap;
    FILE *stream;  /* only set if interpreter-created;
                    * NULL if given a file handle directly */
} Interpreter;

Interpreter *interpreter_from_string(const char *program);
Interpreter *interpreter_from_stream(FILE *stream);
void interpreter_free(Interpreter *this);
int interpreter_parse_line(Interpreter *this, double *result);

#endif
