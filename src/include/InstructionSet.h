#ifndef INSTRUCTIONSET_H
#define INSTRUCTIONSET_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <functional>

enum SemanticType {NONE, FROM, TO, SELF, FROMTO, SELFWITH, SELFBY, FROMWITHTO};
enum OperandType {REGISTER, INSTRUCTION, MEMORY, VALUE};
enum NumOperands {NULLARY, UNARY, BINARY, TERNARY};

class InstructionSet {
    public:
        //Constructor and destructor
        InstructionSet();
        ~InstructionSet() {};

        InstructionSet(const InstructionSet&) = delete;
        InstructionSet& operator=(const InstructionSet&) = delete;

        std::string validateInstruction(std::string line, std::vector<std::string> tokens);


    private:
        //Hash map containing a keyword linked to a parsing function
        std::unordered_map<std::string, std::function<std::string(std::string, std::vector<std::string>)>> m_parsingMap;
        //Hash map containing a set of instruction keywords
        std::unordered_map<std::string, std::pair<NumOperands, SemanticType>> m_instructionMap;
        //Hash map containing a set of operand regex templates
        std::unordered_map<std::string, OperandType> m_operandMap;
        //Hash map containing a set of conjunction keywords
        std::unordered_set<std::string> m_conjunctionSet;

        //Private Parsing Functions
        static std::string parseMove(std::string line, std::vector<std::string> tokens);
        static std::string parseLoad(std::string line, std::vector<std::string> tokens);
        static std::string parseStore(std::string line, std::vector<std::string> tokens);
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

        //Helper Parsing Functions
        static std::string parseUnconditionalJump(std::string line, std::vector<std::string> tokens);
        static std::string parseConditionalJump(std::string line, std::vector<std::string> tokens);
        static std::string parseConditionalComplementJump(std::string line, std::vector<std::string> tokens);
};

#endif