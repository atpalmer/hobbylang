#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>
#include "optype.h"
#include "error.h"

typedef struct object Object;

typedef struct {
    const char *name;
    Object *(*clone)(Object *this);
    void (*destroy)(Object *this);
    int (*to_stream)(Object *this, FILE *out, const char *term);
    double (*as_double)(Object *this);
    Object *(*get)(Object *this, const char *key);
    void (*set)(Object *this, const char *key, Object *value);
    Object *(*binop)(Object *this, Object *other, BinaryOp op);
    Object *(*uop)(Object *this, UnaryOp op);
} ObjectInterface;

struct object {
    const ObjectInterface *as_object;
};

#define OBJECT_INIT(ptr)    ((Object){.as_object = ptr})

static inline const ObjectInterface *Object_as_object(Object *this) {
    if(!this)
        die_f(NullReferenceError, "Cannot call %s(NULL)\n", __func__);
    if(!this->as_object)
        die(NullReferenceError, "NULL ObjectInterface\n");
    return this->as_object;
}

static inline int Object_println(Object *this) {
    if(!Object_as_object(this)->to_stream)
        die_f(NullReferenceError, "%s::%s\n", this->as_object->name, __func__);
    return this->as_object->to_stream(this, stdout, "\n");
}

static inline Object *Object_clone(Object *this) {
    if(!Object_as_object(this)->clone)
        die_f(NullReferenceError, "%s::%s\n", this->as_object->name, __func__);
    return this->as_object->clone(this);
}

static inline void Object_destroy(Object *this) {
    if(!Object_as_object(this)->destroy)
        die_f(NullReferenceError, "%s::%s\n", this->as_object->name, __func__);
    this->as_object->destroy(this);
}

static inline double Object_as_double(Object *this) {
    if(!Object_as_object(this)->as_double)
        die_f(NullReferenceError, "%s::%s\n", this->as_object->name, __func__);
    return this->as_object->as_double(this);
}

static inline Object *Object_get(Object *this, const char *key) {
    if(!Object_as_object(this)->get)
        die_f(NullReferenceError, "%s::%s\n", this->as_object->name, __func__);
    return this->as_object->get(this, key);
}

static inline void Object_set(Object *this, const char *key, Object *value) {
    if(!Object_as_object(this)->set)
        die_f(NullReferenceError, "%s::%s\n", this->as_object->name, __func__);
    this->as_object->set(this, key, value);
}

static inline Object *Object_binop(Object *this, Object *other, BinaryOp op) {
    if(!Object_as_object(this)->binop)
        die_f(NullReferenceError, "%s::%s\n", this->as_object->name, __func__);
    return this->as_object->binop(this, other, op);
}

static inline Object *Object_uop(Object *this, UnaryOp op) {
    if(!Object_as_object(this)->uop)
        die_f(NullReferenceError, "%s::%s\n", this->as_object->name, __func__);
    return this->as_object->uop(this, op);
}

#endif
