#include "include/SemanticAnalyzer.h"

#include <iostream>

using namespace std;
using namespace AST;
using namespace ASTConstants;

SemanticAnalyzer::SemanticAnalyzer() {
    //Add instructions
    m_semanticMap[InstructionType::MOVE] = {
        {OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::LOAD] = {
        {OperandType::MEMORYADDRESS},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::STORE] = {
        {OperandType::REGISTER},
        {OperandType::MEMORYADDRESS}
    };
    m_semanticMap[InstructionType::CREATE] = {
        {OperandType::INTEGER, OperandType::CHARACTER, OperandType::BOOLEAN, OperandType::FLOAT},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::CAST] = {
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::ADD] = {
        {OperandType::REGISTER},
        {OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::SUB] = {
        {OperandType::REGISTER},
        {OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::MULTIPLY] = {
        {OperandType::REGISTER},
        {OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::DIVIDE] = {
        {OperandType::REGISTER},
        {OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::OR] = {
        {OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::AND] = {
        {OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    m_semanticMap[InstructionType::NOT] = {
        {OperandType::REGISTER},
    };
    m_semanticMap[InstructionType::SHIFT] = {
        {OperandType::REGISTER},
        {OperandType::REGISTER}
    };
    
}