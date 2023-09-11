#ifndef COMPILER_H
#define COMPILER_H

#include <string>
#include <vector>

class Compiler {
    public:
        //Structors
        Compiler(std::string pathname) {
            m_pathname = pathname;
            m_lineIndex = 0;
            m_statusMessage = "Compiler initialized";
        }
        ~Compiler() {};

        //Accessors
        int getNumLines() {
            return m_codeLines.size();
        }
        int getLineIndex() {
            return m_lineIndex;
        }
        std::string getStatus() {
            return m_statusMessage;
        }

        //Mutators
        void changePath(std::string pathname) {
            m_pathname = pathname;
        }

        //Code Loading
        bool loadCode();

        //Code Checking
        bool checkCode();

        //Code Compiling
        void compileCode();

    
        
    
    private:
        //Data types
        std::string m_pathname;
        std::vector<std::string> m_codeLines;
        std::string m_statusMessage;
        int m_lineIndex;

        //Helper Functions
        
};

#endif