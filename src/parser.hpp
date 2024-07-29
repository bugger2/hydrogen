#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <optional>
#include <variant>
#include <vector>

#include "tokenization.hpp"

struct NodeExprIntLit {
    Token int_lit;
};

struct NodeExprIdent {
    Token ident;
};

struct NodeExpr {
    std::variant<NodeExprIntLit, NodeExprIdent> var;
};

struct NodeStmtExit {
    NodeExpr expr;
};

struct NodeStmtLet {
    Token ident;
    NodeExpr expr;
};

struct NodeStmt {
    std::variant<NodeStmtExit, NodeStmtLet> var;
};

struct NodeProg {
    std::vector<NodeStmt> stmts;
};

class Parser {
public:
    Parser(std::vector<Token> tokens);
    std::optional<NodeExpr> parse_expr();
    std::optional<NodeStmt> parse_stmt();
    std::optional<NodeProg> parse_prog();

private:
    const std::vector<Token> m_tokens;
    size_t m_index;

    [[nodiscard]] std::optional<Token> peek(int offset = 0) const;
    Token consume();
    
};

#endif
