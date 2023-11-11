#ifndef OPERANDS_H
#define OPERANDS_H

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <map>

class Instruction;

//HIGH LEVEL OPERAND CLASS
//Defines the fundamental characteristics of an operand. Basic structure as mem, reg and imm varry considerably
//Includes a read and write function for data access/manipulation
//Operand is an ABC - read() and write() are pure virtual functions
class Operand {
    public:
        //Constructor and Destructor
        Operand(int ID):
            m_operandID(ID) {};
        virtual ~Operand() {};

        //Getters
        int getID() {return m_operandID;}

        //Read and Write Pure virtual functions
        virtual void write(std::vector<uint8_t> byteArray) = 0;
        virtual std::vector<uint8_t> read() = 0;
        
    private:
        int m_operandID;
};





//MID LEVEL REGISTER CLASS
//Defines the characteristics of a register, identified with operand ID 0
//Registers store one 4-byte array at any time
//Common defined read() method that returns private variable containing register data
//write() is pure virtual as data needs to be validated depending on type of register e.g. PC register needs to verify
//its a valid program address
//Register is an ABC - write() is pure virtual
class Register: public Operand {
    public:
        //Constructor and Destructor
        Register(int RegisterID):
            Operand(0),
            m_registerID(RegisterID) {};
        virtual ~Register() {};
    
        //Getters
        int getRegisterID() {return m_registerID;}

        //Read and Write
        virtual void write(std::vector<uint8_t> byteArray) = 0;
        std::vector<uint8_t> read() {return m_registerValue;}

    protected:
        std::vector<uint8_t> m_registerValue; 
    
    private:
        int m_registerID;

};



//MID LEVEL MEMORY CLASS
//Defines characteristics for memory, identified with operand ID 1
//Memory structures store arrays/collections, contains a validAddress checker as only pure virtual function
//Memory doesn't require a common interface for writing as the child classes will be directly addressed rather than the parent
//Memory is an ABC - validAddress() is pure virtual
class Memory: public Operand {
    public:
        //Constructor and Destructor
        Memory(int MemoryID):
            Operand(1),
            m_memoryID(MemoryID) {};
        virtual ~Memory() {};

        //Getters
        int getMemoryID() {return m_memoryID;}
        
        //Checkers
        virtual bool validAddress(std::vector<uint8_t>) = 0;

    private:
        int m_memoryID;
};






//LOW LEVEL GPREGISTER CLASS
//Definition for general purpose register that can store any multi-width byte array up to 4 bytes (32 bit)
//Contains seperate GPRegister identifier as there can be multiple GPregisters (e.g. r1, r2 etc.)
//Not an ABC - fully defined
class GeneralRegister: public Register {
    public:
        //Constructor and Destructor
        GeneralRegister(int RegisterNum):
            Register(0),
            m_registerNum(RegisterNum) {};
        virtual ~GeneralRegister() {};

        //Getter
        int getRegisterNum() {return m_registerNum;}

        //Write and clear
        virtual void write(std::vector<uint8_t> byteArray) {
            if (byteArray.size() <= 4) {
                m_registerValue.clear();
                m_registerValue = byteArray;
            }
        }
        virtual void clear(){m_registerValue.clear();}

    private:
        int m_registerNum;

};

//LOW LEVEL POINTER REGISTER CLASS
//Definition for Pointer Register classes, most notably the stack and program pointer
//Insertion checks both valid size and presence in memory structure (stack or program memory)
//Not an ABC - fully defined
class PointerRegister: public Register {
    public:
        //Constructor and Destructor
        PointerRegister(Memory* Memory):
            Register(1),
            m_memoryStructure(Memory) {};
        virtual ~PointerRegister();
        
        virtual void write(std::vector<uint8_t> byteArray) {
            if (byteArray.size() <= 4 && m_memoryStructure->validAddress(byteArray)) {
                m_registerValue.clear();
                m_registerValue = byteArray;
            }
        } 
    private:
        Memory* m_memoryStructure;
};



//LOW LEVEL HEAP MEMORY CLASS
//Definition for Heap, where random access/creation is required
//Non contiguous, managed as an unordered_map mapping bytecode to data (hash table - O(1))
//Contains overloaded write() function for both specified and generated memory address
//Allocated dynamically during "execution"
//Not an ABC - Fully defined
class HeapMemory: public Memory {
    //Implement an unordered_map linking a "memory address" to a 4 byte vector
    public:
        //Constructor and Destructor
        HeapMemory():
            Memory(0) {};
        virtual ~HeapMemory() {};

        //Read, write and remove data
        std::vector<uint8_t> read(std::vector<uint8_t> memoryAddress);
        void write(std::vector<uint8_t> byteValue); //Write in random address
        void write(std::vector<uint8_t> byteValue, std::vector<uint8_t> memoryAddress); //Write in pre-determined address
        void remove(std::vector<uint8_t> memoryAddress);

        virtual bool validAddress(std::vector<uint8_t>);

    private:
        std::unordered_map<int, std::vector<uint8_t>> m_heapMemory; 
};

//LOW LEVEL PROGRAM MEMORY CLASS
//Definition for program memory, random and sequential reading required
//Contiguous and ordered, stored as a map mapping bytecode to Instruction Object (binary tree - Ologn)
//Read only, program memory is fully populated in constructor (defined outside)
//Always denoted by Program Counter register
//Not an ABC - fully defined
class ProgramMemory: public Memory {
    //Implement an ordered_map linking an instruction ID/identifier/code/whatever to a 4 byte vector
    public:
        //Constructor and Destructor
        ProgramMemory(std::vector<Instruction*>);
        virtual ~ProgramMemory() {
            std::map<int, Instruction*>::iterator itr;
            for (itr=m_programMemory.begin(); itr!=m_programMemory.end(); itr++) {
                delete itr->second;
            }
        }

        //Read instruction
        Instruction* read(std::vector<uint8_t> memoryAddress);

        virtual bool validAddress(std::vector<uint8_t>);

    private:
        std::map<int, Instruction*> m_programMemory;
};

//LOW LEVEL STACK MEMORY CLASS
//Definition for stack memory, mostly sequential access requried
//Contiguous and ordered, stored as a map mapping bytecode to Instruction Object (binary tree - Ologn)
//Read and write, and writing always requires an address
//Awlays denoted by the Stack Pointer register
//Not an ABC - fully defined
class StackMemory: public Memory {
    //Implement a linked list containing 4 byte vectors
    public:
        //Constructor and Destructor
        StackMemory():
            Memory(2) {};
        virtual ~StackMemory() {};

        //Read, write, remove
        std::vector<uint8_t> read(std::vector<uint8_t> memoryAddress);
        void write(std::vector<uint8_t> byteValue, std::vector<uint8_t> memoryAddress);
        void remove(std::vector<uint8_t> memoryAddress);

        virtual bool validAddress(std::vector<uint8_t>);

    private:
        std::map<std::vector<uint8_t>, std::vector<uint8_t>> m_stackMemory;
    
};

#endif