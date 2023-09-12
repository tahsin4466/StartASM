#include "include/Compiler.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

bool Compiler::loadCode() {
    m_lineIndex = 0;
    m_statusMessage = "Loading code...";

    string currentLine;
    ifstream codeFile(m_pathname);

    if (codeFile.is_open())
    {
        while (getline(codeFile,currentLine)) {
            if (!currentLine.empty()) {
                vector<string> tokenizedLine;
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
                m_codeLines.push_back(tokenizedLine);
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

bool Compiler::checkCode() {
    return true;
}

void Compiler::compileCode() {

}