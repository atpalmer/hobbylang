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

    assert_equal_char(token_type(lexer_peek(lexer)), TOKT_VARIABLE);
    assert_equal_string(token_varname(lexer_peek(lexer)), "x");
    lexer_consume_peek(lexer);

    assert_equal_char(token_type(lexer_peek(lexer)), TOKT_EQ);
    lexer_consume_peek(lexer);

    assert_equal_char(token_type(lexer_peek(lexer)), TOKT_NUMBER);
    assert_equal_double(token_number(lexer_peek(lexer)), 2);
    lexer_consume_peek(lexer);

    assert_equal_char(token_type(lexer_peek(lexer)), TOKT_NEWLINE);
    lexer_consume_peek(lexer);

    assert_equal_char(token_type(lexer_peek(lexer)), TOKT_VARIABLE);
    assert_equal_string(token_varname(lexer_peek(lexer)), "x");
    lexer_consume_peek(lexer);

    assert_equal_char(token_type(lexer_peek(lexer)), TOKT_MULT);
    lexer_consume_peek(lexer);

    assert_equal_char(token_type(lexer_peek(lexer)), TOKT_NUMBER);
    assert_equal_double(token_number(lexer_peek(lexer)), 1.5);
    lexer_consume_peek(lexer);

    assert_equal_char(token_type(lexer_peek(lexer)), TOKT_ADD);
    lexer_consume_peek(lexer);

    assert_equal_char(token_type(lexer_peek(lexer)), TOKT_LPAREN);
    lexer_consume_peek(lexer);

    assert_equal_char(token_type(lexer_peek(lexer)), TOKT_NUMBER);
    assert_equal_double(token_number(lexer_peek(lexer)), 123.0);
    lexer_consume_peek(lexer);

    assert_equal_char(token_type(lexer_peek(lexer)), TOKT_SUB);
    lexer_consume_peek(lexer);

    assert_equal_char(token_type(lexer_peek(lexer)), TOKT_NUMBER);
    assert_equal_double(token_number(lexer_peek(lexer)), 456.0);
    lexer_consume_peek(lexer);

    assert_equal_char(token_type(lexer_peek(lexer)), TOKT_RPAREN);
    lexer_consume_peek(lexer);

    assert_equal_char(token_type(lexer_peek(lexer)), TOKT_NEWLINE);
    lexer_consume_peek(lexer);

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
