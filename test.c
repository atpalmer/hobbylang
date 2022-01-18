#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "token.h"
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

    FILE *stream = fmemopen((void *)PROGRAM, strlen(PROGRAM), "r");

    printf("PROGRAM:\n%s\n", PROGRAM);

    Token *curr;

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_IDENTIFIER);
    assert_equal_string(token_varname(curr), "x");

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_EQ);

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_NUMBER);
    assert_equal_double(token_number(curr), 2);

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_NEWLINE);

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_IDENTIFIER);
    assert_equal_string(token_varname(curr), "x");

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_STAR);

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_NUMBER);
    assert_equal_double(token_number(curr), 1.5);

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_PLUS);

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_LPAREN);

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_NUMBER);
    assert_equal_double(token_number(curr), 123.0);

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_MINUS);

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_NUMBER);
    assert_equal_double(token_number(curr), 456.0);

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_RPAREN);

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_NEWLINE);

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_IDENTIFIER);
    assert_equal_string(token_varname(curr), "x");

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_DUBSLASH);

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_NUMBER);
    assert_equal_double(token_number(curr), 1.5);

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_NEWLINE);

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_IDENTIFIER);
    assert_equal_string(token_varname(curr), "x");

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_DUBEQ);

    curr = token_next(stream);
    assert_equal_int(token_type(curr), TOKT_NUMBER);
    assert_equal_double(token_number(curr), 2);

    fclose(stream);
}

void _test_parser_next(Interpreter *interp, FILE *stream, double expected) {
    Object *result = interpreter_parse_line(interp, stream);
    assert_equal_double(Object_as_double(result), expected);
    Object_destroy(result);
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

    FILE *stream = fmemopen((void *)PROGRAM, strlen(PROGRAM), "r");

    Interpreter *interp = interpreter_new();

    _test_parser_next(interp, stream, 42);
    _test_parser_next(interp, stream, 3);
    _test_parser_next(interp, stream, 8);
    _test_parser_next(interp, stream, 18);
    _test_parser_next(interp, stream, 1);
    _test_parser_next(interp, stream, 7);
    _test_parser_next(interp, stream, 5);
    _test_parser_next(interp, stream, 35);
    _test_parser_next(interp, stream, 3);
    _test_parser_next(interp, stream, 48);
    _test_parser_next(interp, stream, 1);
    _test_parser_next(interp, stream, 1);
    _test_parser_next(interp, stream, 1);
    _test_parser_next(interp, stream, 0);

    interpreter_free(interp);
    fclose(stream);
}

int main(void) {
    test_lexer();
    printf("---\n");
    test_parser();
}
