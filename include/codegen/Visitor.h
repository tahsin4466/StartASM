#ifndef VISITOR_H
#define VISITOR_H

namespace AST {
    //AST template node forward declerations
    class RootNode;
    class InstructionNode;
    class OperandNode;

    //AST Instruction node forward declerations
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

    //AST Operand node forward declerations
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
        virtual void visit(RootNode& node) = 0;
        virtual void visit(InstructionNode& node) = 0;
        virtual void visit(OperandNode& node) = 0;

        virtual void visit(MoveInstruction& node) = 0;
        virtual void visit(LoadInstruction& node) = 0;
        virtual void visit(StoreInstruction& node) = 0;
        virtual void visit(CreateInstruction& node) = 0;
        virtual void visit(CastInstruction& node) = 0;
        virtual void visit(AddInstruction& node) = 0;
        virtual void visit(SubInstruction& node) = 0;
        virtual void visit(MultiplyInstruction& node) = 0;
        virtual void visit(DivideInstruction& node) = 0;
        virtual void visit(OrInstruction& node) = 0;
        virtual void visit(AndInstruction& node) = 0;
        virtual void visit(ShiftInstruction& node) = 0;
        virtual void visit(CompareInstruction& node) = 0;
        virtual void visit(JumpInstruction& node) = 0;
        virtual void visit(CallInstruction& node) = 0;
        virtual void visit(PushInstruction& node) = 0;
        virtual void visit(PopInstruction& node) = 0;
        virtual void visit(ReturnInstruction& node) = 0;
        virtual void visit(StopInstruction& node) = 0;
        virtual void visit(InputInstruction& node) = 0;
        virtual void visit(OutputInstruction& node) = 0;
        virtual void visit(PrintInstruction& node) = 0;
        virtual void visit(LabelInstruction& node) = 0;
        virtual void visit(CommentInstruction& node) = 0;

        virtual void visit(RegisterOperand& node) = 0;
        virtual void visit(InstructionAddressOperand& node) = 0;
        virtual void visit(MemoryAddressOperand& node) = 0;
        virtual void visit(IntegerOperand& node) = 0;
        virtual void visit(FloatOperand& node) = 0;
        virtual void visit(BooleanOperand& node) = 0;
        virtual void visit(StringOperand& node) = 0;
        virtual void visit(NewlineOperand& node) = 0;
        virtual void visit(TypeConditionOperand& node) = 0;
        virtual void visit(ShiftConditionOperand& node) = 0;
        virtual void visit(JumpConditionOperand& node) = 0;

    };
}

#endif
