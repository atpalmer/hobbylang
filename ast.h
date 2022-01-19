#ifndef AST_H
#define AST_H

#include "optype.h"

typedef enum {
    ASTT_DOUBLE,
    ASTT_ID,
    ASTT_ASSIGN,
    ASTT_BINOP,
    ASTT_UOP,
} AstNodeType;

typedef struct {
    AstNodeType type;
} AstNode;

typedef struct {
    AstNode base;
    double value;
} AstDoubleNode;

typedef struct {
    AstNode base;
    char value[];
} AstIdentifierNode;

typedef struct {
    AstNode base;
    AstNode *id;
    AstNode *value;
} AstAssignmentNode;

typedef struct {
    AstNode base;
    BinaryOp op;
    AstNode *left;
    AstNode *right;
} AstBinOpNode;

typedef struct {
    AstNode base;
    UnaryOp op;
    AstNode *operand;
} AstUnaryOpNode;

void ast_free(AstNode *this);
AstNode *ast_double_new(double value);
AstNode *ast_id_new(const char *value);
AstNode *ast_assign_new(AstNode *id, AstNode *value);
AstNode *ast_binop_new(BinaryOp op, AstNode *left, AstNode *right);
AstNode *ast_uop_new(UnaryOp op, AstNode *operand);

#endif
