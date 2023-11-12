#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H

#include <map>
#include <algorithm>
#include <string>

enum SemanticType {NONE, FROM, TO, SELF, FROMTO, SELFWITH, SELFBY, FROMWITHTO};
enum OperandType {REGISTER, INSTRUCTION, MEMORY, VALUE};
enum NumOperands {NULLARY, UNARY, BINARY, TERNARY};

//Broad ASTNode
class ASTNode {
    public:
        virtual std::string getNodeValue() {return m_nodeValue;};

    protected:
        ASTNode(std::string nodeValue):
            m_nodeValue(nodeValue) {};
        virtual ~ASTNode() {};

        std::string m_nodeValue;
};



//Level 2 instruction and operand nodes for the AST
class InstructionNode: public ASTNode {
    public:
        virtual NumOperands getNumOperands() {return m_numOperands;};
        virtual SemanticType getSemanticType() {return m_semanticStructure;};

    protected:
        InstructionNode(std::string operation, NumOperands numOperands, SemanticType semantic):
            ASTNode(operation),
            m_numOperands(numOperands),
            m_semanticStructure(semantic) {};
        
        NumOperands m_numOperands;
        SemanticType m_semanticStructure;
};

class OperandNode: public ASTNode {
    friend class UnaryNode;
    friend class BinaryNode;
    friend class TernaryNode;

    public:
        virtual OperandType getOperandType() {return m_operandType;};

    protected:
        OperandNode(std::string operand, OperandType type):
            ASTNode(operand),
            m_operandType(type) {};
    
    private: 
        OperandType m_operandType;
};



//Level 3 specific instruction nodes (nullary, unary, binary, ternary)
class NullaryNode: public InstructionNode {
    friend class AST;

    protected:
        NullaryNode(std::string operation):
            InstructionNode(operation, NULLARY, NONE) {};
};

class UnaryNode: public InstructionNode {
    friend class AST;

    public:
        OperandNode* getOperandNode1() {return m_operand1;};

    protected:
        UnaryNode(std::string operation, std::string operand1Value, OperandType operand1Type, SemanticType semantic):
            InstructionNode(operation, UNARY, semantic),
            m_operand1(new OperandNode(operand1Value, operand1Type)) {};
        virtual ~UnaryNode() {
            delete m_operand1;
        };

    private:
        OperandNode* m_operand1; 
};

class BinaryNode: public InstructionNode {
    friend class AST;

    public:
        OperandNode* getOperandNode1() {return m_operand1;};
        OperandNode* getOperandNode2() {return m_operand2;};

    protected:
        BinaryNode(std::string operation, std::string operand1Value, OperandType operand1Type, std::string operand2Value, OperandType operand2Type, SemanticType semantic):
            InstructionNode(operation, BINARY, semantic),
            m_operand1(new OperandNode(operand1Value, operand1Type)),
            m_operand2(new OperandNode(operand2Value, operand2Type)) {};
        virtual ~BinaryNode() {
            delete m_operand1;
            delete m_operand2;
        };
        
    private:
        OperandNode* m_operand1; 
        OperandNode* m_operand2;
};

class TernaryNode: public InstructionNode {
    friend class AST;

    public:
        OperandNode* getOperandNode1() {return m_operand1;};
        OperandNode* getOperandNode2() {return m_operand2;};
        OperandNode* getOperandNode3() {return m_operand3;};

    protected:
        TernaryNode(std::string operation, std::string operand1Value, OperandType operand1Type, std::string operand2Value, OperandType operand2Type, std::string operand3Value, OperandType operand3Type, SemanticType semantic):
            InstructionNode(operation, TERNARY, semantic),
            m_operand1(new OperandNode(operand1Value, operand1Type)),
            m_operand2(new OperandNode(operand2Value, operand2Type)),
            m_operand3(new OperandNode(operand3Value, operand3Type)) {};
        virtual ~TernaryNode() {
            delete m_operand1;
            delete m_operand2;
            delete m_operand3;
        };

    private:
        OperandNode* m_operand1; 
        OperandNode* m_operand2;
        OperandNode* m_operand3;
};

//AST class to handle AST nodes
class AST {
    public:
        AST() {};
        ~AST() {
            for(int i=0; i<instructionNodes.size(); i++) {
                delete instructionNodes[i];
            }
        };
        AST(const AST&) = delete;
        AST& operator=(const AST&) = delete;

        int numInstructionNodes() {return instructionNodes.size();};

        void createInstructionNode(int line, std::string operation) {
            instructionNodes[line] = new NullaryNode(operation);
        };
        void createInstructionNode(int line, std::string operation, std::string operand1Value, OperandType operand1Type, SemanticType semantic) {
            instructionNodes[line] = new UnaryNode(operation, operand1Value, operand1Type, semantic);
        };
        void createInstructionNode(int line, std::string operation, std::string operand1Value, OperandType operand1Type, std::string operand2Value, OperandType operand2Type, SemanticType semantic) {
            instructionNodes[line] = new BinaryNode(operation, operand1Value, operand1Type, operand2Value, operand2Type, semantic);
        };
        void createInstructionNode(int line, std::string operation, std::string operand1Value, OperandType operand1Type, std::string operand2Value, OperandType operand2Type, std::string operand3Value, OperandType operand3Type, SemanticType semantic) {
            instructionNodes[line] = new TernaryNode(operation, operand1Value, operand1Type, operand2Value, operand2Type, operand3Value, operand3Type, semantic);
        };

        std::map<int, InstructionNode*> instructionNodes;
};

#endif