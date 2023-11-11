#ifndef COMPILER_H
#define COMPILER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

class InstructionSet;

class Compiler {
    public:
        //Constructors and Destructors
        Compiler(std::string pathname);
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

        //Public facing compile method
        //Code Compiling
        bool compileCode();


    private:
        //Internal parsing methods
        //File loading
        bool loadFile();
        //Code Tokenizing
        void tokenizeCode();
        //Validate the syntax using regex matching
        bool validateSyntax();
        //Resolve symbols and labels
        bool resolveSymbols();
        //Build the AST
        void buildAST();
        //Analyze Semantics
        bool analyzeSemantics();
        //Generate the code
        void generateCode();

        //Private variables
        //Data structures
        //Vector containing code lines
        std::vector<std::string> m_codeLines;
        //Vector containing code tokens
        std::vector<std::vector<std::string>> m_codeTokens;
        //Hash table for symbol resolution, mapping labels to instruction addresses
        std::unordered_map<std::string, std::string> m_labelTable;



        //Variables
        //Pathname
        std::string m_pathname;
        //String containing current status
        std::string m_statusMessage;
        //Int containing current line index
        int m_lineIndex;
        //Instruction set
        InstructionSet* m_instructionSet;
    
        
};

#endif