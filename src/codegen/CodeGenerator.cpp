#include "codegen/CodeGenerator.h"
#include "ast/Instructions.h"
#include "ast/Operands.h"


CodeGenerator::CodeGenerator()
        : context(), builder(context) {
    module = std::make_unique<llvm::Module>("StartASMModule", context);
}

void CodeGenerator::visit(AST::RootNode& node) {
    std::cout << "TODO: Root Node\n";
}

void CodeGenerator::visit(AST::MoveInstruction& node) {
    std::cout << "TODO: MoveInstruction\n";
}

void CodeGenerator::visit(AST::LoadInstruction& node) {
    std::cout << "TODO: LoadInstruction\n";
}

void CodeGenerator::visit(AST::StoreInstruction& node) {
    std::cout << "TODO: StoreInstruction\n";
}

void CodeGenerator::visit(AST::CreateInstruction& node) {
    std::cout << "TODO: CreateInstruction\n";
}

void CodeGenerator::visit(AST::CastInstruction& node) {
    std::cout << "TODO: CastInstruction\n";
}

void CodeGenerator::visit(AST::AddInstruction& node) {
    std::cout << "TODO: AddInstruction\n";
}

void CodeGenerator::visit(AST::SubInstruction& node) {
    std::cout << "TODO: SubInstruction\n";
}

void CodeGenerator::visit(AST::MultiplyInstruction& node) {
    std::cout << "TODO: MultiplyInstruction\n";
}

void CodeGenerator::visit(AST::DivideInstruction& node) {
    std::cout << "TODO: DivideInstruction\n";
}

void CodeGenerator::visit(AST::OrInstruction& node) {
    std::cout << "TODO: OrInstruction\n";
}

void CodeGenerator::visit(AST::AndInstruction& node) {
    std::cout << "TODO: AndInstruction\n";
}

void CodeGenerator::visit(AST::ShiftInstruction& node) {
    std::cout << "TODO: ShiftInstruction\n";
}

void CodeGenerator::visit(AST::CompareInstruction& node) {
    std::cout << "TODO: CompareInstruction\n";
}

void CodeGenerator::visit(AST::JumpInstruction& node) {
    std::cout << "TODO: JumpInstruction\n";
}

void CodeGenerator::visit(AST::CallInstruction& node) {
    std::cout << "TODO: CallInstruction\n";
}

void CodeGenerator::visit(AST::PushInstruction& node) {
    std::cout << "TODO: PushInstruction\n";
}

void CodeGenerator::visit(AST::PopInstruction& node) {
    std::cout << "TODO: PopInstruction\n";
}

void CodeGenerator::visit(AST::ReturnInstruction& node) {
    std::cout << "TODO: ReturnInstruction\n";
}

void CodeGenerator::visit(AST::StopInstruction& node) {
    std::cout << "TODO: StopInstruction\n";
}

void CodeGenerator::visit(AST::InputInstruction& node) {
    std::cout << "TODO: InputInstruction\n";
}

void CodeGenerator::visit(AST::OutputInstruction& node) {
    std::cout << "TODO: OutputInstruction\n";
}

void CodeGenerator::visit(AST::PrintInstruction& node) {
    std::cout << "TODO: PrintInstruction\n";
}

void CodeGenerator::visit(AST::LabelInstruction& node) {
    std::cout << "TODO: LabelInstruction\n";
}

void CodeGenerator::visit(AST::CommentInstruction& node) {
    std::cout << "TODO: CommentInstruction\n";
}

void CodeGenerator::visit(AST::RegisterOperand& node) {
    std::cout << "TODO: RegisterOperand\n";
}

void CodeGenerator::visit(AST::InstructionAddressOperand& node) {
    std::cout << "TODO: InstructionAddressOperand\n";
}

void CodeGenerator::visit(AST::MemoryAddressOperand& node) {
    std::cout << "TODO: MemoryAddressOperand\n";
}

void CodeGenerator::visit(AST::IntegerOperand& node) {
    std::cout << "TODO: IntegerOperand\n";
}

void CodeGenerator::visit(AST::FloatOperand& node) {
    std::cout << "TODO: FloatOperand\n";
}

void CodeGenerator::visit(AST::BooleanOperand& node) {
    std::cout << "TODO: BooleanOperand\n";
}

void CodeGenerator::visit(AST::CharacterOperand& node) {
    std::cout << "TODO: CharacterOperand\n";
}

void CodeGenerator::visit(AST::StringOperand& node) {
    std::cout << "TODO: StringOperand\n";
}

void CodeGenerator::visit(AST::NewlineOperand& node) {
    std::cout << "TODO: NewlineOperand\n";
}

void CodeGenerator::visit(AST::TypeConditionOperand& node) {
    std::cout << "TODO: TypeConditionOperand\n";
}

void CodeGenerator::visit(AST::ShiftConditionOperand& node) {
    std::cout << "TODO: ShiftConditionOperand\n";
}

void CodeGenerator::visit(AST::JumpConditionOperand& node) {
    std::cout << "TODO: JumpConditionOperand\n";
}

void CodeGenerator::printIR() {
    module->print(llvm::outs(), nullptr);
}
