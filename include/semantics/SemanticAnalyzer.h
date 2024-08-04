#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <map>

#include "ast/AbstractSyntaxTree.h"

class SemanticAnalyzer {
    public:
        //Constructor/destructor
        SemanticAnalyzer();
        ~SemanticAnalyzer() {};
        //Remove copy and assignment operator
        SemanticAnalyzer(const SemanticAnalyzer&) = delete;
        SemanticAnalyzer& operator=(const SemanticAnalyzer&) = delete;

        //Semantic Analysis Method
        bool analyzeSemantics (AST::ASTNode* AST, const std::vector<std::string>& codeLines, std::string& errorMessage);

    private:
        //Data structure to store semantic map
        std::unordered_map<ASTConstants::InstructionType, std::vector<std::set<ASTConstants::OperandType>>> m_semanticMap;
        //Data structure for errors
        std::map<int, std::string> m_invalidLines;
        //Helper function to analyze line
        std::string analyzeLine(AST::InstructionNode* instructionNode);
        //Method to convert AST enum to string
        std::string enumToString(ASTConstants::OperandType type);

};
#endif