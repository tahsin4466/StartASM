#ifndef PARSETREE_H
#define PARSETREE_H

#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <functional>

namespace PTConstants {
    enum NodeType {ROOT, GENERAL, OPERAND};
    enum GeneralType {INSTRUCTION, CONJUNCTION, CONDITION, BLANK};
    enum OperandType {REGISTER, INSTRUCTIONADDRESS, MEMORYADDRESS, INTEGER, FLOAT, BOOLEAN, CHARACTER, LABEL, COMMENT, UNKNOWN};
    enum Constants {
        IMPLICIT_INDEX = -1
    };
};

//Broad PTNode
class PTNode { 
    public:
        //Constructor/Destructor
        PTNode(int tokenIndex, std::string nodeValue, PTConstants::NodeType nodeType):
            m_tokenIndex(tokenIndex),
            m_nodeValue(nodeValue),
            m_nodeType(nodeType) {};
        virtual ~PTNode() {
            for(int i =0; i<m_children.size(); i++) {
                delete m_children[i];
            }
            m_children.clear();
        };
        PTNode(const PTNode&) = delete;
        PTNode& operator=(const PTNode&) = delete;

        //Getters
        const virtual std::string getNodeValue() const {return m_nodeValue;};
        const virtual PTConstants::NodeType getNodeType() const {return m_nodeType;};
        const virtual int getIndex() const {return m_tokenIndex;};
        const virtual int getNumChildren() const {return m_children.size();};
        const std::vector<PTNode*>& getChildren() const {
            return m_children;
        }

        //Child insertion and manipulation
        PTNode* insertChild(PTNode* childNode) {
            if(childNode!=nullptr) {
                m_children.push_back(childNode);
                return childNode;
            }
            else {
                return nullptr;
            }
        };
        void deleteLastChild() {
            m_children.pop_back();
        }
        PTNode* childAt(int index) {
            if(index >= m_children.size()) {
                return nullptr;
            }
            else {
                return m_children[index];
            }
        }

    protected:
        int m_tokenIndex;
        std::string m_nodeValue;
        PTConstants::NodeType m_nodeType;
        std::vector<PTNode*> m_children;
};

//Specialized PTNode Classes
class RootNode: public PTNode {
    friend class PT;

    public:
        //Constructor/Destructor
        RootNode():
            PTNode(PTConstants::Constants::IMPLICIT_INDEX, "", PTConstants::ROOT) {};
        virtual ~RootNode() {}; 
        RootNode(const RootNode&) = delete;
        RootNode& operator=(const RootNode&) = delete; 
};

class GeneralNode: public PTNode {
    public:
        //Constructor/Destructor
        GeneralNode(int tokenIndex, std::string nodeValue, PTConstants::GeneralType generalType):
            PTNode(tokenIndex, nodeValue, PTConstants::GENERAL),
            m_generalType(generalType) {};
        virtual ~GeneralNode() {}; 
        GeneralNode(const GeneralNode&) = delete;
        GeneralNode& operator=(const GeneralNode&) = delete; 

        //Getter
        const PTConstants::GeneralType getGeneralType() const {return m_generalType;};
    
    private:
        PTConstants::GeneralType m_generalType;
};

class OperandNode: public PTNode {
    public:
        //Constructor/Destructor
        OperandNode(int tokenIndex, std::string nodeValue, PTConstants::OperandType operandType):
            PTNode(tokenIndex, nodeValue, PTConstants::OPERAND),
            m_operandType(operandType) {};
        virtual ~OperandNode() {};  
        OperandNode(const OperandNode&) = delete;
        OperandNode& operator=(const OperandNode&) = delete;

        //Getter
        const PTConstants::OperandType getOperandType() const {return m_operandType;};
    
    private:
        PTConstants::OperandType m_operandType;
};

class PT {
    public:
        PT() {
            m_root = new RootNode();
        }
        ~PT() {
            delete m_root;
        }
        PTNode* getRoot() {return m_root;};

        void printTree() const {
            printNode(m_root);
        }

    private:
        PTNode* m_root;

        void printNode(const PTNode* node, int level = 0) const {
            if (node == nullptr) return;

            std::string indent(level * 4, ' '); // Increase indentation with each level
            std::cout << indent << node->getNodeValue() << "(" << node->getIndex() << ")" << std::endl;

            // Iterate over each child in the vector and recursively print
            for (const auto& child : node->getChildren()) {
                if (child != nullptr) {
                    printNode(child, level + 1);
                }
            }
        }
};

#endif