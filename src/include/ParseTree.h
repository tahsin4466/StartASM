#ifndef PARSETREE_H
#define PARSETREE_H

#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <functional>

namespace PTConstants {
    enum NodeType {ROOT, GENERAL, OPERAND};
    enum GeneralType {INSTRUCTION, CONJUNCTION, BLANK};
    enum OperandType {REGISTER, INSTRUCTIONADDRESS, MEMORYADDRESS, INTEGER, FLOAT, BOOLEAN, CHARACTER, LABEL, STRING, NEWLINE, TYPECONDITION, JUMPCONDITION, SHIFTCONDITION, UNKNOWN};
    enum Constants {
        IMPLICIT_INDEX = -1
    };
};

//Broad PTNode
namespace PT {
    class PTNode { 
        public:
            //Constructor/Destructor
            PTNode(int tokenIndex, std::string nodeValue, PTConstants::NodeType nodeType):
                m_tokenIndex(tokenIndex),
                m_nodeValue(nodeValue),
                m_nodeType(nodeType) {};
            virtual ~PTNode() {
                //Delete all dynamically allocated children from the vector
                for(int i =0; i<m_children.size(); i++) {
                    delete m_children[i];
                }
                //Clear all dangling references
                m_children.clear();
            };
            //Remove copy and assignment operators
            PTNode(const PTNode&) = delete;
            PTNode& operator=(const PTNode&) = delete;

            //Getters
            const std::string getNodeValue() const {return m_nodeValue;};
            const PTConstants::NodeType getNodeType() const {return m_nodeType;};
            const int getIndex() const {return m_tokenIndex;};
            const int getNumChildren() const {return m_children.size();};
            //Get children method (for recursive methods)
            const std::vector<PTNode*>& getChildren() const {
                return m_children;
            }

            //Setters
            void setNodeValue(std::string value) {m_nodeValue = value;};

            //Child insertion and manipulation
            PTNode* insertChild(PTNode* childNode) {
                //Insert child if not nullptr
                if(childNode!=nullptr) {
                    m_children.push_back(childNode);
                    return childNode;
                }
                else {
                    return nullptr;
                }
            };
            void deleteLastChild() {
                //Delete child then remove dangling pointer
                delete m_children.back();
                m_children.pop_back();
            }
            PTNode* childAt(int index) {
                if(index >= m_children.size()) {
                    //Return nullptr if out of bounds
                    return nullptr;
                }
                else {
                    return m_children[index];
                }
            }

        protected:
            //Variables to be inhereted by derived methods
            int m_tokenIndex;
            std::string m_nodeValue;
            PTConstants::NodeType m_nodeType;
            std::vector<PTNode*> m_children;
    };

    //Specialized PTNode Classes
    class RootNode: public PTNode {
        //Friend class allows only PT to construct a Root Node
        friend class PT;

        public:
            //Constructor/Destructor
            RootNode():
                PTNode(PTConstants::Constants::IMPLICIT_INDEX, "", PTConstants::ROOT) {};
            virtual ~RootNode() {}; 
            RootNode(const RootNode&) = delete;
            RootNode& operator=(const RootNode&) = delete; 
    };

    //General node for non-operands
    class GeneralNode: public PTNode {
        public:
            //Constructor/Destructor
            GeneralNode(int tokenIndex, std::string nodeValue, PTConstants::GeneralType generalType):
                PTNode(tokenIndex, nodeValue, PTConstants::GENERAL),
                m_generalType(generalType) {};
            virtual ~GeneralNode() {}; 
            GeneralNode(const GeneralNode&) = delete;
            GeneralNode& operator=(const GeneralNode&) = delete; 

            //Getters and setters
            const PTConstants::GeneralType getGeneralType() const {return m_generalType;};
            void setGeneralType(PTConstants::GeneralType type) {m_generalType = type;};
        
        private:
            //Store the type of general node
            PTConstants::GeneralType m_generalType;
    };

    //Operand node for operands
    class OperandNode: public PTNode {
        public:
            //Constructor/Destructor
            OperandNode(int tokenIndex, std::string nodeValue, PTConstants::OperandType operandType):
                PTNode(tokenIndex, nodeValue, PTConstants::OPERAND),
                m_operandType(operandType) {};
            virtual ~OperandNode() {};  
            OperandNode(const OperandNode&) = delete;
            OperandNode& operator=(const OperandNode&) = delete;

            //Getters and setters
            const PTConstants::OperandType getOperandType() const {return m_operandType;};
            void setOperandType(PTConstants::OperandType type) {m_operandType = type;};
        
        private:
            //Store the type of operand
            PTConstants::OperandType m_operandType;
    };

    class ParseTree {
        public:
            //Create a new root node to initialise tree
            ParseTree() {
                m_root = new RootNode();
            }
            //Delete root, which will recursively delete whole tree
            ~ParseTree() {
                delete m_root;
            }
            PTNode* getRoot() {return m_root;};

            //Print tree function, which calls recursively
            void printTree() const {
                printNode(m_root);
            }

        private:
            PTNode* m_root;

            //Debugging - printNode called recursively
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
}
#endif