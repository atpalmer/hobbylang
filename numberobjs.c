#include <math.h>
#include <string.h>
#include "syswrap.h"
#include "object.h"
#include "numberobjs.h"
#include "optype.h"
#include "error.h"

typedef struct {
    Object base;
    double value;
} F64Object;

static void F64Object_destroy(Object *this) {
    free(this);
}

static int F64Object_to_stream(Object *this, FILE *out, const char *term) {
    double value = ((F64Object *)this)->value;
    return fprintf(out, "%f%s", value, term);
}

static double F64Object_as_double(Object *this) {
    return ((F64Object *)this)->value;
}

static Object *F64Object_binop(Object *this, Object *other, BinaryOp op) {
    double result = 0.0;

    switch(op) {
    case BINOP_EQ:
        result = Object_as_double(this) == Object_as_double(other);
        break;
    case BINOP_NE:
        result = Object_as_double(this) != Object_as_double(other);
        break;
    case BINOP_LT:
        result = Object_as_double(this) < Object_as_double(other);
        break;
    case BINOP_GT:
        result = Object_as_double(this) > Object_as_double(other);
        break;
    case BINOP_PLUS:
        result = Object_as_double(this) + Object_as_double(other);
        break;
    case BINOP_MINUS:
        result = Object_as_double(this) - Object_as_double(other);
        break;
    case BINOP_MULT:
        result = Object_as_double(this) * Object_as_double(other);
        break;
    case BINOP_DIV:
        result = Object_as_double(this) / Object_as_double(other);
        break;
    case BINOP_FLOORDIV:
        result = floor(Object_as_double(this) / Object_as_double(other));
        break;
    case BINOP_MOD:
        result = fmod(Object_as_double(this), Object_as_double(other));
        break;
    case BINOP_POW:
        result = pow(Object_as_double(this), Object_as_double(other));
        break;
    default:
        die_f(InternalError, "Invalid operator: %d\n", op);
    }

    return F64Object_from_double(result);
}

static Object *F64Object_uop(Object *this, UnaryOp op) {
    double result = 0.0;

    switch(op) {
    case UOP_UPLUS:
        result = +Object_as_double(this);
        break;
    case UOP_UMINUS:
        result = -Object_as_double(this);
        break;
    default:
        die_f(InternalError, "Invalid operator: %d\n", op);
    }

    return F64Object_from_double(result);
}

static Object *F64Object_clone(Object *this) {
    Object *new = malloc(sizeof(F64Object));
    memcpy(new, this, sizeof(F64Object));
    return new;
}

ObjectInterface F64Object_as_object = {
    .name = "f64",
    .clone = F64Object_clone,
    .destroy = F64Object_destroy,
    .to_stream = F64Object_to_stream,
    .as_double = F64Object_as_double,
    .binop = F64Object_binop,
    .uop = F64Object_uop,
};

Object *F64Object_from_double(double value) {
    F64Object *new = malloc_or_die(sizeof *new);
    new->base = OBJECT_INIT(&F64Object_as_object);
    new->value = value;
    return (Object *)new;
}
