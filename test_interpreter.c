#include <stdio.h>
#include "interpreter.h"
#include "test_asserts.h"


#define test_next(interp, stream, expected)\
    do{\
        Object *result = interpreter_parse_line(interp, stream);\
        assert_equal(Object_as_double(result), expected);\
        Object_destroy(result);\
    }while(0)

int main(void) {
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

    printf("%s\n", PROGRAM);

    FILE *stream = fmemopen((void *)PROGRAM, strlen(PROGRAM), "r");

    Interpreter *interp = interpreter_new();

    test_next(interp, stream, 42);
    test_next(interp, stream, 3);
    test_next(interp, stream, 8);
    test_next(interp, stream, 18);
    test_next(interp, stream, 1);
    test_next(interp, stream, 7);
    test_next(interp, stream, 5);
    test_next(interp, stream, 35);
    test_next(interp, stream, 3);
    test_next(interp, stream, 48);
    test_next(interp, stream, 1);
    test_next(interp, stream, 1);
    test_next(interp, stream, 1);
    test_next(interp, stream, 0);

    interpreter_free(interp);
    fclose(stream);
}
