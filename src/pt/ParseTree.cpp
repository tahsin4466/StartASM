#include "pt/ParseTree.h"

namespace PT {
    // PTNode Implementation
    PTNode::PTNode(int tokenIndex, std::string nodeValue, PTConstants::NodeType nodeType)
            : m_tokenIndex(tokenIndex), m_nodeValue(nodeValue), m_nodeType(nodeType) {}

    PTNode::~PTNode() {
        for (int i = 0; i < m_children.size(); i++) {
            delete m_children[i];
        }
        m_children.clear();
    }

    PTNode* PTNode::insertChild(PTNode* childNode) {
        if (childNode != nullptr) {
            m_children.push_back(childNode);
            return childNode;
        } else {
            return nullptr;
        }
    }

    void PTNode::deleteLastChild() {
        delete m_children.back();
        m_children.pop_back();
    }

    PTNode* PTNode::childAt(int index) {
        if (index >= m_children.size()) {
            return nullptr;
        } else {
            return m_children[index];
        }
    }

    // RootNode Implementation
    RootNode::RootNode() : PTNode(PTConstants::Constants::IMPLICIT_INDEX, "", PTConstants::ROOT) {}

    // GeneralNode Implementation
    GeneralNode::GeneralNode(int tokenIndex, std::string nodeValue, PTConstants::GeneralType generalType)
            : PTNode(tokenIndex, nodeValue, PTConstants::GENERAL), m_generalType(generalType) {}

    // OperandNode Implementation
    OperandNode::OperandNode(int tokenIndex, std::string nodeValue, PTConstants::OperandType operandType)
            : PTNode(tokenIndex, nodeValue, PTConstants::OPERAND), m_operandType(operandType) {}

    // ParseTree Implementation
    ParseTree::ParseTree() {
        m_root = new RootNode();
    }

    ParseTree::~ParseTree() {
        delete m_root;
    }

    void ParseTree::printTree() const {
        printNode(m_root);
    }

    void ParseTree::printNode(const PTNode* node, int level) const {
        if (node == nullptr) return;

        std::string indent(level * 4, ' ');
        std::cout << indent << node->getNodeValue() << "(" << node->getIndex() << ")" << std::endl;

        for (const auto& child : node->getChildren()) {
            if (child != nullptr) {
                printNode(child, level + 1);
            }
        }
    }
}
