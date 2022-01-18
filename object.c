#include "syswrap.h"
#include "object.h"

static void object_destroy(Object *this) {
    free(this);
}


/*** DoubleObject ***/

typedef struct {
    Object base;
    double value;
} DoubleObject;

static int DoubleObject_to_stream(Object *this, FILE *out, const char *term) {
    double value = ((DoubleObject *)this)->value;
    return fprintf(out, "%f%s", value, term);
}

static double DoubleObject_as_double(Object *this) {
    return ((DoubleObject *)this)->value;
}

ObjectInterface DoubleObject_as_object = {
    .name = "Double",
    .clone = NULL,
    .destroy = object_destroy,
    .to_stream = DoubleObject_to_stream,
    .as_double = DoubleObject_as_double,
};

Object *DoubleObject_from_double(double value) {
    DoubleObject *new = malloc_or_die(sizeof *new);
    new->base = OBJECT_INIT(&DoubleObject_as_object);
    new->value = value;
    return (Object *)new;
}
