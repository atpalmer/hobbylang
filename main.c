#include <stdio.h>
#include "parser.h"
#include "lexer.h"

void test_lexer() {
    static const char *PROGRAM =
        "x = 2.0\n"
        "x * 1.5 + (123 - 456)\n"
        ;

    Lexer *lexer = lexer_new(PROGRAM);

    printf("PROGRAM:\n%s\n", lexer->data);

    while(lexer_next(lexer)) {
        Token *token = lexer_curr(lexer);
        if(token->type == TOKT_END)
            break;
        else if(token->type == TOKT_NUMBER)
            printf("number: %f\n", ((NumericToken *)token)->value);
        else if(token->type == TOKT_VARIABLE)
            printf("variable: \"%s\"\n", ((VarNameToken *)token)->value);
        else
            printf("Type: %c (%d)\n", token->type, token->type);
    }

    lexer_free(lexer);
}

int main(void) {
    test_lexer();
}
