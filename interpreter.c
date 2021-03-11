#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "interpreter.h"

double _interpret_ast(AstNode *ast) {
    if(ast->type == ASTT_DOUBLE) {
        return ((AstDoubleNode *)ast)->value;
    }
    if(ast->type == ASTT_BINOP) {
        AstBinOpNode *node = (AstBinOpNode *)ast;
        if(node->op == ASTOP_EQ) {
            return _interpret_ast(node->left) == _interpret_ast(node->right);
        }
        if(node->op == ASTOP_NE) {
            return _interpret_ast(node->left) != _interpret_ast(node->right);
        }
        if(node->op == ASTOP_LT) {
            return _interpret_ast(node->left) < _interpret_ast(node->right);
        }
        if(node->op == ASTOP_GT) {
            return _interpret_ast(node->left) > _interpret_ast(node->right);
        }
        if(node->op == ASTOP_PLUS) {
            return _interpret_ast(node->left) + _interpret_ast(node->right);
        }
        if(node->op == ASTOP_MINUS) {
            return _interpret_ast(node->left) - _interpret_ast(node->right);
        }
        if(node->op == ASTOP_MULT) {
            return _interpret_ast(node->left) * _interpret_ast(node->right);
        }
        if(node->op == ASTOP_DIV) {
            return _interpret_ast(node->left) / _interpret_ast(node->right);
        }
        if(node->op == ASTOP_FLOORDIV) {
            return floor(_interpret_ast(node->left) / _interpret_ast(node->right));
        }
        if(node->op == ASTOP_MOD) {
            return fmod(_interpret_ast(node->left), _interpret_ast(node->right));
        }
        if(node->op == ASTOP_POW) {
            return pow(_interpret_ast(node->left), _interpret_ast(node->right));
        }
    }
    if(ast->type == ASTT_UOP) {
        AstUnaryOpNode *node = (AstUnaryOpNode *)ast;
        if(node->op == ASTOP_UPLUS) {
            return +_interpret_ast(node->operand);
        }
        if(node->op == ASTOP_UMINUS) {
            return -_interpret_ast(node->operand);
        }
    }

    fprintf(stderr, "Cannot interpret AstNode. Type: %d\n", ast->type);
    exit(-1);
}

double interpreter_invoke(Parser *parser) {
    AstNode *ast = parser_line(parser);
    double result = _interpret_ast(ast);
    ast_free(ast);
    return result;
}
