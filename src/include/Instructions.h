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

class ShiftLeftArithmeticInstruction: public TwoOperandInstruction {
    public:
        ShiftLeftArithmeticInstruction(InstructionSet* set, GeneralRegister* srcRegister, GeneralRegister* destRegister):
            TwoOperandInstruction(set, 15, srcRegister, destRegister) {};
        virtual ~ShiftLeftArithmeticInstruction() {};

    virtual void execute();
};

class ShiftLeftLogicalInstruction: public TwoOperandInstruction {
    public:
        ShiftLeftLogicalInstruction(InstructionSet* set, GeneralRegister* srcRegister, GeneralRegister* destRegister):
            TwoOperandInstruction(set, 16, srcRegister, destRegister) {};
        virtual ~ShiftLeftLogicalInstruction() {};

    virtual void execute();
};

class ShiftRightArithmeticInstruction: public TwoOperandInstruction {
    public:
        ShiftRightArithmeticInstruction(InstructionSet* set, GeneralRegister* srcRegister, GeneralRegister* destRegister):
            TwoOperandInstruction(set, 17, srcRegister, destRegister) {};
        virtual ~ShiftRightArithmeticInstruction() {};

    virtual void execute();
};

class ShiftRightLogicalInstruction: public TwoOperandInstruction {
    public:
        ShiftRightLogicalInstruction(InstructionSet* set, GeneralRegister* srcRegister, GeneralRegister* destRegister):
            TwoOperandInstruction(set, 18, srcRegister, destRegister) {};
        virtual ~ShiftRightLogicalInstruction() {};

    virtual void execute();
};


class JumpGreaterInstruction: public OneOperandInstruction {
    public:
        JumpGreaterInstruction(InstructionSet* set, ProgramMemory* destProgram):
            OneOperandInstruction(set, 19, destProgram) {};
        virtual ~JumpGreaterInstruction() {};
    
    virtual void execute();
};

class JumpLessInstruction: public OneOperandInstruction {
    public:
        JumpLessInstruction(InstructionSet* set, ProgramMemory* destProgram):
            OneOperandInstruction(set, 20, destProgram) {};
        virtual ~JumpLessInstruction() {};
    
    virtual void execute();
};

class JumpEqualInstruction: public OneOperandInstruction {
    public:
        JumpEqualInstruction(InstructionSet* set, ProgramMemory* destProgram):
            OneOperandInstruction(set, 21, destProgram) {};
        virtual ~JumpEqualInstruction() {};
    
    virtual void execute();
};

class JumpNotEqualInstruction: public OneOperandInstruction {
    public:
        JumpNotEqualInstruction(InstructionSet* set, ProgramMemory* destProgram):
            OneOperandInstruction(set, 22, destProgram) {};
        virtual ~JumpNotEqualInstruction() {};
    
    virtual void execute();
};

class JumpZeroInstruction: public OneOperandInstruction {
    public:
        JumpZeroInstruction(InstructionSet* set, ProgramMemory* destProgram):
            OneOperandInstruction(set, 23, destProgram) {};
        virtual ~JumpZeroInstruction() {};
    
    virtual void execute();
};

class JumpNotZeroInstruction: public OneOperandInstruction {
    public:
        JumpNotZeroInstruction(InstructionSet* set, ProgramMemory* destProgram):
            OneOperandInstruction(set, 24, destProgram) {};
        virtual ~JumpNotZeroInstruction() {};
    
    virtual void execute();
};

class StackPushInstruction: public OneOperandInstruction {
    public:
        StackPushInstruction(InstructionSet* set, GeneralRegister* srcRegister):
            OneOperandInstruction(set, 25, srcRegister) {};
        virtual ~StackPushInstruction() {};
    
    virtual void execute();
};

class StackPopInstruction: public OneOperandInstruction {
    public:
        StackPopInstruction(InstructionSet* set, GeneralRegister* srcRegister):
            OneOperandInstruction(set, 26, srcRegister) {};
        virtual ~StackPopInstruction() {};
    
    virtual void execute();
};

class StopInstruction: public NoOperandInstruction {
    public:
        StopInstruction(InstructionSet* set):
            NoOperandInstruction(set, 28) {};
        virtual ~StopInstruction() {};
    
    virtual void execute();
};

#endif