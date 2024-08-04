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

namespace PT {
    class PTNode {
    public:
        PTNode(int tokenIndex, std::string nodeValue, PTConstants::NodeType nodeType);
        virtual ~PTNode();
        PTNode(const PTNode&) = delete;
        PTNode& operator=(const PTNode&) = delete;

        const std::string getNodeValue() const { return m_nodeValue; }
        const PTConstants::NodeType getNodeType() const { return m_nodeType; }
        const int getIndex() const { return m_tokenIndex; }
        const int getNumChildren() const { return m_children.size(); }
        const std::vector<PTNode*>& getChildren() const { return m_children; }

        void setNodeValue(std::string value) { m_nodeValue = value; }

        PTNode* insertChild(PTNode* childNode);
        void deleteLastChild();
        PTNode* childAt(int index);
        void reserveChildren(int numChildren) { m_children.reserve(numChildren); }

    protected:
        int m_tokenIndex;
        std::string m_nodeValue;
        PTConstants::NodeType m_nodeType;
        std::vector<PTNode*> m_children;
    };

    class RootNode: public PTNode {
        friend class PT;

    public:
        RootNode();
        virtual ~RootNode() = default;
        RootNode(const RootNode&) = delete;
        RootNode& operator=(const RootNode&) = delete;
    };

    class GeneralNode: public PTNode {
    public:
        GeneralNode(int tokenIndex, std::string nodeValue, PTConstants::GeneralType generalType);
        virtual ~GeneralNode() = default;
        GeneralNode(const GeneralNode&) = delete;
        GeneralNode& operator=(const GeneralNode&) = delete;

        const PTConstants::GeneralType getGeneralType() const { return m_generalType; }
        void setGeneralType(PTConstants::GeneralType type) { m_generalType = type; }

    private:
        PTConstants::GeneralType m_generalType;
    };

    class OperandNode: public PTNode {
    public:
        OperandNode(int tokenIndex, std::string nodeValue, PTConstants::OperandType operandType);
        virtual ~OperandNode() = default;
        OperandNode(const OperandNode&) = delete;
        OperandNode& operator=(const OperandNode&) = delete;

        const PTConstants::OperandType getOperandType() const { return m_operandType; }
        void setOperandType(PTConstants::OperandType type) { m_operandType = type; }

    private:
        PTConstants::OperandType m_operandType;
    };

    class ParseTree {
    public:
        ParseTree();
        ~ParseTree();
        PTNode* getRoot() { return m_root; }
        void printTree() const;

    private:
        PTNode* m_root;
        void printNode(const PTNode* node, int level = 0) const;
    };
}
#endif
