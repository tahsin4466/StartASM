#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H

#include <vector>
#include <algorithm>
#include <string>


class AST {
    public:
        //Construct AST with root node and iterator pointing to root
        AST(): m_root(new ASTNode()), m_iterator(m_root) {};
        //Delete tree by calling freeTree on root node
        ~AST() {
            freeTree(m_root);
        }
        //Insert child node at current iterator
        void insert(const std::string& value) {
            if(m_iterator!=nullptr) {
                //Create new node by appending child nodes of current node
                m_iterator->m_children.push_back(new ASTNode(value, m_iterator));
            }
        }
        //Get value at current iterator
        const std::string& getValue() const {
            static const std::string emptyString;
            if (m_iterator!=nullptr) {
                return m_iterator->m_value;
            }
            else{
                return emptyString;
            }
        }
        //Advance iterator (depth first)
        void advanceIterator() {
            if (!m_iterator) return;

            // If the current node has children, visit the left-most unvisited child
            if (!m_iterator->m_children.empty()) {
                m_iterator = m_iterator->m_children[0];
                return;
            }

            // Backtrack to find the next unvisited sibling
            while (m_iterator->m_parent) {
                ASTNode* parent = m_iterator->m_parent;
                size_t index = std::find(parent->m_children.begin(), parent->m_children.end(), m_iterator) - parent->m_children.begin();
                
                // If there's an unvisited sibling, visit it
                if (index+1 < parent->m_children.size()) {
                    m_iterator = parent->m_children[index + 1];
                    return;
                }

                // Otherwise, continue backtracking
                m_iterator = parent;
            }

            // If we've backtracked all the way to the root without finding any unvisited nodes, the traversal is done
            //Set to the root pointer
            m_iterator = m_root;
        }
        //Backtrack iterator to parent node
        void backtrackIterator() {
            if(!m_iterator) return;
            m_iterator = m_iterator->m_parent;
        }
        //Reset iterator to root
        void resetIterator() {
            m_iterator = m_root;
        }

        bool atRoot() {
            return(m_iterator == m_root);
        }

    private:
        struct ASTNode {
            //Root node constructor
            ASTNode(): m_parent(nullptr) {};
            //Child node constructor
            ASTNode(const std::string& value, ASTNode* parentNode): m_value(value), m_parent(parentNode) {};

            //Nodes contain a value, parent reference, and vector of children
            std::string m_value;
            ASTNode* m_parent;
            std::vector<ASTNode*> m_children;
        };

        //Recursive free tree function
        void freeTree(ASTNode* currentNode) {
            //Return if currentNode is null
            if (currentNode == nullptr) {
                return;
            }
            //Traverse every child node
            for(int i=0; i<currentNode->m_children.size(); i++) {
                //Call freeTree again for the child
                freeTree(currentNode->m_children[i]);
            }
            //Delete current node
            delete currentNode;
        };

        //Each Tree contains a root node pointer and an iterator
        ASTNode* m_root;
        ASTNode* m_iterator;
};
#endif