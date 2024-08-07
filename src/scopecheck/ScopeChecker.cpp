#include "scopecheck/ScopeChecker.h"

#include <string>
#include <vector>
#include <regex>

using namespace std;

ScopeChecker::ScopeChecker(std::vector<std::string> &lines): m_codeLines(lines) {};

bool ScopeChecker::checkAddressScopes(AST::ASTNode *AST, std::string &errorMessage, const std::vector<std::string> &codeLines) {
    //Set the code lines to the given argument
    m_codeLines = codeLines;

    //Visit the root and iterate over the AST
    AST->accept(*this);

    //Concatenate status message string with all error messages
    for (const auto& pair : m_invalidLines) {
        errorMessage += pair.second;
    }

    //Return true if no errors, false otherwise
    if (!errorMessage.empty()) {
        return false;
    }
    return true;
}


void ScopeChecker::visit(AST::RegisterOperand& node) {
    int line = node.getLine();
    // Run a regex template (this one with explicit bounds) to determine if the operand is in scope
    if (!std::regex_match(node.getNodeValue(), registerTemplate)) {
#pragma omp critical
        {
            m_invalidLines[line] += "\nScope error at line " + std::to_string(line + 1) + ": " + m_codeLines[line] + "\n" + "Register '" + node.getNodeValue() + "' is out of range. Max register is r9\n";
        }
    }
}

void ScopeChecker::visit(AST::MemoryAddressOperand& node) {
    int line = node.getLine();
    if (!std::regex_match(node.getNodeValue(), memoryTemplate)) {
#pragma omp critical
        {
            m_invalidLines[line] += "\nScope error at line " + std::to_string(line + 1) + ": " + m_codeLines[line] + "\n" + "Memory address '" + node.getNodeValue() + "' is out of range. Max address is m<999999999>\n";
        }
    }
}

void ScopeChecker::visit(AST::InstructionAddressOperand& node) {
    int line = node.getLine();
    // Instruction address both has to adhere to StartASM bounds (4 byte address) and the number of instructions themselves
    // First get the actual instruction index
    std::string localInstructionIndex;
    for (int k = 2; k < node.getNodeValue().size() - 1; k++) {
        localInstructionIndex += node.getNodeValue()[k];
    }
    // If the given instruction index is greater than the number of lines
    if ((std::stoi(localInstructionIndex) > m_codeLines.size())) {
#pragma omp critical
        {
            m_invalidLines[line] += "\nScope error at line " + std::to_string(line + 1) + ": " + m_codeLines[line] + "\n" + "Instruction address '" + node.getNodeValue() + "' is out of range. Expected i[0]-i[" + std::to_string(m_codeLines.size()) + "]\n";
        }
    }
        // If the instruction index is larger than the StartASM limit
    else if (!std::regex_match(node.getNodeValue(), instructionTemplate)) {
#pragma omp critical
        {
            m_invalidLines[line] += "\nScope error at line " + std::to_string(line + 1) + ": " + m_codeLines[line] + "\n" + "Instruction address '" + node.getNodeValue() + "' is out of range. Max address is i[999999999]\n";
        }
    }
}

