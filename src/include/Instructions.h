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
        OneOperandInstruction(InstructionSet* set, int ID, Operand* operand1):
            Instruction(set, ID, 1),
            m_operand1(operand1) {};
        virtual ~OneOperandInstruction() {};

        virtual void execute() = 0;

    protected:
        Operand* m_operand1;
};

class TwoOperandInstruction: public Instruction {
    public:
        TwoOperandInstruction(InstructionSet* set, int ID, Operand* operand1, Operand* operand2):
            Instruction(set, ID, 2),
            m_operand1(operand1),
            m_operand2(operand2) {};
        virtual ~TwoOperandInstruction() {};

        virtual void execute() = 0;

    protected:
        Operand* m_operand1;
        Operand* m_operand2;
};

class ThreeOperandInstruction: public Instruction {
    public:
        ThreeOperandInstruction(InstructionSet* set, int ID, Operand* operand1, Operand* operand2, Operand* operand3):
            Instruction(set, ID, 3),
            m_operand1(operand1),
            m_operand2(operand2),
            m_operand3(operand3) {};
        virtual ~ThreeOperandInstruction() {};

        virtual void execute() = 0;

    protected:
        Operand* m_operand1;
        Operand* m_operand2;
        Operand* m_operand3;
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

class LoadValueInstruction: public OneOperandInstruction {
    public:
        LoadValueInstruction(InstructionSet* set, std::vector<uint8_t> srcValue, GeneralRegister* destRegister):
            OneOperandInstruction(set, 1, destRegister) {};
        virtual ~LoadValueInstruction() {};

    virtual void execute();

};

class LoadMemoryInstruction: public TwoOperandInstruction {
    public:
        LoadMemoryInstruction(InstructionSet* set, HeapMemory* srcMemory, GeneralRegister* destRegister):
            TwoOperandInstruction(set, 2, srcMemory, destRegister) {};
    virtual ~LoadMemoryInstruction() {};

    virtual void execute();
};

class StoreMemoryInstruction: public TwoOperandInstruction {
    public:
        StoreMemoryInstruction(InstructionSet* set, GeneralRegister* srcRegister, HeapMemory* destMemory):
            TwoOperandInstruction(set, 3, srcRegister, destMemory) {};
        virtual ~StoreMemoryInstruction() {};

        virtual void execute();
};

class AdditionInstruction: public ThreeOperandInstruction {
    public:
        AdditionInstruction(InstructionSet* set, GeneralRegister* srcRegister1, GeneralRegister* srcRegister2, GeneralRegister* destRegister):
            ThreeOperandInstruction(set, 4, srcRegister1, srcRegister2, destRegister) {};
        virtual ~AdditionInstruction() {};

    virtual void execute();
};

class SubtractionInstruction: public ThreeOperandInstruction {
    public:
        SubtractionInstruction(InstructionSet* set, GeneralRegister* srcRegister1, GeneralRegister* srcRegister2, GeneralRegister* destRegister):
            ThreeOperandInstruction(set, 5, srcRegister1, srcRegister2, destRegister) {};
        virtual ~SubtractionInstruction() {};

    virtual void execute();
};

class MultiplyInstruction: public ThreeOperandInstruction {
    public:
        MultiplyInstruction(InstructionSet* set, GeneralRegister* srcRegister1, GeneralRegister* srcRegister2, GeneralRegister* destRegister):
            ThreeOperandInstruction(set, 6, srcRegister1, srcRegister2, destRegister) {};
        virtual ~MultiplyInstruction() {};
    
    virtual void execute();
};

class DivideInstruction: public ThreeOperandInstruction {
    public:
        DivideInstruction(InstructionSet* set, GeneralRegister* srcRegister1, GeneralRegister* srcRegister2, GeneralRegister* destRegister):
            ThreeOperandInstruction(set, 7, srcRegister1, srcRegister2, destRegister) {};
        virtual ~DivideInstruction() {};

        virtual void execute();
};

class OrInstruction: public TwoOperandInstruction {
    public:
        OrInstruction(InstructionSet* set, GeneralRegister* srcRegister1, GeneralRegister* srcRegister2):
            TwoOperandInstruction(set, 8, srcRegister1, srcRegister2) {};
        virtual ~OrInstruction() {};
        
        virtual void execute();
};

class NorInstruction: public TwoOperandInstruction {
    public:
        NorInstruction(InstructionSet* set, GeneralRegister* srcRegister1, GeneralRegister* srcRegister2):
            TwoOperandInstruction(set, 9, srcRegister1, srcRegister2) {};
        virtual ~NorInstruction() {};

        virtual void execute();
};

class XorInstruction: public TwoOperandInstruction {
    public:
        XorInstruction(InstructionSet* set, GeneralRegister* srcRegister1, GeneralRegister* srcRegister2):
            TwoOperandInstruction(set, 10, srcRegister1, srcRegister2) {};
        virtual ~XorInstruction() {};

        virtual void execute();
};

class AndInstruction: public TwoOperandInstruction {
    public:
        AndInstruction(InstructionSet* set, GeneralRegister* srcRegister1, GeneralRegister* srcRegister2):
            TwoOperandInstruction(set, 11, srcRegister1, srcRegister2) {};
        virtual ~AndInstruction() {};

        virtual void execute();
};

class NotInstruction: public OneOperandInstruction {
    public:
        NotInstruction(InstructionSet* set, GeneralRegister* srcRegister):
            OneOperandInstruction(set, 12, srcRegister) {};
        virtual ~NotInstruction() {};

        virtual void execute();
};

class IncrementInstruction: public OneOperandInstruction {
    public:
        IncrementInstruction(InstructionSet* set, GeneralRegister* srcRegister):
            OneOperandInstruction(set, 13, srcRegister) {};
        virtual ~IncrementInstruction() {};

        virtual void execute();
};

class DecrementInstruction: public OneOperandInstruction {
    public:
        DecrementInstruction(InstructionSet* set, GeneralRegister* srcRegister):
            OneOperandInstruction(set, 14, srcRegister) {};
        virtual ~DecrementInstruction() {};

    virtual void execute();
};

#endif