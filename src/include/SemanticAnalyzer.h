#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>

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
        //Data structure to store semantic map
        std::unordered_map<ASTConstants::InstructionType, std::vector<std::set<ASTConstants::OperandType>>> m_semanticMap;
        //Method to convert AST enum to string
        std::string enumToString(ASTConstants::OperandType type);

};
#endif