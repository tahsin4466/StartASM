#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "AbstractSyntaxTree.h"

class SemanticAnalyzer {
    public:
        //Constructor/destructor
        SemanticAnalyzer();
        ~SemanticAnalyzer() {};
        //Remove copy and assignment operator
        SemanticAnalyzer(const SemanticAnalyzer&) = delete;
        SemanticAnalyzer& operator=(const SemanticAnalyzer&) = delete;

        //Semantic Analysis Method
        std::string analyzeSemantics(AST::InstructionNode* instructionNode);

    private:
        std::unordered_map<ASTConstants::InstructionType, std::vector<std::unordered_set<ASTConstants::OperandType>>> m_semanticMap;

};
#endif