#ifndef INSTRUCTIONSET_H
#define INSTRUCTIONSET_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

class Instruction;
class Register;

class InstructionSet {
    public:
        //Constructor and destructor
        InstructionSet();
        ~InstructionSet() {};

        bool isValidInstruction(std::string keyword);
        bool isValidRegister(std::string keyword);
        bool isValidMemoryAddress(std::string keyword);


    private:
        //Hash map containing a keyword linked to a template
        std::unordered_map<std::string, std::vector<std::string>> m_templateMap;
        //Hash map containing a keyword linked to a compilation function
        std::unordered_map<std::string, std::function<bool()>> m_compilerMap;

        //Private Parzing Functions
        bool parseMove();
        bool parseLoad();
        bool parseStore();
        bool parseAdd();
        bool parseSub();
        bool parseMultiply();
        bool parseDivide();
        bool parseOr();
        bool parseNor();
        bool parseXor();
        bool parseAnd();
        bool parseNot();
        bool parseShift();
        bool parseJump();
        bool parsePush();
        bool parsePop();


};

#endif