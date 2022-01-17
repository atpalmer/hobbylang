#include <stdio.h>
#include "error.h"
#include "syswrap.h"
#include "lexer.h"
#include "parser.h"

Parser *parser_new(FILE *stream) {
    Parser *new = malloc_or_die(sizeof *new);
    new->lexer = lexer_new(stream);
    new->curr = NULL;
    return new;
}

void parser_free(Parser *this) {
    lexer_free(this->lexer);
    token_free(this->curr);
    free(this);
}

Token *parser_curr(Parser *this) {
    if(!this->curr)
        this->curr = lexer_next(this->lexer);
    return this->curr;
}

void parser_next(Parser *this) {
    if(this->curr)
        token_free(this->curr);
    this->curr = lexer_next(this->lexer);
}

void parser_accept(Parser *this, TokenType token_type) {
    Token *curr = parser_curr(this);
    token_ensure_type(curr, token_type);
    parser_next(this);
}

AstNode *parser_expr_ast(Parser *this);

AstNode *parser_handle_assignment_ast(Parser *this, const char *key) {
    parser_accept(this, TOKT_EQ);

    AstNode *id = ast_id_new(key);
    AstNode *value = parser_expr_ast(this);

    return ast_binop_new(ASTOP_ASSIGN, id, value);
}

AstNode *parser_handle_variable_ast(Parser *this) {
    Token *var = parser_curr(this);
    char *key = strdup_or_die(token_varname(var));
    parser_accept(this, TOKT_IDENTIFIER);

    Token *eq = parser_curr(this);

    AstNode *result = token_type(eq) == TOKT_EQ
        ? parser_handle_assignment_ast(this, key)
        : ast_id_new(key);

    free(key);
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

    fprintf(stderr, "TokenType Error: Cannot parse atom. Position: %ld. Found: '%c' (%d).\n",
        lexer_pos(this->lexer), token_type(curr), token_type(curr));
    exit(-1);
}

AstNode *parser_signed_atom_ast(Parser *this) {
    Token *curr = parser_curr(this);
    switch(token_type(curr)) {
    case TOKT_MINUS:
        parser_accept(this, TOKT_MINUS);
        return ast_uop_new(ASTOP_UMINUS, parser_atom_ast(this));
    case TOKT_PLUS:
        parser_accept(this, TOKT_PLUS);
        return ast_uop_new(ASTOP_UPLUS, parser_atom_ast(this));
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
        return ast_binop_new(ASTOP_POW, base, parser_factor_ast(this));
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
            result = ast_binop_new(ASTOP_MULT, result, parser_factor_ast(this));
            break;
        case TOKT_SLASH:
            parser_accept(this, TOKT_SLASH);
            result = ast_binop_new(ASTOP_DIV, result, parser_factor_ast(this));
            break;
        case TOKT_DUBSLASH:
            parser_accept(this, TOKT_DUBSLASH);
            result = ast_binop_new(ASTOP_FLOORDIV, result, parser_factor_ast(this));
            break;
        case TOKT_PERCENT:
            parser_accept(this, TOKT_PERCENT);
            result = ast_binop_new(ASTOP_MOD, result, parser_factor_ast(this));
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
            result = ast_binop_new(ASTOP_PLUS, result, parser_term_ast(this));
            break;
        case TOKT_MINUS:
            parser_accept(this, TOKT_MINUS);
            result = ast_binop_new(ASTOP_MINUS, result, parser_term_ast(this));
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
            result = ast_binop_new(ASTOP_EQ, result, parser_sum_ast(this));
            break;
        case TOKT_NE:
            parser_accept(this, TOKT_NE);
            result = ast_binop_new(ASTOP_NE, result, parser_sum_ast(this));
            break;
        case TOKT_LT:
            parser_accept(this, TOKT_LT);
            result = ast_binop_new(ASTOP_LT, result, parser_sum_ast(this));
            break;
        case TOKT_GT:
            parser_accept(this, TOKT_GT);
            result = ast_binop_new(ASTOP_GT, result, parser_sum_ast(this));
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
