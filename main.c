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
        Token *peek = lexer_peek(lexer);
        if(!peek)
            break;

        switch(token_type(peek)) {
        case TOKT_NUMBER:
            printf("number: %f\n", token_number(peek));
            lexer_consume_peek(lexer);
            break;
        case TOKT_VARIABLE:
            printf("variable: \"%s\"\n", token_varname(peek));
            lexer_consume_peek(lexer);
            break;
        default:
            printf("Type: %c (%d)\n", token_type(peek), token_type(peek));
            lexer_consume_peek(lexer);
            break;
        }
    }

    lexer_free(lexer);
}

void test_parser() {
    Parser *parser = parser_new(
            "42\n"
            "x = 3\n"
            "-2 + x * 2 + 5 - 1\n"
            "alfa = 1\n"
            "bravo = 7\n"
            "alfa = 5\n"
            "alfa * bravo\n");

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
