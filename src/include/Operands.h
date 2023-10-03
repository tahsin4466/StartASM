#ifndef REGISTERS_H
#define REGISTERS_H

#include <vector>
#include <string>
#include <iostream>

class Operand {
    public:
        //Constructor and Destructor
        Operand(int ID):
            m_OperandID(ID) {};
        virtual ~Operand() {};

        //Getters
        int getID() {return m_OperandID;}

        //Read and Write Pure virtual functions
        virtual void write(std::vector<uint8_t> byteArray) = 0;
        virtual std::vector<uint8_t> read() = 0;
        
    private:
        int m_OperandID;
};

class Register: public Operand {
    public:
        //Constructor and Destructor
        Register(int RegisterID):
            Operand(0),
            m_RegisterID(RegisterID) {};
        virtual ~Register() {};
    
        //Getters
        int getRegisterID() {return m_RegisterID;}

        //Read and Write
        virtual void write(std::vector<uint8_t> byteArray) = 0;
        virtual std::vector<uint8_t> read() {return m_registerValue;}

    protected:
        std::vector<uint8_t> m_registerValue; 
    
    private:
        int m_RegisterID;

};

class Memory: public Operand {
    public:
    private:
};

class GPRegister: public Register {
    public:
        //Constructor and Destructor
        GPRegister(int RegisterID, int RegisterNum):
            Register(0),
            m_RegisterNum(RegisterNum) {};
        virtual ~GPRegister() {};

        //Getter
        int getRegisterNum() {return m_RegisterNum;}

        virtual void write(std::vector<uint8_t> byteArray) {
            if (byteArray.size() <= 4) {
                m_registerValue.clear();
                m_registerValue = byteArray;
            }
        }
    private:
        int m_RegisterNum;

};

#endif