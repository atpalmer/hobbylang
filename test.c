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

    double result;

    interpreter_parse_line(interp, stream, &result);
    assert_equal_double(result, 42);

    interpreter_parse_line(interp, stream, &result);
    assert_equal_double(result, 3);

    interpreter_parse_line(interp, stream, &result);
    assert_equal_double(result, 8);

    interpreter_parse_line(interp, stream, &result);
    assert_equal_double(result, 18);

    interpreter_parse_line(interp, stream, &result);
    assert_equal_double(result, 1);

    interpreter_parse_line(interp, stream, &result);
    assert_equal_double(result, 7);

    interpreter_parse_line(interp, stream, &result);
    assert_equal_double(result, 5);

    interpreter_parse_line(interp, stream, &result);
    assert_equal_double(result, 35);

    interpreter_parse_line(interp, stream, &result);
    assert_equal_double(result, 3);

    interpreter_parse_line(interp, stream, &result);
    assert_equal_double(result, 48);

    interpreter_parse_line(interp, stream, &result);
    assert_equal_double(result, 1);

    interpreter_parse_line(interp, stream, &result);
    assert_equal_double(result, 1);

    interpreter_parse_line(interp, stream, &result);
    assert_equal_double(result, 1);

    interpreter_parse_line(interp, stream, &result);
    assert_equal_double(result, 0);

    interpreter_free(interp);
    fclose(stream);
}

int main(void) {
    test_lexer();
    printf("---\n");
    test_parser();
}
