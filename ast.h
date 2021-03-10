#ifndef AST_H
#define AST_H

typedef enum {
    ASTOP_EQ,
    ASTOP_NE,
    ASTOP_GT,
    ASTOP_LT,
    ASTOP_PLUS,
    ASTOP_MINUS,
    ASTOP_MULT,
    ASTOP_DIV,
    ASTOP_FLOORDIV,
    ASTOP_MOD,
} AstOp;

typedef enum {
    ASTT_DOUBLE,
    ASTT_BINOP,
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
    AstOp op;
    AstNode *left;
    AstNode *right;
} AstBinOpNode;

void ast_free(AstNode *this);
AstNode *ast_double_new(double value);
AstNode *ast_binop_new(AstOp op, AstNode *left, AstNode *right);

#endif
