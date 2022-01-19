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

Object *_interpret_ast(AstNode *ast, Object *vars) {
    if(ast->type == ASTT_DOUBLE) {
        return DoubleObject_from_double(((AstDoubleNode *)ast)->value);
    }

    if(ast->type == ASTT_ID) {
        return Object_get(vars, ((AstIdentifierNode *)ast)->value);
    }

    if(ast->type == ASTT_ASSIGN) {
        AstAssignmentNode *node = (AstAssignmentNode *)ast;

        const char *id = ((AstIdentifierNode *)node->id)->value;

        Object *val = _interpret_ast(node->value, vars);
        Object_set(vars, id, val);

        return Object_clone(val);
    }

    if(ast->type == ASTT_BINOP) {
        AstBinOpNode *node = (AstBinOpNode *)ast;

        Object *left = _interpret_ast(node->left, vars);
        Object *right = _interpret_ast(node->right, vars);

        Object *result = Object_binop(left, right, node->op);

        Object_destroy(left);
        Object_destroy(right);

        return result;
    }

    if(ast->type == ASTT_UOP) {
        AstUnaryOpNode *node = (AstUnaryOpNode *)ast;

        Object *operand = _interpret_ast(node->operand, vars);

        Object *result = Object_uop(operand, node->op);

        Object_destroy(operand);

        return result;
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
