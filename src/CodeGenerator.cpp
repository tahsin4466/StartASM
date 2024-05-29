#include "include/CodeGenerator.h"
#include <llvm/Support/raw_ostream.h>

CodeGenerator::CodeGenerator()
        : TheModule(std::make_unique<llvm::Module>("StartASM Module", TheContext)),
          Builder(TheContext) {}

CodeGenerator::~CodeGenerator() {}

void CodeGenerator::generateCode(AST::AbstractSyntaxTree* ast) {
    // Implement code generation from the AST here
    // This will involve traversing the AST and generating corresponding LLVM IR
}

void CodeGenerator::printIR() {
    TheModule->print(llvm::outs(), nullptr);
}
