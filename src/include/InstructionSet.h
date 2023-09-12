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
        InstructionSet() {};
        ~InstructionSet() {
            std::unordered_map<std::string, Instruction*>::iterator iti;
            for (iti=m_instructionMap.begin(); iti!=m_instructionMap.end(); iti++) {
                delete iti->second;
            }

            std::unordered_map<std::string, Register*>::iterator itr;
            for (itr=m_registerMap.begin(); itr!=m_registerMap.end(); itr++) {
                delete itr->second;
            }
        };

        bool isValidInstruction(std::string keyword);
        bool isValidRegister(std::string keyword);
        bool isValidMemoryAddress(std::string keyword);

        std::vector<std::string> getTemplate(std::string keyword) {return m_templateMap.at(keyword);}


    private:
        //Hash map containing a keyword linked to an instruction object
        std::unordered_map<std::string, Instruction*> m_instructionMap;
        //Hash map containing a keyword linked to a register
        std::unordered_map<std::string, Register*> m_registerMap;
        //Hash map containing a keyword linked to a template
        std::unordered_map<std::string, std::vector<std::string>> m_templateMap;


};

#endif