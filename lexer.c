#include <stdlib.h>
#include <string.h>
#include "lexer.h"

Lexer *lexer_new(const char *data) {
    Lexer *new = malloc(sizeof *new);
    new->data = data;
    new->pos = 0;
    new->curr = NULL;
    return new;
}

void lexer_free(Lexer *this) {
    if(this->curr)
        free(this->curr);
    free(this);
}

Token *lexer_curr(Lexer *this) {
    return this->curr;
}

static int _is_whitespace(char c) {
    static const char VALID[] = " \t\r\n";
    return strchr(VALID, c) ? 1 : 0;
}

static int _is_alpha(char c) {
    static const char VALID[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_";
    return strchr(VALID, c) ? 1 : 0;
}

static int _is_numeric(char c) {
    static const char VALID[] = "0123456789.";
    return strchr(VALID, c) ? 1 : 0;
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
    return strchr(VALID, c) ? 1 : 0;
}

static double _read_val(Lexer *this) {
    char valstr[32] = {0};
    char *valp = valstr;
    while(_is_numeric(this->data[this->pos])) {
        *(valp++) = this->data[this->pos++];
        if(valp - valstr >= 32)
            break;
    }
    return atof(valstr);
}

static void _read_varname(Lexer *this, char *buff, size_t bufflen) {
    char *valp = buff;
    while(_is_alpha(this->data[this->pos])) {
        *(valp++) = this->data[this->pos++];
        if(valp - buff >= bufflen - 1)
            break;
    }
    *valp = '\0';
}

static Token *token_new_literal(Lexer *lexer) {
    Token *new = malloc(sizeof *new);
    new->type = lexer->data[lexer->pos++];
    return new;
}

static Token *token_new_numeric(Lexer *lexer) {
    NumericToken *new = malloc(sizeof *new);
    new->type = TOKT_NUMBER;
    new->value = _read_val(lexer);
    return (Token *)new;
}

static Token *token_new_varname(Lexer *lexer) {
    VarNameToken *new = malloc(sizeof *new + 32 * sizeof(*new->value));
    new->type = TOKT_VARIABLE;
    _read_varname(lexer, new->value, 32);
    return (Token *)new;
}

Token *lexer_next(Lexer *this) {
    if(this->curr) {
        free(this->curr);
        this->curr = NULL;
    }

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
