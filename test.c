#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"

static void assert_equal_char(int left, int right) {
    if(left == right)
        printf("OK: '%c' (%d) == '%c' (%d)\n", left, left, right, right);
    else
        fprintf(stderr, "ERROR: '%c' (%d)!= '%c' (%d)\n", left, left, right, right);
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

void test_lexer(void) {
    static const char *PROGRAM =
        "x = 2.0\n"
        "x * 1.5 + (123 - 456)\n"
        ;

    Lexer *lexer = lexer_new(PROGRAM);

    printf("PROGRAM:\n%s\n", lexer->data);

    Token *curr;

    curr = lexer_next(lexer);
    assert_equal_char(token_type(curr), TOKT_IDENTIFIER);
    assert_equal_string(token_varname(curr), "x");

    curr = lexer_next(lexer);
    assert_equal_char(token_type(curr), TOKT_EQ);

    curr = lexer_next(lexer);
    assert_equal_char(token_type(curr), TOKT_NUMBER);
    assert_equal_double(token_number(curr), 2);

    curr = lexer_next(lexer);
    assert_equal_char(token_type(curr), TOKT_NEWLINE);

    curr = lexer_next(lexer);
    assert_equal_char(token_type(curr), TOKT_IDENTIFIER);
    assert_equal_string(token_varname(curr), "x");

    curr = lexer_next(lexer);
    assert_equal_char(token_type(curr), TOKT_MULT);

    curr = lexer_next(lexer);
    assert_equal_char(token_type(curr), TOKT_NUMBER);
    assert_equal_double(token_number(curr), 1.5);

    curr = lexer_next(lexer);
    assert_equal_char(token_type(curr), TOKT_ADD);

    curr = lexer_next(lexer);
    assert_equal_char(token_type(curr), TOKT_LPAREN);

    curr = lexer_next(lexer);
    assert_equal_char(token_type(curr), TOKT_NUMBER);
    assert_equal_double(token_number(curr), 123.0);

    curr = lexer_next(lexer);
    assert_equal_char(token_type(curr), TOKT_SUB);

    curr = lexer_next(lexer);
    assert_equal_char(token_type(curr), TOKT_NUMBER);
    assert_equal_double(token_number(curr), 456.0);

    curr = lexer_next(lexer);
    assert_equal_char(token_type(curr), TOKT_RPAREN);

    curr = lexer_next(lexer);
    assert_equal_char(token_type(curr), TOKT_NEWLINE);

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
