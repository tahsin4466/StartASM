#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <memory>
#include <iostream>

#include "ast/Visitor.h"
#include "ast/AbstractSyntaxTree.h"

class CodeGenerator : public AST::Visitor {
    public:
        CodeGenerator();
        ~CodeGenerator() = default;
        //Remove copy and assignment operator
        CodeGenerator(const CodeGenerator&) = delete;
        CodeGenerator& operator=(const CodeGenerator&) = delete;
        void visit(AST::RootNode& node) override;


        // Specific visit methods for each instruction
        void visit(AST::MoveInstruction& node) override;
        void visit(AST::LoadInstruction& node) override;
        void visit(AST::StoreInstruction& node) override;
        void visit(AST::CreateInstruction& node) override;
        void visit(AST::CastInstruction& node) override;
        void visit(AST::AddInstruction& node) override;
        void visit(AST::SubInstruction& node) override;
        void visit(AST::MultiplyInstruction& node) override;
        void visit(AST::DivideInstruction& node) override;
        void visit(AST::OrInstruction& node) override;
        void visit(AST::AndInstruction& node) override;
        void visit(AST::ShiftInstruction& node) override;
        void visit(AST::CompareInstruction& node) override;
        void visit(AST::JumpInstruction& node) override;
        void visit(AST::CallInstruction& node) override;
        void visit(AST::PushInstruction& node) override;
        void visit(AST::PopInstruction& node) override;
        void visit(AST::ReturnInstruction& node) override;
        void visit(AST::StopInstruction& node) override;
        void visit(AST::InputInstruction& node) override;
        void visit(AST::OutputInstruction& node) override;
        void visit(AST::PrintInstruction& node) override;
        void visit(AST::LabelInstruction& node) override;
        void visit(AST::CommentInstruction& node) override;

        // Specific visit methods for each operand
        void visit(AST::RegisterOperand& node) override;
        void visit(AST::InstructionAddressOperand& node) override;
        void visit(AST::MemoryAddressOperand& node) override;
        void visit(AST::IntegerOperand& node) override;
        void visit(AST::FloatOperand& node) override;
        void visit(AST::BooleanOperand& node) override;
        void visit(AST::CharacterOperand& node) override;
        void visit(AST::StringOperand& node) override;
        void visit(AST::NewlineOperand& node) override;
        void visit(AST::TypeConditionOperand& node) override;
        void visit(AST::ShiftConditionOperand& node) override;
        void visit(AST::JumpConditionOperand& node) override;

        void printIR();

    private:
        llvm::LLVMContext context;
        llvm::IRBuilder<> builder;
        std::unique_ptr<llvm::Module> module;
};

#endif
