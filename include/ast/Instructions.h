#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "ast/AbstractSyntaxTree.h"

#include <string>

namespace AST {
    class MoveInstruction : public InstructionNode {
    public:
        //Constructor - specifies num operands automatically
        MoveInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::MOVE, ASTConstants::NumOperands::BINARY,
                                  line) {}

        //Accept method
        void accept(Visitor &visitor) override {
            //Visit for the instruction first
            visitor.visit(*this);
            //Iterate and visit for children
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class LoadInstruction : public InstructionNode {
    public:
        LoadInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::LOAD, ASTConstants::NumOperands::BINARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class StoreInstruction : public InstructionNode {
    public:
        StoreInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::STORE, ASTConstants::NumOperands::BINARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class CreateInstruction : public InstructionNode {
    public:
        CreateInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::CREATE, ASTConstants::NumOperands::TERNARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class CastInstruction : public InstructionNode {
    public:
        CastInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::CAST, ASTConstants::NumOperands::BINARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class AddInstruction : public InstructionNode {
    public:
        AddInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::ADD, ASTConstants::NumOperands::TERNARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class SubInstruction : public InstructionNode {
    public:
        SubInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::SUB, ASTConstants::NumOperands::TERNARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class MultiplyInstruction : public InstructionNode {
    public:
        MultiplyInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::MULTIPLY,
                                  ASTConstants::NumOperands::TERNARY, line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class DivideInstruction : public InstructionNode {
    public:
        DivideInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::DIVIDE, ASTConstants::NumOperands::TERNARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class OrInstruction : public InstructionNode {
    public:
        OrInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::OR, ASTConstants::NumOperands::BINARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class AndInstruction : public InstructionNode {
    public:
        AndInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::AND, ASTConstants::NumOperands::BINARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class NotInstruction : public InstructionNode {
    public:
        NotInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::NOT, ASTConstants::NumOperands::UNARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class ShiftInstruction : public InstructionNode {
    public:
        ShiftInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::SHIFT, ASTConstants::NumOperands::TERNARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class CompareInstruction : public InstructionNode {
    public:
        CompareInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::COMPARE, ASTConstants::NumOperands::BINARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class JumpInstruction : public InstructionNode {
    public:
        JumpInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::JUMP, ASTConstants::NumOperands::BINARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class CallInstruction : public InstructionNode {
    public:
        CallInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::CALL, ASTConstants::NumOperands::UNARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class PushInstruction : public InstructionNode {
    public:
        PushInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::PUSH, ASTConstants::NumOperands::UNARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class PopInstruction : public InstructionNode {
    public:
        PopInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::POP, ASTConstants::NumOperands::UNARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class ReturnInstruction : public InstructionNode {
    public:
        ReturnInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::RETURN, ASTConstants::NumOperands::NULLARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class StopInstruction : public InstructionNode {
    public:
        StopInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::STOP, ASTConstants::NumOperands::NULLARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class InputInstruction : public InstructionNode {
    public:
        InputInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::INPUT, ASTConstants::NumOperands::BINARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class OutputInstruction : public InstructionNode {
    public:
        OutputInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::OUTPUT, ASTConstants::NumOperands::UNARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class PrintInstruction : public InstructionNode {
    public:
        PrintInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::PRINT, ASTConstants::NumOperands::UNARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class LabelInstruction : public InstructionNode {
    public:
        LabelInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::LABEL, ASTConstants::NumOperands::UNARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };

    class CommentInstruction : public InstructionNode {
    public:
        CommentInstruction(const std::string &nodeValue, int line)
                : InstructionNode(nodeValue, ASTConstants::InstructionType::COMMENT, ASTConstants::NumOperands::UNARY,
                                  line) {}

        void accept(Visitor &visitor) override {
            visitor.visit(*this);
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    };
}
#endif