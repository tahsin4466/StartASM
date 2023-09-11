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

bool Compiler::checkCode() {
    return true;
}

void Compiler::compileCode() {

}