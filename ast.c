#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "error.h"

void ast_free(AstNode *this) {
    switch(this->type) {
    case ASTT_DOUBLE:
        break;
    case ASTT_ID:
        break;
    case ASTT_ASSIGN:
        ast_free((AstNode *)((AstAssignmentNode *)this)->id);
        ast_free(((AstAssignmentNode *)this)->value);
        break;
    case ASTT_BINOP:
        ast_free(((AstBinOpNode *)this)->left);
        ast_free(((AstBinOpNode *)this)->right);
        break;
    case ASTT_UOP:
        ast_free(((AstUnaryOpNode *)this)->operand);
        break;
    default:
        die_f(InternalError, "Illegal AstNodeType: %d\n", this->type);
    }
    free(this);
}

void _base_init(AstNode *base, AstNodeType type) {
    base->type = type;
}

AstNode *ast_double_new(double value) {
    AstDoubleNode *new = malloc(sizeof *new);
    _base_init(&new->base, ASTT_DOUBLE);
    new->value = value;
    return (AstNode *)new;
}

AstNode *ast_id_new(const char *value) {
    AstIdentifierNode *new = malloc(sizeof *new + (sizeof(char) * (strlen(value) + 1)));
    _base_init(&new->base, ASTT_ID);
    strcpy(new->value, value);
    return (AstNode *)new;
}

AstNode *ast_assign_new(AstIdentifierNode *id, AstNode *value) {
    AstAssignmentNode *new = malloc(sizeof *new);
    _base_init(&new->base, ASTT_ASSIGN);
    new->id = id;
    new->value = value;
    return (AstNode *)new;
}

AstNode *ast_binop_new(BinaryOp op, AstNode *left, AstNode *right) {
    AstBinOpNode *new = malloc(sizeof *new);
    _base_init(&new->base, ASTT_BINOP);
    new->op = op;
    new->left = left;
    new->right = right;
    return (AstNode *)new;
}

AstNode *ast_uop_new(UnaryOp op, AstNode *operand) {
    AstUnaryOpNode *new = malloc(sizeof *new);
    _base_init(&new->base, ASTT_UOP);
    new->op = op;
    new->operand = operand;
    return (AstNode *)new;
}
