#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "lexer.h"

Lexer *lexer_new(const char *data) {
    Lexer *new = malloc(sizeof *new);
    if(!new)
        goto fail;
    new->data = data;
    new->len = strlen(data);
    new->pos = 0;
    new->peek = NULL;
    return new;

fail:
    perror("Error");
    exit(errno);
}

void _token_free(Token **this) {
    if(*this)
        free(*this);
    *this = NULL;
}

void lexer_free(Lexer *this) {
    _token_free(&this->peek);
    free(this);
}

static int _is_whitespace(char c) {
    static const char VALID[] = " \t";
    return BOOL(memchr(VALID, c, strlen(VALID)));
}

static int _is_alpha(char c) {
    static const char VALID[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_";
    return BOOL(memchr(VALID, c, strlen(VALID)));
}

static int _is_numeric(char c) {
    static const char VALID[] = "0123456789.";
    return BOOL(memchr(VALID, c, strlen(VALID)));
}

static int _is_eof(char c) {
    return c == '\0';
}

static int _is_literal(char c) {
    static const char VALID[] = {
        TOKT_NEWLINE,
        TOKT_ADD,
        TOKT_SUB,
        TOKT_MULT,
        TOKT_DIV,
        TOKT_MOD,
        TOKT_EQ,
        TOKT_LPAREN,
        TOKT_RPAREN,
    };
    return BOOL(memchr(VALID, c, sizeof VALID / sizeof *VALID));
}

static int _read_double(const char *data, double *result) {
    const char *p = data;
    *result = strtod(data, (void *)&p);
    return p - data;
}

static int _read_identifier(const char *data, char *buff, int bufflen) {
    char *valp = buff;
    while(_is_alpha(*data)) {
        *(valp++) = *data++;
        if(valp - buff >= bufflen - 1)
            break;
    }
    *valp = '\0';
    return valp - buff;
}

static void token_base_init(Token *base, TokenType type, int bytes_read) {
    base->type = type;
    base->bytes_read = bytes_read;
}

static Token *token_new_literal(const char *data) {
    Token *new = malloc(sizeof *new);
    if(!new)
        goto fail;
    token_base_init(new, *data, 1);
    return new;

fail:
    perror("Error");
    exit(errno);
}

static Token *token_new_numeric(const char *data) {
    NumericToken *new = malloc(sizeof *new);
    if(!new)
        goto fail;
    int bytes_read = _read_double(data, &new->value);
    token_base_init(&new->base, TOKT_NUMBER, bytes_read);
    return (Token *)new;

fail:
    perror("Error");
    exit(errno);
}

static Token *token_new_varname(const char *data) {
    VarNameToken *new = malloc(sizeof *new + 32 * sizeof(*new->value));
    if(!new)
        goto fail;
    int bytes_read = _read_identifier(data, new->value, 32);
    token_base_init(&new->base, TOKT_IDENTIFIER, bytes_read);
    return (Token *)new;

fail:
    perror("Error");
    exit(errno);
}

static Token *_peek(const char *data) {
    while(_is_whitespace(*data))
        ++data;

    if(_is_eof(*data))
        return NULL;
    else if(_is_literal(*data))
        return token_new_literal(data);
    else if(_is_numeric(*data))
        return token_new_numeric(data);
    else if(_is_alpha(*data))
        return token_new_varname(data);

    fprintf(stderr, "Lex error. Unhandled char: '%c'\n", *data);
    exit(-1);
}

Token *lexer_peek(Lexer *this) {
    if(this->peek)
        return this->peek;
    if(this->pos >= this->len)
        return NULL;
    this->peek = _peek(&this->data[this->pos]);
    return this->peek;
}

void lexer_consume_peek(Lexer *this) {
    if(!this->peek)
        return;
    while(_is_whitespace(this->data[this->pos]))
        ++this->pos;
    this->pos += this->peek->bytes_read;
    _token_free(&this->peek);
}
