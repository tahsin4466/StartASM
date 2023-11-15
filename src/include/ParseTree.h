#ifndef PARSETREE_H
#define PARSETREE_H

#include <map>
#include <vector>
#include <algorithm>
#include <string>

enum NodeType {ROOT, GENERAL, OPERAND};
enum GeneralType {INSTRUCTION, CONJUNCTION, CONDITION};
enum OperandType {REGISTER, INSTRUCTIONADDRESS, MEMORYADDRESS, INTEGER, FLOAT, BOOLEAN, CHARACTER, LABEL, COMMENT};
int NULLINDEX = -1;


//Broad PTNode
class PTNode { 
    public:
        //Constructor/Destructor
        PTNode(int tokenIndex, std::string nodeValue, NodeType nodeType):
            m_tokenIndex(tokenIndex),
            m_nodeValue(nodeValue),
            m_nodeType(nodeType) {};
        virtual ~PTNode() {
            for(auto itr = m_children.begin(); itr!=m_children.end(); itr++) {
                delete itr->second;
            }
            m_children.clear();
        };
        PTNode(const PTNode&) = delete;
        PTNode& operator=(const PTNode&) = delete;

        //Getters
        const virtual std::string getNodeValue() const {return m_nodeValue;};
        const virtual NodeType getNodeType() const {return m_nodeType;};
        const virtual int getNumChildren() const {return m_children.size();};

        //Child insertion and manipulation
        PTNode* insertChild(int index, PTNode* childNode) {
            if(childNode!=nullptr) {
                m_children[index] = childNode;
                return childNode;
            }
            else {
                return nullptr;
            }
        };
        bool deleteChild(int index) {
            if(m_children.find(index) != m_children.end()) {
                delete m_children[index];
                m_children.erase(index);
                return true;
            }
            else {
                return false;
            }
        }
        PTNode* childAt(int index) {
            if(m_children.find(index) != m_children.end()) {
                return m_children[index];
            }
            else {
                return nullptr;
            }
        }

    protected:
        int m_tokenIndex;
        std::string m_nodeValue;
        NodeType m_nodeType;
        std::map<int, PTNode*> m_children;
};

//Specialized PTNode Classes
class RootNode: public PTNode {
    friend class PT;

    public:
        //Constructor/Destructor
        RootNode():
            PTNode(NULLINDEX, "", ROOT) {};
        virtual ~RootNode() {}; 
        RootNode(const RootNode&) = delete;
        RootNode& operator=(const RootNode&) = delete; 
};

class GeneralNode: public PTNode {
    public:
        //Constructor/Destructor
        GeneralNode(int tokenIndex, std::string nodeValue, GeneralType generalType):
            PTNode(tokenIndex, nodeValue, GENERAL),
            m_generalType(generalType) {};
        virtual ~GeneralNode() {}; 
        GeneralNode(const GeneralNode&) = delete;
        GeneralNode& operator=(const GeneralNode&) = delete; 

        //Getter
        const GeneralType getGeneralType() const {return m_generalType;};
    
    private:
        GeneralType m_generalType;
};

class OperandNode: public PTNode {
    public:
        //Constructor/Destructor
        OperandNode(int tokenIndex, std::string nodeValue, OperandType operandType):
            PTNode(tokenIndex, nodeValue, OPERAND),
            m_operandType(operandType) {};
        virtual ~OperandNode() {};  
        OperandNode(const OperandNode&) = delete;
        OperandNode& operator=(const OperandNode&) = delete;

        //Getter
        const OperandType getOperandType() const {return m_operandType;};
    
    private:
        OperandType m_operandType;
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
        
    private:
        PTNode* m_root;
};

#endif