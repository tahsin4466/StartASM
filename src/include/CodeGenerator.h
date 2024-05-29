#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include "AbstractSyntaxTree.h"

class CodeGenerator {
public:
    CodeGenerator();
    ~CodeGenerator();

    void generateCode(AST::AbstractSyntaxTree* ast);
    void printIR();

private:
    llvm::LLVMContext TheContext;
    std::unique_ptr<llvm::Module> TheModule;
    llvm::IRBuilder<> Builder;
};

#endif // CODEGENERATOR_H
