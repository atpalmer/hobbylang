#include <stdlib.h>
#include "lexer.h"

Lexer *lexer_new(void) {
    Lexer *new = malloc(sizeof *new);
    return new;
}

void lexer_free(Lexer *this) {
    free(this);
}

Token *lexer_curr(Lexer *this) {
    return this->curr;
}

/* TODO:
Token *lexer_next(Lexer *this) {
}
*/
