#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

class InstructionSet;

#include "Operands.h"

#include <vector>
#include <string>
#include <iostream>



//HIGH LEVEL INSTRUCTION CLASS
//Defines the fundamental characteristics for an instruction, including a pointer to the set, an ID, num operands and the template to
//verify syntax
//Instruction is an ABC - it contains the execute() method that is defined in the most derived instruction class (e.g. mov, div, xor etc)
class Instruction {
    public:
        //Constructor and destructor, taking the InstructionSet pointer, an ID and number of operands
        Instruction(InstructionSet* set, int ID, int numOperands):
            m_instructionSet(set),
            m_instructionID(ID),
            m_numOperands(numOperands) {};
        virtual ~Instruction() {};

        //Getters
        int getID() {return m_instructionID;}
        int getNumOperands() {return m_numOperands;}

        //Virtual execute() function, Instruction class is an ABC
        virtual void execute() = 0;

    private:
        //Pointer to Instruction Set
        InstructionSet* m_instructionSet;
        //Instruction ID
        int m_instructionID;
        //Number of Operands
        int m_numOperands;
        //Template, to check the syntax structure for error checking. Validates the order and spelling of tokens for each unqiue instruction
        std::vector<std::string> m_template;
        
};



//MID LEVEL OPERAND CLASS
//Defines the parent class of each instruction based on number of operands
//Contains protected pointers to each operand to be accessed by most derived instruction classes
//Opernad classes are ABCS - execute() is defined only in most inhereited class
class NoOperandInstruction: public Instruction {
    public:
        NoOperandInstruction(InstructionSet* set, int ID):
            Instruction(set, ID, 0) {};
        virtual ~NoOperandInstruction() {};

        virtual void execute() = 0;
};

class OneOperandInstruction: public Instruction {
    public:
        OneOperandInstruction(InstructionSet* set, int ID, Operand* srcOperand):
            Instruction(set, ID, 1),
            m_srcOperand(srcOperand) {};
        virtual ~OneOperandInstruction() {};

        virtual void execute() = 0;

    protected:
        Operand* m_srcOperand;
};

class TwoOperandInstruction: public Instruction {
    public:
        TwoOperandInstruction(InstructionSet* set, int ID, Operand* srcOperand, Operand* destOperand):
            Instruction(set, ID, 2),
            m_srcOperand(srcOperand),
            m_destOperand(destOperand) {};
        virtual ~TwoOperandInstruction() {};

        virtual void execute() = 0;

    protected:
        Operand* m_srcOperand;
        Operand* m_destOperand;
};

class ThreeOperandInstruction: public Instruction {
    public:
        ThreeOperandInstruction(InstructionSet* set, int ID, Operand* srcOperand1, Operand* srcOperand2, Operand* destOperand):
            Instruction(set, ID, 3),
            m_srcOperand1(srcOperand1),
            m_srcOperand2(srcOperand2),
            m_destOperand(destOperand) {};
        virtual ~ThreeOperandInstruction() {};

        virtual void execute() = 0;

    protected:
        Operand* m_srcOperand1;
        Operand* m_srcOperand2;
        Operand* m_destOperand;
};



//LOW LEVEL INSTRUCTION CLASSES
//Contains defenition for execute() method
//Can access inherited member variables of reigster pointers
//Are not ABCS - execute() is defined
class MoveInstruction: public TwoOperandInstruction {
    public:
        MoveInstruction(InstructionSet* set, GeneralRegister* srcRegister, GeneralRegister* destRegister):
            TwoOperandInstruction(set, 0, srcRegister, destRegister) {};
        virtual ~MoveInstruction() {};

    virtual void execute();

};

#endif