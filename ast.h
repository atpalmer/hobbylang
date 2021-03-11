#ifndef AST_H
#define AST_H

typedef enum {
    /* Binary Ops */
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
    ASTOP_POW,
    ASTOP_ASSIGN,

    /* Unary Ops */
    ASTOP_UPLUS,
    ASTOP_UMINUS,
} AstOp;

typedef enum {
    ASTT_DOUBLE,
    ASTT_ID,
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
    int len;
    char value[];
} AstIdentifierNode;

typedef struct {
    AstNode base;
    AstOp op;
    AstNode *left;
    AstNode *right;
} AstBinOpNode;

typedef struct {
    AstNode base;
    AstOp op;
    AstNode *operand;
} AstUnaryOpNode;

void ast_free(AstNode *this);
AstNode *ast_double_new(double value);
AstNode *ast_id_new(const char *value, int len);
AstNode *ast_binop_new(AstOp op, AstNode *left, AstNode *right);
AstNode *ast_uop_new(AstOp op, AstNode *operand);

#endif
