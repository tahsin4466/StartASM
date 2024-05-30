#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include "AbstractSyntaxTree.h"
#include <unordered_map>
#include <string>

class CodeGenerator {
public:
    CodeGenerator();
    ~CodeGenerator();

    void generateCode(AST::InstructionNode* node);
    void printIR();

private:
    llvm::LLVMContext TheContext;
    std::unique_ptr<llvm::Module> TheModule;
    llvm::IRBuilder<> Builder;

    std::unordered_map<std::string, llvm::Value*> m_namedValues;

    // Handlers for specific instructions
    void handleMove(AST::InstructionNode* node);
    void handleCreate(AST::InstructionNode* node);
    void handleAdd(AST::InstructionNode* node);
};

#endif // CODEGENERATOR_H