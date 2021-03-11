#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

void ast_free(AstNode *this) {
    switch(this->type) {
    case ASTT_DOUBLE:
        break;
    case ASTT_ID:
        break;
    case ASTT_BINOP:
        ast_free(((AstBinOpNode *)this)->left);
        ast_free(((AstBinOpNode *)this)->right);
        break;
    case ASTT_UOP:
        ast_free(((AstUnaryOpNode *)this)->operand);
        break;
    default:
        fprintf(stderr, "Illegal AstNodeType: %d\n", this->type);
        exit(-1);
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

AstNode *ast_id_new(const char *value, int len) {
    AstIdentifierNode *new = malloc(sizeof *new + sizeof *new->value * len);
    _base_init(&new->base, ASTT_ID);
    new->len = len;
    memcpy(new->value, value, len);
    return (AstNode *)new;
}

AstNode *ast_binop_new(AstOp op, AstNode *left, AstNode *right) {
    AstBinOpNode *new = malloc(sizeof *new);
    _base_init(&new->base, ASTT_BINOP);
    new->op = op;
    new->left = left;
    new->right = right;
    return (AstNode *)new;
}

AstNode *ast_uop_new(AstOp op, AstNode *operand) {
    AstUnaryOpNode *new = malloc(sizeof *new);
    _base_init(&new->base, ASTT_UOP);
    new->op = op;
    new->operand = operand;
    return (AstNode *)new;
}
