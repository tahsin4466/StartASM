#ifndef STARTASM_SCOPECHECKER_H
#define STARTASM_SCOPECHECKER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <regex>
#include <map>

#include "ast/Instructions.h"
#include "ast/Operands.h"
#include "ast/Visitor.h"

class ScopeChecker: public AST::Visitor {
public:
    //Constructor/destructor
    explicit ScopeChecker(std::vector<std::string>& lines);
    ~ScopeChecker() = default;
    //Delete copy and assignment
    ScopeChecker(const ScopeChecker&) = delete;
    ScopeChecker& operator=(const ScopeChecker&) = delete;

    //Main address scope checking function
    bool checkAddressScopes(AST::ASTNode* AST, std::string& errorMessage, const std::vector<std::string>& codeLines);

private:
    //Error messages map and code lines
    std::vector<std::string> m_codeLines;
    std::map<int, std::string> m_invalidLines;

    //Regex templates
    const std::regex registerTemplate = std::regex("r[0-9]");
    const std::regex instructionTemplate = std::regex("i\\[[0-9]{1,9}\\]");
    const std::regex memoryTemplate = std::regex("m<[0-9]{1,9}>");

    // Visitor Methods
    void visit(AST::RootNode& node) override {};

    void visit(AST::MoveInstruction& node) override {};
    void visit(AST::LoadInstruction& node) override {};
    void visit(AST::StoreInstruction& node) override {};
    void visit(AST::CreateInstruction& node) override {};
    void visit(AST::CastInstruction& node) override {};
    void visit(AST::AddInstruction& node) override {};
    void visit(AST::SubInstruction& node) override {};
    void visit(AST::MultiplyInstruction& node) override {};
    void visit(AST::DivideInstruction& node) override {};
    void visit(AST::OrInstruction& node) override {};
    void visit(AST::AndInstruction& node) override {};
    void visit(AST::NotInstruction& node) override {};
    void visit(AST::ShiftInstruction& node) override {};
    void visit(AST::CompareInstruction& node) override {};
    void visit(AST::JumpInstruction& node) override {};
    void visit(AST::CallInstruction& node) override {};
    void visit(AST::PushInstruction& node) override {};
    void visit(AST::PopInstruction& node) override {};
    void visit(AST::ReturnInstruction& node) override {};
    void visit(AST::StopInstruction& node) override {};
    void visit(AST::InputInstruction& node) override {};
    void visit(AST::OutputInstruction& node) override {};
    void visit(AST::PrintInstruction& node) override {};
    void visit(AST::LabelInstruction& node) override {};
    void visit(AST::CommentInstruction& node) override {};

    void visit(AST::RegisterOperand& node) override;
    void visit(AST::InstructionAddressOperand& node) override;
    void visit(AST::MemoryAddressOperand& node) override;
    void visit(AST::IntegerOperand& node) override {};
    void visit(AST::FloatOperand& node) override {};
    void visit(AST::BooleanOperand& node) override {};
    void visit(AST::CharacterOperand& node) override {};
    void visit(AST::StringOperand& node) override {};
    void visit(AST::NewlineOperand& node) override {};
    void visit(AST::TypeConditionOperand& node) override {};
    void visit(AST::ShiftConditionOperand& node) override {};
    void visit(AST::JumpConditionOperand& node) override {};

};

#endif //STARTASM_SCOPECHECKER_H
