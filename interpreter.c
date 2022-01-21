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
#include "error.h"

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

static Object *_interpret_block(AstBlockNode *block, Object *_vars) {
    if(block->count < 1)
        die(InternaleError, "AstBlockNode must contain at least one child");

    Object *scope_vars = _vars ? Object_clone(_vars) : MapObject_empty();

    unsigned i = 0;
    for(; i < block->count - 1; ++i) {
        Object *discard = _interpret_ast(block->nodes[i], scope_vars);
        Object_destroy(discard);  /* discard intermediate results */
    }
    Object *result = _interpret_ast(block->nodes[i], scope_vars);
    Object_destroy(scope_vars);
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
    case ASTT_BLOCK:
        return _interpret_block((AstBlockNode *)ast, vars);
    default:
        break;
    }

    die_f(InternalError, "Cannot interpret AstNode. Type: %d\n", ast->type);
}

Object *interpreter_eval(FILE *stream, Object *varmap) {
    AstNode *ast = parser_parse(stream);
    if(!ast)
        return NULL;
    Object *result = _interpret_ast(ast, varmap);
    ast_free(ast);
    return result;
}
