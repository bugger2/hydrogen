#include <cstdlib>
#include <iostream>
#include <sstream>
#include <variant>
#include <format>

#include "generation.hpp"
#include "parser.hpp"

Generator::Generator(NodeProg prog)
    : m_prog(std::move(prog))
{
    m_stack_size = 0;
}

void Generator::push(const std::string& reg) {
    m_output << "push " << reg << "\n";
    m_stack_size++;
}

void Generator::pop(const std::string& reg) {
    m_output << "pop " << reg << "\n";
    m_stack_size--;
}

void Generator::gen_expr(const NodeExpr& expr) {
    struct ExprVisitor {
        Generator* gen;
        
        void operator()(const NodeExprIntLit& expr_int_lit) const {
            gen->m_output << "mov rax, " << expr_int_lit.int_lit.value.value() << "\n";
            gen->push("rax");
        }

        void operator()(const NodeExprIdent& expr_ident) {
            std::string ident = expr_ident.ident.value.value();

            if (!gen->m_vars.contains(ident)) {
                std::cerr << "Undeclared identifier `" << ident << "`\n";
                exit(EXIT_FAILURE);
            }

            size_t offset = (gen->m_stack_size - gen->m_vars.at(ident).stack_loc - 1) * 8;
            gen->push(std::format("QWORD [rsp + {}]", offset));
        }
    };

    ExprVisitor visitor { .gen = this };
    std::visit(visitor, expr.var);
}

void Generator::gen_stmt(const NodeStmt& stmt) {
    struct StmtVisitor {
        Generator* gen;
        
        void operator()(const NodeStmtExit& stmt_exit) const {
            gen->gen_expr(stmt_exit.expr);
            gen->m_output << "mov rax, 60\n";
            gen->pop("rdi");
            gen->m_output << "syscall\n";
        }

        void operator()(const NodeStmtLet& stmt_let) const {
            std::string ident = stmt_let.ident.value.value();
            if(gen->m_vars.contains(ident)) {
                std::cerr << "Identifier `" << ident << "` already exists.\n";
                exit(EXIT_FAILURE);
            }

            gen->m_vars.insert({ident, Var {.stack_loc = gen->m_stack_size } });

            // put the expression at the top of the stack
            gen->gen_expr(stmt_let.expr);

            
        }
    };

    StmtVisitor visitor { .gen = this };
    std::visit(visitor, stmt.var);
}

[[nodiscard]] std::string Generator::gen_prog() {
    m_output << "global _start\n_start:\n";

    for(const NodeStmt& node_stmt : m_prog.stmts) {
        gen_stmt(node_stmt);
    }

    m_output << "mov rax, 60\n";
    m_output << "mov rdi, 0\n";
    m_output << "syscall\n";

    return m_output.str();
}
