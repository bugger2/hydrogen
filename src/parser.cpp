#include "parser.hpp"
#include <iostream>

Parser::Parser(std::vector<Token> tokens)
    : m_tokens(std::move(tokens))
{
    m_index = 0;
}

std::optional<NodeExpr> Parser::parse_expr() {
    if (peek().has_value() && peek().value().type == TokenType::int_lit) {
        return NodeExpr { .var = NodeExprIntLit { .int_lit = consume() } };
    } else if (peek().has_value() && peek().value().type == TokenType::ident) {
        return NodeExpr { .var = NodeExprIdent { .ident = consume() } };
    } else {
        return {};
    }
}

std::optional<NodeStmt> Parser::parse_stmt() {
    if (peek().value().type == TokenType::exit
        && peek(1).has_value()
        && peek(1).value().type == TokenType::open_paren)
    {
        consume();
        consume();

        NodeStmtExit stmt_exit;

        if (auto node_expr = parse_expr()) {
            stmt_exit = { .expr = node_expr.value() };
        } else {
            std::cerr << "Expected expression";
            exit(EXIT_FAILURE);
        }

        if (peek().has_value() && peek().value().type == TokenType::close_paren) {
            consume();
        } else {
            std::cerr << "Expected `)`";
            exit(EXIT_FAILURE);
        }
            
        if (peek().has_value() && peek().value().type == TokenType::semi) {
            consume();
        } else {
            std::cerr << "Expected `;`";
            exit(EXIT_FAILURE);
        }

        return NodeStmt { .var = stmt_exit };
    } else if (peek().has_value() && peek().value().type == TokenType::let
               && peek(1).has_value() && peek(1).value().type == TokenType::ident
               && peek(2).has_value() && peek(2).value().type == TokenType::eq)
    {
        consume();
        NodeStmtLet stmt_let = NodeStmtLet { .ident = consume() };
        consume();
        if (auto expr = parse_expr()) {
            stmt_let.expr = expr.value();
        } else {
            std::cerr << "Invalid variable assignment\n";
            exit(EXIT_FAILURE);
        }

        if (peek().has_value() && peek().value().type == TokenType::semi) {
            consume();
        } else {
            std::cerr << "Expected `;`\n";
            exit(EXIT_FAILURE);
        }

        return NodeStmt { .var = stmt_let };
    } else {
        return {};   
    }
}

std::optional<NodeProg> Parser::parse_prog() {
    NodeProg prog;

    while (peek().has_value()) {
        if (auto stmt = parse_stmt()) {
            prog.stmts.push_back(stmt.value());
        } else {
            std::cerr << "Invalid statment\n";
            exit(EXIT_FAILURE);
        }
    }

    return prog;
}

[[nodiscard]] std::optional<Token> Parser::peek(int offset) const {
    if (m_index + offset >= m_tokens.size()) {
        return {};
    } else {
        return m_tokens.at(m_index + offset);
    }
}

Token Parser::consume() {
    return m_tokens.at(m_index++);
}
