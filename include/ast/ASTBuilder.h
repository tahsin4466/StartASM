#ifndef STARTASM_ASTBUILDER_H
#define STARTASM_ASTBUILDER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <regex>

#include "pt/ParseTree.h"
#include "ast/Instructions.h"

class ASTBuilder {
    public:
        ASTBuilder() = default;
       ~ASTBuilder() = default;
        ASTBuilder(const ASTBuilder&) = delete;
        ASTBuilder& operator=(const ASTBuilder&) = delete;

        void buildAST (PT::PTNode* parseTree, AST::AbstractSyntaxTree* abstractSyntaxTree);

    private:
        AST::InstructionNode* instructionBuilder(ASTConstants::InstructionType nodeType, std::string value, int line);
        void operandBuilder(ASTConstants::OperandType nodeType, std::string value, int line);
};
#endif //STARTASM_ASTBUILDER_H
