#ifndef INSTRUCTIONSET_H
#define INSTRUCTIONSET_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <functional>

class Instruction;
class Register;

class InstructionSet {
    public:
        //Constructor and destructor
        InstructionSet();
        ~InstructionSet() {};

        std::string validateInstruction(std::string line, std::vector<std::string> tokens);
        bool isValidRegister(std::string keyword);
        bool isValidMemoryAddress(std::string keyword);


    private:
        //Hash map containing a keyword linked to a parsing function
        std::unordered_map<std::string, std::function<std::string(std::string, std::vector<std::string>)>> m_parsingMap;


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