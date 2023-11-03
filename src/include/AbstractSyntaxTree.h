#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H

#include <vector>
#include <algorithm>
#include <string>

enum SemanticType {NONE, FROM, TO, SELF, FROMTO, SELFWITH, FROMWITHTO};
enum OperandType {REGISTER, MEMORY, VALUE};
enum NumOperands {NULLARY, UNARY, BINARY, TERNARY};

//Broad ASTNode
class ASTNode {
    public:
        virtual ~ASTNode() = default;

        virtual std::string getNodeValue() {return m_nodeValue;};

    protected:
        ASTNode(std::string nodeValue):
            m_nodeValue(nodeValue) {};
        std::string m_nodeValue;

};



//Level 2 instruction and operand nodes for the AST
class InstructionNode: public ASTNode {
    public:
        InstructionNode(std::string operation, NumOperands numOperands, SemanticType semantic):
            ASTNode(operation),
            m_numOperands(numOperands),
            m_semanticStructure(semantic) {};
        virtual ~InstructionNode() = default;

        virtual NumOperands getNumOperands() {return m_numOperands;};
        virtual SemanticType getSemanticType() {return m_semanticStructure;};

    protected:
        NumOperands m_numOperands;
        SemanticType m_semanticStructure;
};

class OperandNode: public ASTNode {
    public:
        OperandNode(std::string operand, OperandType type):
            ASTNode(operand),
            m_operandType(type) {};
        virtual ~OperandNode() = default;

        virtual OperandType getOperandType() {return m_operandType;};
    
    private: 
        OperandType m_operandType;
};



//Level 3 specific instruction nodes (nullary, unary, binary, ternary)
class NullaryNode: public InstructionNode {
    public:
        NullaryNode(std::string operation):
            InstructionNode(operation, NULLARY, NONE) {};
        virtual ~NullaryNode() = default;
};

class UnaryNode: public InstructionNode {
    public:
        UnaryNode(std::string operation, std::string operand1Value, OperandType operand1Type, SemanticType semantic):
            InstructionNode(operation, UNARY, semantic),
            m_operand1(new OperandNode(operand1Value, operand1Type)) {};
        ~UnaryNode() override {
            delete m_operand1;
        }

    private:
        OperandNode* m_operand1; 
};

class BinaryNode: public InstructionNode {
    public:
        BinaryNode(std::string operation, std::string operand1Value, OperandType operand1Type, std::string operand2Value, OperandType operand2Type, SemanticType semantic):
            InstructionNode(operation, BINARY, semantic),
            m_operand1(new OperandNode(operand1Value, operand1Type)),
            m_operand2(new OperandNode(operand2Value, operand2Type)) {};
        ~BinaryNode() override {
            delete m_operand1;
            delete m_operand2;
        }

    private:
        OperandNode* m_operand1; 
        OperandNode* m_operand2;
};

class TernaryNode: public InstructionNode {
    public:
        TernaryNode(std::string operation, std::string operand1Value, OperandType operand1Type, std::string operand2Value, OperandType operand2Type, std::string operand3Value, OperandType operand3Type, SemanticType semantic):
            InstructionNode(operation, TERNARY, semantic),
            m_operand1(new OperandNode(operand1Value, operand1Type)),
            m_operand2(new OperandNode(operand2Value, operand2Type)),
            m_operand3(new OperandNode(operand3Value, operand3Type)) {};
        ~TernaryNode() override {
            delete m_operand1;
            delete m_operand2;
            delete m_operand3;
        }

    private:
        OperandNode* m_operand1; 
        OperandNode* m_operand2;
        OperandNode* m_operand3;
};


#endif