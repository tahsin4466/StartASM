#include "include/Compiler.h"
#include "include/InstructionSet.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <omp.h>

using namespace std;

bool Compiler::loadFile() {
    m_lineIndex = 0;
    m_statusMessage = "Loading code...";

    string currentLine;
    ifstream codeFile(m_pathname);

    if (codeFile.is_open())
    {
        while (getline(codeFile,currentLine)) {
            if (!currentLine.empty()) {
                m_codeLines.push_back(currentLine);
            }
        }
        codeFile.close();
    
        m_statusMessage = "Code loaded";
        return true;
    }
    else {
        m_statusMessage = "Invalid path!";
        return false;
    }
        
}

bool Compiler::tokenizeCode() {
    /*vector<string> tokenizedLine;
                string token;
                for (int i=0; i<currentLine.size(); i++) {
                    if (currentLine[i] != ' ') {
                        token.push_back(currentLine[i]);
                    }
                    else {
                        tokenizedLine.push_back(token);
                        token.clear();
                    }
                }
                tokenizedLine.push_back(token);
                m_codeTokens.push_back(tokenizedLine); */
    return true;
}

bool Compiler::checkInitialSyntax() {
    int numLines = m_codeLines.size();
    string invalidLines = "";

    #pragma omp parallel for
    for (int i=0; i<numLines; i++) {
        if (!m_instructionSet.isValidInstruction(m_codeLines[i])) {
            invalidLines += "Invalid syntax at line " + to_string(i + 1) + ": '" + m_codeLines[i] + "'\n";
        }
    }

    if (!invalidLines.empty()) {
        m_statusMessage = invalidLines;
        return false;
    }

    m_statusMessage = "Syntax validated";
    return true;
}

bool Compiler::compileCode() {
    return true;
}