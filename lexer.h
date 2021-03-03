#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKT_NULL = 0,
    TOKT_NEWLINE = '\n',
    TOKT_ADD = '+',
    TOKT_SUB = '-',
    TOKT_MULT = '*',
    TOKT_DIV = '/',
    TOKT_MOD = '%',
    TOKT_EQ = '=',
    TOKT_LPAREN = '(',
    TOKT_RPAREN = ')',
    TOKT_NUMBER = -1,
    TOKT_IDENTIFIER = -2,
} TokenType;

typedef struct {
    TokenType type;
    int bytes_read;
} Token;

typedef struct {
    Token base;
    char value[];
} IdentifierToken;

typedef struct {
    Token base;
    double value;
} NumericToken;

typedef struct {
    const char *data;
    int len;
    int pos;
} Lexer;


Lexer *lexer_new(const char *data);
void lexer_free(Lexer *this);
Token *lexer_next(Lexer *this);

TokenType token_type(Token *this);
double token_number(Token *this);
const char *token_varname(Token *this);

void token_free(Token *this);

#endif
