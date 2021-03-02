#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
    if(!new)
        goto fail;
    new->key = strdup(key);
    if(!new->key)
        goto fail;
    new->value = value;
    new->next = NULL;
    *curr = new;
    return;

fail:
    perror("Error");
    exit(errno);
}

static double varmap_getval(VarEntry *this, const char *key) {
    VarEntry *curr = this;
    while(curr) {
        if(strcmp(curr->key, key) == 0)
            return curr->value;
        curr = curr->next;
    }

    fprintf(stderr, "Error: No value for key: %s\n", key);
    exit(-1);
}

Parser *parser_new(const char *program) {
    Parser *new = malloc(sizeof *new);
    if(!new)
        goto fail;
    new->lexer = lexer_new(program);
    new->varmap = NULL;
    return new;

fail:
    perror("Error");
    exit(errno);
}

void parser_continue(Parser *this, const char *program) {
    free(this->lexer);
    this->lexer = lexer_new(program);
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

static inline void parser_error(Lexer *lexer, const char *expected) {
    fprintf(stderr, "Parse error. Position: %d. Expected: %s. Found: '%c' (%d)\n",
        lexer->pos, expected, token_type(lexer->peek), token_type(lexer->peek));
}

double parser_next_expr(Parser *this);

double parser_handle_variable(Parser *this) {
    Token *peek = lexer_peek(this->lexer);

    if(token_type(peek) != TOKT_IDENTIFIER)
        goto fail;

    char *key = strdup(token_varname(peek));
    lexer_consume_peek(this->lexer);

    peek = lexer_peek(this->lexer);

    double result;

    if(token_type(peek) == TOKT_EQ) {
        lexer_consume_peek(this->lexer);
        result = parser_next_expr(this);
        varmap_setval(&this->varmap, key, result);
    } else {
        result = varmap_getval(this->varmap, key);
    }

    free(key);
    return result;

fail:
    parser_error(this->lexer, "TOKT_IDENTIFIER");
    exit(-1);
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
    case TOKT_LPAREN:
        {
            lexer_consume_peek(this->lexer);
            double result = parser_next_expr(this);
            Token *rparen = lexer_peek(this->lexer);
            if(token_type(rparen) != TOKT_RPAREN) {
                parser_error(this->lexer, "TOKT_RPAREN");
                exit(-1);
            }
            lexer_consume_peek(this->lexer);
            return result;
        }
    case TOKT_IDENTIFIER:
        {
            return parser_handle_variable(this);
        }
    default:
        break;
    }

    parser_error(this->lexer, "TOKT_NUMBER");
    exit(-1);
}

double parser_next_term(Parser *this) {
    double result = parser_next_atom(this);

    for(;;) {
        Token *peek = lexer_peek(this->lexer);

        switch(token_type(peek)) {
        case TOKT_MULT:
            lexer_consume_peek(this->lexer);
            result *= parser_next_atom(this);
            break;
        case TOKT_DIV:
            lexer_consume_peek(this->lexer);
            result /= parser_next_atom(this);
            break;
        case TOKT_MOD:
            lexer_consume_peek(this->lexer);
            result = fmod(result, parser_next_atom(this));
            break;
        default:
            goto done;
        }
    }

done:
    return result;
}

double parser_next_expr(Parser *this) {
    double result = parser_next_term(this);

    for(;;) {
        Token *peek = lexer_peek(this->lexer);

        switch(token_type(peek)) {
        case TOKT_ADD:
            lexer_consume_peek(this->lexer);
            result += parser_next_term(this);
            break;
        case TOKT_SUB:
            lexer_consume_peek(this->lexer);
            result -= parser_next_term(this);
            break;
        default:
            goto done;
        }
    }

done:
    return result;
}

double parser_next_line(Parser *this) {
    double result = parser_next_expr(this);

    Token *peek = lexer_peek(this->lexer);

    if(token_type(peek) != TOKT_NEWLINE)
        goto fail;

    lexer_consume_peek(this->lexer);
    return result;

fail:
    parser_error(this->lexer, "TOKT_NEWLINE");
    exit(-1);
}

int parser_has_next(Parser *this) {
    return lexer_peek(this->lexer) != NULL;
}
