#include "ScopeChecker.h"

#include <string>
#include <vector>
#include <regex>
#include <mutex>

using namespace std;

bool ScopeChecker::checkAddressScopes(AST::ASTNode *AST, std::string &errorMessage, const std::vector<std::string> &codeLines) {
    //Declare temporary variables for error reporting and finding instruction address index
    std::string errorString;
    std::mutex errorMutex;
    std::string instructionIndex;
    m_codeLines = codeLines;
    int numInstructions = AST->getNumChildren();

    // Loop through all child (instruction) nodes in AST
    // Use OpenMP to parallelize the loop
    #pragma omp parallel for schedule(dynamic) default(none) shared(AST, errorString, errorMutex, numInstructions, registerTemplate, instructionTemplate, memoryTemplate, codeLines)
    for (int i = 0; i < numInstructions; i++) {
        AST::ASTNode* InstructionNode = AST->childAt(i);
        std::string localErrorString;

        // Loop through all operands in instruction parent
        for (int j = 0; j < InstructionNode->getNumChildren(); j++) {
            // Get child node and cast dynamically to operand node
            auto operandNode = dynamic_cast<AST::OperandNode*>(InstructionNode->childAt(j));
            if (operandNode != nullptr) {
                // Switch checking based on operand type
                ASTConstants::OperandType operandType = operandNode->getOperandType();
                switch (operandType) {
                    case ASTConstants::REGISTER:
                        checkRegisterScope(operandNode, localErrorString, i);
                        break;
                    case ASTConstants::MEMORYADDRESS:
                        checkMemoryScope(operandNode, localErrorString, i);
                        break;
                    case ASTConstants::INSTRUCTIONADDRESS:
                        checkInstructionScope(operandNode, localErrorString, i);
                        break;
                    default:
                        break;
                }
            }
        }

        // Lock the mutex before modifying the shared errorString
        if (!localErrorString.empty()) {
            std::lock_guard<std::mutex> lock(errorMutex);
            errorString += localErrorString;
        }
    }

    // Return false if there are errors present in the string
    if (!errorString.empty()) {
        errorMessage += errorString;
        return false;
    }
    return true;
}


void ScopeChecker::checkRegisterScope(AST::OperandNode* operandNode, std::string &errorMessage, int line) {
    // Run a regex template (this one with explicit bounds) to determine if the operand is in scope
    if (!std::regex_match(operandNode->getNodeValue(), registerTemplate)) {
        errorMessage += "\nScope error at line " + std::to_string(line + 1) + ": " + m_codeLines[line] + "\n" + "Register '" + operandNode->getNodeValue() + "' is out of range. Max register is r9\n";
    }
}

void ScopeChecker::checkMemoryScope(AST::OperandNode* operandNode, std::string &errorMessage, int line) {
    if (!std::regex_match(operandNode->getNodeValue(), memoryTemplate)) {
        errorMessage += "\nScope error at line " + std::to_string(line + 1) + ": " + m_codeLines[line] + "\n" + "Memory address '" + operandNode->getNodeValue() + "' is out of range. Max address is m<999999999>\n";
    }
}

void ScopeChecker::checkInstructionScope(AST::OperandNode *operandNode, std::string &errorMessage, int line) {
    // Instruction address both has to adhere to StartASM bounds (4 byte address) and the number of instructions themselves
    // First get the actual instruction index
    std::string localInstructionIndex;
    for (int k = 2; k < operandNode->getNodeValue().size() - 1; k++) {
        localInstructionIndex += operandNode->getNodeValue()[k];
    }
    // If the given instruction index is greater than the number of lines
    if ((std::stoi(localInstructionIndex) > m_codeLines.size())) {
        errorMessage += "\nScope error at line " + std::to_string(line + 1) + ": " + m_codeLines[line] + "\n" + "Instruction address '" + operandNode->getNodeValue() + "' is out of range. Expected i[0]-i[" + std::to_string(m_codeLines.size()) + "]\n";
    }
        // If the instruction index is larger than the StartASM limit
    else if (!std::regex_match(operandNode->getNodeValue(), instructionTemplate)) {
        errorMessage += "\nScope error at line " + std::to_string(line + 1) + ": " + m_codeLines[line] + "\n" + "Instruction address '" + operandNode->getNodeValue() + "' is out of range. Max address is i[999999999]\n";
    }
}

