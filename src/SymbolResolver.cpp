#include "include/SymbolResolver.h"

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

bool SymbolResolver::resolveSymbols(unordered_map<string, pair<string, int>> &symbolTable, PT::PTNode *parseTree, string &errorMessage, const std::vector<std::string>& codeLines) {
    //Perform main steps of symbol resolution
    buildSymbolTable(symbolTable, parseTree, codeLines);
    bindSymbols(symbolTable, parseTree, codeLines);

    //Concatenate invalidLines string from all errors accumulated out of order
    string invalidLines;
    for (const auto& pair : m_invalidLinesMap) {
        invalidLines += pair.second;
    }

    //Return false if invalidLines not empty (errors encountered)
    if (!invalidLines.empty()) {
        errorMessage = invalidLines;
        return false;
    }
    return true;
}

void SymbolResolver::buildSymbolTable(unordered_map<string, pair<string, int>> &symbolTable, PT::PTNode *parseTree, const std::vector<std::string>& codeLines) {
    int parseTreeSize = parseTree->getNumChildren();
    //Look for label declarations in parse tree and add to the label table
    //Iterate over all children in the loop leveraging OMP
    #pragma omp parallel for schedule(dynamic) default(none) shared(parseTree, parseTreeSize, symbolTable, codeLines)
    for (int i=0; i<parseTreeSize; i++) {
        //Iterate over every child in the root node
        if(parseTree->childAt(i)->getNodeValue() == "label") {
            string labelValue = parseTree->childAt(i)->childAt(0)->childAt(0)->getNodeValue();
            //Check if label is already declared in the symbol table
            auto itr = symbolTable.find(labelValue);
            if (itr != symbolTable.end()) {
                //Critical section as modifying STL container
                #pragma omp critical
                {
                    m_invalidLinesMap[i] = "\nLabel error at line " + to_string(i+1) + ": " +  codeLines[i] + "\nDuplicate label " + labelValue + " already declared at line " + to_string(itr->second.second+1) + "\n";
                }
            }
            //Critical section as modifying STL container
            #pragma omp critical
            {
                //Include the label (child of instruction child) and corresponding address (root node index + 1)
                symbolTable.emplace(labelValue, make_pair("i[" + to_string(i+1) +  "]", i));
            }
        }
    }
}

void SymbolResolver::bindSymbols(unordered_map<string, pair<string, int>> &symbolTable, PT::PTNode *parseTree, const std::vector<std::string>& codeLines) {
    int parseTreeSize = parseTree->getNumChildren();
    #pragma omp parallel for schedule(dynamic) default(none) shared(parseTree, parseTreeSize, symbolTable, codeLines)
    for (int i=0; i<parseTreeSize; i++) {
        //Get the node pointer for the line and size (frequent access)
        PT::PTNode* lineNode = parseTree->childAt(i);
        int lineSize = lineNode->getNumChildren();
        for(int j=0; j<lineSize; j++) {
            //If the child is an operand node (L2 nodes are atomic with only one child)
            if (lineNode->childAt(j)->childAt(0)->getNodeType() == PTConstants::NodeType::OPERAND) {
                //Cast to operandNode to check type
                auto labelNode = dynamic_cast<PT::OperandNode*>(lineNode->childAt(j)->childAt(0));
                //Check if successful cast and type is a label
                if (labelNode != nullptr && labelNode->getOperandType() == PTConstants::OperandType::LABEL) {
                    //Decision logic - check if a part of symbolTable
                    auto itr = symbolTable.find(labelNode->getNodeValue());
                    if (itr == symbolTable.end()) {
                        //Throw an undefined error if not found in symbol table
                        //Modifying section - use critical
                        #pragma omp critical
                        {
                            m_invalidLinesMap[i] = "\nLabel error at line " + to_string(i+1) + ": " +  codeLines[i] + "\nUndefined label " + labelNode->getNodeValue() + "\n";
                        }
                    }
                    else {
                        //Change operand value and operand type to instruction address
                        //Tree is NOT thread safe writing - critical section
                        #pragma omp critical
                        {
                            labelNode->setNodeValue(itr->second.first);
                            labelNode->setOperandType(PTConstants::OperandType::INSTRUCTIONADDRESS);
                        }
                    }
                }
            }
        }
    }
}
