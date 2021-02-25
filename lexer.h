#ifndef LEXER_H
#define LEXER_H

enum token_type {
    TOKT_END,
    TOKT_PRINT = ';',
    TOKT_PLUS = '+',
    TOKT_MINUS = '-',
    TOKT_MULT = '*',
    TOKT_DIV = '/',
    TOKT_ASSIGN = '=',
    TOKT_LPAREN = '(',
    TOKT_RPAREN = ')',
    TOKT_NUMBER,
    TOKT_VARIABLE,
};

typedef struct {
    enum token_type type;
    char *name;
    double value;
} Token;

typedef struct {
    Token *curr;
} Lexer;


Lexer *lexer_new(void);
void lexer_free(Lexer *this);

#endif
