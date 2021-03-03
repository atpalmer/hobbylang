#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "error.h"
#include "lexer.h"
#include "parser.h"

void parser_setlinevar(Parser *this, double value) {
    varmap_setval(&this->varmap, "_", value);
}

Parser *parser_new(const char *program) {
    Parser *new = malloc(sizeof *new);
    error_ensure_errno_ok();

    new->lexer = lexer_new(program);
    new->varmap = NULL;
    return new;
}

void parser_set_buff(Parser *this, const char *program) {
    lexer_free(this->lexer);
    this->lexer = lexer_new(program);
}

void parser_free(Parser *this) {
    lexer_free(this->lexer);
    VarEntry *entry = this->varmap;
    while(entry) {
        VarEntry *next = entry->next;
        free(entry);
        entry = next;
    }
    free(this);
}

void parser_accept(Parser *this, TokenType token_type) {
    Token *peek = lexer_peek(this->lexer);
    error_ensure_token_type(peek, token_type);
    lexer_consume_peek(this->lexer);
}

double parser_next_expr(Parser *this);

double parser_handle_assignment(Parser *this, const char *key) {
    parser_accept(this, TOKT_EQ);
    double result = parser_next_expr(this);
    varmap_setval(&this->varmap, key, result);
    return result;
}

double parser_handle_variable(Parser *this) {
    Token *var = lexer_peek(this->lexer);
    char *key = strdup(token_varname(var));
    error_ensure_errno_ok();
    parser_accept(this, TOKT_IDENTIFIER);

    Token *eq = lexer_peek(this->lexer);

    double result = token_type(eq) == TOKT_EQ
        ? parser_handle_assignment(this, key)
        : varmap_getval(this->varmap, key);

    free(key);
    return result;
}

double parser_handle_parens(Parser *this) {
    parser_accept(this, TOKT_LPAREN);
    double result = parser_next_expr(this);
    parser_accept(this, TOKT_RPAREN);
    return result;
}

double parser_next_number(Parser *this) {
    Token *peek = lexer_peek(this->lexer);
    double result = token_number(peek);
    parser_accept(this, TOKT_NUMBER);
    return result;
}

double parser_next_atom(Parser *this) {
    Token *peek = lexer_peek(this->lexer);

    switch(token_type(peek)) {
    case TOKT_NUMBER:
        return parser_next_number(this);
    case TOKT_SUB:
        parser_accept(this, TOKT_SUB);
        return -parser_next_atom(this);
    case TOKT_LPAREN:
        return parser_handle_parens(this);
    case TOKT_IDENTIFIER:
        return parser_handle_variable(this);
    default:
        break;
    }

    fprintf(stderr, "TokenType Error: Cannot parse atom. Position: %d. Found: '%c' (%d).\n",
        this->lexer->pos, token_type(peek), token_type(peek));
    exit(-1);
}

double parser_next_term(Parser *this) {
    double result = parser_next_atom(this);

    for(;;) {
        Token *peek = lexer_peek(this->lexer);

        switch(token_type(peek)) {
        case TOKT_MULT:
            parser_accept(this, TOKT_MULT);
            result *= parser_next_atom(this);
            break;
        case TOKT_DIV:
            parser_accept(this, TOKT_DIV);
            result /= parser_next_atom(this);
            break;
        case TOKT_MOD:
            parser_accept(this, TOKT_MOD);
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
            parser_accept(this, TOKT_ADD);
            result += parser_next_term(this);
            break;
        case TOKT_SUB:
            parser_accept(this, TOKT_SUB);
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
    parser_accept(this, TOKT_NEWLINE);
    return result;
}

int parser_has_next(Parser *this) {
    return lexer_peek(this->lexer) != NULL;
}
