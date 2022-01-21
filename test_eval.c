#include <stdio.h>
#include "eval.h"
#include "test_asserts.h"


#define test_case(text, expected)\
    {\
        Object *varmap = MapObject_empty();\
        Object *result = eval_string(text, varmap);\
        assert_equal(Object_as_double(result), expected);\
        Object_destroy(result);\
        Object_destroy(varmap);\
    }

int main(void) {
    test_case("42\n", 42);

    test_case(
        "alfa = 1\n"
        "bravo = 7\n"
        "alfa = 5\n"
        "alfa * bravo\n"
    , 35)

    test_case(
        "value = 2\n"
        "value += 7\n"
    , 9)

    test_case(
        "value = 2\n"
        "value -= 7\n"
    , -5)

    test_case(
        "value = 2\n"
        "value *= 7\n"
    , 14)

    test_case(
        "value = 1\n"
        "value /= 2\n"
    , 0.5)

    test_case(
        "value = 10\n"
        "value //= 3\n"
    , 3)
}
