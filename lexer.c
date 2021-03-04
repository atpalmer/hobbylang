#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "syswrap.h"
#include "lexer.h"

TokenType token_type(Token *this) {
    return this ? this->type : TOKT_NULL;
}

Token *token_ensure_type(Token *this, TokenType expect) {
    if(token_type(this) == expect)
        return this;
    fprintf(stderr, "TokenTypeError: Expected: %d. Received: %d\n",
        expect, token_type(this));
    exit(-1);
}

double token_number(Token *this) {
    NumericToken *numtok = (NumericToken *)token_ensure_type(this, TOKT_NUMBER);
    return numtok->value;
}

const char *token_varname(Token *this) {
    IdentifierToken *idtok = (IdentifierToken *)token_ensure_type(this, TOKT_IDENTIFIER);
    return idtok->value;
}

Lexer *lexer_new(const char *data) {
    Lexer *new = malloc_or_die(sizeof *new);
    new->data = data;
    new->len = strlen(data);
    new->pos = 0;
    return new;
}

void lexer_free(Lexer *this) {
    free(this);
}

static int _is_whitespace(char c) {
    static const char VALID[] = " \t";
    return !!memchr(VALID, c, strlen(VALID));
}

static int _is_alpha(char c) {
    static const char VALID[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_";
    return !!memchr(VALID, c, strlen(VALID));
}

static int _is_eof(char c) {
    return c == '\0';
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

static inline int _set_symbol(const char *op, TokenType t, TokenType *result) {
    *result = t;
    return strlen(op);
}

static inline int _startswith(const char *str, const char *substr) {
    int len = strlen(substr);
    return strncmp(str, substr, len) == 0;
}

static int _read_symbol(const char *data, TokenType *type) {
    switch(*data) {
    case '\n':
        return _set_symbol("\n", TOKT_NEWLINE, type);

    case '+':
        return _set_symbol("+", TOKT_ADD, type);

    case '-':
        return _set_symbol("-", TOKT_SUB, type);

    case '*':
        if(_startswith(data, "**"))
            return _set_symbol("**", TOKT_DUBSTAR, type);
        else
            return _set_symbol("*", TOKT_MULT, type);

    case '/':
        if(_startswith(data, "//"))
            return _set_symbol("//", TOKT_FLOORDIV, type);
        else
            return _set_symbol("/", TOKT_DIV, type);

    case '%':
        return _set_symbol("%", TOKT_MOD, type);

    case '=':
        if(_startswith(data, "=="))
            return _set_symbol("==", TOKT_DUBEQ, type);
        else
            return _set_symbol("=", TOKT_EQ, type);

    case '<':
        return _set_symbol("<", TOKT_LT, type);

    case '>':
        return _set_symbol("<", TOKT_GT, type);

    case '!':
        if(_startswith(data, "!="))
            return _set_symbol("!=", TOKT_NE, type);
        else
            return _set_symbol("!", TOKT_NOT, type);

    case '(':
        return _set_symbol("(", TOKT_LPAREN, type);

    case ')':
        return _set_symbol(")", TOKT_RPAREN, type);

    default:
        break;
    };

    *type = TOKT_NULL;
    return 0;
}

static void token_base_init(Token *base, TokenType type, int bytes_read) {
    base->type = type;
    base->bytes_read = bytes_read;
}

static Token *token_try_new_symbol(const char *data) {
    TokenType type = TOKT_NULL;
    int bytes_read = _read_symbol(data, &type);
    if(!bytes_read)
        return NULL;
    Token *new = malloc_or_die(sizeof *new);
    token_base_init(new, type, bytes_read);
    return new;
}

static Token *token_try_new_numeric(const char *data) {
    double value;
    int bytes_read = _read_double(data, &value);
    if(!bytes_read)
        return NULL;
    NumericToken *new = malloc_or_die(sizeof *new);
    new->value = value;
    token_base_init(&new->base, TOKT_NUMBER, bytes_read);
    return (Token *)new;
}

static Token *token_try_new_identifier(const char *data) {
    if(!_is_alpha(*data))
        return NULL;
    IdentifierToken *new = malloc_or_die(sizeof *new + 32 * sizeof(*new->value));
    int bytes_read = _read_identifier(data, new->value, 32);
    token_base_init(&new->base, TOKT_IDENTIFIER, bytes_read);
    return (Token *)new;
}

static Token *_peek(const char *data) {
    while(_is_whitespace(*data))
        ++data;

    if(_is_eof(*data))
        return NULL;

    Token *result = NULL;

    result = token_try_new_symbol(data);
    if(result)
        return result;

    result = token_try_new_numeric(data);
    if(result)
        return result;

    result = token_try_new_identifier(data);
    if(result)
        return result;

    fprintf(stderr, "Lex error. Unhandled char: '%c'\n", *data);
    exit(-1);
}

Token *lexer_next(Lexer *this) {
    if(this->pos >= this->len)
        return NULL;
    Token *peek = _peek(&this->data[this->pos]);
    if(!peek)
        return NULL;
    while(_is_whitespace(this->data[this->pos]))
        ++this->pos;
    this->pos += peek->bytes_read;
    return peek;
}

void token_free(Token *this) {
    free(this);
}
