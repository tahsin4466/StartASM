#include "include/CodeGenerator.h"
#include "include/AbstractSyntaxTree.h"
#include <llvm/Support/raw_ostream.h>

CodeGenerator::CodeGenerator()
        : TheModule(std::make_unique<llvm::Module>("StartASM Module", TheContext)),
          Builder(TheContext) {
    // Define a main function in the module
    llvm::FunctionType *funcType = llvm::FunctionType::get(Builder.getInt32Ty(), false);
    llvm::Function *mainFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", TheModule.get());

    // Create a basic block to insert instructions
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(TheContext, "entry", mainFunc);
    Builder.SetInsertPoint(entry);

    // Allocate space for registers
    for (int i = 0; i < 10; ++i) {
        std::string regName = "r" + std::to_string(i);
        m_namedValues[regName] = Builder.CreateAlloca(Builder.getInt32Ty(), nullptr, regName);
    }
}

CodeGenerator::~CodeGenerator() {}

void CodeGenerator::generateCode(AST::InstructionNode* node) {
    if (node == nullptr) {
        return;
    }

    // Handle different IR generations for each instruction type
    switch (node->getInstructionType()) {
        case ASTConstants::InstructionType::MOVE:
            handleMove(node);
            break;
        case ASTConstants::InstructionType::ADD:
            handleAdd(node);
            break;
        case ASTConstants::InstructionType::CREATE:
            handleCreate(node);
            break;
            // TODO: add more cases for other instruction types
        default:
            break;
    }
}

void CodeGenerator::printIR() {
    TheModule->print(llvm::outs(), nullptr);
}

void CodeGenerator::handleAdd(AST::InstructionNode* node) {
    auto leftRegNode = dynamic_cast<AST::OperandNode*>(node->childAt(0));
    auto rightRegNode = dynamic_cast<AST::OperandNode*>(node->childAt(1));
    auto destRegNode = dynamic_cast<AST::OperandNode*>(node->childAt(2));

    if (leftRegNode && rightRegNode && destRegNode) {
        auto leftValueIt = m_namedValues.find(leftRegNode->getNodeValue());
        auto rightValueIt = m_namedValues.find(rightRegNode->getNodeValue());

        if (leftValueIt != m_namedValues.end() && rightValueIt != m_namedValues.end()) {
            llvm::Value* leftPtr = leftValueIt->second;
            llvm::Value* rightPtr = rightValueIt->second;

            llvm::Value* leftValue = Builder.CreateLoad(Builder.getInt32Ty(), leftPtr);
            llvm::Value* rightValue = Builder.CreateLoad(Builder.getInt32Ty(), rightPtr);
            llvm::Value* result = Builder.CreateAdd(leftValue, rightValue);

            llvm::Value* destPtr = m_namedValues[destRegNode->getNodeValue()];
            Builder.CreateStore(result, destPtr);
        } else {
            llvm::errs() << "LLVM Error: Undefined register used in add operation.\n";
        }
    }
}

void CodeGenerator::handleCreate(AST::InstructionNode* node) {
    auto operandType = dynamic_cast<AST::OperandNode*>(node->childAt(0));
    auto valueNode = dynamic_cast<AST::OperandNode*>(node->childAt(1));
    auto destRegNode = dynamic_cast<AST::OperandNode*>(node->childAt(2));

    if (operandType && valueNode && destRegNode) {
        if (valueNode->getOperandType() == ASTConstants::OperandType::INTEGER) {
            int value = std::stoi(valueNode->getNodeValue());
            llvm::Value* llvmValue = llvm::ConstantInt::get(llvm::Type::getInt32Ty(TheContext), value);

            llvm::Value* destPtr = m_namedValues[destRegNode->getNodeValue()];
            Builder.CreateStore(llvmValue, destPtr);

        } else {
            llvm::errs() << "LLVM Error: Unsupported operand type in create operation.\n";
        }
    } else {
        llvm::errs() << "LLVM Error: Invalid create operation.\n";
    }
}

void CodeGenerator::handleMove(AST::InstructionNode* node) {
    auto srcRegNode = dynamic_cast<AST::OperandNode*>(node->childAt(0));
    auto destRegNode = dynamic_cast<AST::OperandNode*>(node->childAt(1));

    if (srcRegNode && destRegNode) {
        auto srcValueIt = m_namedValues.find(srcRegNode->getNodeValue());

        if (srcValueIt != m_namedValues.end()) {
            llvm::Value* srcPtr = srcValueIt->second;
            llvm::Value* srcValue = Builder.CreateLoad(Builder.getInt32Ty(), srcPtr);

            llvm::Value* destPtr = m_namedValues[destRegNode->getNodeValue()];
            Builder.CreateStore(srcValue, destPtr);
        } else {
            llvm::errs() << "LLVM Error: Undefined register used in move operation.\n";
        }
    }
}