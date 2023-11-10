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

bool Compiler::compileCode() {
    if(!loadFile()) {
        return false;
    }
    tokenizeCode();
    if(!validateSyntax()) {
        return false;
    }
    if(!resolveSymbols()) {
        return false;
    }
    buildAST();
    if(!analyzeSemantics()) {
        return false;
    }
    generateCode();
    return true;
    
}

bool Compiler::loadFile() {
    //Set line index to 0
    m_lineIndex = 0;
    //Status = loading
    m_statusMessage = "Loading code...";

    //Attempt to open codeFile
    string currentLine;
    ifstream codeFile(m_pathname);

    if (codeFile.is_open())
    {
        //While line exists
        while (getline(codeFile,currentLine)) {
            if (!currentLine.empty()) {
                //Ignore empty lines, push back non empty lines
                m_codeLines.push_back(currentLine);
            }
        }
        codeFile.close();
    
        //Call tokenize() helper function
        tokenizeCode();
        m_statusMessage = "Code loaded";
        return true;
    }
    else {
        //Return error if path not found
        m_statusMessage = "Invalid path!";
        return false;
    }
        
}

void Compiler::tokenizeCode() {
    //split each line into individual tokens
    //Do NOT leverage OMP as order must be maintained
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

bool Compiler::validateSyntax() {
    int numLines = m_codeLines.size();
    //Create an ordered map that links an integer (line number) to an erorr message
    //This is to leverage OMP parallelization while maintaining the order of errors as they appear in the code
    map<int, string> invalidLines;

    //Create error string
    string error;
    //Parallelize error checking as it is more resource intensive
    #pragma omp parallel for private(error)
    for (int i=0; i<numLines; i++) {
        //Call validateInstruction in InstructionSet
        error = m_instructionSet.validateInstruction(m_codeLines[i], m_codeTokens[i]);
        //If an error is present
        if (error != "") {
            //Print the excepted line and the syntax error returned from validateInstruction()
            invalidLines[i] = "\nInvalid syntax at line " + to_string(i + 1) + ": '" + m_codeLines[i] + "'\n" + error + "\n";
        }
        error.clear();
    }

    //Concatenate the statusMessage string from the map (which should be ordered already)
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

bool Compiler::resolveSymbols() {
    return true;
}

void Compiler::buildAST() {
    
}

bool Compiler::analyzeSemantics() {
    return true;
}


void Compiler::generateCode() {

}