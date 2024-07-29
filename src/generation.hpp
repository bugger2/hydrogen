#ifndef _GENERATION_HPP_
#define _GENERATION_HPP_

#include "parser.hpp"
#include <sstream>
#include <string>
#include <unordered_map>

class Generator {
public:
    Generator(NodeProg root);

    void gen_expr(const NodeExpr& node_expr);
    void gen_stmt(const NodeStmt& node_stmt);
    [[nodiscard]] std::string gen_prog();

private:
    struct Var {
        size_t stack_loc;
    };

    void push(const std::string& reg);
    void pop(const std::string& reg);
    
    const NodeProg m_prog;
    std::stringstream m_output;
    size_t m_stack_size;
    std::unordered_map<std::string, Var> m_vars;
};

#endif
