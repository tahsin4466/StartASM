#ifndef STARTASM_ASTBUILDER_H
#define STARTASM_ASTBUILDER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <regex>

#include "ParseTree.h"
#include "AbstractSyntaxTree.h"

class ASTBuilder {
    public:
        ASTBuilder() = default;
       ~ASTBuilder() = default;
        ASTBuilder(const ASTBuilder&) = delete;
        ASTBuilder& operator=(const ASTBuilder&) = delete;

        void buildAST (PT::PTNode* parseTree, AST::AbstractSyntaxTree* abstractSyntaxTree);
};
#endif //STARTASM_ASTBUILDER_H
