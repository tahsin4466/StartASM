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
        auto ASTInstructionNode = instructionBuilder(
            abstractSyntaxTree->getInstructionType(PTInstructionNode->getNodeValue()),
            PTInstructionNode->getNodeValue(),
            i + 1
        );

        // Store the instruction node in the vector
        instructionNodes[i] = ASTInstructionNode;

        // Add all operands from the PT for the AST
        for (int j = 0; j < PTInstructionNode->getNumChildren(); j++) {
            // Get the operand node from the PT and cast to an OperandNode (parser guarantees this)
            auto PTOperandNode = dynamic_cast<PT::OperandNode*>(PTInstructionNode->childAt(j)->childAt(0));

            // Do a check anyway to make sure dynamic cast was successful
            if (PTOperandNode != nullptr && ASTInstructionNode != nullptr) {
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

AST::InstructionNode* ASTBuilder::instructionBuilder(ASTConstants::InstructionType nodeType, std::string value, int line) {
    AST::InstructionNode* returnNode = nullptr;
    switch (nodeType) {
        case ASTConstants::MOVE:
            returnNode = new AST::MoveInstruction(value, line);
            break;
        case ASTConstants::LOAD:
            returnNode = new AST::LoadInstruction(value, line);
            break;
        case ASTConstants::STORE:
            returnNode = new AST::StoreInstruction(value, line);
            break;
        case ASTConstants::CREATE:
            returnNode = new AST::CreateInstruction(value, line);
            break;
        case ASTConstants::CAST:
            returnNode = new AST::CastInstruction(value, line);
            break;
        case ASTConstants::ADD:
            returnNode = new AST::AddInstruction(value, line);
            break;
        case ASTConstants::SUB:
            returnNode = new AST::SubInstruction(value, line);
            break;
        case ASTConstants::MULTIPLY:
            returnNode = new AST::MultiplyInstruction(value, line);
            break;
        case ASTConstants::DIVIDE:
            returnNode = new AST::DivideInstruction(value, line);
            break;
        case ASTConstants::OR:
            returnNode = new AST::OrInstruction(value, line);
            break;
        case ASTConstants::AND:
            returnNode = new AST::AndInstruction(value, line);
            break;
        case ASTConstants::NOT:
            returnNode = new AST::NotInstruction(value, line);
            break;
        case ASTConstants::SHIFT:
            returnNode = new AST::ShiftInstruction(value, line);
            break;
        case ASTConstants::COMPARE:
            returnNode = new AST::CompareInstruction(value, line);
            break;
        case ASTConstants::JUMP:
            returnNode = new AST::JumpInstruction(value, line);
            break;
        case ASTConstants::CALL:
            returnNode = new AST::CallInstruction(value, line);
            break;
        case ASTConstants::PUSH:
            returnNode = new AST::PushInstruction(value, line);
            break;
        case ASTConstants::POP:
            returnNode = new AST::PopInstruction(value, line);
            break;
        case ASTConstants::RETURN:
            returnNode = new AST::ReturnInstruction(value, line);
            break;
        case ASTConstants::STOP:
            returnNode = new AST::StopInstruction(value, line);
            break;
        case ASTConstants::INPUT:
            returnNode = new AST::InputInstruction(value, line);
            break;
        case ASTConstants::OUTPUT:
            returnNode = new AST::OutputInstruction(value, line);
            break;
        case ASTConstants::PRINT:
            returnNode = new AST::PrintInstruction(value, line);
            break;
        case ASTConstants::LABEL:
            returnNode = new AST::LabelInstruction(value, line);
            break;
        case ASTConstants::COMMENT:
            returnNode = new AST::CommentInstruction(value, line);
            break;
        case ASTConstants::NONE:
        default:
            break;
    }
    return returnNode;

}