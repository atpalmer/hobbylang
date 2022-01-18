#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>
#include "ast.h"

typedef struct object Object;

typedef struct {
    const char *name;
    Object *(*clone)(Object *this);
    void (*destroy)(Object *this);
    int (*to_stream)(Object *this, FILE *out, const char *term);
    double (*as_double)(Object *this);
    Object *(*get)(Object *this, const char *key);
    void (*set)(Object *this, const char *key, Object *value);
    Object *(*binop)(Object *this, Object *other, AstOp op);
    Object *(*uop)(Object *this, AstOp op);
} ObjectInterface;

struct object {
    const ObjectInterface *as_object;
};

#define OBJECT_INIT(ptr)    ((Object){.as_object = ptr})

static inline int Object_println(Object *this) {
    if(!this->as_object->to_stream) {
        fprintf(stderr, "Error: %s.println\n", this->as_object->name);
        exit(-1);
    }
    return this->as_object->to_stream(this, stdout, "\n");
}

static inline Object *Object_clone(Object *this) {
    if(!this->as_object->clone) {
        fprintf(stderr, "Error: %s.clone\n", this->as_object->name);
        exit(-1);
    }
    return this->as_object->clone(this);
}

static inline void Object_destroy(Object *this) {
    if(!this->as_object->destroy) {
        fprintf(stderr, "Error: %s.destroy\n", this->as_object->name);
        exit(-1);
    }
    this->as_object->destroy(this);
}

static inline double Object_as_double(Object *this) {
    if(!this->as_object->as_double) {
        fprintf(stderr, "Error: %s.as_double\n", this->as_object->name);
        exit(-1);
    }
    return this->as_object->as_double(this);
}

static inline Object *Object_get(Object *this, const char *key) {
    if(!this->as_object->get) {
        fprintf(stderr, "Error: %s.get\n", this->as_object->name);
        exit(-1);
    }
    return this->as_object->get(this, key);
}

static inline void Object_set(Object *this, const char *key, Object *value) {
    if(!this->as_object->set) {
        fprintf(stderr, "Error: %s.set\n", this->as_object->name);
        exit(-1);
    }
    this->as_object->set(this, key, value);
}

static inline Object *Object_binop(Object *this, Object *other, AstOp op) {
    if(!this->as_object->binop) {
        fprintf(stderr, "Error: %s.binop\n", this->as_object->name);
        exit(-1);
    }
    return this->as_object->binop(this, other, op);
}

static inline Object *Object_uop(Object *this, AstOp op) {
    if(!this->as_object->uop) {
        fprintf(stderr, "Error: %s.uop\n", this->as_object->name);
        exit(-1);
    }
    return this->as_object->uop(this, op);
}

#endif
