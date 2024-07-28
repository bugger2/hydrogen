#ifndef _GENERATION_HPP_
#define _GENERATION_HPP_

#include "parser.hpp"

class Generator {
public:
    Generator(NodeExit root);

    [[nodiscard]] std::string generate() const;

private:
    const NodeExit m_root;
};

#endif
