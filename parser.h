#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "varmap.h"
#include "ast.h"

typedef struct {
    FILE *stream;
    Token *curr;
} Parser;

Parser *parser_new(FILE *stream);
void parser_free(Parser *this);
AstNode *parser_parse(FILE *stream);

#endif
