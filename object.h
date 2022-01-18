#ifndef OBJECT_H
#define OBJECT_H

typedef struct object Object;

typedef struct {
    const char *name;
    Object *(*clone)(Object *this);
    void (*destroy)(Object *this);
    int (*to_stream)(Object *this, FILE *out, const char *term);
    double (*as_double)(Object *this);
} ObjectInterface;

struct object {
    ObjectInterface *as_object;
};

#define OBJECT_INIT(ptr)    ((Object){.as_object = ptr})

static inline int Object_println(Object *this) {
    return this->as_object->to_stream(this, stdout, "\n");
}

static inline void Object_destroy(Object *this) {
    this->as_object->destroy(this);
}

static inline double Object_as_double(Object *this) {
    return this->as_object->as_double(this);
}

Object *DoubleObject_from_double(double value);

#endif
