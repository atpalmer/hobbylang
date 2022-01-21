#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syswrap.h"
#include "mapobj.h"

struct mapentry {
    char *key;
    Object *value;
    struct mapentry *next;
};

/* steals reference to "value" */
void mapentry_set(struct mapentry **head, const char *key, Object *value) {
    struct mapentry **curr = head;
    while(*curr) {
        if(strcmp((*curr)->key, key) == 0) {
            Object_destroy((*curr)->value);
            (*curr)->value = value;
            return;
        }
        curr = &(*curr)->next;
    }

    struct mapentry *new = malloc_or_die(sizeof *new);
    new->key = strdup_or_die(key);
    new->value = value;
    new->next = NULL;
    *curr = new;
}

/* returns new reference (or NULL if not found) */
Object *mapentry_get(struct mapentry *head, const char *key) {
    struct mapentry *curr = head;
    while(curr) {
        if(strcmp(curr->key, key) == 0)
            return Object_clone(curr->value);
        curr = curr->next;
    }

    return NULL;
}

void mapentry_free(struct mapentry *this) {
    if(!this)
        return;
    free(this->next);
    free(this);
}

typedef struct {
    Object base;
    struct mapentry *head;
} MapObject;

static Object *MapObject_clone(Object *_this) {
    MapObject *this = (MapObject *)_this;
    Object *new = MapObject_empty();

    struct mapentry **read = &this->head;
    while(*read) {
        Object *valclone = Object_clone((*read)->value);
        mapentry_set(&((MapObject *)new)->head, (*read)->key, valclone);
        read = &(*read)->next;
    }

    return new;
}

static void MapObject_destroy(Object *this) {
    free(this);
}

static Object *MapObject_get(Object *this, const char *key) {
    return mapentry_get(((MapObject *)this)->head, key);
}

static void MapObject_set(Object *this, const char *key, Object *value) {
    mapentry_set(&((MapObject *)this)->head, key, value);
}

static const ObjectInterface MapObject_as_object = {
    .name = "MutableMap",
    .clone = MapObject_clone,
    .destroy = MapObject_destroy,
    .to_stream = NULL,
    .as_double = NULL,
    .set = MapObject_set,
    .get = MapObject_get,
};

Object *MapObject_empty(void) {
    MapObject *new = malloc_or_die(sizeof *new);
    new->base = OBJECT_INIT(&MapObject_as_object);
    new->head = NULL;
    return (Object *)new;
}
