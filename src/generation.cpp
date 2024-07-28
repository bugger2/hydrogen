#include <sstream>

#include "generation.hpp"

Generator::Generator(NodeExit root)
    : m_root(std::move(root))
{
    
}

[[nodiscard]] std::string Generator::generate() const {
    std::stringstream output;

    output << "global _start\n_start:\n";
    output << "    mov rax, 60\n";
    output << "    mov rdi, " << m_root.expr.int_lit.value.value() << "\n"; 
    output << "    syscall\n";

    return output.str();
}
