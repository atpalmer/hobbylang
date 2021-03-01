#include <stdio.h>
#include "parser.h"
#include "lexer.h"

static void assert_equal_double(double left, double right) {
    if(left == right)
        printf("OK: %f == %f\n", left, right);
    else
        fprintf(stderr, "ERROR: %f != %f\n", left, right);
}

static void assert_true(int val) {
    if(val)
        printf("OK: %d is true\n", val);
    else
        fprintf(stderr, "ERROR: %d is false\n", val);
}

static void assert_false(int val) {
    if(!val)
        printf("OK: %d is false\n", val);
    else
        fprintf(stderr, "ERROR: %d is true\n", val);
}

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

void test_parser(void) {
    static const char *PROGRAM =
            "42\n"
            "x = 3\n"
            "-2 + x * 2 + 5 - 1\n"
            "-2 + x * (2 + 5) - 1\n"
            "alfa = 1\n"
            "bravo = 7\n"
            "alfa = 5\n"
            "alfa * bravo\n"
            ;

    Parser *parser = parser_new(PROGRAM);

    assert_equal_double(parser_next_line(parser), 42);
    assert_equal_double(parser_next_line(parser), 3);
    assert_equal_double(parser_next_line(parser), 8);
    assert_equal_double(parser_next_line(parser), 18);
    assert_equal_double(parser_next_line(parser), 1);
    assert_equal_double(parser_next_line(parser), 7);
    assert_equal_double(parser_next_line(parser), 5);
    assert_true(parser_has_next(parser));
    assert_equal_double(parser_next_line(parser), 35);
    assert_false(parser_has_next(parser));

    parser_free(parser);
}

int main(void) {
    test_lexer();
    printf("---\n");
    test_parser();
}
