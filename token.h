#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKT_NULL = 0,
    TOKT_NEWLINE,
    TOKT_PLUS,
    TOKT_MINUS,
    TOKT_STAR,
    TOKT_DUBSTAR,
    TOKT_SLASH,
    TOKT_DUBSLASH,
    TOKT_PERCENT,
    TOKT_EQ,
    TOKT_DUBEQ,
    TOKT_NE,
    TOKT_LT,
    TOKT_GT,
    TOKT_LPAREN,
    TOKT_RPAREN,
    TOKT_NUMBER,
    TOKT_IDENTIFIER,
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

Token *token_next(FILE *stream);
TokenType token_type(Token *this);
Token *token_ensure_type(Token *this, TokenType expect);
double token_number(Token *this);
const char *token_varname(Token *this);
void token_free(Token *this);

#endif
