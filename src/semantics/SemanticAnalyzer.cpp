#include "semantics/SemanticAnalyzer.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <set>

using namespace std;
using namespace AST;
using namespace ASTConstants;

SemanticAnalyzer::SemanticAnalyzer() {
    //Constructor for SemanticAnalyzer
    //Initialize hash map with instructions and vectors containing sets of valid operands
    m_semanticMap[InstructionType::MOVE] = {
        {OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::LOAD] = {
        {OperandType::MEMORYADDRESS, OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::STORE] = {
        {OperandType::REGISTER},
        {OperandType::MEMORYADDRESS, OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::CREATE] = {
        {OperandType::TYPECONDITION},
        {OperandType::INTEGER, OperandType::CHARACTER, OperandType::BOOLEAN, OperandType::FLOAT, OperandType::MEMORYADDRESS, OperandType::INSTRUCTIONADDRESS},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::CAST] = {
        {OperandType::TYPECONDITION},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::ADD] = {
        {OperandType::REGISTER},
        {OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::SUB] = {
        {OperandType::REGISTER},
        {OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::MULTIPLY] = {
        {OperandType::REGISTER},
        {OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::DIVIDE] = {
        {OperandType::REGISTER},
        {OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::OR] = {
        {OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::AND] = {
        {OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::NOT] = {
        {OperandType::REGISTER},
    };
    m_semanticMap[InstructionType::SHIFT] = {
        {OperandType::SHIFTCONDITION},
        {OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::COMPARE] = {
        {OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::JUMP] = {
        {OperandType::JUMPCONDITION},
        {OperandType::INSTRUCTIONADDRESS, OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::CALL] = {
        {OperandType::INSTRUCTIONADDRESS, OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::PUSH] = {
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::POP] = {
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::RETURN] = {
    };
    m_semanticMap[InstructionType::STOP] = {
    };
    m_semanticMap[InstructionType::INPUT] = {
        {OperandType::TYPECONDITION},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::OUTPUT] = {
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::PRINT] = {
        {OperandType::STRING, OperandType::NEWLINE},
    };
    m_semanticMap[InstructionType::LABEL] = {
        {OperandType::INSTRUCTIONADDRESS}
    };
    m_semanticMap[InstructionType::COMMENT] = {
        {OperandType::STRING}
    };
}

bool SemanticAnalyzer::analyzeSemantics(AST::ASTNode *AST, const std::vector<std::string>& codeLines, std::string &errorMessage) {
    //Parallelize semantic analysis as each instruction is independent and AST is immutable in this state
    #pragma omp parallel for schedule(dynamic) default(none) shared(AST, m_invalidLines, codeLines)
    //Iterate over all children of the AST (instruction nodes)
    for (int i=0; i<AST->getNumChildren(); i++) {
        //Cast the ASTNode to an instruction node
        auto instructionNode = dynamic_cast<AST::InstructionNode*>(AST->childAt(i));
        //If not nullptr and if the node isn't empty
        if (instructionNode != nullptr && !instructionNode->getNodeValue().empty()) {
            //Call the semantic analyzer and analyze the given node
            string error = analyzeLine(instructionNode);
            if (!error.empty()) {
                //If error is present
                //Critical section - STL manipulations are not thread safe
                #pragma omp critical
                {
                    //Add error to error map at index i
                    m_invalidLines[i] = "\nInvalid syntax at line " + to_string(i + 1) + ": " + codeLines[i] + "\n" + error;
                }
            }
        }
    }

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

string SemanticAnalyzer::analyzeLine(AST::InstructionNode *instructionNode) {
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
}

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
