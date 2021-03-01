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
    Token *peek = lexer_peek(this->lexer);

    switch(token_type(peek)) {
    case TOKT_NUMBER:
        {
            double result = token_number(peek);
            lexer_consume_peek(this->lexer);
            return result;
        }
    case TOKT_SUB:
        {
            lexer_consume_peek(this->lexer);
            return -parser_next_atom(this);
        }
    default:
        break;
    }

    fprintf(stderr, "Parse error. Position: %d. Expected: TOKT_NUMBER. Found: '%c' (%d)\n",
            this->lexer->pos, token_type(peek), token_type(peek));
    exit(-1);
}

double parser_next_term(Parser *this) {
    double left = parser_next_atom(this);

    Token *peek = lexer_peek(this->lexer);

    switch(token_type(peek)) {
    case TOKT_MULT:
        lexer_consume_peek(this->lexer);
        return left * parser_next_term(this);
    case TOKT_DIV:
        lexer_consume_peek(this->lexer);
        return left / parser_next_term(this);
    default:
        break;
    }

    return left;
}

double parser_next_expr(Parser *this) {
    double left = parser_next_term(this);

    Token *peek = lexer_peek(this->lexer);

    switch(token_type(peek)) {
    case TOKT_ADD:
        lexer_consume_peek(this->lexer);
        return left + parser_next_expr(this);
    case TOKT_SUB:
        lexer_consume_peek(this->lexer);
        return left - parser_next_expr(this);
    default:
        break;
    }

    return left;
}

int parser_has_next(Parser *this) {
    return lexer_peek(this->lexer) != NULL;
}
