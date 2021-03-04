#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"
#include "interpreter.h"

static void assert_equal_int(int left, int right) {
    if(left == right)
        printf("OK: %d == %d\n", left, right);
    else
        fprintf(stderr, "ERROR: %d != %d\n", left, right);
}

static void assert_equal_double(double left, double right) {
    if(left == right)
        printf("OK: %f == %f\n", left, right);
    else
        fprintf(stderr, "ERROR: %f != %f\n", left, right);
}

static void assert_equal_string(const char *left, const char *right) {
    if(strcmp(left, right) == 0)
        printf("OK: \"%s\" == \"%s\"\n", left, right);
    else
        fprintf(stderr, "ERROR: \"%s\" != \"%s\"\n", left, right);
}

void test_lexer(void) {
    static const char *PROGRAM =
        "x = 2.0\n"
        "x * 1.5 + (123 - 456)\n"
        "x // 1.5\n"
        "x == 2\n"
        ;

    Lexer *lexer = lexer_new(PROGRAM);

    printf("PROGRAM:\n%s\n", lexer->data);

    Token *curr;

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_IDENTIFIER);
    assert_equal_string(token_varname(curr), "x");

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_EQ);

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_NUMBER);
    assert_equal_double(token_number(curr), 2);

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_NEWLINE);

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_IDENTIFIER);
    assert_equal_string(token_varname(curr), "x");

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_STAR);

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_NUMBER);
    assert_equal_double(token_number(curr), 1.5);

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_PLUS);

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_LPAREN);

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_NUMBER);
    assert_equal_double(token_number(curr), 123.0);

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_MINUS);

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_NUMBER);
    assert_equal_double(token_number(curr), 456.0);

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_RPAREN);

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_NEWLINE);

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_IDENTIFIER);
    assert_equal_string(token_varname(curr), "x");

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_DUBSLASH);

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_NUMBER);
    assert_equal_double(token_number(curr), 1.5);

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_NEWLINE);

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_IDENTIFIER);
    assert_equal_string(token_varname(curr), "x");

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_DUBEQ);

    curr = lexer_next(lexer);
    assert_equal_int(token_type(curr), TOKT_NUMBER);
    assert_equal_double(token_number(curr), 2);

    lexer_free(lexer);
}

void test_parser(void) {
    static const char *PROGRAM =
            "42\n"
            "x = 2 + 1\n"
            "-2 + x * 2 + 5 - 1\n"
            "-2 + x * (2 + 5) - 1\n"
            "alfa = 1\n"
            "bravo = 7\n"
            "alfa = 5\n"
            "alfa * bravo\n"
            "bravo // 2\n"
            "3 * 2 ** 4\n"
            "4 * 3 == 135 % 123\n"
            "0 > -5\n"
            "10 - 20 < 10000000\n"
            "2 + 2 != 4\n"
            ;

    Parser *parser = parser_new(PROGRAM);

    assert_equal_double(interpreter_invoke(parser), 42);
    assert_equal_double(interpreter_invoke(parser), 3);
    assert_equal_double(interpreter_invoke(parser), 8);
    assert_equal_double(interpreter_invoke(parser), 18);
    assert_equal_double(interpreter_invoke(parser), 1);
    assert_equal_double(interpreter_invoke(parser), 7);
    assert_equal_double(interpreter_invoke(parser), 5);
    assert_equal_double(interpreter_invoke(parser), 35);
    assert_equal_double(interpreter_invoke(parser), 3);
    assert_equal_double(interpreter_invoke(parser), 48);
    assert_equal_double(interpreter_invoke(parser), 1);
    assert_equal_double(interpreter_invoke(parser), 1);
    assert_equal_double(interpreter_invoke(parser), 1);
    assert_equal_double(interpreter_invoke(parser), 0);

    parser_free(parser);
}

int main(void) {
    test_lexer();
    printf("---\n");
    test_parser();
}
