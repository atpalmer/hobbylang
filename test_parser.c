#pragma GCC diagnostic ignored "-Wgnu-statement-expression"

#include <stdio.h>
#include "parser.h"
#include "optype.h"
#include "test_asserts.h"

#define assert_double_node(node, val)\
    do{\
        assert_equal(((AstNode *)node)->type, ASTT_DOUBLE);\
        assert_equal(((AstDoubleNode *)node)->value, val);\
    }while(0)

#define assert_id_node(node, val)\
    do{\
        assert_equal(((AstNode *)node)->type, ASTT_ID);\
        assert_equal(((AstIdentifierNode *)node)->value, val);\
    }while(0)

#define assert_assignment_node(node)\
    ({{\
        assert_equal(((AstNode *)node)->type, ASTT_ASSIGN);\
    }; ((AstAssignmentNode *)node);})

#define assert_binop_node(node, optp)\
    ({{\
        assert_equal(((AstNode *)node)->type, ASTT_BINOP);\
        assert_equal(((AstBinOpNode *)node)->op, optp);\
    }; ((AstBinOpNode *)node);})

int main(void) {

    /*** TEST CASE ***/
    {
        const char *program = "42\n";
        FILE *stream = fmemopen((void *)program, strlen(program), "r");

        AstNode *node = parser_parse(stream);
        assert_double_node(node, 42);

        fclose(stream);
    }

    /*** TEST CASE ***/
    {
        const char *program = "x = 2 + 1\n";
        FILE *stream = fmemopen((void *)program, strlen(program), "r");

        AstNode *_topnode = parser_parse(stream);

        AstAssignmentNode *topnode = assert_assignment_node(_topnode);
        assert_id_node(topnode->id, "x");

        AstBinOpNode *valnode = assert_binop_node(topnode->value, BINOP_PLUS);
        assert_double_node(valnode->left, 2);
        assert_double_node(valnode->right, 1);

        fclose(stream);
    }

}
