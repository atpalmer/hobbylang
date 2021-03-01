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

    for(;;) {
        Token *token = lexer_peek(lexer);
        if(!token)
            break;

        switch(token_type(token)) {
        case TOKT_NUMBER:
            lexer_handle(lexer, token);
            printf("number: %f\n", token_number(token));
            break;
        case TOKT_VARIABLE:
            lexer_handle(lexer, token);
            printf("variable: \"%s\"\n", token_varname(token));
            break;
        default:
            lexer_handle(lexer, token);
            printf("Type: %c (%d)\n", token_type(token), token_type(token));
            break;
        }
        token_free(&token);
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
