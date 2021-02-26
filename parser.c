#include <assert.h>
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

double parser_next_expr(Parser *this) {
    Token *curr = lexer_next(this->lexer);

    if(token_type(curr) == TOKT_NUMBER)
        return token_number(curr);
    if(token_type(curr) == TOKT_SUB)
        return -parser_next_expr(this);

    assert(0);  /* TODO: report error */
}

int parser_has_next(Parser *this) {
    return lexer_has_next(this->lexer);
}
