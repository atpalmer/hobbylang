#ifndef AST_H
#define AST_H

#include "optype.h"

typedef enum {
    ASTT_DOUBLE,
    ASTT_ID,
    ASTT_ASSIGN,
    ASTT_BINOP,
    ASTT_UOP,
    ASTT_BLOCK,
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
    AstIdentifierNode *id;
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

typedef struct {
    AstNode base;
    unsigned count;
    AstNode *nodes[];
} AstBlockNode;

void ast_free(AstNode *this);
AstNode *ast_double_new(double value);
AstNode *ast_id_new(const char *value);
AstNode *ast_id_clone(AstIdentifierNode *node);
AstNode *ast_assign_new(AstIdentifierNode *id, AstNode *value);
AstNode *ast_binop_new(BinaryOp op, AstNode *left, AstNode *right);
AstNode *ast_uop_new(UnaryOp op, AstNode *operand);
void ast_block_append(AstBlockNode **this, AstNode *node);

#endif
