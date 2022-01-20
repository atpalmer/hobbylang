#include <math.h>
#include <string.h>
#include "syswrap.h"
#include "object.h"
#include "doubleobj.h"
#include "optype.h"
#include "error.h"

typedef struct {
    Object base;
    double value;
} DoubleObject;

static void DoubleObject_destroy(Object *this) {
    free(this);
}

static int DoubleObject_to_stream(Object *this, FILE *out, const char *term) {
    double value = ((DoubleObject *)this)->value;
    return fprintf(out, "%f%s", value, term);
}

static double DoubleObject_as_double(Object *this) {
    return ((DoubleObject *)this)->value;
}

static Object *DoubleObject_binop(Object *this, Object *other, BinaryOp op) {
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

    return DoubleObject_from_double(result);
}

static Object *DoubleObject_uop(Object *this, UnaryOp op) {
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

    return DoubleObject_from_double(result);
}

static Object *DoubleObject_clone(Object *this) {
    Object *new = malloc(sizeof(DoubleObject));
    memcpy(new, this, sizeof(DoubleObject));
    return new;
}

ObjectInterface DoubleObject_as_object = {
    .name = "Double",
    .clone = DoubleObject_clone,
    .destroy = DoubleObject_destroy,
    .to_stream = DoubleObject_to_stream,
    .as_double = DoubleObject_as_double,
    .binop = DoubleObject_binop,
    .uop = DoubleObject_uop,
};

Object *DoubleObject_from_double(double value) {
    DoubleObject *new = malloc_or_die(sizeof *new);
    new->base = OBJECT_INIT(&DoubleObject_as_object);
    new->value = value;
    return (Object *)new;
}