#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "varmap.h"
#include "interpreter.h"
#include "parser.h"
#include "syswrap.h"

double _interpret_ast(AstNode *ast, VarEntry **vars) {
    if(ast->type == ASTT_DOUBLE) {
        return ((AstDoubleNode *)ast)->value;
    }
    if(ast->type == ASTT_ID) {
        return varmap_getval(*vars, ((AstIdentifierNode *)ast)->value);
    }
    if(ast->type == ASTT_BINOP) {
        AstBinOpNode *node = (AstBinOpNode *)ast;
        if(node->op == ASTOP_EQ) {
            return _interpret_ast(node->left, vars) == _interpret_ast(node->right, vars);
        }
        if(node->op == ASTOP_NE) {
            return _interpret_ast(node->left, vars) != _interpret_ast(node->right, vars);
        }
        if(node->op == ASTOP_LT) {
            return _interpret_ast(node->left, vars) < _interpret_ast(node->right, vars);
        }
        if(node->op == ASTOP_GT) {
            return _interpret_ast(node->left, vars) > _interpret_ast(node->right, vars);
        }
        if(node->op == ASTOP_PLUS) {
            return _interpret_ast(node->left, vars) + _interpret_ast(node->right, vars);
        }
        if(node->op == ASTOP_MINUS) {
            return _interpret_ast(node->left, vars) - _interpret_ast(node->right, vars);
        }
        if(node->op == ASTOP_MULT) {
            return _interpret_ast(node->left, vars) * _interpret_ast(node->right, vars);
        }
        if(node->op == ASTOP_DIV) {
            return _interpret_ast(node->left, vars) / _interpret_ast(node->right, vars);
        }
        if(node->op == ASTOP_FLOORDIV) {
            return floor(_interpret_ast(node->left, vars) / _interpret_ast(node->right, vars));
        }
        if(node->op == ASTOP_MOD) {
            return fmod(_interpret_ast(node->left, vars), _interpret_ast(node->right, vars));
        }
        if(node->op == ASTOP_POW) {
            return pow(_interpret_ast(node->left, vars), _interpret_ast(node->right, vars));
        }
        if(node->op == ASTOP_ASSIGN) {
            if(node->left->type != ASTT_ID) {
                fprintf(stderr, "Cannot assign to AstNode type: %d\n", node->left->type);
                exit(-1);
            }
            const char *id = ((AstIdentifierNode *)node->left)->value;
            double val = _interpret_ast(node->right, vars);
            varmap_setval(vars, id, val);
            return val;
        }
    }
    if(ast->type == ASTT_UOP) {
        AstUnaryOpNode *node = (AstUnaryOpNode *)ast;
        if(node->op == ASTOP_UPLUS) {
            return +_interpret_ast(node->operand, vars);
        }
        if(node->op == ASTOP_UMINUS) {
            return -_interpret_ast(node->operand, vars);
        }
    }

    fprintf(stderr, "Cannot interpret AstNode. Type: %d\n", ast->type);
    exit(-1);
}

Interpreter *interpreter_from_string(const char *program) {
    Interpreter *new = malloc_or_die(sizeof *new);
    FILE *stream = fmemopen((void *)program, strlen(program), "r");
    new->parser = parser_new(stream);
    new->varmap = NULL;
    new->stream = stream;
    return new;
}

Interpreter *interpreter_from_stream(FILE *stream) {
    Interpreter *new = malloc_or_die(sizeof *new);
    new->parser = parser_new(stream);
    new->varmap = NULL;
    new->stream = NULL;  /* if we receive a file handle, we don't own it! */
    return new;
}

void interpreter_free(Interpreter *this) {
    if(this->stream)
        fclose(this->stream);
    parser_free(this->parser);
    varmap_free(this->varmap);
    free(this);
}

int interpreter_parse_line(Interpreter *this, double *result) {
    AstNode *ast = parser_line(this->parser);
    if(!ast)
        return 0;
    *result = _interpret_ast(ast, &this->varmap);
    ast_free(ast);
    varmap_setval(&this->varmap, "_", *result);
    return 1;
}
