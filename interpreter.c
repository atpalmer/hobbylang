#include <stdlib.h>
#include "ast.h"
#include "interpreter.h"

double interpreter_invoke(Parser *parser) {
    AstNode *ast = parser_line(parser);
    double result = 0.0;
    if(ast->type == ASTT_DOUBLE) {
        result = ((AstDoubleNode *)ast)->value;
        goto done;
    }

    exit(-1);

done:
    ast_free(ast);
    return result;
}
