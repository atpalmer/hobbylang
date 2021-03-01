#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "lexer.h"
#include "parser.h"


static void varmap_setval(VarEntry **this, const char *key, double value) {
    VarEntry **curr = this;
    while(*curr) {
        if(strcmp((*curr)->key, key) == 0) {
            (*curr)->value = value;
            return;
        }
        curr = &(*curr)->next;
    }

    VarEntry *new = malloc(sizeof *new);
    new->key = strdup(key);
    new->value = value;
    new->next = NULL;
    *curr = new;
}

static double varmap_getval(VarEntry *this, const char *key, double default_) {
    VarEntry *curr = this;
    while(curr) {
        if(strcmp(curr->key, key) == 0)
            return curr->value;
        curr = curr->next;
    }
    return default_;
}

Parser *parser_new(const char *program) {
    Parser *new = malloc(sizeof *new);
    new->lexer = lexer_new(program);
    new->varmap = NULL;
    return new;
}

void parser_free(Parser *this) {
    free(this->lexer);
    VarEntry *entry = this->varmap;
    while(entry) {
        free(entry);
        entry = entry->next;
    }
    free(this);
}

double parser_next_atom(Parser *this) {
    Token *curr = lexer_peek(this->lexer);
    if(!curr)
        goto done;

    switch(token_type(curr)) {
    case TOKT_NUMBER:
        lexer_handle(this->lexer, curr);
        return token_number(curr);
    case TOKT_SUB:
        lexer_handle(this->lexer, curr);
        return -parser_next_atom(this);
    default:
        break;
    }

    token_free(&curr);

    fprintf(stderr, "Parse error. Position: %d. Expected: TOKT_NUMBER. Found: '%c' (%d)\n",
            this->lexer->pos, token_type(curr), token_type(curr));
    exit(-1);

done:
    fprintf(stderr, "Parse error. Position: %d. Expected: TOKT_NUMBER. Found: EOF\n",
            this->lexer->pos, token_type(curr), token_type(curr));
    exit(-1);
}

double parser_next_term(Parser *this) {
    double result = parser_next_atom(this);

    Token *op = lexer_peek(this->lexer);
    if(!op)
        goto done;

    switch(token_type(op)) {
    case TOKT_MULT:
        lexer_handle(this->lexer, op);
        result *= parser_next_term(this);
        break;
    case TOKT_DIV:
        lexer_handle(this->lexer, op);
        result /= parser_next_term(this);
        break;
    default:
        break;
    }

    token_free(&op);

done:
    return result;
}

double parser_next_expr(Parser *this) {
    double result = parser_next_term(this);

    Token *op = lexer_peek(this->lexer);
    if(!op)
        goto done;

    switch(token_type(op)) {
    case TOKT_ADD:
        lexer_handle(this->lexer, op);
        result += parser_next_expr(this);
        break;
    case TOKT_SUB:
        lexer_handle(this->lexer, op);
        result -= parser_next_expr(this);
        break;
    default:
        break;
    }

    token_free(&op);

done:
    return result;
}

int parser_has_next(Parser *this) {
    return lexer_has_next(this->lexer);
}
