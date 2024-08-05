#ifndef STARTASM_SCOPECHECKER_H
#define STARTASM_SCOPECHECKER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <regex>
#include <map>

#include "ast/AbstractSyntaxTree.h"

class ScopeChecker {
    public:
        //Constructor/destructor
        ScopeChecker() = default;
        ~ScopeChecker() = default;
        //Delete copy and assignment
        ScopeChecker(const ScopeChecker&) = delete;
        ScopeChecker& operator=(const ScopeChecker&) = delete;
        //Main address scope checking function
        bool checkAddressScopes(AST::ASTNode* AST, std::string& errorMessage, const std::vector<std::string>& codeLines);

    private:
        //Helper operand scope checkers
        void checkRegisterScope(AST::OperandNode* operandNode, std::string& errorMessage, int line);
        void checkMemoryScope(AST::OperandNode* operandNode, std::string& errorMessage, int line);
        void checkInstructionScope(AST::OperandNode* operandNode, std::string& errorMessage, int line);

        //Error messages map and code lines
        std::vector<std::string> m_codeLines;
        std::map<int, std::string> m_invalidLinesMap;

        //Regex templates
        const std::regex registerTemplate = std::regex("r[0-9]");
        const std::regex instructionTemplate = std::regex("i\\[[0-9]{1,9}\\]");
        const std::regex memoryTemplate = std::regex("m<[0-9]{1,9}>");

};

#endif //STARTASM_SCOPECHECKER_H
