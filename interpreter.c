#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "object.h"
#include "doubleobj.h"
#include "mapobj.h"
#include "optype.h"
#include "interpreter.h"
#include "parser.h"
#include "syswrap.h"

static Object *_interpret_ast(AstNode *ast, Object *vars);

static Object *_interpret_assignment(AstAssignmentNode *node, Object *vars) {
    Object *val = _interpret_ast(node->value, vars);
    Object_set(vars, node->id->value, val);
    return Object_clone(val);
}

static Object *_interpret_binop(AstBinOpNode *node, Object *vars) {
    Object *left = _interpret_ast(node->left, vars);
    Object *right = _interpret_ast(node->right, vars);

    Object *result = Object_binop(left, right, node->op);

    Object_destroy(left);
    Object_destroy(right);

    return result;
}

static Object *_interpret_uop(AstUnaryOpNode *node, Object *vars) {
    Object *operand = _interpret_ast(node->operand, vars);
    Object *result = Object_uop(operand, node->op);
    Object_destroy(operand);
    return result;
}

static Object *_interpret_ast(AstNode *ast, Object *vars) {
    switch(ast->type) {
    case ASTT_DOUBLE:
        return DoubleObject_from_double(((AstDoubleNode *)ast)->value);
    case ASTT_ID:
        return Object_get(vars, ((AstIdentifierNode *)ast)->value);
    case ASTT_ASSIGN:
        return _interpret_assignment((AstAssignmentNode *)ast, vars);
    case ASTT_BINOP:
        return _interpret_binop((AstBinOpNode *)ast, vars);
    case ASTT_UOP:
        return _interpret_uop((AstUnaryOpNode *)ast, vars);
    default:
        break;
    }

    fprintf(stderr, "Cannot interpret AstNode. Type: %d\n", ast->type);
    exit(-1);
}

Interpreter *interpreter_new(void) {
    Interpreter *new = malloc_or_die(sizeof *new);
    new->varmap = MapObject_empty();
    return new;
}

void interpreter_free(Interpreter *this) {
    Object_destroy(this->varmap);
    free(this);
}

Object *interpreter_parse_line(Interpreter *this, FILE *stream) {
    AstNode *ast = parser_parse(stream);
    if(!ast)
        return NULL;
    Object *result = _interpret_ast(ast, this->varmap);
    ast_free(ast);
    Object_set(this->varmap, "_", result);
    return Object_clone(result);
}
