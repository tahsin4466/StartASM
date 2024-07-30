
#ifndef STARTASM_SYMBOLRESOLVER_H
#define STARTASM_SYMBOLRESOLVER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <regex>

#include "ParseTree.h"

class SymbolResolver {
    public:
        //Constructor/destructor
        SymbolResolver() = default;
        ~SymbolResolver() = default;
        //Delete copy and assignment
        SymbolResolver(const SymbolResolver&) = delete;
        SymbolResolver& operator=(const SymbolResolver&) = delete;

        //Main symbol resolution function
        bool resolveSymbols(std::unordered_map<std::string, std::pair<std::string, int>>& symbolTable, PT::PTNode* parseTree, std::string& errorMessage, const std::vector<std::string>& codeLines);

    private:
        //Helper functions
        void buildSymbolTable(std::unordered_map<std::string, std::pair<std::string, int>>& symbolTable, PT::PTNode* parseTree, const std::vector<std::string>& codeLines);
        void bindSymbols(std::unordered_map<std::string, std::pair<std::string, int>>& symbolTable, PT::PTNode* parseTree, const std::vector<std::string>& codeLines);

        //Error messages map
        std::map<int, std::string> m_invalidLinesMap;

};

#endif //STARTASM_SYMBOLRESOLVER_H
