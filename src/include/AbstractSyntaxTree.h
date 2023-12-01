#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H

#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

namespace ASTConstants {
    enum NodeType {ROOT, INSTRUCTION, OPERAND};
    enum InstructionType {MOVE, LOAD, STORE, CREATE, CAST, ADD, SUB, MULTIPLY, DIVIDE, OR, AND, NOT, SHIFT, COMPARE, JUMP, CALL, PUSH, POP, RETURN, STOP, INPUT, OUTPUT, PRINT, LABEL, COMMENT};
    enum OperandType {REGISTER, INSTRUCTIONADDRESS, MEMORYADDRESS, INTEGER, FLOAT, BOOLEAN, CHARACTER, STRING, NEWLINE, UNKNOWN};
    enum NumOperands {NULLARY, UNARY, BINARY, TERNARY, INVALID};
};

namespace AST {
    class ASTNode {
        public:
            //Constructor/destructor
            ASTNode(ASTConstants::NodeType type, std::string value):
                m_nodeType(type),
                m_nodeValue(value) {};
            virtual ~ASTNode() {
                for (int i=0; i<m_children.size(); i++) {
                    delete m_children[i];
                }
                m_children.clear();
            }
            ASTNode(const ASTNode&) = delete;
            ASTNode& operator=(const ASTNode&) = delete;

            //Getters
            const std::string getNodeValue() const {return m_nodeValue;};
            const ASTConstants::NodeType getNodeType() const {return m_nodeType;};
            const int getNumChildren() const {return m_children.size();};
            const std::vector<ASTNode*>& getChildren() const {
                return m_children;
            }

            //Setters
            void setNodeValue(std::string value) {m_nodeValue = value;};


            //Child insertion and manipulation
            ASTNode* insertChild(ASTNode* childNode) {
                if(childNode!=nullptr) {
                    m_children.push_back(childNode);
                    return childNode;
                }
                else {
                    return nullptr;
                }
            };
            void deleteLastChild() {
                delete m_children.back();
                m_children.pop_back();
            }
            ASTNode* childAt(int index) {
                if(index >= m_children.size()) {
                    return nullptr;
                }
                else {
                    return m_children[index];
                }
            }
            
        protected:
            ASTConstants::NodeType m_nodeType;
            std::string m_nodeValue;
            std::vector<ASTNode*> m_children;
    };

    //Specialized ASTNode Classes
    class RootNode: public ASTNode {
        friend class AST;

        public:
            //Constructor/Destructor
            RootNode():
                ASTNode(ASTConstants::NodeType::ROOT, "") {};
            virtual ~RootNode() {}; 
            RootNode(const RootNode&) = delete;
            RootNode& operator=(const RootNode&) = delete; 
    };

    //Instruction Node Class
    class InstructionNode: public ASTNode {
        public:
            //Constructor/Destructor
            InstructionNode(std::string nodeValue, ASTConstants::InstructionType instructionType, ASTConstants::NumOperands numOperands):
                ASTNode(ASTConstants::NodeType::INSTRUCTION, nodeValue),
                m_instructionType(instructionType),
                m_numOperands(numOperands) {};
            virtual ~InstructionNode() {}; 
            InstructionNode(const InstructionNode&) = delete;
            InstructionNode& operator=(const InstructionNode&) = delete; 

            //Getters and setters
            const ASTConstants::InstructionType getInstructionType() const {return m_instructionType;};
            const ASTConstants::NumOperands getNumOperands() const {return m_numOperands;};
            void setInstructionType(ASTConstants::InstructionType type) {m_instructionType = type;};
            void setNumOperands(ASTConstants::NumOperands num) {m_numOperands = num;};
        
        private:
            ASTConstants::InstructionType m_instructionType;
            ASTConstants::NumOperands m_numOperands;
    };

    //Instruction Node Class
    class OperandNode: public ASTNode {
        public:
            //Constructor/Destructor
            OperandNode(std::string nodeValue, ASTConstants::OperandType operandType):
                ASTNode(ASTConstants::NodeType::OPERAND, nodeValue),
                m_operandType(operandType) {};
            virtual ~OperandNode() {}; 
            OperandNode(const OperandNode&) = delete;
            OperandNode& operator=(const OperandNode&) = delete; 

            //Getters and setters
            const ASTConstants::OperandType getOperandType() const {return m_operandType;};
            void setOperandType(ASTConstants::OperandType type) {m_operandType = type;};
        
        private:
            ASTConstants::OperandType m_operandType;
    };

    class AbstractSyntaxTree {
        public:
            AbstractSyntaxTree() {
                m_root = new RootNode();
            }
            ~AbstractSyntaxTree() {
                delete m_root;
            }
            ASTNode* getRoot() {return m_root;};

        private:
            ASTNode* m_root;

    };
}
#endif