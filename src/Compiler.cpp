#include "include/Compiler.h"
#include "include/InstructionSet.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <omp.h>
#include <sstream>
#include <map>

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
    
        tokenizeCode();
        m_statusMessage = "Code loaded";
        return true;
    }
    else {
        m_statusMessage = "Invalid path!";
        return false;
    }
        
}

void Compiler::tokenizeCode() {
    for (int i = 0; i < m_codeLines.size(); i++) {
        stringstream ss(m_codeLines[i]);
        string token;
        vector<string> tokenizedLine;

        while (ss >> token) {
            tokenizedLine.push_back(token);
        }

        m_codeTokens.push_back(tokenizedLine);
    }
}

bool Compiler::parseCode() {
    int numLines = m_codeLines.size();
    map<int, string> invalidLines;

    string error;
    #pragma omp parallel for private(error)
    for (int i=0; i<numLines; i++) {
        error = m_instructionSet.validateInstruction(m_codeLines[i], m_codeTokens[i]);
        if (error != "") {
            invalidLines[i] = "\nInvalid syntax at line " + to_string(i + 1) + ": '" + m_codeLines[i] + "'\n" + error + "\n";
        }
        error.clear();
    }

    if (!invalidLines.empty()) {
        m_statusMessage.clear();
        for (const auto& line : invalidLines) {
            m_statusMessage += line.second;
        }
        return false;
    }

    m_statusMessage = "Syntax validated";
    return true;
}

bool Compiler::compileCode() {
    return true;
}