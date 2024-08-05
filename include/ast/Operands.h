#ifndef STARTASM_OPERANDS_H
#define STARTASM_OPERANDS_H

#include "ast/AbstractSyntaxTree.h"
#include "Visitor.h"
#include <string>

namespace AST {
    class RegisterOperand: public OperandNode {
    public:
        explicit RegisterOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::REGISTER) {}

        void accept(Visitor &visitor) override {visitor.visit(*this);};
    };

    class InstructionAddressOperand: public OperandNode {
    public:
        explicit InstructionAddressOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::INSTRUCTIONADDRESS) {}

        void accept(Visitor &visitor) override {visitor.visit(*this);};
    };

    class MemoryAddressOperand: public OperandNode {
    public:
        explicit MemoryAddressOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::MEMORYADDRESS) {}

        void accept(Visitor &visitor) override {visitor.visit(*this);};
    };

    class IntegerOperand: public OperandNode {
    public:
        explicit IntegerOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::INTEGER) {}

        void accept(Visitor &visitor) override {visitor.visit(*this);};
    };

    class FloatOperand: public OperandNode {
    public:
        explicit FloatOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::FLOAT) {}

        void accept(Visitor &visitor) override {visitor.visit(*this);};
    };

    class BooleanOperand: public OperandNode {
    public:
        explicit BooleanOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::BOOLEAN) {}

        void accept(Visitor &visitor) override {visitor.visit(*this);};
    };

    class CharacterOperand: public OperandNode {
    public:
        explicit CharacterOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::CHARACTER) {}

        void accept(Visitor &visitor) override {visitor.visit(*this);};
    };

    class StringOperand: public OperandNode {
    public:
        explicit StringOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::STRING) {}

        void accept(Visitor &visitor) override {visitor.visit(*this);};
    };

    class NewlineOperand: public OperandNode {
    public:
        explicit NewlineOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::NEWLINE) {}

        void accept(Visitor &visitor) override {visitor.visit(*this);};
    };

    class TypeConditionOperand: public OperandNode {
    public:
        explicit TypeConditionOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::TYPECONDITION) {}

        void accept(Visitor &visitor) override {visitor.visit(*this);};
    };

    class ShiftConditionOperand: public OperandNode {
    public:
        explicit ShiftConditionOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::SHIFTCONDITION) {}

        void accept(Visitor &visitor) override {visitor.visit(*this);};
    };

    class JumpConditionOperand: public OperandNode {
    public:
        explicit JumpConditionOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::JUMPCONDITION) {}

        void accept(Visitor &visitor) override {visitor.visit(*this);};
    };
}

#endif //STARTASM_OPERANDS_H
