#ifndef STARTASM_ASTBUILDER_H
#define STARTASM_ASTBUILDER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <regex>
#include <functional>

#include "pt/ParseTree.h"
#include "ast/Instructions.h"
#include "ast/Operands.h"

class ASTBuilder {
public:
    ASTBuilder();
    ~ASTBuilder() = default;
    ASTBuilder(const ASTBuilder&) = delete;
    ASTBuilder& operator=(const ASTBuilder&) = delete;

    void buildAST(PT::PTNode* parseTree, AST::AbstractSyntaxTree* abstractSyntaxTree);

private:
    using InstructionFactory = std::function<AST::InstructionNode*(const std::string&, int)>;
    std::unordered_map<ASTConstants::InstructionType, InstructionFactory> instructionFactoryMap;

    using OperandFactory = std::function<AST::OperandNode*(const std::string&)>;
    std::unordered_map<ASTConstants::OperandType, OperandFactory> operandFactoryMap;

    void initializeFactoryMaps();
    AST::InstructionNode* instructionBuilder(ASTConstants::InstructionType nodeType, const std::string& value, int line);
    AST::OperandNode* operandBuilder(ASTConstants::OperandType nodeType, const std::string& value);
};

#endif // STARTASM_ASTBUILDER_H
