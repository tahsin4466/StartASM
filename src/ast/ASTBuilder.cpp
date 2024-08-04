#include "ast/ASTBuilder.h"

#include <vector>

using namespace std;

void ASTBuilder::buildAST(PT::PTNode *parseTree, AST::AbstractSyntaxTree *abstractSyntaxTree) {
    int PTSize = parseTree->getNumChildren();
    // Get the AST root node
    AST::ASTNode* ASTRoot = abstractSyntaxTree->getRoot();

    // Vector to store AST instruction nodes
    std::vector<AST::InstructionNode*> instructionNodes(PTSize);

    // Iterate over all children (instructions) in the parse tree
    // Parallelize the creation of instruction nodes and their children
    #pragma omp parallel for schedule(dynamic) default(none) shared(parseTree, PTSize, instructionNodes, abstractSyntaxTree)
    for (int i = 0; i < PTSize; i++) {
        // Get the pointer to the instruction node from the PT
        PT::PTNode* PTInstructionNode = parseTree->childAt(i);

        // Initialize a new AST instruction node, using built-in conversion methods found in the AST class
        auto ASTInstructionNode = new AST::InstructionNode(
            PTInstructionNode->getNodeValue(),
            abstractSyntaxTree->getInstructionType(PTInstructionNode->getNodeValue()),
            abstractSyntaxTree->getNumOperands(PTInstructionNode->getNumChildren()),
            i + 1
        );

        // Store the instruction node in the vector
        instructionNodes[i] = ASTInstructionNode;

        // Add all operands from the PT for the AST
        for (int j = 0; j < PTInstructionNode->getNumChildren(); j++) {
            // Get the operand node from the PT and cast to an OperandNode (parser guarantees this)
            auto PTOperandNode = dynamic_cast<PT::OperandNode*>(PTInstructionNode->childAt(j)->childAt(0));

            // Do a check anyway to make sure dynamic cast was successful
            if (PTOperandNode != nullptr) {
                // Add a child for the instruction node in the AST, using conversion functions from the AST as necessary
                ASTInstructionNode->insertChild(new AST::OperandNode(PTOperandNode->getNodeValue(), abstractSyntaxTree->convertOperandType(PTOperandNode->getOperandType())));
            }
        }
    }

    // Insert instruction nodes into the AST root node (sequential part to ensure thread safety)
    ASTRoot->reserveChildren(PTSize);
    for (int i = 0; i < PTSize; i++) {
        ASTRoot->insertChild(instructionNodes[i]);
    }
}