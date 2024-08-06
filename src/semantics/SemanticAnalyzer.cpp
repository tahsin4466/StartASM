#include "semantics/SemanticAnalyzer.h"

#include <string>
#include <vector>

using namespace std;
using namespace AST;
using namespace ASTConstants;

bool SemanticAnalyzer::analyzeSemantics(AST::ASTNode *AST, const std::vector<std::string>& codeLines, std::string &errorMessage) {
    m_lines = codeLines;
    //Prepopulate the local semantic context - an operation will never have >3 operands
    std::vector<ASTConstants::OperandType> localContext(3, ASTConstants::EMPTY); //Set initial operands to empty for easier matching
    //Perallocate the global semantic context based on the number of lines
    m_semanticContext = std::vector<std::vector<ASTConstants::OperandType>>(codeLines.size()+1, localContext);

    //Visit the root and iterate over the AST
    AST->accept(*this);

    //Clear the context
    m_semanticContext.clear();

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

void SemanticAnalyzer::visit(AST::MoveInstruction& node) {
    //CASE 1 - Atomic type instruction
    //Expected semantic structure of instruction
    const vector<ASTConstants::OperandType> semanticTemplate = {REGISTER, REGISTER, EMPTY};
    int line = node.getLine();
    //Check local semantic context
    if (m_semanticContext[line] == semanticTemplate) {
        return; //Return instantly if match found
    }
    //Pass to error handler if a match isn't found
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::CreateInstruction& node) {
    //CASE 2 - Multiple type instruction
    //Expected semantic structure of instruction - some operands can be of multiple types
    const vector<unordered_set<ASTConstants::OperandType>> semanticTemplate = {
            {TYPECONDITION},
            {INTEGER, CHARACTER, BOOLEAN, FLOAT, MEMORYADDRESS, INSTRUCTIONADDRESS},
            {REGISTER}
    };
    int line = node.getLine();
    vector<ASTConstants::OperandType> localContext = m_semanticContext[line];
    //Iterate over possible semantics
    for (int i=0; i<localContext.size(); i++) {
        //Check if every operand in the context is a part of valid operands in the template
        if (semanticTemplate[i].find(localContext[i]) == semanticTemplate[i].end()) {
            //Handle error if not
            handleMultipleInstructionError(line, semanticTemplate, node);
        }
    }
    //Returns successfuly if end of loop is reached
}

void SemanticAnalyzer::visit(AST::RegisterOperand& node) {
    //Insert its type in the local semantic context
    m_semanticContext[node.getLine()][node.getPos()] = node.getOperandType();
}

void SemanticAnalyzer::handleAtomicInstructionError(int line, const std::vector<ASTConstants::OperandType> &expectedTemplate, AST::InstructionNode &node) {
    //Create the invalid line log first
    string errorLine = "Invalid syntax at line " + to_string(line) + ": " + m_lines[line-1] + "\n";
    vector<ASTConstants::OperandType> localContext = m_semanticContext[line];
    //Check every mismatched operand
    for (int i=0; i<localContext.size(); i++) {
        if (localContext[i] != expectedTemplate[i]) {
            if (expectedTemplate[i] != EMPTY) {
                //Unrecognized operand if not expecting an empty space
                 errorLine += "Unrecognized operand '" + node.childAt(i)->getNodeValue() + "'. Expected " + enumToString(expectedTemplate[i]) + "\n";
            }
            else {
                //Excess operand if expecting an empty space
                errorLine += "Unexpected extra operand '" + node.childAt(i)->getNodeValue() + "'\n";
            }
        }
    }
    //Add to the invalid lines map
    m_invalidLines[line] = errorLine;
}

void SemanticAnalyzer::handleMultipleInstructionError(int line, const std::vector<std::unordered_set<ASTConstants::OperandType>> &expectedTemplate, AST::InstructionNode &node) {
    //Create the invalid line log first
    const unordered_set<ASTConstants::OperandType> emptyTemplate = {EMPTY};
    string errorLine = "Invalid syntax at line " + to_string(line) + ": " + m_lines[line-1] + "\n";
    vector<ASTConstants::OperandType> localContext = m_semanticContext[line];

    //Iterate over all given operands in the local context
    for (int i=0; i<localContext.size(); i++) {
        //If a local context token doesn't match any in the template for that index
        if (expectedTemplate[i].find(localContext[i]) == expectedTemplate[i].end()) {
            if (expectedTemplate[i] != emptyTemplate) {
                errorLine += "Unrecognized operand '" + node.childAt(i)->getNodeValue() + "'. Expected ";
                //Add all possible expected operands
                for (auto it = expectedTemplate[i].begin(); it != expectedTemplate[i].end(); ++it) {
                    if (it != expectedTemplate[i].begin()) {
                        errorLine += " or ";
                    }
                    errorLine += enumToString(*it);
                }
                errorLine += "\n";
            }
            else {
                //Excess operand if expecting an empty space
                errorLine += "Unexpected extra operand '" + node.childAt(i)->getNodeValue() + "'\n";
            }
        }
    }
}

/*string SemanticAnalyzer::analyzeLine(AST::InstructionNode *instructionNode) {
    //Create a return string that's empty by default
    string returnString = "";
    //Get the instruction type from the AST for easy access
    InstructionType instructionType = instructionNode->getInstructionType();
    //Find the instruction type in the semantic map
    auto instructionItr = m_semanticMap.find(instructionType);
    //Edge case - instruction isn't found in map
    if (instructionItr == m_semanticMap.end()) {
        return "Compiler error. Could not find decleration of instruction in semantic map";
    }
    //Nullary instruction case - return true (as no children)
    else if (instructionItr->second.size() == 0) {
        return "";
    }
    //Edge case - number of children in the semantic map don't match the AST
    else if (instructionItr->second.size() != instructionNode->getNumChildren()) {
        return "Compiler error. AST nodes do not match number of children required for template";
    }
    else {
        //Get the semantic template for the given instruction
        vector<set<OperandType>>& semanticTemplate = instructionItr->second;
        //Iterate over each valid set of operands for each index in the vector (corresponding to a child index for the instruction)
        for(int i=0; i<semanticTemplate.size(); i++) {
            //Cast the ASTNode to an operand
            OperandNode* operandChild = dynamic_cast<OperandNode*>(instructionNode->childAt(i));
            //Edge case - ASTNode isn't an operand
            if (operandChild == nullptr) {
                return "Compiler error. Child node in AST is not of operand type";
            }
            else {
                //Find if the operand type in the code matches the set deemed valid at the child index
                auto operandItr = semanticTemplate[i].find(operandChild->getOperandType());
                //Throw an error if not found
                if (operandItr==semanticTemplate[i].end()) {
                    //First concatenate return string with the position of the unrecognized syntax type
                    returnString += "Unrecognized syntax type '" + operandChild->getNodeValue() + "'. Expected ";
                    //Next, iterate over all valid operand types adding 'or' at the end
                    for (auto it = semanticTemplate[i].begin(); it != std::prev(semanticTemplate[i].end()); ++it) {
                        returnString += enumToString(*it) + " or ";
                    }
                    //Concatenate the string with the final type, this time without the or
                    returnString += enumToString(*semanticTemplate[i].rbegin()) + "\n";
                }
            }
        }
    }
    //Return the string after checking the whole instruction semantic template
    return returnString;
}*/

string SemanticAnalyzer::enumToString(OperandType type) {
    //This method simply switches an OperandType with a string outlining the expected operand for easy error printing
    switch (type) {
        case REGISTER:
            return "register r0-r9";
        case INSTRUCTIONADDRESS:
            return "instruction address i[...]";
        case MEMORYADDRESS:
            return "memory address m<...>";
        case INTEGER:
            return "integer";
        case FLOAT:
            return "float";
        case BOOLEAN:
            return "boolean";
        case CHARACTER:
            return "character";
        case STRING:
            return "comment string";
        case NEWLINE:
            return "newline";
        case TYPECONDITION:
            return "type condition: integer/float/boolean/character/memory/instruction";
        case SHIFTCONDITION:
            return "shift condition: left/right";
        case JUMPCONDITION:
            return "jump condition: unconditional/greater/less/equal/zero/unequal/nonzero";
        default:
            return "undefined";
    }
}
