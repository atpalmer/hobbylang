#include <stdio.h>
#include <string.h>
#include "token.h"
#include "test_asserts.h"


int main(void) {
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
    assert_equal(token_type(curr), TOKT_IDENTIFIER);
    assert_equal(token_varname(curr), "x");

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_EQ);

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_NUMBER);
    assert_equal(token_number(curr), 2);

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_NEWLINE);

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_IDENTIFIER);
    assert_equal(token_varname(curr), "x");

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_STAR);

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_NUMBER);
    assert_equal(token_number(curr), 1.5);

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_PLUS);

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_LPAREN);

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_NUMBER);
    assert_equal(token_number(curr), 123.0);

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_MINUS);

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_NUMBER);
    assert_equal(token_number(curr), 456.0);

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_RPAREN);

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_NEWLINE);

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_IDENTIFIER);
    assert_equal(token_varname(curr), "x");

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_DUBSLASH);

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_NUMBER);
    assert_equal(token_number(curr), 1.5);

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_NEWLINE);

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_IDENTIFIER);
    assert_equal(token_varname(curr), "x");

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_DUBEQ);

    curr = token_next(stream);
    assert_equal(token_type(curr), TOKT_NUMBER);
    assert_equal(token_number(curr), 2);

    fclose(stream);
}
