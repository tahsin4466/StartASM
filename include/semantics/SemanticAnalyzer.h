#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include <string>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <set>
#include <map>

#include "ast/Instructions.h"
#include "ast/Operands.h"
#include "ast/Visitor.h"


class SemanticAnalyzer: public AST::Visitor {
    public:
        //Constructor/destructor
        SemanticAnalyzer() = default;
        ~SemanticAnalyzer() = default;
        //Remove copy and assignment operator
        SemanticAnalyzer(const SemanticAnalyzer&) = delete;
        SemanticAnalyzer& operator=(const SemanticAnalyzer&) = delete;

        //Main Semantic Analysis Method
        bool analyzeSemantics(AST::ASTNode *AST, const std::vector<std::string>& codeLines, std::string &errorMessage);


    private:
        //Data structure to store local semantic context
        std::vector<std::vector<ASTConstants::OperandType>> m_semanticContext;
        //Data structure for errors
        std::map<int, std::string> m_invalidLines;
        //Reference to code lines
        std::vector<std::string>& m_lines;

        //Visitor Methods
        void visit(AST::RootNode& node) override {};

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

        //Helper functions
        void handleAtomicInstructionError(int line, const std::vector<ASTConstants::OperandType>& expectedTemplate, AST::InstructionNode& node); //Handle error logging for atomic instructions
        void handleMultipleInstructionError(int line, const std::vector<std::unordered_set<ASTConstants::OperandType>>& expectedTemplate, AST::InstructionNode& node); //Handle error logging for multiple type instructions
        std::string enumToString(ASTConstants::OperandType type); //Error logging helper function

};
#endif