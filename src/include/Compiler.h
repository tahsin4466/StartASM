#ifndef COMPILER_H
#define COMPILER_H

#include "Lexer.h"
#include "AbstractSyntaxTree.h"

#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

class Parser;
class SemanticAnalyzer;
class CodeGenerator;

class Compiler {
    public:
        //Constructors and Destructors
        Compiler(std::string& pathname, bool cmdSilent, bool cmdTimings, bool cmdTree, bool cmdIr);
        ~Compiler();

        Compiler(const Compiler&) = delete;
        Compiler& operator=(const Compiler&) = delete;

        //Accessors
        //Get number of lines
        [[nodiscard]] int getNumLines() const {
            return int(m_codeLines.size());
        }
        //Get current status
        [[nodiscard]] std::string getStatus() const {
            return m_statusMessage;
        }

        //Mutators
        //Change pathname
        void changePath(std::string& pathname) {
            m_pathname = pathname;
        }

        //Printers
        void cmdPrint(const std::string& message) const;
        void cmdTimingPrint(const std::string& message) const;

        //Public facing compile method
        //Code Compiling
        bool compileCode();


    private:
        //Internal parsing methods
        //Resolve symbols and labels
        bool resolveSymbols();
        //Build the AST
        void buildAST();
        //Analyze Semantics
        bool analyzeSemantics();
        //Check address scope
        bool checkAddressScopes();
        //Generate the code
        void generateCode();

        //Private variables
        //Data structures
        //Vector containing code lines
        std::vector<std::string> m_codeLines;
        //Vector containing code tokens and tags
        std::vector<std::vector<std::pair<std::string, LexerConstants::TokenType>>> m_codeTokens;
        //Parse tree for the code
        PT::ParseTree* m_parseTree;
        //Hash table for symbol resolution, mapping labels to instruction addresses
        std::unordered_map<std::string, std::pair<std::string, int>> m_symbolTable;

        //Variables
        //Pathname
        std::string m_pathname;
        //String containing current status
        std::string m_statusMessage;
        //Lexer
        Lexer* m_lexer;
        //Parser (PT nested inside parser)
        Parser* m_parser;
        //Pointer to AST (used directly by the compiler at multiple stages)
        AST::AbstractSyntaxTree* m_AST;   
        //Pointer to semantic analyzer
        SemanticAnalyzer* m_semanticAnalyzer;
        //Pointer to code generator
        CodeGenerator* m_codeGenerator;

        //Terminal options
        bool cmd_silent;
        bool cmd_timings;
        bool cmd_tree;
        bool cmd_ir;
};

#endif