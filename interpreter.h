#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include "varmap.h"

typedef struct {
    Parser *parser;
    VarEntry *varmap;
} Interpreter;

Interpreter *interpreter_new(const char *program);
void interpreter_free(Interpreter *this);
void interpreter_set_buff(Interpreter *this, const char *buff);
double interpreter_parse_line(Interpreter *this);

#endif
