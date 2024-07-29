#include <iostream>

#include "tokenization.hpp"

Tokenizer::Tokenizer(const std::string src)
    : m_src(std::move(src))
{
    m_index = 0;
}

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;
    std::string buf;

    while(peek().has_value()) {
        if (std::isalpha(peek().value())) {
            buf.push_back(consume());

            while(peek().has_value() && std::isalnum(peek().value())) {
                buf.push_back(consume());
            }
            
            if (buf == "exit") {
                tokens.push_back({.type = TokenType::exit});
                buf.clear();
                continue;
            } else if (buf == "let") {
                tokens.push_back({.type = TokenType::let});
                buf.clear();
                continue;
            } else {
                tokens.push_back({.type = TokenType::ident, .value = buf});
                buf.clear();
                continue;
            }
        } else if (std::isdigit(peek().value())) {
            buf.push_back(consume());

            while (peek().has_value() && std::isdigit(peek().value())) {
                buf.push_back(consume());
            }

            tokens.push_back({.type = TokenType::int_lit, .value = buf});
            buf.clear();
            continue;
            
        } else if (peek().value() == '(') {
            consume();
            tokens.push_back({.type = TokenType::open_paren});
        } else if (peek().value() == ')') {
            consume();
            tokens.push_back({.type = TokenType::close_paren});
        } else if (peek().value() == ';') {
            consume();
            tokens.push_back({.type = TokenType::semi});
            continue;
            
        } else if (peek().value() == '=') {
            consume();
            tokens.push_back({.type = TokenType::eq});
        } else if (std::isspace(peek().value())) {
            consume();
            buf.clear();
            continue;
        } else {
            std::cerr << "womp womp\n";
            exit(EXIT_FAILURE);
        }
    }

    m_index = 0;
    return tokens;
}

[[nodiscard]] std::optional<char> Tokenizer::peek(int offset) const {
    if (m_index + offset >= m_src.length()) {
        return {};
    } else {
        return m_src.at(m_index + offset);
    }
}

char Tokenizer::consume() {
    return m_src.at(m_index++);
}
