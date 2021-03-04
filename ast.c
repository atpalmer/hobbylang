#include <stdlib.h>
#include "ast.h"

void ast_free(AstNode *this) {
    free(this);
}

AstNode *ast_double_new(double value) {
    AstDoubleNode *new = malloc(sizeof *new);
    new->base.type = ASTT_DOUBLE;
    new->value = value;
    return (AstNode *)new;
}
