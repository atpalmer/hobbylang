#ifndef OPTYPE_H
#define OPTYPE_H

typedef enum {
    BINOP_EQ,
    BINOP_NE,
    BINOP_GT,
    BINOP_LT,
    BINOP_PLUS,
    BINOP_MINUS,
    BINOP_MULT,
    BINOP_DIV,
    BINOP_FLOORDIV,
    BINOP_MOD,
    BINOP_POW,
} BinaryOp;

typedef enum {
    UOP_UPLUS,
    UOP_UMINUS,
} UnaryOp;

#endif
