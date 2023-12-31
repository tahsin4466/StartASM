#ifndef INSTRUCTIONSET_H
#define INSTRUCTIONSET_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <functional>
#include "AbstractSyntaxTree.h"

class InstructionSet {
    public:
        //Constructor and destructor
        InstructionSet();
        ~InstructionSet() {};

        //Delete copy and assignment
        InstructionSet(const InstructionSet&) = delete;
        InstructionSet& operator=(const InstructionSet&) = delete;

        //Getters
        //Get num operands
        int getNumOperands() {return m_operandList.size();};
        //Get num instructions
        int getNumInstructions() {return m_instructionMap.size();};

        //Validate instruction method
        std::string validateInstruction(std::string line, std::vector<std::string> tokens);

        //Return pair of operand type and semantic type for given instruction
        std::pair<NumOperands, SemanticType>& returnInstructionInfo(std::string instructionKeyword);
        //Return pair of regex template and operand type for operand
        std::pair<std::string, OperandType>& returnOperandInfo(int index) {return m_operandList.at(index);};

    private:
        //Hash map containing a keyword linked to a parsing function
        std::unordered_map<std::string, std::function<std::string(std::string, std::vector<std::string>)>> m_parsingMap;
        //Hash map containing a set of instruction keywords
        std::unordered_map<std::string, std::pair<NumOperands, SemanticType>> m_instructionMap;
        //Vector containing a set of operand regex templates
        std::vector<std::pair<std::string, OperandType>> m_operandList;

        //Private Parsing Functions
        static std::string parseMove(std::string line, std::vector<std::string> tokens);
        static std::string parseLoad(std::string line, std::vector<std::string> tokens);
        static std::string parseStore(std::string line, std::vector<std::string> tokens);
        static std::string parseCreate(std::string line, std::vector<std::string> tokens);
        static std::string parseCast(std::string line, std::vector<std::string> tokens);
        static std::string parseAdd(std::string line, std::vector<std::string> tokens);
        static std::string parseSub(std::string line, std::vector<std::string> tokens);
        static std::string parseMultiply(std::string line, std::vector<std::string> tokens);
        static std::string parseDivide(std::string line, std::vector<std::string> tokens);
        static std::string parseOr(std::string line, std::vector<std::string> tokens);
        static std::string parseAnd(std::string line, std::vector<std::string> tokens);
        static std::string parseNot(std::string line, std::vector<std::string> tokens);
        static std::string parseShift(std::string line, std::vector<std::string> tokens);
        static std::string parseCompare(std::string line, std::vector<std::string> tokens); 
        static std::string parseJump(std::string line, std::vector<std::string> tokens);
        static std::string parseCall(std::string line, std::vector<std::string> tokens);
        static std::string parsePush(std::string line, std::vector<std::string> tokens);
        static std::string parsePop(std::string line, std::vector<std::string> tokens);
        static std::string parseReturn(std::string line, std::vector<std::string> tokens);
        static std::string parseStop(std::string line, std::vector<std::string> tokens);
        static std::string parseLabel(std::string line, std::vector<std::string> tokens);
        static std::string parseComment(std::string line, std::vector<std::string> tokens);

        //Helper Jump Parsing Functions
        static std::string parseUnconditionalJump(std::string line, std::vector<std::string> tokens);
        static std::string parseConditionalJump(std::string line, std::vector<std::string> tokens);
        static std::string parseConditionalComplementJump(std::string line, std::vector<std::string> tokens);

        //Helper Create Parsing Functions
        static std::string parseCreateInteger(std::string line, std::vector<std::string> tokens);
        static std::string parseCreateFloat(std::string line, std::vector<std::string> tokens);
        static std::string parseCreateCharacter(std::string line, std::vector<std::string> tokens);
        static std::string parseCreateBoolean(std::string line, std::vector<std::string> tokens);
        static std::string parseCreateAddress(std::string line, std::vector<std::string> tokens);
};

#endif