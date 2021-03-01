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
        if(token->type == TOKT_NUMBER)
            printf("number: %f\n", ((NumericToken *)token)->value);
        else if(token->type == TOKT_VARIABLE)
            printf("variable: \"%s\"\n", ((VarNameToken *)token)->value);
        else
            printf("Type: %c (%d)\n", token->type, token->type);
    }

    lexer_free(lexer);
}

void test_parser() {
    Parser *parser = parser_new("-2 + 3 * 2 + 5 - 1");

    while(parser_has_next(parser)) {
        double val = parser_next_expr(parser);
        printf("Result: %f\n", val);
    }

    parser_free(parser);
}

int main(void) {
    test_lexer();
    printf("---\n");
    test_parser();
}
