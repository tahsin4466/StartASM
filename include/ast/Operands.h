#ifndef STARTASM_OPERANDS_H
#define STARTASM_OPERANDS_H

#include "ast/AbstractSyntaxTree.h"

#include <string>

namespace AST {
    class RegisterOperand: public OperandNode {
    public:
        RegisterOperand(const std::string &nodeValue)
            : OperandNode(nodeValue, ASTConstants::OperandType::REGISTER) {}

        void accept(Visitor &visitor) override;
    };

    class InstructionAddressOperand: public OperandNode {
    public:
        InstructionAddressOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::INSTRUCTIONADDRESS) {}

        void accept(Visitor &visitor) override;
    };

    class MemoryAddressOperand: public OperandNode {
    public:
        MemoryAddressOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::MEMORYADDRESS) {}

        void accept(Visitor &visitor) override;
    };

    class IntegerOperand: public OperandNode {
    public:
        IntegerOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::INTEGER) {}

        void accept(Visitor &visitor) override;
    };

    class FloatOperand: public OperandNode {
    public:
        FloatOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::FLOAT) {}

        void accept(Visitor &visitor) override;
    };

    class BooleanOperand: public OperandNode {
    public:
        BooleanOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::BOOLEAN) {}

        void accept(Visitor &visitor) override;
    };

    class CharacterOperand: public OperandNode {
    public:
        CharacterOperand(const std::string &nodeValue)
            : OperandNode(nodeValue, ASTConstants::OperandType::CHARACTER) {}

        void accept(Visitor &visitor) override;
    };

    class StringOperand: public OperandNode {
    public:
        StringOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::STRING) {}

        void accept(Visitor &visitor) override;
    };

    class NewlineOperand: public OperandNode {
    public:
        NewlineOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::NEWLINE) {}

        void accept(Visitor &visitor) override;
    };

    class TypeConditionOperand: public OperandNode {
    public:
        TypeConditionOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::TYPECONDITION) {}

        void accept(Visitor &visitor) override;
    };

    class ShiftConditionOperand: public OperandNode {
    public:
        ShiftConditionOperand(const std::string &nodeValue)
            : OperandNode(nodeValue, ASTConstants::OperandType::SHIFTCONDITION) {}

        void accept(Visitor &visitor) override;
    };

    class JumpConditionOperand: public OperandNode {
    public:
        JumpConditionOperand(const std::string &nodeValue)
                : OperandNode(nodeValue, ASTConstants::OperandType::JUMPCONDITION) {}

        void accept(Visitor &visitor) override;
    };
}

#endif //STARTASM_OPERANDS_H
