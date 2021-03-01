#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "lexer.h"

Lexer *lexer_new(const char *data) {
    Lexer *new = malloc(sizeof *new);
    new->data = data;
    new->len = strlen(data);
    new->pos = 0;
    new->curr = NULL;
    return new;
}

void lexer_free(Lexer *this) {
    token_free(&this->curr);
    free(this);
}

static int _is_whitespace(char c) {
    static const char VALID[] = " \t\r\n";
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

static int _is_literal(char c) {
    static const char VALID[] = {
        TOKT_END,
        TOKT_PRINT,
        TOKT_ADD,
        TOKT_SUB,
        TOKT_MULT,
        TOKT_DIV,
        TOKT_EQ,
        TOKT_LPAREN,
        TOKT_RPAREN,
    };
    return BOOL(memchr(VALID, c, sizeof VALID / sizeof *VALID));
}

static int _read_val(Lexer *this, double *result) {
    char valstr[32] = {0};
    char *valp = valstr;
    while(_is_numeric(this->data[this->pos])) {
        *(valp++) = this->data[this->pos++];
        if(valp - valstr >= 32)
            break;
    }
    *result = atof(valstr);
    return valp - valstr;
}

static int _read_varname(Lexer *this, char *buff, int bufflen) {
    char *valp = buff;
    while(_is_alpha(this->data[this->pos])) {
        *(valp++) = this->data[this->pos++];
        if(valp - buff >= bufflen - 1)
            break;
    }
    *valp = '\0';
    return valp - buff;
}

static void token_base_init(Token *base, enum token_type type, int bytes_read) {
    base->type = type;
    base->bytes_read = bytes_read;
}

static Token *token_new_literal(Lexer *lexer) {
    Token *new = malloc(sizeof *new);
    token_base_init(new, lexer->data[lexer->pos++], 1);
    return new;
}

static Token *token_new_numeric(Lexer *lexer) {
    NumericToken *new = malloc(sizeof *new);
    int bytes_read = _read_val(lexer, &new->value);
    token_base_init(&new->base, TOKT_NUMBER, bytes_read);
    return (Token *)new;
}

static Token *token_new_varname(Lexer *lexer) {
    VarNameToken *new = malloc(sizeof *new + 32 * sizeof(*new->value));
    int bytes_read = _read_varname(lexer, new->value, 32);
    token_base_init(&new->base, TOKT_VARIABLE, bytes_read);
    return (Token *)new;
}

void token_free(Token **this) {
    if(*this)
        free(*this);
    *this = NULL;
}

Token *lexer_next(Lexer *this) {
    token_free(&this->curr);

    if(!lexer_has_next(this))
        return NULL;

    while(_is_whitespace(this->data[this->pos]))
        ++this->pos;

    char c = this->data[this->pos];

    if(_is_literal(c))
        this->curr = token_new_literal(this);
    else if(_is_numeric(c))
        this->curr = token_new_numeric(this);
    else if(_is_alpha(c))
        this->curr = token_new_varname(this);

    return this->curr;
}

Token *lexer_peek(Lexer *this) {
    Lexer tmp = {
        .data = this->data,
        .len = this->len,
        .pos = this->pos,
        .curr = NULL,
    };

    return lexer_next(&tmp);
}

void lexer_handle(Lexer *this, Token *token) {
    while(_is_whitespace(this->data[this->pos]))
        ++this->pos;

    this->pos += token->bytes_read;
}
