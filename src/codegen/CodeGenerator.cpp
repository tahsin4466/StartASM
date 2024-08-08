#include "codegen/CodeGenerator.h"
#include "ast/Instructions.h"
#include "ast/Operands.h"

using namespace std;
using namespace llvm;
using namespace AST;

//Static LLVM Components Decleration
std::unique_ptr<llvm::LLVMContext> CodeGenerator::TheContext;
std::unique_ptr<llvm::IRBuilder<>> CodeGenerator::Builder;
std::unique_ptr<llvm::Module> CodeGenerator::TheModule;
std::map<std::string, llvm::Value *> CodeGenerator::NamedValues;


CodeGenerator::CodeGenerator() {
    // Initialize the LLVM context
    TheContext = make_unique<llvm::LLVMContext>();
    // Initialize the IRBuilder with the context
    Builder = make_unique<llvm::IRBuilder<>>(*TheContext);
    // Initialize the module with a name and the context
    TheModule = make_unique<llvm::Module>("StartASM_module", *TheContext);
    if (!TheContext || !Builder || !TheModule) {
        cerr << "Initialization failed" << std::endl;
        exit(0);
    }
}

CodeGenerator::~CodeGenerator() {
    //Just to ensure proper order of deletion of LLVM components
    TheModule.reset();
    Builder.reset();
    TheContext.reset();
}

void CodeGenerator::generateCode(AST::ASTNode *AST, int numLines) {
    //Initialize LLVM Value Data Structures
    vector<pair<ASTConstants::OperandType, Value*>> localValues(3, pair(ASTConstants::EMPTY, nullptr));
    OperandValues = vector<vector<pair<ASTConstants::OperandType, Value*>>>(numLines+1, localValues);
    InstructionValues = vector<Value*>(numLines+1, nullptr);

    //Visit the root and iterate over the AST
    AST->accept(*this);
}

//Root Node Visitor (Do nothing)
void CodeGenerator::visit(AST::RootNode& node) {}


//Instruction Generators
void CodeGenerator::visit(AST::MoveInstruction& node) {
    //std::cout << "TODO: MoveInstruction\n";
}

void CodeGenerator::visit(AST::LoadInstruction& node) {
   // std::cout << "TODO: LoadInstruction\n";
}

void CodeGenerator::visit(AST::StoreInstruction& node) {
    //std::cout << "TODO: StoreInstruction\n";
}

void CodeGenerator::visit(AST::CreateInstruction& node) {
    //std::cout << "TODO: CreateInstruction\n";
}

void CodeGenerator::visit(AST::CastInstruction& node) {
    //std::cout << "TODO: CastInstruction\n";
}

void CodeGenerator::visit(AST::AddInstruction& node) {
    //std::cout << "TODO: AddInstruction\n";
}

void CodeGenerator::visit(AST::SubInstruction& node) {
    //std::cout << "TODO: SubInstruction\n";
}

void CodeGenerator::visit(AST::MultiplyInstruction& node) {
    //std::cout << "TODO: MultiplyInstruction\n";
}

void CodeGenerator::visit(AST::DivideInstruction& node) {
    //std::cout << "TODO: DivideInstruction\n";
}

void CodeGenerator::visit(AST::OrInstruction& node) {
    //std::cout << "TODO: OrInstruction\n";
}

void CodeGenerator::visit(AST::AndInstruction& node) {
    //std::cout << "TODO: AndInstruction\n";
}

void CodeGenerator::visit(AST::NotInstruction& node) {
    //std::cout << "TODO: AndInstruction\n";
}

void CodeGenerator::visit(AST::ShiftInstruction& node) {
    //std::cout << "TODO: ShiftInstruction\n";
}

void CodeGenerator::visit(AST::CompareInstruction& node) {
    //std::cout << "TODO: CompareInstruction\n";
}

void CodeGenerator::visit(AST::JumpInstruction& node) {
    //std::cout << "TODO: JumpInstruction\n";
}

void CodeGenerator::visit(AST::CallInstruction& node) {
    //std::cout << "TODO: CallInstruction\n";
}

void CodeGenerator::visit(AST::PushInstruction& node) {
    //std::cout << "TODO: PushInstruction\n";
}

void CodeGenerator::visit(AST::PopInstruction& node) {
    //std::cout << "TODO: PopInstruction\n";
}

void CodeGenerator::visit(AST::ReturnInstruction& node) {
    //std::cout << "TODO: ReturnInstruction\n";
}

void CodeGenerator::visit(AST::StopInstruction& node) {
    //std::cout << "TODO: StopInstruction\n";
}

void CodeGenerator::visit(AST::InputInstruction& node) {
    //std::cout << "TODO: InputInstruction\n";
}

void CodeGenerator::visit(AST::OutputInstruction& node) {
    //std::cout << "TODO: OutputInstruction\n";
}

void CodeGenerator::visit(AST::PrintInstruction& node) {
    //std::cout << "TODO: PrintInstruction\n";
}

void CodeGenerator::visit(AST::LabelInstruction& node) {
    //std::cout << "TODO: LabelInstruction\n";
}

void CodeGenerator::visit(AST::CommentInstruction& node) {
    //std::cout << "TODO: CommentInstruction\n";
}


//Operand Generators
void CodeGenerator::visit(AST::RegisterOperand& node) {
    std::cout << "-> TODO: RegisterOperand\n";
}

void CodeGenerator::visit(AST::InstructionAddressOperand& node) {
    std::cout << "-> TODO: InstructionAddressOperand\n";
}

void CodeGenerator::visit(AST::MemoryAddressOperand& node) {
    std::cout << "-> TODO: MemoryAddressOperand\n";
}

void CodeGenerator::visit(AST::IntegerOperand& node) {
    //Basic 32bit int
    cout << "Converting int" << endl;
    OperandValues[node.getLine()][node.getPos()] = pair(node.getOperandType(),ConstantInt::get(*TheContext, APInt(32, stoi(node.getNodeValue()), true)));
}

void CodeGenerator::visit(AST::FloatOperand& node) {
    //Basic 32bit float
    cout << "Converting float" << endl;
    OperandValues[node.getLine()][node.getPos()] = pair(node.getOperandType(), ConstantFP::get(*TheContext, APFloat(stof(node.getNodeValue()))));
}

void CodeGenerator::visit(AST::BooleanOperand& node) {
    //Bool is treated as 1 bit integer
    cout << "Converting bool" << endl;
    OperandValues[node.getLine()][node.getPos()] = pair(node.getOperandType(), ConstantInt::get(*TheContext, APInt(1, (node.getNodeValue() == "true"), false)));
}

void CodeGenerator::visit(AST::CharacterOperand& node) {
    //Char is treated as 8-bit ASCII integer
    cout << "Converting char" << endl;
    OperandValues[node.getLine()][node.getPos()] = pair(node.getOperandType(), ConstantInt::get(*TheContext, APInt(8, node.getNodeValue()[0], false)));
}

//String and newline operands are for runtime library print functions
void CodeGenerator::visit(AST::StringOperand& node) {
    OperandValues[node.getLine()][node.getPos()] = pair(node.getOperandType(), nullptr);
}

void CodeGenerator::visit(AST::NewlineOperand& node) {
    OperandValues[node.getLine()][node.getPos()] = pair(node.getOperandType(), nullptr);
}

//Conditions are for instruction type decision-making
void CodeGenerator::visit(AST::TypeConditionOperand& node) {
    OperandValues[node.getLine()][node.getPos()] = pair(node.getOperandType(), nullptr);
}

void CodeGenerator::visit(AST::ShiftConditionOperand& node) {
    OperandValues[node.getLine()][node.getPos()] = pair(node.getOperandType(), nullptr);
}

void CodeGenerator::visit(AST::JumpConditionOperand& node) {
    OperandValues[node.getLine()][node.getPos()] = pair(node.getOperandType(), nullptr);
}

void CodeGenerator::printIR() {

}
