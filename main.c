#include <stdio.h>
#include "parser.h"
#include "lexer.h"

void test_lexer() {
    Lexer *lexer = lexer_new("123 + 456");

    printf("data: %s\n", lexer->data);

    while(lexer_next(lexer)) {
        Token *token = lexer_curr(lexer);
        if(token->type == TOKT_NUMBER)
            printf("number: %f\n", ((NumericToken *)token)->value);
        else
            printf("Type: %c (%d)\n", token->type, token->type);
    }

    lexer_free(lexer);
}

int main(void) {
    test_lexer();
}
