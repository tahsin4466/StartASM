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

class Compiler {
    public:
        //Constructors and Destructors
        Compiler(std::string pathname, bool cmdSilent, bool cmdTimings, bool cmdTree);
        ~Compiler();

        Compiler(const Compiler&) = delete;
        Compiler& operator=(const Compiler&) = delete;

        //Accessors
        //Get number of lines
        int getNumLines() {
            return m_codeLines.size();
        }
        //Get current line index
        int getLineIndex() {
            return m_lineIndex;
        }
        //Get current status
        std::string getStatus() {
            return m_statusMessage;
        }

        //Mutators
        //Change pathname
        void changePath(std::string pathname) {
            m_pathname = pathname;
        }

        //Printers
        void cmdPrint(std::string message);
        void cmdTimingPrint(std::string message);

        //Public facing compile method
        //Code Compiling
        bool compileCode();


    private:
        //Internal parsing methods
        //File loading
        bool loadFile();
        //Code Tokenizing
        void lexCode();
        //Validate the syntax using regex matching
        bool parseCode();
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
        //Hash table for symbol resolution, mapping labels to instruction addresses
        std::unordered_map<std::string, std::pair<std::string, int>> m_symbolTable;

        //Variables
        //Pathname
        std::string m_pathname;
        //String containing current status
        std::string m_statusMessage;
        //Int containing current line index
        int m_lineIndex;
        //Lexer
        Lexer* m_lexer;
        //Parser (PT nested inside parser)
        Parser* m_parser;
        //Pointer to AST (used directly by the compiler at multiple stages)
        AST::AbstractSyntaxTree* m_AST;   
        //Pointer to semantic analyzer
        SemanticAnalyzer* m_semanticAnalyzer;

        //Terminal options
        bool cmd_silent;
        bool cmd_timings;
        bool cmd_tree;
};

#endif