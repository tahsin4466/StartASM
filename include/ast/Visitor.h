#ifndef VISITOR_H
#define VISITOR_H

namespace AST {
    //AST root node forward declarations
    class RootNode;

    //AST Instruction node forward declarations
    class MoveInstruction;
    class LoadInstruction;
    class StoreInstruction;
    class CreateInstruction;
    class CastInstruction;
    class AddInstruction;
    class SubInstruction;
    class MultiplyInstruction;
    class DivideInstruction;
    class OrInstruction;
    class AndInstruction;
    class NotInstruction;
    class ShiftInstruction;
    class CompareInstruction;
    class JumpInstruction;
    class CallInstruction;
    class PushInstruction;
    class PopInstruction;
    class ReturnInstruction;
    class StopInstruction;
    class InputInstruction;
    class OutputInstruction;
    class PrintInstruction;
    class LabelInstruction;
    class CommentInstruction;

    //AST Operand node forward declarations
    class RegisterOperand;
    class InstructionAddressOperand;
    class MemoryAddressOperand;
    class IntegerOperand;
    class FloatOperand;
    class BooleanOperand;
    class CharacterOperand;
    class StringOperand;
    class NewlineOperand;
    class TypeConditionOperand;
    class ShiftConditionOperand;
    class JumpConditionOperand;

    class Visitor {
    public:
        virtual void visit(AST::RootNode &node) = 0;

        virtual void visit(AST::MoveInstruction &node) = 0;
        virtual void visit(AST::LoadInstruction &node) = 0;
        virtual void visit(AST::StoreInstruction &node) = 0;
        virtual void visit(AST::CreateInstruction &node) = 0;
        virtual void visit(AST::CastInstruction &node) = 0;
        virtual void visit(AST::AddInstruction &node) = 0;
        virtual void visit(AST::SubInstruction &node) = 0;
        virtual void visit(AST::MultiplyInstruction &node) = 0;
        virtual void visit(AST::DivideInstruction &node) = 0;
        virtual void visit(AST::OrInstruction &node) = 0;
        virtual void visit(AST::AndInstruction &node) = 0;
        virtual void visit(AST::NotInstruction &node) = 0;
        virtual void visit(AST::ShiftInstruction &node) = 0;
        virtual void visit(AST::CompareInstruction &node) = 0;
        virtual void visit(AST::JumpInstruction &node) = 0;
        virtual void visit(AST::CallInstruction &node) = 0;
        virtual void visit(AST::PushInstruction &node) = 0;
        virtual void visit(AST::PopInstruction &node) = 0;
        virtual void visit(AST::ReturnInstruction &node) = 0;
        virtual void visit(AST::StopInstruction &node) = 0;
        virtual void visit(AST::InputInstruction &node) = 0;
        virtual void visit(AST::OutputInstruction &node) = 0;
        virtual void visit(AST::PrintInstruction &node) = 0;
        virtual void visit(AST::LabelInstruction &node) = 0;
        virtual void visit(AST::CommentInstruction &node) = 0;

        virtual void visit(AST::RegisterOperand &node) = 0;
        virtual void visit(AST::InstructionAddressOperand &node) = 0;
        virtual void visit(AST::MemoryAddressOperand &node) = 0;
        virtual void visit(AST::IntegerOperand &node) = 0;
        virtual void visit(AST::FloatOperand &node) = 0;
        virtual void visit(AST::BooleanOperand &node) = 0;
        virtual void visit(AST::CharacterOperand &node) = 0;
        virtual void visit(AST::StringOperand &node) = 0;
        virtual void visit(AST::NewlineOperand &node) = 0;
        virtual void visit(AST::TypeConditionOperand &node) = 0;
        virtual void visit(AST::ShiftConditionOperand &node) = 0;
        virtual void visit(AST::JumpConditionOperand &node) = 0;
    };
}
#endif
