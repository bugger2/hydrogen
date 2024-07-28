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
            
            if(buf == "exit") {
                tokens.push_back({.type = TokenType::exit});
                buf.clear();
                continue;
            } else {
                std::cerr << "womp womp\n";
                exit(EXIT_FAILURE);
            }
        } else if (std::isdigit(peek().value())) {
            buf.push_back(consume());

            while (peek().has_value() && std::isdigit(peek().value())) {
                buf.push_back(consume());
            }

            tokens.push_back({.type = TokenType::int_lit, .value = buf});
            buf.clear();
            continue;
        } else if (peek().value() == ';') {
            consume();
            tokens.push_back({.type = TokenType::semi});
            buf.clear();
            continue;
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

[[nodiscard]] std::optional<char> Tokenizer::peek(int ahead) const {
    if (m_index + ahead > m_src.length()) {
        return {};
    } else {
        return m_src.at(m_index);
    }
}

char Tokenizer::consume() {
    return m_src.at(m_index++);
}
