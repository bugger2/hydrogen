#ifndef _TOKENIZATION_HPP_
#define _TOKENIZATION_HPP_

#include <string>
#include <vector>
#include <optional>

enum class TokenType {
    exit,
    int_lit,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer {
public:
    Tokenizer(const std::string src);

    std::vector<Token> tokenize();
private:
    [[nodiscard]] std::optional<char> peek(int ahead = 1) const;
    char consume();
    
    std::string m_src;
    int m_index;
};

#endif
