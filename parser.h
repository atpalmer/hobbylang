#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"

typedef struct {
    FILE *stream;
    Token *curr;
} Parser;

AstNode *parser_parse(FILE *stream);

#endif
