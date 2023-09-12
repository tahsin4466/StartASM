#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

class InstructionSet;
class Register;
class MemoryAddress;

#include <vector>
#include <string>
#include <iostream>

class Instruction {
    public:
        //Constructor and destructor, taking the InstructionSet pointer, an ID and number of operands
        Instruction(InstructionSet* set, int ID, int numOperands):
            m_instructionSet(set),
            m_instructionID(ID),
            m_numOperands(numOperands) {};
        virtual ~Instruction() {};

        //Virtual execute() function, Instruction class is an ABC
        virtual void execute() = 0;

    private:
        //Pointer to Instruction Set
        InstructionSet* m_instructionSet;
        //Instruction ID
        int m_instructionID;
        //Number of Operands
        int m_numOperands;
        //Template
        std::vector<std::string> m_template;
        
}; 

class MoveInstruction: public Instruction {
    public:
        MoveInstruction(InstructionSet* set, int ID):
            Instruction(set, 0, 2) {};
        virtual ~MoveInstruction() {};

    virtual void execute();

    private:
        Register* m_srcRegister;
        Register* m_destRegister;

};

#endif