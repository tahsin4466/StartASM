#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <iostream>
#include <mutex>
#include "pt/ParseTree.h"

namespace ASTConstants {
    enum NodeType {ROOT, INSTRUCTION, OPERAND};
    enum InstructionType {MOVE, LOAD, STORE, CREATE, CAST, ADD, SUB, MULTIPLY, DIVIDE, OR, AND, NOT, SHIFT, COMPARE, JUMP, CALL, PUSH, POP, RETURN, STOP, INPUT, OUTPUT, PRINT, LABEL, COMMENT, NONE};
    enum OperandType {REGISTER, INSTRUCTIONADDRESS, MEMORYADDRESS, INTEGER, FLOAT, BOOLEAN, CHARACTER, STRING, NEWLINE, TYPECONDITION, SHIFTCONDITION, JUMPCONDITION, UNKNOWN};
    enum NumOperands {NULLARY, UNARY, BINARY, TERNARY, INVALID};
};

namespace AST {
    // Visitor forward declaration
    class Visitor;

    // Broad AST Node
    class ASTNode {
    public:
        //Constructor and destructor
        ASTNode(ASTConstants::NodeType type, std::string value);
        virtual ~ASTNode();
        //Remove copy and assignment
        ASTNode(const ASTNode&) = delete;
        ASTNode& operator=(const ASTNode&) = delete;

        //Visitor accept method
        virtual void accept(Visitor& visitor) = 0;

        //Getters
        const std::string getNodeValue() const { return m_nodeValue; }
        const ASTConstants::NodeType getNodeType() const { return m_nodeType; }
        const int getNumChildren() const { return m_children.size(); }
        const std::vector<ASTNode*>& getChildren() const { return m_children; }

        //Setters
        void setNodeValue(std::string value) { m_nodeValue = value; }
        ASTNode* insertChild(ASTNode* childNode);
        ASTNode* childAt(int index);
        void reserveChildren(int numChildren);

    protected:
        ASTConstants::NodeType m_nodeType;
        std::string m_nodeValue;
        std::vector<ASTNode*> m_children;
        mutable std::mutex m_mutex;
    };

    //Specailized root node class (top level in AST)
    class RootNode: public ASTNode {
        friend class AST;
    public:
        RootNode();
        virtual ~RootNode();
        RootNode(const RootNode&) = delete;
        RootNode& operator=(const RootNode&) = delete;

        void accept(Visitor& visitor) override;
    };

    //Template instruction node class
    class InstructionNode: public ASTNode {
    public:
        InstructionNode(std::string nodeValue, ASTConstants::InstructionType instructionType, ASTConstants::NumOperands numOperands, int line);
        virtual ~InstructionNode();
        InstructionNode(const InstructionNode&) = delete;
        InstructionNode& operator=(const InstructionNode&) = delete;

        const ASTConstants::InstructionType getInstructionType() const;
        const ASTConstants::NumOperands getNumOperands() const;
        void setInstructionType(ASTConstants::InstructionType type);
        void setNumOperands(ASTConstants::NumOperands num);

    private:
        ASTConstants::InstructionType m_instructionType;
        ASTConstants::NumOperands m_numOperands;
        int m_line;
    };

    //Operand Node Class
    class OperandNode: public ASTNode {
    public:
        OperandNode(std::string nodeValue, ASTConstants::OperandType operandType);
        virtual ~OperandNode();
        OperandNode(const OperandNode&) = delete;
        OperandNode& operator=(const OperandNode&) = delete;

        const ASTConstants::OperandType getOperandType() const;
        void setOperandType(ASTConstants::OperandType type);

    private:
        ASTConstants::OperandType m_operandType;
    };

    //AST wrapper class
    class AbstractSyntaxTree {
    public:
        AbstractSyntaxTree();
        ~AbstractSyntaxTree();
        ASTNode* getRoot();
        ASTConstants::InstructionType getInstructionType(std::string instruction);
        ASTConstants::NumOperands getNumOperands(int num);
        ASTConstants::OperandType convertOperandType(PTConstants::OperandType type);
        void printTree() const;

    private:
        ASTNode* m_root;
        std::unordered_map<std::string, ASTConstants::InstructionType> m_instructionDictionary;
        mutable std::mutex m_mutex;
        void printNode(const ASTNode* node, int level) const;
    };
}
#endif
