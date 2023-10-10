#ifndef COMPILER_H
#define COMPILER_H

#include <string>
#include <vector>

#include "InstructionSet.h"

class Compiler {
    public:
        //Constructors and Destructors
        Compiler(std::string pathname):
            m_pathname(pathname),
            m_statusMessage("Compiler Initialized"),
            m_lineIndex(0) {};
        ~Compiler() {};

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

        //Code Loading
        bool loadCode();

        //Code Checking
        bool checkCode();

        //Code Compiling
        bool compileCode();

        //Code Tokenizing
        bool tokenizeCode();

    private:
        //pathname
        std::string m_pathname;
        //Vector containing code lines
        std::vector<std::string> m_codeLines;
        //Vector containing code tokens
        std::vector<std::vector<std::string>> m_codeTokens;
        //String containing current status
        std::string m_statusMessage;
        //Int containing current line index
        int m_lineIndex;
        //Instruction set
        InstructionSet m_instructionSet;

        //Helper Functions
        
};

#endif