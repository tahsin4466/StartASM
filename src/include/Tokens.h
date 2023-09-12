#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>

class InstructionSet;

class Token {
    public:
        //Constructor and destructor
        Token(InstructionSet* set, std::string name):
            m_set(set),
            m_name(name) {};
        virtual ~Token() {};

        //Getters
        std::string name() {return m_name;}

        //Classification functions
        virtual const std::string tokenType() = 0;

    private:
        //Name
        std::string m_name;
        //Pointer to residing instruction set
        InstructionSet* m_set;
};

class Operation: public Token {
    public:
        //Constructor and destructor
        Operation(InstructionSet* set, std::string name):
            Token(set, name) {};
        virtual ~Operation() {};

        //Classification functions
        virtual const std::string tokenType() {return "operation";}
        virtual const int numOperands() = 0;
    
    private:
};

class Operand: public Token {
    public:
        Operand(InstructionSet* set, std::string name):
            Token(set, name) {};
        virtual ~Operand() {};

        //Classification functions
        virtual const std::string tokenType() {return "operand";}
        virtual const std::string operandSource() = 0;

    private:

};

class Phrase: public Token {
    public:
        Phrase(InstructionSet* set, std::string name):
            Token(set, name) {};
        virtual ~Phrase() {};

        //Classification functions
        virtual const std::string tokenType() {return "phrase";}
};

class ZeroOperation: public Operation {
    public:
        ZeroOperation(InstructionSet* set, std::string name):
            Operation(set, name) {};
        virtual ~ZeroOperation() {};

        virtual void execute() = 0;
        virtual const int numOperands() {return 0;}

    private:
};

class OneOperation: public Operation {
    public:
        OneOperation(InstructionSet* set, std::string name):
            Operation(set, name) {};
        virtual ~OneOperation() {};

        virtual void execute() = 0;
        virtual const int numOperands() {return 1;}

    private:
};


#endif