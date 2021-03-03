#ifndef ERROR_H
#define ERROR_H

#define ERRSTR_MemoryError      "MemoryError"
#define ERRSTR_OSError          "OSError"

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

static inline void error_ensure_token_type(Token *token, TokenType expected) {
    if(token_type(token) == expected)
        return;
    fprintf(stderr, "TokenType Error: Expected: %d. Received: %d\n",
        expected, token_type(token));
    exit(-1);
}

#endif
