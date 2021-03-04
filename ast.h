#ifndef AST_H
#define AST_H

typedef enum {
    ASTT_DOUBLE,
} AstNodeType;

typedef struct {
    AstNodeType type;
} AstNode;

typedef struct {
    AstNode base;
    double value;
} AstDoubleNode;

void ast_free(AstNode *this);
AstNode *ast_double_new(double value);

#endif
