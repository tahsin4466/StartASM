#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <functional>

class Parser {
    public:
        //Constructor and destructor
        Parser();
        ~Parser() {};

        //Delete copy and assignment
        Parser(const Parser&) = delete;
        Parser& operator=(const Parser&) = delete;

        //Validate instruction method
        std::string validateInstruction(std::vector<std::string> tokens);

    private:
        //Hash map containing a keyword linked to an instruction parsing function
        std::unordered_map<std::string, std::function<std::string(std::vector<std::string>)>> m_instructionMap;

        //Instruction Parsing Functions
        static std::string instructionMove(std::vector<std::string> tokens);
        static std::string instructionLoad(std::vector<std::string> tokens);
        static std::string instructionStore(std::vector<std::string> tokens);
        static std::string instructionCreate(std::vector<std::string> tokens);
        static std::string instructionCast(std::vector<std::string> tokens);
        static std::string instructionAdd(std::vector<std::string> tokens);
        static std::string instructionSub(std::vector<std::string> tokens);
        static std::string instructionMultiply(std::vector<std::string> tokens);
        static std::string instructionDivide(std::vector<std::string> tokens);
        static std::string instructionOr(std::vector<std::string> tokens);
        static std::string instructionAnd(std::vector<std::string> tokens);
        static std::string instructionNot(std::vector<std::string> tokens);
        static std::string instructionShift(std::vector<std::string> tokens);
        static std::string instructionCompare(std::vector<std::string> tokens); 
        static std::string instructionJump(std::vector<std::string> tokens);
        static std::string instructionCall(std::vector<std::string> tokens);
        static std::string instructionPush(std::vector<std::string> tokens);
        static std::string instructionPop(std::vector<std::string> tokens);
        static std::string instructionReturn(std::vector<std::string> tokens);
        static std::string instructionStop(std::vector<std::string> tokens);
        static std::string instructionLabel(std::vector<std::string> tokens);
        static std::string instructionComment(std::vector<std::string> tokens);

        //Operand Parsing Functions
        static std::string operandRegister(std::string token);
        static std::string operandMemory(std::string token);
        static std::string operandInstruction(std::string token);
        static std::string operandInteger(std::string token);
        static std::string operandFloat(std::string token);
        static std::string operandBoolean(std::string token);
        static std::string operandCharacter(std::string token);
        static std::string operandLabel(std::string token);
        static std::string operandComment(std::string token);

        //Conjunction Parsing Functions
        static std::string conjunctionFrom(std::string token);
        static std::string conjunctionWith(std::string token);
        static std::string conjunctionSelf(std::string token);
        static std::string conjunctionTo(std::string token);
        static std::string conjunctionBy(std::string token);

        //Condition Parsing Functions
        //Shifting
        static std::string conditionLeft(std::string token);
        static std::string conditionRight(std::string token);
        static std::string conditionLogically(std::string token);
        static std::string conditionArithmetically(std::string token);

        //Jumping
        static std::string conditionIf(std::string token);
        static std::string conditionNot(std::string token);
        static std::string conditionGreater(std::string token);
        static std::string conditionLess(std::string token);
        static std::string conditionEqual(std::string token);
        static std::string conditionZero(std::string token);

        //Creating
        static std::string conditionInteger(std::string token);
        static std::string conditionFloat(std::string token);
        static std::string conditionBool(std::string token);
        static std::string conditionCharacter(std::string token);
        static std::string conditionAddress(std::string token);


};

#endif