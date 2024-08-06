#include "ast/ASTBuilder.h"
#include <vector>

using namespace std;

ASTBuilder::ASTBuilder() {
    initializeFactoryMaps();
}

void ASTBuilder::initializeFactoryMaps() {
    //Factory map for creating instruction nodes
    instructionFactoryMap = {
            {ASTConstants::MOVE, [](const std::string& value, int line) { return new AST::MoveInstruction(value, line); }},
            {ASTConstants::LOAD, [](const std::string& value, int line) { return new AST::LoadInstruction(value, line); }},
            {ASTConstants::STORE, [](const std::string& value, int line) { return new AST::StoreInstruction(value, line); }},
            {ASTConstants::CREATE, [](const std::string& value, int line) { return new AST::CreateInstruction(value, line); }},
            {ASTConstants::CAST, [](const std::string& value, int line) { return new AST::CastInstruction(value, line); }},
            {ASTConstants::ADD, [](const std::string& value, int line) { return new AST::AddInstruction(value, line); }},
            {ASTConstants::SUB, [](const std::string& value, int line) { return new AST::SubInstruction(value, line); }},
            {ASTConstants::MULTIPLY, [](const std::string& value, int line) { return new AST::MultiplyInstruction(value, line); }},
            {ASTConstants::DIVIDE, [](const std::string& value, int line) { return new AST::DivideInstruction(value, line); }},
            {ASTConstants::OR, [](const std::string& value, int line) { return new AST::OrInstruction(value, line); }},
            {ASTConstants::AND, [](const std::string& value, int line) { return new AST::AndInstruction(value, line); }},
            {ASTConstants::NOT, [](const std::string& value, int line) { return new AST::NotInstruction(value, line); }},
            {ASTConstants::SHIFT, [](const std::string& value, int line) { return new AST::ShiftInstruction(value, line); }},
            {ASTConstants::COMPARE, [](const std::string& value, int line) { return new AST::CompareInstruction(value, line); }},
            {ASTConstants::JUMP, [](const std::string& value, int line) { return new AST::JumpInstruction(value, line); }},
            {ASTConstants::CALL, [](const std::string& value, int line) { return new AST::CallInstruction(value, line); }},
            {ASTConstants::PUSH, [](const std::string& value, int line) { return new AST::PushInstruction(value, line); }},
            {ASTConstants::POP, [](const std::string& value, int line) { return new AST::PopInstruction(value, line); }},
            {ASTConstants::RETURN, [](const std::string& value, int line) { return new AST::ReturnInstruction(value, line); }},
            {ASTConstants::STOP, [](const std::string& value, int line) { return new AST::StopInstruction(value, line); }},
            {ASTConstants::INPUT, [](const std::string& value, int line) { return new AST::InputInstruction(value, line); }},
            {ASTConstants::OUTPUT, [](const std::string& value, int line) { return new AST::OutputInstruction(value, line); }},
            {ASTConstants::PRINT, [](const std::string& value, int line) { return new AST::PrintInstruction(value, line); }},
            {ASTConstants::LABEL, [](const std::string& value, int line) { return new AST::LabelInstruction(value, line); }},
            {ASTConstants::COMMENT, [](const std::string& value, int line) { return new AST::CommentInstruction(value, line); }},
    };

    //Factory map for creating operand nodes
    operandFactoryMap = {
            {ASTConstants::REGISTER, [](const std::string& value, int line) { return new AST::RegisterOperand(value, line); }},
            {ASTConstants::INSTRUCTIONADDRESS, [](const std::string& value, int line) { return new AST::InstructionAddressOperand(value, line); }},
            {ASTConstants::MEMORYADDRESS, [](const std::string& value, int line) { return new AST::MemoryAddressOperand(value, line); }},
            {ASTConstants::INTEGER, [](const std::string& value, int line) { return new AST::IntegerOperand(value, line); }},
            {ASTConstants::FLOAT, [](const std::string& value, int line) { return new AST::FloatOperand(value, line); }},
            {ASTConstants::BOOLEAN, [](const std::string& value, int line) { return new AST::BooleanOperand(value, line); }},
            {ASTConstants::CHARACTER, [](const std::string& value, int line) { return new AST::CharacterOperand(value, line); }},
            {ASTConstants::STRING, [](const std::string& value, int line) { return new AST::StringOperand(value, line); }},
            {ASTConstants::NEWLINE, [](const std::string& value, int line) { return new AST::NewlineOperand(value, line); }},
            {ASTConstants::TYPECONDITION, [](const std::string& value, int line) { return new AST::TypeConditionOperand(value, line); }},
            {ASTConstants::SHIFTCONDITION, [](const std::string& value, int line) { return new AST::ShiftConditionOperand(value, line); }},
            {ASTConstants::JUMPCONDITION, [](const std::string& value, int line) { return new AST::JumpConditionOperand(value, line); }},
    };
}

void ASTBuilder::buildAST(PT::PTNode* parseTree, AST::AbstractSyntaxTree* abstractSyntaxTree) {
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
                ASTInstructionNode->insertChild(operandBuilder(
                        abstractSyntaxTree->convertOperandType(PTOperandNode->getOperandType()),
                        PTOperandNode->getNodeValue(),
                        i+1
                ));
            }
        }
    }

    // Insert instruction nodes into the AST root node (sequential part to ensure thread safety)
    ASTRoot->reserveChildren(PTSize);
    for (int i = 0; i < PTSize; i++) {
        ASTRoot->insertChild(instructionNodes[i]);
    }
}

AST::InstructionNode* ASTBuilder::instructionBuilder(ASTConstants::InstructionType nodeType, const std::string& value, int line) {
    auto it = instructionFactoryMap.find(nodeType);
    if (it != instructionFactoryMap.end()) {
        return it->second(value, line);
    }
    return nullptr;
}

AST::OperandNode* ASTBuilder::operandBuilder(ASTConstants::OperandType nodeType, const std::string& value, int line) {
    auto it = operandFactoryMap.find(nodeType);
    if (it != operandFactoryMap.end()) {
        return it->second(value, line);
    }
    return nullptr;
}
