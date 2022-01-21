#include <stdio.h>
#include "error.h"
#include "syswrap.h"
#include "token.h"
#include "parser.h"
#include "optype.h"

Parser *parser_new(FILE *stream) {
    Parser *new = malloc_or_die(sizeof *new);
    new->stream = stream;
    new->curr = NULL;
    return new;
}

void parser_free(Parser *this) {
    /* do not close this->stream */
    token_free(this->curr);
    free(this);
}

Token *parser_curr(Parser *this) {
    if(!this->curr)
        this->curr = token_next(this->stream);
    return this->curr;
}

void parser_accept(Parser *this, TokenType token_type) {
    Token *curr = parser_curr(this);
    token_ensure_type(curr, token_type);
    token_free(this->curr);
    this->curr = NULL;
}

AstNode *parser_expr_ast(Parser *this);

AstNode *parser_handle_variable_ast(Parser *this) {
    Token *var = parser_curr(this);
    AstNode *id = ast_id_new(token_varname(var));
    parser_accept(this, TOKT_IDENTIFIER);

    Token *op = parser_curr(this);

    AstNode *result = NULL;

    switch(token_type(op)) {
    case TOKT_EQ:
        parser_accept(this, TOKT_EQ);
        result = parser_expr_ast(this);
        result = ast_assign_new((AstIdentifierNode *)id, result);
        break;
    case TOKT_PLUSEQ:
        parser_accept(this, TOKT_PLUSEQ);
        result = parser_expr_ast(this);
        result = ast_binop_new(BINOP_PLUS, id, result);
        result = ast_assign_new(
            (AstIdentifierNode *)ast_id_clone((AstIdentifierNode *)id),
            result);
        break;
    case TOKT_MINUSEQ:
        parser_accept(this, TOKT_MINUSEQ);
        result = parser_expr_ast(this);
        result = ast_binop_new(BINOP_MINUS, id, result);
        result = ast_assign_new(
            (AstIdentifierNode *)ast_id_clone((AstIdentifierNode *)id),
            result);
        break;
    case TOKT_STAREQ:
        parser_accept(this, TOKT_STAREQ);
        result = parser_expr_ast(this);
        result = ast_binop_new(BINOP_MULT, id, result);
        result = ast_assign_new(
            (AstIdentifierNode *)ast_id_clone((AstIdentifierNode *)id),
            result);
        break;
    case TOKT_SLASHEQ:
        parser_accept(this, TOKT_SLASHEQ);
        result = parser_expr_ast(this);
        result = ast_binop_new(BINOP_DIV, id, result);
        result = ast_assign_new(
            (AstIdentifierNode *)ast_id_clone((AstIdentifierNode *)id),
            result);
        break;
    case TOKT_DUBSLASHEQ:
        parser_accept(this, TOKT_DUBSLASHEQ);
        result = parser_expr_ast(this);
        result = ast_binop_new(BINOP_FLOORDIV, id, result);
        result = ast_assign_new(
            (AstIdentifierNode *)ast_id_clone((AstIdentifierNode *)id),
            result);
        break;
    default:
        result = id;
        break;
    }

    return result;
}

AstNode *parser_paren_expr_ast(Parser *this) {
    parser_accept(this, TOKT_LPAREN);
    AstNode *result = parser_expr_ast(this);
    parser_accept(this, TOKT_RPAREN);
    return result;
}

AstNode *parser_number_ast(Parser *this) {
    Token *curr = parser_curr(this);
    double result = token_number(curr);
    parser_accept(this, TOKT_NUMBER);
    return ast_double_new(result);
}

AstNode *parser_atom_ast(Parser *this) {
    Token *curr = parser_curr(this);

    switch(token_type(curr)) {
    case TOKT_NUMBER:
        return parser_number_ast(this);
    case TOKT_LPAREN:
        return parser_paren_expr_ast(this);
    case TOKT_IDENTIFIER:
        return parser_handle_variable_ast(this);
    default:
        break;
    }

    die_f(ParseError,
        "Cannot parse atom. Position: %ld. Found: '%c' (%d).\n",
        ftell(this->stream), token_type(curr), token_type(curr));
}

AstNode *parser_signed_atom_ast(Parser *this) {
    Token *curr = parser_curr(this);
    switch(token_type(curr)) {
    case TOKT_MINUS:
        parser_accept(this, TOKT_MINUS);
        return ast_uop_new(UOP_UMINUS, parser_atom_ast(this));
    case TOKT_PLUS:
        parser_accept(this, TOKT_PLUS);
        return ast_uop_new(UOP_UPLUS, parser_atom_ast(this));
    default:
        break;
    }

    return parser_atom_ast(this);
}

AstNode *parser_factor_ast(Parser *this) {
    AstNode *base = parser_signed_atom_ast(this);

    Token *curr = parser_curr(this);
    switch(token_type(curr)) {
    case TOKT_DUBSTAR:
        parser_accept(this, TOKT_DUBSTAR);
        return ast_binop_new(BINOP_POW, base, parser_factor_ast(this));
    default:
        break;
    }

    return base;
}

AstNode *parser_term_ast(Parser *this) {
    AstNode *result = parser_factor_ast(this);

    for(;;) {
        Token *curr = parser_curr(this);

        switch(token_type(curr)) {
        case TOKT_STAR:
            parser_accept(this, TOKT_STAR);
            result = ast_binop_new(BINOP_MULT, result, parser_factor_ast(this));
            break;
        case TOKT_SLASH:
            parser_accept(this, TOKT_SLASH);
            result = ast_binop_new(BINOP_DIV, result, parser_factor_ast(this));
            break;
        case TOKT_DUBSLASH:
            parser_accept(this, TOKT_DUBSLASH);
            result = ast_binop_new(BINOP_FLOORDIV, result, parser_factor_ast(this));
            break;
        case TOKT_PERCENT:
            parser_accept(this, TOKT_PERCENT);
            result = ast_binop_new(BINOP_MOD, result, parser_factor_ast(this));
            break;
        default:
            goto done;
        }
    }

done:
    return result;
}

AstNode *parser_sum_ast(Parser *this) {
    AstNode *result = parser_term_ast(this);

    for(;;) {
        Token *curr = parser_curr(this);

        switch(token_type(curr)) {
        case TOKT_PLUS:
            parser_accept(this, TOKT_PLUS);
            result = ast_binop_new(BINOP_PLUS, result, parser_term_ast(this));
            break;
        case TOKT_MINUS:
            parser_accept(this, TOKT_MINUS);
            result = ast_binop_new(BINOP_MINUS, result, parser_term_ast(this));
            break;
        default:
            goto done;
        }
    }

done:
    return result;
}

AstNode *parser_expr_ast(Parser *this) {
    AstNode *result = parser_sum_ast(this);

    for(;;) {
        Token *curr = parser_curr(this);

        switch(token_type(curr)) {
        case TOKT_DUBEQ:
            parser_accept(this, TOKT_DUBEQ);
            result = ast_binop_new(BINOP_EQ, result, parser_sum_ast(this));
            break;
        case TOKT_NE:
            parser_accept(this, TOKT_NE);
            result = ast_binop_new(BINOP_NE, result, parser_sum_ast(this));
            break;
        case TOKT_LT:
            parser_accept(this, TOKT_LT);
            result = ast_binop_new(BINOP_LT, result, parser_sum_ast(this));
            break;
        case TOKT_GT:
            parser_accept(this, TOKT_GT);
            result = ast_binop_new(BINOP_GT, result, parser_sum_ast(this));
            break;
        default:
            goto done;
        }
    }

done:
    return result;
}

AstNode *parser_line(Parser *this) {
    if(!parser_curr(this))
        return NULL;
    AstNode *result = parser_expr_ast(this);
    parser_accept(this, TOKT_NEWLINE);
    return result;
}

AstNode *parser_parse(FILE *stream) {
    AstBlockNode *block = NULL;
    Parser *parser = parser_new(stream);
    for(;;) {
        AstNode *result = parser_line(parser);
        if(!result)
            break;
        ast_block_append(&block, result);
    }
    parser_free(parser);
    return (AstNode *)block;
}
