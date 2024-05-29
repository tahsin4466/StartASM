#include "include/CodeGenerator.h"
#include "include/AbstractSyntaxTree.h"

#include <llvm/Support/raw_ostream.h>

CodeGenerator::CodeGenerator()
        : TheModule(std::make_unique<llvm::Module>("StartASM Module", TheContext)),
          Builder(TheContext) {}

CodeGenerator::~CodeGenerator() {}

void CodeGenerator::generateCode(AST::InstructionNode* node) {
    if (node == nullptr) {
        return;
    }

    //Handle different IR generations for each instruction type
    switch (node->getInstructionType()) {
        case ASTConstants::InstructionType::CREATE:
            handleCreate(node);
            break;
        case ASTConstants::InstructionType::MOVE:
            handleMove(node);
            break;
        case ASTConstants::InstructionType::LOAD:
            handleLoad(node);
            break;
        case ASTConstants::InstructionType::ADD:
            handleAdd(node);
            break;
            // TODO add more cases for other instruction types
        default:
            break;
    }
}

void CodeGenerator::printIR() {
    TheModule->print(llvm::outs(), nullptr);
}

void CodeGenerator::handleAdd(AST::InstructionNode *node) {

}

void CodeGenerator::handleCreate(AST::InstructionNode *node) {

}

void CodeGenerator::handleLoad(AST::InstructionNode *node) {

}

void CodeGenerator::handleMove(AST::InstructionNode *node) {

}