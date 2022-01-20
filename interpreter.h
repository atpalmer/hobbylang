#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "mapobj.h"

Object *interpreter_eval(FILE *stream, Object *varmap);

#endif
