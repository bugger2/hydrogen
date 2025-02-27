#include <iostream>
#include <fstream>
#include <sstream>

#include "generation.hpp"
#include "parser.hpp"
#include "tokenization.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Incorrect usage\n";
        std::cerr << "Usage: hydro <input.hy>\n";
        return EXIT_FAILURE;
    }

    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    Tokenizer tokenizer(std::move(contents));
    std::vector<Token> tokens = tokenizer.tokenize();

    Parser parser(std::move(tokens));
    std::optional<NodeProg> prog = parser.parse_prog();
    if (!prog.has_value()) {
        std::cerr << "Invalid program.\n";
        exit(EXIT_FAILURE);
    }

    Generator generator(prog.value());

    {
        std::fstream file("build/out.asm", std::ios::out);
        file << generator.gen_prog();
    }

    auto _ = system("nasm -felf64 build/out.asm");
    _ = system("ld -o build/out build/out.o");
    
    return EXIT_SUCCESS;
}
