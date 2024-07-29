#ifndef _TOKENIZATION_HPP_
#define _TOKENIZATION_HPP_

#include <string>
#include <vector>
#include <optional>

enum class TokenType {
    exit,
    int_lit,
    semi,
    open_paren,
    close_paren,
    ident,
    let,
    eq
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
    [[nodiscard]] std::optional<char> peek(int offset = 0) const;
    char consume();
    
    std::string m_src;
    int m_index;
};

#endif
