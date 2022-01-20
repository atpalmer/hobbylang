#include <stdio.h>
#include "interpreter.h"
#include "test_asserts.h"


#define test_case(text, expected)\
    {\
        FILE *stream = fmemopen((void *)text, strlen(text), "r");\
        Interpreter *interp = interpreter_new();\
        Object *result = interpreter_parse_line(interp, stream);\
        assert_equal(Object_as_double(result), expected);\
        Object_destroy(result);\
        interpreter_free(interp);\
        fclose(stream);\
    }

int main(void) {
    test_case("42\n", 42);

    test_case(
        "alfa = 1\n"
        "bravo = 7\n"
        "alfa = 5\n"
        "alfa * bravo\n"
    , 35)
}
