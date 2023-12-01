#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <iostream>
#include "ParseTree.h"

namespace ASTConstants {
    enum NodeType {ROOT, INSTRUCTION, OPERAND};
    enum InstructionType {MOVE, LOAD, STORE, CREATE, CAST, ADD, SUB, MULTIPLY, DIVIDE, OR, AND, NOT, SHIFT, COMPARE, JUMP, CALL, PUSH, POP, RETURN, STOP, INPUT, OUTPUT, PRINT, LABEL, COMMENT, NONE};
    enum OperandType {REGISTER, INSTRUCTIONADDRESS, MEMORYADDRESS, INTEGER, FLOAT, BOOLEAN, CHARACTER, STRING, NEWLINE, UNKNOWN};
    enum NumOperands {NULLARY, UNARY, BINARY, TERNARY, INVALID};
};

namespace AST {
    //Broad AST Node
    class ASTNode {
        public:
            //Constructor/destructor
            ASTNode(ASTConstants::NodeType type, std::string value):
                m_nodeType(type),
                m_nodeValue(value) {};
            virtual ~ASTNode() {
                //Delete over all children and remove dangling pointers
                //Recursive deletion
                for (int i=0; i<m_children.size(); i++) {
                    delete m_children[i];
                }
                m_children.clear();
            }
            //Remove copy and assignment operators
            ASTNode(const ASTNode&) = delete;
            ASTNode& operator=(const ASTNode&) = delete;

            //Getters
            const std::string getNodeValue() const {return m_nodeValue;};
            const ASTConstants::NodeType getNodeType() const {return m_nodeType;};
            const int getNumChildren() const {return m_children.size();};
            //Get children method (for recursive operations)
            const std::vector<ASTNode*>& getChildren() const {
                return m_children;
            }

            //Setters
            void setNodeValue(std::string value) {m_nodeValue = value;};


            //Child insertion and manipulation
            ASTNode* insertChild(ASTNode* childNode) {
                //Insert a new child if not nullptr
                if(childNode!=nullptr) {
                    m_children.push_back(childNode);
                    return childNode;
                }
                else {
                    //Return nullptr otherwise
                    return nullptr;
                }
            };
            void deleteLastChild() {
                //Delete child and remove dangling pointer
                delete m_children.back();
                m_children.pop_back();
            }
            ASTNode* childAt(int index) {
                //Return nullptr if out of bounds
                if(index >= m_children.size()) {
                    return nullptr;
                }
                else {
                    return m_children[index];
                }
            }
            
        protected:
            //Protected variables to be inhereted by derived classes
            ASTConstants::NodeType m_nodeType;
            std::string m_nodeValue;
            std::vector<ASTNode*> m_children;
    };

    //Specialized ASTNode Classes
    class RootNode: public ASTNode {
        //Only AST can construdct RootNode
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
            InstructionNode(std::string nodeValue, ASTConstants::InstructionType instructionType, ASTConstants::NumOperands numOperands, int line):
                ASTNode(ASTConstants::NodeType::INSTRUCTION, nodeValue),
                m_instructionType(instructionType),
                m_numOperands(numOperands),
                m_line(line) {};
            virtual ~InstructionNode() {}; 
            InstructionNode(const InstructionNode&) = delete;
            InstructionNode& operator=(const InstructionNode&) = delete; 

            //Getters and setters
            const ASTConstants::InstructionType getInstructionType() const {return m_instructionType;};
            const ASTConstants::NumOperands getNumOperands() const {return m_numOperands;};
            void setInstructionType(ASTConstants::InstructionType type) {m_instructionType = type;};
            void setNumOperands(ASTConstants::NumOperands num) {m_numOperands = num;};
        
        private:
            //Store the instruction type, number of operands and line number
            //AST discards syntactic information so line number is important for error reporting
            ASTConstants::InstructionType m_instructionType;
            ASTConstants::NumOperands m_numOperands;
            int m_line;
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
            //Store the operand type (direct conversion from parse tree constants)
            ASTConstants::OperandType m_operandType;
    };

    class AbstractSyntaxTree {
        public:
            AbstractSyntaxTree() {
                //When creating a new AST, create a new root node and populate the instruction dictionary
                m_root = new RootNode();
                m_instructionDictionary.emplace("move", ASTConstants::InstructionType::MOVE);
                m_instructionDictionary.emplace("load", ASTConstants::InstructionType::LOAD);
                m_instructionDictionary.emplace("store", ASTConstants::InstructionType::STORE);
                m_instructionDictionary.emplace("create", ASTConstants::InstructionType::CREATE);
                m_instructionDictionary.emplace("cast", ASTConstants::InstructionType::CAST);
                m_instructionDictionary.emplace("add", ASTConstants::InstructionType::ADD);
                m_instructionDictionary.emplace("sub", ASTConstants::InstructionType::SUB);
                m_instructionDictionary.emplace("multiply", ASTConstants::InstructionType::MULTIPLY);
                m_instructionDictionary.emplace("divide", ASTConstants::InstructionType::DIVIDE);
                m_instructionDictionary.emplace("OR", ASTConstants::InstructionType::OR);
                m_instructionDictionary.emplace("and", ASTConstants::InstructionType::AND);
                m_instructionDictionary.emplace("not", ASTConstants::InstructionType::NOT);
                m_instructionDictionary.emplace("shift", ASTConstants::InstructionType::SHIFT);
                m_instructionDictionary.emplace("compare", ASTConstants::InstructionType::COMPARE);
                m_instructionDictionary.emplace("jump", ASTConstants::InstructionType::JUMP);
                m_instructionDictionary.emplace("call", ASTConstants::InstructionType::CALL);
                m_instructionDictionary.emplace("push", ASTConstants::InstructionType::PUSH);
                m_instructionDictionary.emplace("pop", ASTConstants::InstructionType::POP);
                m_instructionDictionary.emplace("return", ASTConstants::InstructionType::RETURN);
                m_instructionDictionary.emplace("stop", ASTConstants::InstructionType::STOP);
                m_instructionDictionary.emplace("input", ASTConstants::InstructionType::INPUT);
                m_instructionDictionary.emplace("output", ASTConstants::InstructionType::OUTPUT);
                m_instructionDictionary.emplace("print", ASTConstants::InstructionType::PRINT);
                m_instructionDictionary.emplace("label", ASTConstants::InstructionType::LABEL);
                m_instructionDictionary.emplace("comment", ASTConstants::InstructionType::COMMENT);
            }
            ~AbstractSyntaxTree() {
                //Delete the root, which will recursively delete all nodes
                delete m_root;
            }
            ASTNode* getRoot() {return m_root;};
            ASTConstants::InstructionType getInstructionType(std::string instruction) {
                //Search the dictionary and return the constant type if found, NONE otherwise
                auto itr = m_instructionDictionary.find(instruction);
                if (itr != m_instructionDictionary.end()) {
                    return itr->second;
                }
                else {
                    return ASTConstants::InstructionType::NONE;
                }
            }
            //Switch statement to switch between number of children and numOperands constants
            ASTConstants::NumOperands getNumOperands(int num) {
                switch (num) {
                    case 0:
                        return ASTConstants::NumOperands::NULLARY;
                    case 1:
                        return ASTConstants::NumOperands::UNARY;
                    case 2:
                        return ASTConstants::NumOperands::BINARY;
                    case 3:
                        return ASTConstants::NumOperands::TERNARY;
                    default:
                        return ASTConstants::NumOperands::INVALID;
                }
            }

            //Direct mapping of PTConstants to ASTConstants
            ASTConstants::OperandType convertOperandType(PTConstants::OperandType type) {
                switch (type) {
                    case PTConstants::OperandType::REGISTER:
                        return ASTConstants::OperandType::REGISTER;
                    case PTConstants::OperandType::INSTRUCTIONADDRESS:
                        return ASTConstants::OperandType::INSTRUCTIONADDRESS;
                    case PTConstants::OperandType::MEMORYADDRESS:
                        return ASTConstants::OperandType::MEMORYADDRESS;
                    case PTConstants::OperandType::FLOAT:
                        return ASTConstants::OperandType::FLOAT;
                    case PTConstants::OperandType::INTEGER:
                        return ASTConstants::OperandType::INTEGER;
                    case PTConstants::OperandType::BOOLEAN:
                        return ASTConstants::OperandType::BOOLEAN;
                    case PTConstants::OperandType::CHARACTER:
                        return ASTConstants::OperandType::CHARACTER;
                    case PTConstants::OperandType::STRING:
                        return ASTConstants::OperandType::STRING;
                    case PTConstants::OperandType::NEWLINE:
                        return ASTConstants::OperandType::NEWLINE;
                    case PTConstants::OperandType::UNKNOWN:
                        return ASTConstants::OperandType::UNKNOWN;
                    default:
                        return ASTConstants::OperandType::UNKNOWN;
                }
            }

            //Recursive printTree method
            void printTree() const {
                printNode(m_root, 0);
            }

        private:
            ASTNode* m_root;
            //Dictionary stores in an unordered map for O(1) lookup
            std::unordered_map<std::string, ASTConstants::InstructionType> m_instructionDictionary;

            //Recursive print method
            void printNode(const ASTNode* node, int level) const {
                if (node == nullptr) return;

                std::string indent(level * 4, ' '); // 4 spaces per level of indentation
                std::cout << indent << node->getNodeValue() << " (" << node->getNumChildren() << " children)" << std::endl;

                // Recursively print each child
                for (const auto& child : node->getChildren()) {
                    printNode(child, level + 1);
                }
            }

    };
}
#endif