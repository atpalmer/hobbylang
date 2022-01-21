#ifndef EVAL_H
#define EVAL_H

#include "ast.h"
#include "mapobj.h"

Object *eval_ast(AstNode *ast, Object *varmap);
Object *eval_stream(FILE *stream, Object *varmap);
Object *eval_string(const char *program, Object *varmap);

#endif
