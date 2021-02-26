#ifndef LEXER_H
#define LEXER_H

enum token_type {
    TOKT_END = 0,
    TOKT_PRINT = ';',
    TOKT_ADD = '+',
    TOKT_SUB = '-',
    TOKT_MULT = '*',
    TOKT_DIV = '/',
    TOKT_EQ = '=',
    TOKT_LPAREN = '(',
    TOKT_RPAREN = ')',
    TOKT_NUMBER,
    TOKT_VARIABLE,
};

typedef struct {
    enum token_type type;
} Token;

typedef struct {
    enum token_type type;
    char value[];
} VarNameToken;

typedef struct {
    enum token_type type;
    double value;
} NumericToken;

typedef struct {
    const char *data;
    int pos;
    Token *curr;
} Lexer;


Lexer *lexer_new(const char *data);
void lexer_free(Lexer *this);

#endif
