#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * <char *> in _Generic catches <char[]> and gives us <const char *> (to use as an assignable type)
 * Using `const` qualifier regardless simplifies later type-checks
 */
#define TYPEOF(arg)\
    __typeof__(_Generic((arg), char *: (const char *)0, const char *: (const char *)0, default: arg))

#define TYPEFMT(arg)\
    _Generic((arg), int: "%d", double: "%f", unsigned: "%u", const char *: "\"%s\"")

#define DEFAULT_COMPARE(lval, op, rval) ((lval) op (rval))
#define STRING_COMPARE(lval, op, rval)  (strcmp((char *)(uintptr_t)(lval), (char *)(uintptr_t)(rval)) op 0)

#define IS_STRING(arg) (__builtin_types_compatible_p(const char *, __typeof__(arg)))

#define CONDITION(l,o,r) ((IS_STRING(l) && IS_STRING(r)) ? STRING_COMPARE(l,o,r) : DEFAULT_COMPARE(l,o,r))


/*** TEST API ***/

#define assert_compare(left, op, right)\
    do{\
        FILE *out = stdout;\
        TYPEOF(left) lval = left;\
        TYPEOF(right) rval = right;\
        int cond = CONDITION(lval, op, rval);\
        const char *msg = (cond) ? "OK" : "FAIL";\
        fprintf(out, "%s (%s line %d): ", msg, __FILE__, __LINE__);\
        fprintf(out, TYPEFMT(lval), lval);\
        fprintf(out, " " #op " ");\
        fprintf(out, TYPEFMT(rval), rval);\
        fprintf(out, " <%s " #op " %s>\n", #left, #right);\
        if(!cond) exit(-1);\
    }while(0)

#define assert_equal(left, right)   assert_compare(left, ==, right)

