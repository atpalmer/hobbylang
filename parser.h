#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "varmap.h"

typedef struct {
    Lexer *lexer;
    Token *curr;
    VarEntry *varmap;
} Parser;

Parser *parser_new(const char *program);
void parser_set_buff(Parser *this, const char *program);
void parser_free(Parser *this);
int parser_has_next(Parser *this);
double parser_line(Parser *this);
void parser_setlinevar(Parser *this, double value);

#endif
