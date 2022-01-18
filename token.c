#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "syswrap.h"
#include "token.h"

static int _is_whitespace(int c) {
    static const char VALID[] = " \t";
    return !!memchr(VALID, c, strlen(VALID));
}

static int _is_alpha(int c) {
    static const char VALID[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_";
    return !!memchr(VALID, c, strlen(VALID));
}

struct symmap_entry {
    const char *match;
    TokenType type;
};

static const struct symmap_entry symmap[] = {
    {"\n",  TOKT_NEWLINE},
    {"+",   TOKT_PLUS},
    {"-",   TOKT_MINUS},
    {"**",  TOKT_DUBSTAR},
    {"*",   TOKT_STAR},
    {"//",  TOKT_DUBSLASH},
    {"/",   TOKT_SLASH},
    {"%",   TOKT_PERCENT},
    {"==",  TOKT_DUBEQ},
    {"=",   TOKT_EQ},
    {"<",   TOKT_LT},
    {">",   TOKT_GT},
    {"!=",  TOKT_NE},
    {"(",   TOKT_LPAREN},
    {")",   TOKT_RPAREN},
    {0},
};

static int _read_symbol(FILE *stream, TokenType *type) {
    const struct symmap_entry *entry = &symmap[0];
    while(entry->match) {
        size_t len = strlen(entry->match);
        char buff[len];
        size_t bytes_read = fread(&buff, 1, len, stream);
        if(bytes_read == len && strncmp(buff, entry->match, bytes_read) == 0) {
            *type = entry->type;
            return bytes_read;
        }
        fseek(stream, -bytes_read, SEEK_CUR);
        ++entry;
    }
    *type = TOKT_NULL;
    return 0;
}

static void token_base_init(Token *base, TokenType type, int bytes_read) {
    base->type = type;
    base->bytes_read = bytes_read;
}

static Token *token_try_new_symbol(FILE *stream) {
    TokenType type = TOKT_NULL;
    int bytes_read = _read_symbol(stream, &type);
    if(!bytes_read)
        return NULL;
    Token *new = malloc_or_die(sizeof *new);
    token_base_init(new, type, bytes_read);
    return new;
}

static Token *token_try_new_numeric(FILE *stream) {
    double value;
    int bytes_read = fscanf(stream, "%lf", &value);
    if(!bytes_read)
        return NULL;
    NumericToken *new = malloc_or_die(sizeof *new);
    new->value = value;
    token_base_init(&new->base, TOKT_NUMBER, bytes_read);
    return (Token *)new;
}

static Token *token_try_new_identifier(FILE *stream) {
    int c = getc(stream);

    if(!_is_alpha(c)) {
        ungetc(c, stream);
        return NULL;
    }

    IdentifierToken *new = malloc_or_die(sizeof *new + 32 * sizeof(*new->value));

    char *w = new->value;
    while(w - new->value < 32) {
        *w++ = c;
        c = getc(stream);
        if(!_is_alpha(c)) {
            ungetc(c, stream);
            break;
        }
    }
    *w = '\0';

    int bytes_read = w - new->value;  /* trailing '\0' not included */
    token_base_init(&new->base, TOKT_IDENTIFIER, bytes_read);
    return (Token *)new;
}

void token_free(Token *this) {
    if(!this)
        return;
    free(this);
}

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

static Token *read_token(FILE *stream) {
    Token *result = NULL;

    result = token_try_new_symbol(stream);
    if(result)
        return result;
    result = token_try_new_numeric(stream);
    if(result)
        return result;
    result = token_try_new_identifier(stream);
    if(result)
        return result;

    fprintf(stderr, "Lex error. Unhandled char at pos %ld\n", ftell(stream));
    exit(-1);
}

Token *token_next(FILE *stream) {
    int curr = getc(stream);

    while(_is_whitespace(curr))
        curr = getc(stream);
    if(curr == EOF)
        return NULL;

    ungetc(curr, stream);

    return read_token(stream);
}
