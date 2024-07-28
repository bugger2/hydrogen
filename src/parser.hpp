#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <optional>
#include <vector>

#include "tokenization.hpp"

struct NodeExpr {
    Token int_lit;
};

struct NodeExit {
    NodeExpr expr;
};

class Parser {
public:
    Parser(std::vector<Token> tokens);
    std::optional<NodeExit> parse();
    std::optional<NodeExpr> parse_expr();

private:
    const std::vector<Token> m_tokens;
    size_t m_index;

    [[nodiscard]] std::optional<Token> peek(int ahead = 1) const;
    Token consume();
    
};

#endif
